#pragma once
#include "MRKWindowSystem.h"
#include "MRKDevice.h"
#include "MRKSwapchain.h"
#include "MRKInstance.h"
#include "MRKResourceManager.h"
#include "MRKPipeline.h"
#include "Precompiled.h"

namespace mrk
{
    struct GraphicsSystemCreateInfo;
    typedef GraphicsSystemCreateInfo CreateInfo;

    class GraphicsSystem
	{
    public:
        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
        GraphicsSystem(GraphicsSystem const &) = delete;
        GraphicsSystem& operator=(GraphicsSystem const &) = delete;
        GraphicsSystem(GraphicsSystem &&) = delete;
        GraphicsSystem& operator=(GraphicsSystem &&) = delete;

        // Needed for singleton
        GraphicsSystem() = default;

        ~GraphicsSystem()
        {
            pipeline.cleanUp();
            device.logicalDevice_.destroyCommandPool(graphicsPool);
        }

        // Order of members is crucial for destruction
		WindowSystem windowSystem;
        mrk::Instance instance;
		vk::SurfaceKHR surface;

		mrk::Device device;

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;
        vk::CommandPool graphicsPool;

		mrk::Swapchain swapChain;
        mrk::ResourceManager resourceManager;
        mrk::Pipeline pipeline;
	};

    extern GraphicsSystem g_graphicsSystemSingleton;
}

