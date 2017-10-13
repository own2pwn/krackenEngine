#pragma once
#include "MRKResource.h"
#include "Precompiled.h"

namespace mrk
{
	class Image : public Resource
	{
	public:
		struct CreateInfo
		{
			CreateInfo() = default;
			uint32_t width;
			uint32_t height;
			vk::Format format;
			vk::ImageTiling tiling;
			vk::ImageUsageFlags usage;
			vk::MemoryPropertyFlags properties;
			vk::ImageAspectFlags aspectFlags;
			const char* texturePath = nullptr;
		};

		mrk::Image::CreateInfo info_;

		vk::Image mImage;
		vk::ImageView mImageView;

		// texture specific information
		vk::Sampler mSampler;

		explicit Image(mrk::Image::CreateInfo const & createInfo);
		Image(mrk::Image && other) noexcept;

		Image() : info_(),
            mImage(),
            mImageView(),
            mSampler()
        {}

        Image & operator=(Image && other) noexcept;
		~Image();;
		vk::Image & Image::createImage(mrk::Image::CreateInfo const & createInfo);
		vk::ImageView createImageView(vk::Format format, vk::ImageAspectFlags aspectFlags);
		vk::Sampler & createImageSampler();
		static void createImageViews(vk::Format format, vk::ImageAspectFlags aspectFlags, std::vector<vk::Image>& images, std::vector<vk::ImageView>& views);
		void destroy();
		void transitionLayout(vk::Format const format, vk::ImageLayout const oldLayout, vk::ImageLayout const newLayout, vk::CommandPool const& commandPool, vk::Queue const& deviceQueue) const;
		void copyBuffer(vk::Buffer buffer, vk::CommandPool &commandPool, vk::Queue & deviceQueue) const;
	};
}
