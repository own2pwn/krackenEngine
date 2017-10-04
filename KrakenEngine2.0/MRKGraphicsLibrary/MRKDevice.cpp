/** @file MRKDevice.cpp
*
*  @brief mrk::Device implementation
*
*  @author Juan Ramos
*/
#include "MRKDevice.h"
#include "MRKVulkanTools.h"
#include "MRKDebugCallback.h"

#include <set>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include "MRKGraphicsSystem.h"

namespace
{
    constexpr std::array<const char *, 1> mrkDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME } ;
}

namespace mrk
{
	Device::Device(createInfo const& info) : 
      physicalDevice_(pickPhysicalDevice(info)),
      queueFamilyIndices_(QueueFamilyIndices::findQueueFamilies(physicalDevice_, info.surface)),
      physicalDeviceProperties_(physicalDevice_.getProperties()),
      physicalDeviceMemoryProperties_(physicalDevice_.getMemoryProperties()),
	  physicalDeviceFeatures_(physicalDevice_.getFeatures()),
      logicalDevice_(getLogicalDevice())
	{
	}

    Device& Device::operator=(Device&& other) noexcept
    {
        physicalDevice_ = other.physicalDevice_;
        queueFamilyIndices_ = other.queueFamilyIndices_;
        physicalDeviceProperties_ = other.physicalDeviceProperties_;
        physicalDeviceMemoryProperties_ = other.physicalDeviceMemoryProperties_;
        physicalDeviceFeatures_ = other.physicalDeviceFeatures_;
        logicalDevice_ = other.logicalDevice_;

        other.physicalDevice_ = nullptr;
        other.logicalDevice_ = nullptr;

        return *this;
    }

    Device::~Device()
	{
		logicalDevice_.destroy();
	}

    // JUAN THIS CODE NEEDS ATTENTION
    vk::Format Device::findDepthFormat() const
    {
        return findSupportedFormat({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment);
    }

    // JUAN THIS CODE NEEDS ATTENTION
    vk::Format Device::findSupportedFormat(std::vector<vk::Format> const &canidates,
        vk::ImageTiling const &tiling, vk::FormatFeatureFlagBits const & formatFeature) const
    {
        for (auto const &format : canidates)
        {
            /*
            * The VkFormatProperties struct contains three fields:
            *  linearTilingFeatures: Use cases that are supported with linear tiling
            *  optimalTilingFeatures: Use cases that are supported with optimal tiling
            *  bufferFeatures: Use cases that are supported for buffers
            */
            vk::FormatProperties props = physicalDevice_.getFormatProperties(format);

            if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & formatFeature) == formatFeature)
            {
                return format;
            }
            else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & formatFeature) == formatFeature)
            {
                return format;
            }
        }

        throw_line("Failed to find supported format")
    }

    /**
     * \brief 
     *  Picks the best physical device for the system
     * \param info 
     *  The info needed is the instance and surface for creating and choosing
     *  the physical device respectively
     * \return
     *  Returns the best physical device 
     */
    vk::PhysicalDevice Device::pickPhysicalDevice(createInfo const& info)
    {
        vk::PhysicalDevice physicalDevice;

        std::vector<vk::PhysicalDevice> physicalDevices = info.instance.enumeratePhysicalDevices();
        if (physicalDevices.empty())
        {
            throw_line("No physical devices that support Vulkan to choose from")
        }

#ifdef _DEBUG
        // Eventually these two variables will be used decide on which device to use

        // Get the devices properties
        // EX: Name, type, supported Vulkan version
        vk::PhysicalDeviceProperties deviceProperties;

        // Support for optional features like texture compression, 64 bit floats, and
        // multiviewport rendering (Useful for VR)
        vk::PhysicalDeviceFeatures deviceFeatures;

        std::cout << "Displaying each physical device's (that supports Vulkan) properties and features" << std::string(2, '\n');
#endif

        // Extension that we need the device to support
        auto constexpr deviceExtensions = mrkDeviceExtensions;
        vk::SurfaceKHR const & surface = info.surface;

        for (auto const & curPhysicalDevice : physicalDevices)
        {
#ifdef _DEBUG
            curPhysicalDevice.getProperties(&deviceProperties);
            curPhysicalDevice.getFeatures(&deviceFeatures);

            std::cout << "Device Name: " << deviceProperties.deviceName << std::endl;
            std::cout << "\t" << "Support for Gemoetry Shader: " << std::boolalpha << static_cast<bool>(deviceFeatures.geometryShader) << std::endl;
            std::cout << "\t" << "Support for Multi-View-Port Rendering: " << std::boolalpha << static_cast<bool>(deviceFeatures.multiViewport) << std::endl;
            std::cout << std::noboolalpha << std::endl;
#endif
            // Checking for if graphics the device has a queue families that support graphics and presentation
            QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(curPhysicalDevice, surface);
            if (QueueFamilyIndices::isComplete(indices.graphicsFamilyIndex, indices.presentFamilyIndex) == false) { continue; }

            // Checking for device extension support
            std::set<std::string> requiredExtension(deviceExtensions.begin(), deviceExtensions.end());

            for (auto const & extension : curPhysicalDevice.enumerateDeviceExtensionProperties())
            {
                requiredExtension.erase(extension.extensionName);
            }

            if (requiredExtension.empty() == false) { continue; }

            // Checking for swap chain support
            // Making sure there are available formats and presentation modes
            if (curPhysicalDevice.getSurfaceFormatsKHR(surface).empty() && curPhysicalDevice.getSurfacePresentModesKHR(surface).empty())
            {
                continue;
            }

            physicalDevice = curPhysicalDevice;
        }

		if (physicalDevice == vk::PhysicalDevice{/*VK_NULL_HANDLE*/}) // couldn't convert int to physical device
        {
            throw_line("Failed to find a suitable GPU")
        }

        return physicalDevice;
    }

    /**
     * \brief
     *  Creates the logical device while setting up for queue creation
     * \return
     *  Returns the created logical device 
     */
    vk::Device Device::getLogicalDevice() const
    {
		std::set<uint32_t> uniqueQueueFamilies = { queueFamilyIndices_.graphicsFamilyIndex, queueFamilyIndices_.presentFamilyIndex}; 
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

		float queuePriority = 1.0f;
		for (auto const queueFamily : uniqueQueueFamilies)
		{
			auto queueCreateInfo = vk::DeviceQueueCreateInfo()
				.setQueueFamilyIndex(queueFamily)
                // The current available drivers will only allow you to create a low number of queues for each family 
                // queue. But you don't need more than 1. Because you can create all of the command buffers on multiple threads
                // and then submit them all at once on the main thread with a single low-overhead call.
				.setQueueCount(1)
                // Vulkan allows assignment of priorities to queues to influence the scheduling of the command buffer
                // execution using floating point number between 0.0f - 1.0
				.setPQueuePriorities(&queuePriority);

			queueCreateInfos.push_back(queueCreateInfo);
		}

        auto constexpr deviceExtensions = mrkDeviceExtensions;

        vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo()
            .setQueueCreateInfoCount(static_cast<uint32_t>(queueCreateInfos.size()))
            .setPQueueCreateInfos(queueCreateInfos.data())
            .setEnabledExtensionCount(static_cast<uint32_t>(deviceExtensions.size()))
            .setPpEnabledExtensionNames(deviceExtensions.data())
            .setPEnabledFeatures(&physicalDeviceFeatures_);

        vk::Device device;
		MRK_CATCH(device = physicalDevice_.createDevice(createInfo, nullptr));

        return device;
    }

    /*
    * Note that it's very likely that these end up being the same queue family after
    * all, but throughout the program we will treat them as if they were separate
    * queues for a uniform approach. Nevertheless, you could add logic to explicitly
    * prefer a physical device that supports drawing and presentation in the same
    * queue for improved performance.
    */
    Device::QueueFamilyIndices Device::QueueFamilyIndices::findQueueFamilies(vk::PhysicalDevice const& physicalDevice, vk::SurfaceKHR const& surface)
    {
        uint32_t graphicsFamilyIndex = NOT_FOUND;
        uint32_t presentFamilyIndex = NOT_FOUND;

        // VkQueueFamilyProperties
        // Contains details about that queue family,
        // including the type of operations that are supported and the number of queues
        // that can be created based on that family. We need to find at least one queue family
        // that supports VK_QUEUE_GRAPHICS_BIT
        std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueCount > 0)
            {
                if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
                {
                    graphicsFamilyIndex = i;
                }

                vk::Bool32 presentSupport = VK_FALSE;
                MRK_CATCH(presentSupport = physicalDevice.getSurfaceSupportKHR(i, surface))

                if (presentSupport == VK_TRUE)
                {
                    presentFamilyIndex = i;
                }
            }

            if (isComplete(graphicsFamilyIndex, presentFamilyIndex))
            {
                break;
            }

            ++i;
        }

        return QueueFamilyIndices(graphicsFamilyIndex, presentFamilyIndex);
    }

    /**
     * \brief 
     *  Checks to see if the queue family indices struct has valid member
     * \param graphicsFamilyIndex
     *  Param says all 
     * \param presentFamilyIndex 
     *  Param says all 
     * \return
     *  Returns true if the indices are valid 
     */
    bool Device::QueueFamilyIndices::isComplete(uint32_t graphicsFamilyIndex, uint32_t presentFamilyIndex)
    {
        return graphicsFamilyIndex != NOT_FOUND && presentFamilyIndex != NOT_FOUND;
    }

    vk::CommandPool Device::createCommandPool(uint32_t familyIndex) const
    {
        auto poolInfo = vk::CommandPoolCreateInfo()
            .setQueueFamilyIndex(familyIndex);

        vk::CommandPool pool;
        MRK_CATCH(pool = logicalDevice_.createCommandPool(poolInfo))

        return pool;
    }
}
