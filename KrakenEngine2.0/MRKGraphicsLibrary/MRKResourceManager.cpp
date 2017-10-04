#include "MRKResourceManager.h"
#include "MRKGraphicsSystem.h"
#include <fstream>

namespace mrk
{
    ResourceManager::~ResourceManager()
    {
        g_graphicsSystemSingleton.device.logicalDevice_.destroyShaderModule(vertexShader_);
        g_graphicsSystemSingleton.device.logicalDevice_.destroyShaderModule(fragmentShader_);
    }

    void ResourceManager::load(loadInfo const& info)
    {
        houseUniformBuffer_ = mrk::Buffer(sizeof(UniformBufferObject), vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        houseModel_.load(info.houseModelPath);

        vertexShader_ = loadShaderModule(info.vertexShaderPath);
        fragmentShader_ = loadShaderModule(info.fragmentShaderPath);

		std::vector<mrk::Vertex> verts;
		std::vector<uint32_t> inds;

		for (unsigned i = 0; i < houseModel_.meshes.size(); ++i)
		{
			std::vector<mrk::Vertex> & vertRef = houseModel_.meshes[i].vertices;
			std::vector<uint32_t> & indexRef = houseModel_.meshes[i].indices;

			verts.insert(verts.end(), vertRef.begin(), vertRef.end());
			inds.insert(inds.end(), indexRef.begin(), indexRef.end());
		}

        houseVertexBuffer_.createVertexBuffer(verts, g_graphicsSystemSingleton.graphicsPool_, g_graphicsSystemSingleton.graphicsQueue_);
        houseIndexBuffer_.createIndexBuffer(inds, g_graphicsSystemSingleton.graphicsPool_, g_graphicsSystemSingleton.graphicsQueue_);

        mrk::Image::CreateInfo createInfo = { 
            0, 0, 
            vk::Format::eR8G8B8A8Unorm, 
            vk::ImageTiling::eOptimal, 
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, 
            vk::MemoryPropertyFlagBits::eDeviceLocal, 
            vk::ImageAspectFlagBits::eColor, 
            info.houseModelTexturePath};

		houseTexture_.info_ = createInfo;
		houseTexture_.createImage(createInfo);
		houseTexture_.createImageView(createInfo.format, createInfo.aspectFlags);
		houseTexture_.createImageSampler();

		descriptor_.setup(Descriptor::createInfo{ &houseUniformBuffer_.buffer_, &houseTexture_.mImageView, &houseTexture_.mSampler });
    }

    vk::ShaderModule const& ResourceManager::getVertexShader() const
    {
        return vertexShader_;
    }

    vk::ShaderModule const& ResourceManager::getFragmentShader() const
    {
        return fragmentShader_;
    }

    mrk::Descriptor const& ResourceManager::getDescriptor() const
    {
        return descriptor_;
    }

    mrk::Buffer const& ResourceManager::getUniformBuffer() const
    {
        return houseUniformBuffer_;
    }

	mrk::Buffer const& ResourceManager::getVertexBuffer() const
	{
		return houseVertexBuffer_;
	}

	mrk::Buffer const& ResourceManager::getIndexBuffer() const
	{
		return houseIndexBuffer_;
	}

	vk::ShaderModule ResourceManager::loadShaderModule(char const* const shaderPath)
    {
        // ate: Start reading at the end of the file
        // binary: Read the file as a binary
        std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw_line("Failed to open shader file")
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> spirVCode(fileSize);

        file.seekg(0);
        file.read(spirVCode.data(), fileSize);

        file.close();

        // Now create the module with the given info
        auto createInfo = vk::ShaderModuleCreateInfo()
            .setCodeSize(spirVCode.size())
            .setPCode(reinterpret_cast<uint32_t const *>(spirVCode.data()));

        vk::ShaderModule shaderModule;

        MRK_CATCH(shaderModule = g_graphicsSystemSingleton.device.logicalDevice_.createShaderModule(createInfo))

        return shaderModule;
    }

}
