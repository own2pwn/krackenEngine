#include "MRKDescriptor.h"
#include "MRKVulkanTools.h"
#include "MRKGraphicsSystem.h"
#include "Precompiled.h"

namespace mrk
{
Descriptor::Descriptor(createInfo const & info) :
    mPool(createPool(info)),
    mLayout(createLayout(info)),
    mSet(createSets(info))
{
}

void Descriptor::setup(createInfo const& info)
{
	mPool = createPool(info);
	mLayout = createLayout(info);
	mSet = createSets(info);
}

vk::DescriptorPool Descriptor::createPool(createInfo const& info)
{
    // Create descriptors pools for uniform buffer and sampler
    vk::DescriptorPoolSize uniformPool(vk::DescriptorType::eUniformBuffer, 1);
	// as many image sampler as there are images on the model
    vk::DescriptorPoolSize samplerPool(vk::DescriptorType::eCombinedImageSampler, static_cast<uint32_t>(info.textures->size()));

    std::array<vk::DescriptorPoolSize, 2> poolSizes = { uniformPool, samplerPool };

    auto poolInfo = vk::DescriptorPoolCreateInfo()
        .setPoolSizeCount(static_cast<uint32_t>(poolSizes.size()))
        .setPPoolSizes(poolSizes.data())
        .setMaxSets(1);

    MRK_CATCH(mPool = g_graphicsSystemSingleton.device.logicalDevice_.createDescriptorPool(poolInfo))
    return mPool;
}

vk::DescriptorSetLayout Descriptor::createLayout(createInfo const& info)
{
    // Create descriptor set layouts for uniform buffer and sampling
    auto uboLayoutBinding = vk::DescriptorSetLayoutBinding()
        .setBinding(0)
        .setDescriptorCount(1)
        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
        .setStageFlags(vk::ShaderStageFlagBits::eVertex);

    auto samplerLayoutBinding = vk::DescriptorSetLayoutBinding()
        .setBinding(1)
        .setDescriptorCount(static_cast<uint32_t>(info.textures->size()))
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setStageFlags(vk::ShaderStageFlagBits::eFragment);

    std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

    auto layoutInfo = vk::DescriptorSetLayoutCreateInfo()
        .setBindingCount(static_cast<uint32_t>(bindings.size()))
        .setPBindings(bindings.data());

    MRK_CATCH(mLayout = g_graphicsSystemSingleton.device.logicalDevice_.createDescriptorSetLayout(layoutInfo))

    return mLayout;
}

vk::DescriptorSet Descriptor::createSets(createInfo const & info)
{
    vk::DescriptorSetLayout layouts[] = { mLayout };

    auto allocInfo = vk::DescriptorSetAllocateInfo()
        .setDescriptorPool(mPool)
        .setDescriptorSetCount(1)
        .setPSetLayouts(layouts);

    std::vector<vk::DescriptorSet> sets;
    MRK_CATCH(sets = g_graphicsSystemSingleton.device.logicalDevice_.allocateDescriptorSets(allocInfo))
    vk::DescriptorSet set = sets[0];

    auto bufferInfo = vk::DescriptorBufferInfo()
        .setBuffer(*info.uniformBuffer)
        .setOffset(0)
        .setRange(sizeof(UniformBufferObject));

	std::vector<vk::DescriptorImageInfo> imageInfo;
	imageInfo.resize(info.textures->size());

	for (size_t i = 0; i < info.textures->size(); ++i)
	{
		imageInfo[i].setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
		imageInfo[i].setImageView((*info.textures)[i].mImageView);
		imageInfo[i].setSampler((*info.textures)[i].mSampler);
	}

    std::array<vk::WriteDescriptorSet, 2> descriptorWrites = {};
        descriptorWrites[0].dstSet = set;
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;
        descriptorWrites[0].pImageInfo = nullptr;
        descriptorWrites[0].pTexelBufferView = nullptr;
    
        descriptorWrites[1].dstSet = set;
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
        descriptorWrites[1].descriptorCount = static_cast<uint32_t>(info.textures->size());
        descriptorWrites[1].pBufferInfo = nullptr;
        descriptorWrites[1].pImageInfo = imageInfo.data();
        descriptorWrites[1].pTexelBufferView = nullptr;

    g_graphicsSystemSingleton.device.logicalDevice_.updateDescriptorSets(descriptorWrites, nullptr);
	mSet = set;
    return set;
}

Descriptor::~Descriptor()
{
    g_graphicsSystemSingleton.device.logicalDevice_.destroyDescriptorPool(mPool);
    g_graphicsSystemSingleton.device.logicalDevice_.destroyDescriptorSetLayout(mLayout);
}

}
