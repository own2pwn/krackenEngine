#pragma once
#include <vulkan/vulkan.hpp>
#include "MRKResource.h"
#include "MRKModel.h"

namespace mrk
{
	class Buffer : public Resource
	{
	public:
		//data
		vk::Buffer buffer_;

		//functions
		Buffer();
		Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties);
		~Buffer();
		void destroy();
		void createVertexBuffer(std::vector<Vertex> const & vertices, vk::CommandPool commandPool, vk::Queue deviceQueue);
		void createIndexBuffer(std::vector<uint32_t> const & indices, vk::CommandPool commandPool, vk::Queue deviceQueue);
		void copyBuffer(mrk::Buffer & source, vk::DeviceSize size, vk::CommandPool commandPool, vk::Queue deviceQueue);
	};
}
