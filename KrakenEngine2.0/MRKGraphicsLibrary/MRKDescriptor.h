#pragma once
#include "Precompiled.h"

namespace mrk
{

class Descriptor
{
public:
    struct createInfo
    {
        vk::Buffer* uniformBuffer;
        vk::ImageView* textureView;
        vk::Sampler* textureSampler;
    };

    Descriptor() = default;
    explicit Descriptor(createInfo const & info);
    ~Descriptor();

    vk::DescriptorPool mPool;
    vk::DescriptorSetLayout mLayout;
    vk::DescriptorSet mSet;

	void setup(createInfo const & info);

private:
    vk::DescriptorPool createPool();
    vk::DescriptorSetLayout createLayout();
    vk::DescriptorSet createSets(createInfo const & info);
};

}

