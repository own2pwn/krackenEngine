/** @file MRKDebugCallback.h
*
*  @brief
*   Interface for mrk::DebugCallback
*
*  @author Juan Ramos
*/
#pragma once
#ifdef _DEBUG

#include <vulkan/vulkan.hpp>

namespace mrk
{
    class DebugCallback
    {
    public:
        explicit DebugCallback(vk::Instance instance);
        DebugCallback() = default;

        ~DebugCallback();
        DebugCallback &operator=(DebugCallback &&other);
    private:
        vk::DebugReportCallbackEXT createCallback() const;

        vk::Instance _instance;
        vk::DebugReportCallbackEXT _callback;
    };
}

#endif