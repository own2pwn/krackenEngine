#include "MRKResource.h"
#include "MRKVulkanTools.h"
#include "MRKGraphicsSystem.h"

namespace mrk
{
	Resource::Resource(vk::DeviceSize size) : mSize(size)
	{
	}

	Resource::~Resource()
	{
	}

	/**
	 * \brief 
	 * \param memRequirements - requirements to the memory that will be allocated
	 * \param properties      - properties for the memory Ex. Device Local
	 */
	void Resource::allocateMemory(vk::MemoryRequirements const& memRequirements, vk::MemoryPropertyFlags const& properties)
	{

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		MRK_CATCH(mMemory = g_graphicsSystemSingleton.device.logicalDevice_.allocateMemory(allocInfo));

		mSize = memRequirements.size;
	}

	void Resource::allocateMemory(vk::Image const& image, vk::MemoryPropertyFlags const& properties, vk::DeviceMemory & memory) const
	{
		vk::MemoryRequirements memRequirements = this->getImageMemoryRequirements(image);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		MRK_CATCH(memory = g_graphicsSystemSingleton.device.logicalDevice_.allocateMemory(allocInfo));
	}

	/**
	 * \brief 
	 * \param typeFilter 
	 * \param properties 
	 * \return 
	 */
	uint32_t Resource::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
	{
		uint32_t memoryType = static_cast<uint32_t>(-1);

		vk::PhysicalDeviceMemoryProperties memProps = g_graphicsSystemSingleton.device.physicalDevice_.getMemoryProperties();

		for (uint32_t i = 0; memProps.memoryTypeCount; ++i)
		{
			// checking against different types until acceptable type is found
			if ((typeFilter & (1 << i)) && (memProps.memoryTypes[i].propertyFlags & properties) == properties)
			{
				memoryType = i;
				break;
			}
		}

		if (memoryType == -1)
			throw_line("Failed to find memory type!");

		return memoryType;
	}

	void Resource::bindMemory(vk::Buffer const& buffer) const // BUG LOOK HERE? (constant ref)
	{
		g_graphicsSystemSingleton.device.logicalDevice_.bindBufferMemory(buffer, mMemory, 0);
	}

	void Resource::bindMemory(vk::Image const& image, vk::DeviceMemory const* memory) const
	{
		if (memory == nullptr)
			memory = &mMemory;

		g_graphicsSystemSingleton.device.logicalDevice_.bindImageMemory(image, *memory, 0);
	}

	vk::MemoryRequirements Resource::getBufferMemoryRequirements(vk::Buffer const& buffer)
	{
		return g_graphicsSystemSingleton.device.logicalDevice_.getBufferMemoryRequirements(buffer);
	}

	vk::MemoryRequirements Resource::getImageMemoryRequirements(vk::Image const & image)
	{
		return g_graphicsSystemSingleton.device.logicalDevice_.getImageMemoryRequirements(image);
	}

	/**
	 * \brief Sets up commandBuffer to record commands to
	 * \param commandBuffer - command buffer to begin recording to
	 * \param commandPool   - pool to allocate the command buffer from
	 */
	void Resource::beginSingleTimeCommands(std::vector<vk::CommandBuffer> & commandBuffer, vk::CommandPool const& commandPool)
	{
		// start buffer
		auto allocInfo = vk::CommandBufferAllocateInfo()
			.setLevel(vk::CommandBufferLevel::ePrimary)
			.setCommandPool(commandPool)
			.setCommandBufferCount(1);
;
		MRK_CATCH(commandBuffer = g_graphicsSystemSingleton.device.logicalDevice_.allocateCommandBuffers(allocInfo));

		auto beginInfo = vk::CommandBufferBeginInfo()
			.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

		commandBuffer[0].begin(beginInfo);

	}

	/**
	 * \brief Cleans up and submits a command buffer
	 * \param commandBuffer - commandBuffer to submit
	 * \param commandPool   - pool that commandBuffer was allocated from
	 * \param deviceQueue   - queue to submit the command buffer to
	 */
	void Resource::endSingleTimeCommands(std::vector<vk::CommandBuffer>& commandBuffer, vk::CommandPool const& commandPool, vk::Queue const& deviceQueue)
	{
		// end command buffer
		commandBuffer[0].end();


		auto submitInfo = vk::SubmitInfo()
			.setCommandBufferCount(static_cast<uint32_t>(commandBuffer.size()))
			.setPCommandBuffers(commandBuffer.data());

		deviceQueue.submit(submitInfo, {});
		deviceQueue.waitIdle();

		g_graphicsSystemSingleton.device.logicalDevice_.freeCommandBuffers(commandPool, commandBuffer);
	}

	void Resource::freeMemory()
	{
		if (mSize > 0)
		{
			g_graphicsSystemSingleton.device.logicalDevice_.freeMemory(mMemory);

			mSize = 0;
		}
	}

	void Resource::map(void *& data) const
	{
		data = g_graphicsSystemSingleton.device.logicalDevice_.mapMemory(mMemory, 0, mSize, vk::MemoryMapFlags());
	}

	void Resource::unmap() const
	{
		g_graphicsSystemSingleton.device.logicalDevice_.unmapMemory(mMemory);
	}
}
