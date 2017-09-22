#pragma once
#include "MRKWindowSystem.h"
#include "MRKDevice.h"
#include "MRKSwapchain.h"
#include "MRKInstance.h"
#include "MRKResourceManager.h"
#include "MRKPipeline.h"
#include <vulkan/vulkan.hpp>

namespace mrk
{
    struct GraphicsSystemCreateInfo;
    typedef GraphicsSystemCreateInfo CreateInfo;

    class GraphicsSystem
	{
	public:

		explicit GraphicsSystem(CreateInfo const& createInfo); 
        void loadResources();
		void draw();
        static void recreateWindowDependentResources(GLFWwindow * window, int width, int height);
        ~GraphicsSystem();

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
        GraphicsSystem(GraphicsSystem const &) = delete;
        GraphicsSystem& operator=(const GraphicsSystem&) = delete;
        GraphicsSystem(GraphicsSystem &&) = delete;
        GraphicsSystem & operator=(GraphicsSystem &&) = delete;

		// Data
		WindowSystem const windowSystem_;
        mrk::Instance const instance_;
		vk::SurfaceKHR const surface_;

		mrk::Device const device_;

        vk::Queue const graphicsQueue_;
        vk::Queue const presentQueue_;
        vk::CommandPool const graphicsPool_;

		mrk::Swapchain swapchain_;
        mrk::ResourceManager resourceManager_;
        mrk::Pipeline pipeline_;
	};

    extern mrk::GraphicsSystem g_graphicsSystemSingleton;
}

