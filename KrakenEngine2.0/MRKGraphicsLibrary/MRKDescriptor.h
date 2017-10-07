#pragma once
#include <vulkan/vulkan.hpp>

namespace mrk
{
	class Image;

class Descriptor
{
public:
    struct createInfo
    {
        vk::Buffer* uniformBuffer;
		std::vector<mrk::Image>* textures;
    };

    Descriptor() = default;
    explicit Descriptor(createInfo const & info);
    ~Descriptor();

    vk::DescriptorPool mPool;
    vk::DescriptorSetLayout mLayout;
    vk::DescriptorSet mSet;

	void setup(createInfo const & info);

private:
    vk::DescriptorPool createPool(createInfo const& info);
    vk::DescriptorSetLayout createLayout(createInfo const& info);
    vk::DescriptorSet createSets(createInfo const & info);
};

}

