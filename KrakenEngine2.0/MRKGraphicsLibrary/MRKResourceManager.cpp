#include "Precompiled.h"
#include "MRKResourceManager.h"
#include "MRKGraphicsSystem.h"
#include "mrk.h"

namespace mrk
{
    ResourceManager::~ResourceManager()
    {
        for (auto & shader_group : shaderGroups)
        {
            for (std::tuple<vk::ShaderModule, vk::ShaderStageFlagBits, const char*> & tuple : shader_group.second)
            {
                g_graphicsSystemSingleton.device.logicalDevice_.destroyShaderModule(std::get<0>(tuple));
            }
        }
    }

    void ResourceManager::load(LoadResourcesCreateInfo const& info)
    {
        houseUniformBuffer_ = mrk::Buffer(sizeof(UniformBufferObject), vk::BufferUsageFlagBits::eUniformBuffer,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

        houseModel_.load(info.modelsToLoad[0].modelPath);

        setUpShaderGroups();

		std::vector<mrk::Vertex> verts;
		std::vector<uint32_t> inds;

		for (unsigned i = 0; i < houseModel_.meshes.size(); ++i)
		{
			std::vector<mrk::Vertex> & vertRef = houseModel_.meshes[i].vertices;
			std::vector<uint32_t> & indexRef = houseModel_.meshes[i].indices;

			verts.insert(verts.end(), vertRef.begin(), vertRef.end());
			inds.insert(inds.end(), indexRef.begin(), indexRef.end());
		}

        houseVertexBuffer_.createVertexBuffer(verts, g_graphicsSystemSingleton.graphicsPool, g_graphicsSystemSingleton.graphicsQueue);
        houseIndexBuffer_.createIndexBuffer(inds, g_graphicsSystemSingleton.graphicsPool, g_graphicsSystemSingleton.graphicsQueue);

        mrk::Image::CreateInfo createInfo = { 
            0, 0, 
            vk::Format::eR8G8B8A8Unorm, 
            vk::ImageTiling::eOptimal, 
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, 
            vk::MemoryPropertyFlagBits::eDeviceLocal, 
            vk::ImageAspectFlagBits::eColor, 
            info.modelsToLoad[0].texturePath[0]};

		size_t textureCount = houseModel_.loadedTextures.size();
		
		houseTextures_.resize(textureCount);

		// the chalet does not have a mtl so assimp will not get its textures
		// this will use the passed in path instead
		if (textureCount == 0)
		{
			createInfo.texturePath = info.houseModelTexturePath;
			houseTextures_.push_back(mrk::Image(createInfo));
		}

		for (size_t i = 0; i < textureCount; ++i)
		{
			createInfo.texturePath = houseModel_.loadedTextures[i].path.C_Str();

			houseTextures_[i] = (mrk::Image(createInfo));
		}

		descriptor_.setup(Descriptor::createInfo{ &houseUniformBuffer_.buffer_, &houseTextures_ });
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

    std::vector<std::tuple<vk::ShaderModule, vk::ShaderStageFlagBits, char const*>> const& ResourceManager::getShadersFromShaderType(ShaderType type)
    {
        auto search = shaderGroups.find(type);
        throw_line_exp(search == shaderGroups.end(), "Asked for a non-established shader group")

        return search->second;
    }

    void ResourceManager::setUpShaderGroups()
    {
        char const * const DEFAULT_VERTEX_SHADER_PATH = "Assets/shaders/vert.spv";
        char const * const DEFAULT_FRAGMENT_SHADER_PATH = "Assets/shaders/frag.spv";
		
        std::vector< std::tuple<vk::ShaderModule, vk::ShaderStageFlagBits, char const *> > defaultShaders;
        defaultShaders.push_back( {loadShaderModule(DEFAULT_VERTEX_SHADER_PATH), vk::ShaderStageFlagBits::eVertex, "main"} );
        defaultShaders.push_back( {loadShaderModule(DEFAULT_FRAGMENT_SHADER_PATH), vk::ShaderStageFlagBits::eFragment, "main"} );

        shaderGroups.emplace(ShaderType::DEFAULT, std::move(defaultShaders));
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

