#include "MRKImage.h"
#include "MRKVulkanTools.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "MRKBuffer.h"
#include "MRKGraphicsSystem.h"

namespace mrk
{
	/**
	 * \brief createInfo structure if you want to have a texture image:
	 * format      = vk::Format::eR8G8B8A8Unorm
	 * tiling      = vk::ImageTiling::eOptimal
	 * usage       = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled
	 * properties  = vk::MemoryPropertyFlagBits::eDeviceLocal
	 * texturePath = path to texture
	 * \param createInfo 
	 */
	Image::Image(mrk::Image::CreateInfo createInfo) :
		Resource(),
		info_(createInfo),
	    mImage(createImage(createInfo)),
	    mImageView(createImageView(createInfo.format, createInfo.aspectFlags)),
		mSampler(createImageSampler())
	{
	}


	Image::~Image()
	{
		if (mSize > 0)
			destroy();
	}

	vk::Image & Image::createImage(mrk::Image::CreateInfo & createInfo)
	{
        // TODO look into getting rid of these dimension parameters
		int width = static_cast<int>(createInfo.width);
		int height = static_cast<int>(createInfo.height);
		bool hasTexture = { (createInfo.texturePath != nullptr) };
		vk::DeviceSize imageSize = {0};
		stbi_uc* pixels = {nullptr};

		if(hasTexture)
		{
			//texture loading
			int texChannels;

			pixels = stbi_load(createInfo.texturePath, &width, &height, &texChannels, STBI_rgb_alpha);
			info_.width = width;
			info_.height = height;

			imageSize = width * height * 4; // 4 because 4 bytes per channel in STBI_rgb_alpha

			if (!pixels)
				throw_line("Failed to load texture!")
		}

		auto imageInfo = vk::ImageCreateInfo()
			.setImageType(vk::ImageType::e2D)
			.setExtent({ static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 })
			.setMipLevels(1)
			.setArrayLayers(1)
			.setFormat(createInfo.format)
			.setTiling(createInfo.tiling)
			.setInitialLayout(vk::ImageLayout::ePreinitialized)
			.setUsage(createInfo.usage)
			.setSamples(vk::SampleCountFlagBits::e1)
			.setSharingMode(vk::SharingMode::eExclusive);

		MRK_CATCH(mImage = g_graphicsSystemSingleton.device_.logicalDevice_.createImage(imageInfo));

		vk::MemoryRequirements memRequirements = this->getImageMemoryRequirements(mImage);

		this->allocateMemory(memRequirements, createInfo.properties);

		this->bindMemory(mImage);

		if (hasTexture)
		{
			// host visible memory
			mrk::Buffer stagingBuffer(
				imageSize,
				vk::BufferUsageFlagBits::eTransferSrc,
				vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

			void* data;
			stagingBuffer.map(data);
			memcpy(data, pixels, static_cast<size_t>(imageSize));
			stagingBuffer.unmap();

			stbi_image_free(pixels);

			vk::CommandPool commandPool = g_graphicsSystemSingleton.graphicsPool_;
			vk::Queue deviceQueue = g_graphicsSystemSingleton.graphicsQueue_;

			this->transitionLayout(vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eTransferDstOptimal, commandPool, deviceQueue);

			this->copyBuffer(stagingBuffer.buffer_, commandPool, deviceQueue);

			this->transitionLayout(vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, commandPool, deviceQueue);

			stagingBuffer.destroy();
		}

		return mImage;
	}

	vk::ImageView Image::createImageView(vk::Format format, vk::ImageAspectFlags aspectFlags)
	{
		auto viewInfo = vk::ImageViewCreateInfo()
			.setImage(mImage)
			.setViewType(vk::ImageViewType::e2D)
			.setFormat(format)
			.setComponents({})
			.setSubresourceRange({aspectFlags, 0, 1, 0, 1});

		MRK_CATCH(mImageView = g_graphicsSystemSingleton.device_.logicalDevice_.createImageView(viewInfo));

		return mImageView;
	}

	vk::Sampler & Image::createImageSampler()
	{
		if (info_.texturePath != nullptr)
		{
			vk::SamplerCreateInfo samplerInfo = vk::SamplerCreateInfo()
				.setMagFilter(vk::Filter::eLinear)
				.setMinFilter(vk::Filter::eLinear)
				.setAddressModeU(vk::SamplerAddressMode::eRepeat)
				.setAddressModeV(vk::SamplerAddressMode::eRepeat)
				.setAddressModeW(vk::SamplerAddressMode::eRepeat)
				.setAnisotropyEnable(VK_TRUE)
				.setMaxAnisotropy(16)
				.setBorderColor(vk::BorderColor::eIntOpaqueBlack)
				.setUnnormalizedCoordinates(VK_FALSE)
				.setCompareEnable(VK_FALSE)
				.setCompareOp(vk::CompareOp::eAlways)
				.setMipmapMode(vk::SamplerMipmapMode::eLinear)
				.setMipLodBias(0.0f)
				.setMinLod(0.0f)
				.setMaxLod(0.0f);

			MRK_CATCH(mSampler = g_graphicsSystemSingleton.device_.logicalDevice_.createSampler(samplerInfo, nullptr))
		}

		return mSampler;
	}

	void Image::createImageViews(vk::Format format, vk::ImageAspectFlags aspectFlags, std::vector<vk::Image>& images, std::vector<vk::ImageView>& views) 
	{
		vk::Device const& device = g_graphicsSystemSingleton.device_.logicalDevice_;

		auto viewInfo = vk::ImageViewCreateInfo()
			.setViewType(vk::ImageViewType::e2D)
			.setFormat(format)
			.setComponents({})
			.setSubresourceRange({ aspectFlags, 0, 1, 0, 1 });

		int i = 0;

		for (auto const & image : images)
		{
			viewInfo.image = image;

			MRK_CATCH(views[i++] = device.createImageView(viewInfo));
		}
	}

	void Image::destroy()
	{
		vk::Device const& device = g_graphicsSystemSingleton.device_.logicalDevice_;

		// if image sampler was used
		if ((info_.usage & vk::ImageUsageFlagBits::eSampled) == vk::ImageUsageFlagBits::eSampled)
			device.destroySampler(mSampler);

		device.destroyImageView(mImageView);
		device.destroyImage(mImage);
		this->freeMemory();
	}

	/**
	 * \brief 
	 * \param format 
	 * \param oldLayout 
	 * \param newLayout 
	 * \param commandPool 
	 * \param deviceQueue 
	 */
	void Image::transitionLayout(vk::Format const format, vk::ImageLayout const oldLayout, vk::ImageLayout const newLayout, vk::CommandPool const& commandPool, vk::Queue const& deviceQueue) const
	{
		//start buffer
		std::vector<vk::CommandBuffer> commandBuffer;
		this->beginSingleTimeCommands(commandBuffer, commandPool);

		//commands
		vk::ImageAspectFlags aspectMask;

		if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
		{
			aspectMask = vk::ImageAspectFlagBits::eDepth;

			if (format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint)
			{
				aspectMask |= vk::ImageAspectFlagBits::eStencil;
			}
		}
		else
		{
			aspectMask = vk::ImageAspectFlagBits::eColor;
		}

		vk::ImageSubresourceRange subresourceRange = vk::ImageSubresourceRange()
			.setAspectMask(aspectMask)
			.setBaseArrayLayer(0)
			.setBaseMipLevel(0)
			.setLayerCount(1)
			.setLevelCount(1);

		vk::AccessFlags srcAccessMask = {};
		vk::AccessFlags dstAccessMask;

		if (oldLayout == vk::ImageLayout::ePreinitialized && newLayout == vk::ImageLayout::eTransferDstOptimal)
		{
			srcAccessMask = vk::AccessFlagBits::eHostWrite;
			dstAccessMask = vk::AccessFlagBits::eTransferWrite;
		}
		else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
		{
			srcAccessMask = vk::AccessFlagBits::eTransferWrite;
			dstAccessMask = vk::AccessFlagBits::eShaderRead;
		}
		else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
		{
			// cant use 0 so do nothing? srcAccessMask = 0;
			dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
		}
		else
		{
			throw_line("unsupported layout transition!");
		}

		vk::ImageMemoryBarrier barrier = vk::ImageMemoryBarrier()
			.setImage(mImage)
			.setOldLayout(oldLayout)
			.setNewLayout(newLayout)
			.setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
			.setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
			.setSubresourceRange(subresourceRange)
			.setSrcAccessMask(srcAccessMask)
			.setDstAccessMask(dstAccessMask);

		commandBuffer[0].pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTopOfPipe, {}, {}, {}, barrier);

		//end buffer (submit)
		this->endSingleTimeCommands(commandBuffer, commandPool, deviceQueue);
	}

	void Image::copyBuffer(vk::Buffer buffer, vk::CommandPool& commandPool, vk::Queue& deviceQueue) const
	{
		//start buffer
		std::vector<vk::CommandBuffer> commandBuffer;
		this->beginSingleTimeCommands(commandBuffer, commandPool);

		// commands
		auto subresourceRange = vk::ImageSubresourceLayers()
			.setAspectMask(vk::ImageAspectFlagBits::eColor)
			.setBaseArrayLayer(0)
			.setMipLevel(0)
			.setLayerCount(1);

		vk::BufferImageCopy region = vk::BufferImageCopy()
			.setBufferImageHeight(0)
			.setBufferOffset(0)
			.setBufferRowLength(0)
			.setImageExtent({ info_.width, info_.height, 1 })
			.setImageOffset({ 0, 0, 0 })
			.setImageSubresource(subresourceRange);

		commandBuffer[0].copyBufferToImage(buffer, mImage, vk::ImageLayout::eTransferDstOptimal, region);

		//end buffer (submit)
		this->endSingleTimeCommands(commandBuffer, commandPool, deviceQueue);
	}

}
