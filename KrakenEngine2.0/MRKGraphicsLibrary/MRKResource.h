#pragma once
#include <vulkan/vulkan.hpp>
#include "MRKDevice.h"

namespace mrk
{
	class Resource
	{
	public:
		//data
		vk::DeviceMemory mMemory;
		vk::DeviceSize mSize;
		//functions
		Resource(vk::DeviceSize size = 0);
		~Resource();

		void allocateMemory(vk::MemoryRequirements const& memRequirements, vk::MemoryPropertyFlags const& properties);
		void allocateMemory(vk::Image const& image, vk::MemoryPropertyFlags const& properties, vk::DeviceMemory & memory) const;
		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;
		void bindMemory(vk::Buffer const& buffer) const;
		void bindMemory(vk::Image const& image, vk::DeviceMemory const* memory = nullptr) const;
		static vk::MemoryRequirements getBufferMemoryRequirements(vk::Buffer const& buffer);
		static vk::MemoryRequirements getImageMemoryRequirements(vk::Image const& image);
		static void beginSingleTimeCommands(std::vector<vk::CommandBuffer>& commandBuffer, vk::CommandPool const& commandPool);
		static void endSingleTimeCommands(std::vector<vk::CommandBuffer>& commandBuffer, vk::CommandPool const& commandPool, vk::Queue const& deviceQueue);
		void freeMemory();
		void map(void*& data) const;
		void unmap() const;
	};
}
