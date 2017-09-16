/** @file MRKDebugCallback.cpp
*
*  @brief
*   Implementation for mrk::DebugCallback
*
*  @author Juan Ramos
*/
#include "MRKGraphicsSystem.h"
#ifdef _DEBUG

#include "vulkan/vulkan.h"
#include "MRKDebugCallback.h"
#include "MRKVulkanTools.h"

// This code is here to be defined for vulkan.h so it is easier to use for vulkan.hpp functionality
#ifdef VK_EXT_debug_report
static PFN_vkCreateDebugReportCallbackEXT pfn_vkCreateDebugReportCallbackEXT = nullptr;
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(
    VkInstance                                  instance,
    const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDebugReportCallbackEXT*                   pCallback)
{
    return pfn_vkCreateDebugReportCallbackEXT(
        instance,
        pCreateInfo,
        pAllocator,
        pCallback
    );
}

static PFN_vkDestroyDebugReportCallbackEXT pfn_vkDestroyDebugReportCallbackEXT = nullptr;
VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(
    VkInstance                                  instance,
    VkDebugReportCallbackEXT                    callback,
    const VkAllocationCallbacks*                pAllocator)
{
    pfn_vkDestroyDebugReportCallbackEXT(
        instance,
        callback,
        pAllocator
    );
}
#endif /* VK_EXT_debug_report */

namespace mrk
{
VKAPI_ATTR VkBool32 VKAPI_CALL messageCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t srcObject, size_t location, int32_t msgCode, const char* pLayerPrefix, const char* pMsg, void* pUserData);

DebugCallback::DebugCallback(vk::Instance const & instance) : _instance(instance), _callback(createCallback()) {}

DebugCallback::~DebugCallback()
{
    g_graphicsSystemSingleton.instance_.mInstance.destroyDebugReportCallbackEXT(_callback);
}

vk::DebugReportCallbackEXT DebugCallback::createCallback() const
{
#ifdef VK_EXT_debug_report
    pfn_vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(_instance, "vkCreateDebugReportCallbackEXT"));
    pfn_vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(_instance, "vkDestroyDebugReportCallbackEXT"));
#endif /* VK_EXT_debug_report */

    if (pfn_vkCreateDebugReportCallbackEXT == nullptr || pfn_vkDestroyDebugReportCallbackEXT == nullptr)
    {
        throw_line("Not able to get instance proc address for either Create or Destroy debug callback")
    }

    vk::DebugReportCallbackEXT callback;

    auto createInfo = vk::DebugReportCallbackCreateInfoEXT()
        .setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning)
        .setPfnCallback(messageCallback);

    MRK_CATCH(callback = _instance.createDebugReportCallbackEXT(createInfo))

    return callback;
}

VKAPI_ATTR VkBool32 VKAPI_CALL messageCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObject,
	size_t location,
	int32_t msgCode,
	const char* pLayerPrefix,
	const char* pMsg,
	void* pUserData)
{
    (void)objType;
    (void)srcObject;
    (void)location;
    (void)pUserData;

	// Select prefix depending on flags passed to the callback
	// Note that multiple flags may be set for a single validation message
	std::string prefix("");

	// Error that may result in undefined behavior
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		prefix += "ERROR:";
	};
	// Warnings may hint at unexpected / non-spec API usage
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		prefix += "WARNING:";
	};
	// May indicate sub-optimal usage of the API
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		prefix += "PERFORMANCE:";
	};
	// Informal messages that may become handy during debugging
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		prefix += "INFO:";
	}
	// Diagnostic info from the Vulkan loader and layers
	// Usually not helpful in terms of API usage, but may help to debug layer and loader problems 
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		prefix += "DEBUG:";
	}

	// Display message to default output (console/logcat)
	std::stringstream debugMessage;
	debugMessage << prefix << " [" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;

	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		std::cerr << debugMessage.str() << "\n" << std::flush;
	}
	else {
		std::cout << debugMessage.str() << "\n" << std::flush;
	}

	// The return value of this callback controls whether the Vulkan call that caused
	// the validation message will be aborted or not
	// We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message 
	// (and return a VkResult) to abort
	// If you instead want to have calls abort, pass in VK_TRUE and the function will 
	// return VK_ERROR_VALIDATION_FAILED_EXT 
	return VK_FALSE;
}

} // END OF NAMESPACE MRK

#endif