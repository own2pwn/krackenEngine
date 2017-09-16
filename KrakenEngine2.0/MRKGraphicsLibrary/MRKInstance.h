/** @file MRKInstance.h
*
*  @brief mrk::Instance interface
*
*  @author Juan Ramos 
*/
#pragma once
#include <vulkan/vulkan.hpp>
#include <array>
#include "MRKVulkanTools.h"

#ifdef _DEBUG
#include "MRKDebugCallback.h"
#endif

namespace mrk
{
class WindowSystem;

class Instance
{
public:
    struct createInfo
    {
        createInfo(char const* application_name, char const* engine_name, mrk::WindowSystem const& window_system)
            : applicationName(application_name),
              engineName(engine_name),
              windowSystem(window_system)
        {
        }

        char const *applicationName; 
        char const *engineName; 
        mrk::WindowSystem const &windowSystem;
    };

    explicit Instance(createInfo const & info);
    explicit operator vk::Instance const &() const;

    vk::Instance const mInstance;
private:
#ifdef _DEBUG
    mrk::DebugCallback const _callback;
#endif

    /**
     * \brief 
     * Order matters for the elements in the array
     * https://vulkan.lunarg.com/doc/view/1.0.54.0/windows/LoaderAndLayerInterface.html#user-content-application-layer-usage
     */
    static std::array<char const *, 1> constexpr validationLayers = { "VK_LAYER_LUNARG_standard_validation" };
    static bool constexpr validationLayersAreEnabled()
    {
        bool enable = false;

    #ifdef _DEBUG
        enable = true;
    #endif

        return enable;
    }

    vk::Instance createInstance(createInfo const & info) const;
    static void checkValidationLayers();
    static bool validationLayersAreSupported();
    static void printOutAvailableExtensions();
};

} // END OF NAMESPACE