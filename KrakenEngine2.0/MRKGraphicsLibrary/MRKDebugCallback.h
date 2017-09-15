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
        explicit DebugCallback(vk::Instance const & instance);
        ~DebugCallback();
    private:
        vk::DebugReportCallbackEXT createCallback() const;

        vk::Instance const & _instance;
        vk::DebugReportCallbackEXT const _callback;
    };
}

#endif