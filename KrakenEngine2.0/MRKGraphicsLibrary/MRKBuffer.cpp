#include "MRKBuffer.h"
#include "MRKVulkanTools.h"
#include "MRKGraphicsSystem.h"

namespace mrk
{
	Buffer::Buffer()
	{
	}

	/**
	 * \brief 
	 *		Creates a buffer, allocates memory for it, and binds it to that memory.
	 * \param size		 - size of the buffer
	 * \param usage		 - flags for buffer usage
	 * \param properties - memory properties for the buffer
	 */
	Buffer::Buffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties) : Resource(size)
	{
		vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
			.setUsage(usage)
			.setSize(size)
			.setSharingMode(vk::SharingMode::eExclusive);

		// device_ is coming from the resource
		MRK_CATCH(buffer_ = g_graphicsSystemSingleton.device.logicalDevice_.createBuffer(createInfo));

		vk::MemoryRequirements memReqs = this->getBufferMemoryRequirements(buffer_);
		this->allocateMemory(memReqs, properties);
		this->bindMemory(buffer_);
	}

	Buffer::~Buffer()
	{
		if (mSize > 0)
			destroy();
	}

	/**
	 * \brief destroy the buffer and free its memory
	 */
	void Buffer::destroy()
	{
		g_graphicsSystemSingleton.device.logicalDevice_.destroyBuffer(buffer_);
		this->freeMemory();
	}

	/**
	 * \brief Convert buffer into a vertex buffer by filling it with vertex data.
	 * \param vertices - vertex data to put into the buffer
	 */
	void Buffer::createVertexBuffer(std::vector<Model::Vertex> const & vertices, vk::CommandPool commandPool, vk::Queue deviceQueue)
	{
		vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		// staging buffer in "slow" hot visible/coherent memory
		mrk::Buffer stagingBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		void* data;
		stagingBuffer.map(data); //map the staging buffer memory

		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize)); // fill memory with vertex data

		stagingBuffer.unmap();

		if (mSize <= 0) // duplicate
		{
			vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
				.setUsage(vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst)
				.setSize(bufferSize)
				.setSharingMode(vk::SharingMode::eExclusive);

			// device_ is coming from the resource
			MRK_CATCH(buffer_ = g_graphicsSystemSingleton.device.logicalDevice_.createBuffer(createInfo));

			vk::MemoryRequirements memReqs = this->getBufferMemoryRequirements(buffer_);
			this->allocateMemory(memReqs, vk::MemoryPropertyFlagBits::eDeviceLocal);
			this->bindMemory(buffer_);
		}

		// copy into the buffer with "fast" device local memory
		copyBuffer(stagingBuffer, bufferSize, commandPool, deviceQueue);
	}

	/**
	 * \brief copy in a buffer
	 * \param source	  - buffer to copy
	 * \param size		  - size of the buffer to copy
	 * \param commandPool - command pool to get commandBuffer from to perform copy operation
	 * \param deviceQueue - device queue to submit commands to
	 */
	void Buffer::copyBuffer(mrk::Buffer & source, vk::DeviceSize size, vk::CommandPool commandPool, vk::Queue deviceQueue)
	{
		if (size > mSize)
			throw_line("Buffer is to big to copy!");

		// start buffer
		std::vector<vk::CommandBuffer> commandBuffer;
		this->beginSingleTimeCommands(commandBuffer, commandPool);
		
		// copy command
		vk::BufferCopy copyRegion = vk::BufferCopy()
			.setSize(size);

		commandBuffer[0].copyBuffer(source.buffer_, buffer_, copyRegion);

		// end command buffer
		this->endSingleTimeCommands(commandBuffer, commandPool, deviceQueue);
	}

	/**
	 * \brief Convert buffer into a index buffer by filling it with index data.
	 * \param indices - index data to fill buffer with
	 */
	void Buffer::createIndexBuffer(std::vector<uint32_t> const & indices, vk::CommandPool commandPool, vk::Queue deviceQueue)
	{
		vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		mrk::Buffer stagingBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		void* data;
		stagingBuffer.map(data);

		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));

		stagingBuffer.unmap();

		if (mSize <= 0) // duplicate
		{
			vk::BufferCreateInfo createInfo = vk::BufferCreateInfo()
				.setUsage(vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst)
				.setSize(bufferSize)
				.setSharingMode(vk::SharingMode::eExclusive);

			// device_ is coming from the resource
			MRK_CATCH(buffer_ = g_graphicsSystemSingleton.device.logicalDevice_.createBuffer(createInfo));

			vk::MemoryRequirements memReqs = this->getBufferMemoryRequirements(buffer_);
			this->allocateMemory(memReqs, vk::MemoryPropertyFlagBits::eDeviceLocal);
			this->bindMemory(buffer_);
		}

		copyBuffer(stagingBuffer, bufferSize, commandPool, deviceQueue);
	}
}
