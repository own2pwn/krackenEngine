/** @file MRKInstance.cpp
*
*  @brief The implementation for the mrk::Instance class
*
*  @author Juan Ramos
*/
#include "MRKInstance.h"
#include "MRKWindowSystem.h"
#include "MRKVulkanTools.h"

namespace mrk
{
Instance::Instance(createInfo const& info) : mInstance(createInstance(info)) 
#ifdef _DEBUG
    , _callback(mInstance)
#endif
{
}

Instance::operator vk::Instance const &() const
{
    return mInstance;
}

Instance::~Instance()
{
}

Instance& Instance::operator=(Instance&& other) noexcept
{
    mInstance = other.mInstance;
    other.mInstance = nullptr;

#ifdef _DEBUG
    this->_callback = std::move(other._callback);
#endif

    return *this;
}

vk::Instance Instance::createInstance(createInfo const & info) const
{
    checkValidationLayers();
    printOutAvailableExtensions();

    auto appInfo = vk::ApplicationInfo()
		.setPApplicationName(info.applicationName)
		.setApplicationVersion(1)
		.setPEngineName(info.engineName)
		.setEngineVersion(1)
		.setApiVersion(VK_API_VERSION_1_0)
		.setPNext(nullptr);

	std::vector<const char*> extensions = info.windowSystem.mRequiredExtensions;
    if (validationLayersAreEnabled())
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    auto instanceCreateInfo = vk::InstanceCreateInfo()
        .setPNext(nullptr)
        .setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
        .setPpEnabledExtensionNames(extensions.data())
        .setPApplicationInfo(&appInfo)
        .setPpEnabledLayerNames(validationLayersAreEnabled() ? validationLayers.data() : nullptr)
        .setEnabledLayerCount(validationLayersAreEnabled() ? static_cast<uint32_t>(validationLayers.size()) : 0);

    vk::Instance instance;
    MRK_CATCH(instance = vk::createInstance(instanceCreateInfo));

    return instance;
}

void Instance::checkValidationLayers()
{
    if (validationLayersAreEnabled() == true && validationLayersAreSupported() == false)
    {
       throw_line("Validation Layers are not supported") 
    }
}

bool Instance::validationLayersAreSupported() 
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    // Go through the required layers needed for validation
    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        // Go though the layers available to us
        for (const auto& layerProperties : availableLayers)
        {
            // If a match is found break and set bool to true
            if (std::strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        // If a match wasn't found then break because the layers needed aren't
        // available
        if (layerFound == false)
        {
            return false;
        }
    }

    return true;
}

void Instance::printOutAvailableExtensions()
{
#ifdef _DEBUG
    std::vector<vk::ExtensionProperties> extensions;
    
    // https://www.khronos.org/registry/vulkan/specs/1.0/man/html/vkEnumerateInstanceExtensionProperties.html
    MRK_CATCH(extensions = vk::enumerateInstanceExtensionProperties())

    std::cout << "All available extensions" << std::endl;
    for (const auto& extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
    }
    std::cout << std::endl;
#endif
}

} // END OF NAMESPACE