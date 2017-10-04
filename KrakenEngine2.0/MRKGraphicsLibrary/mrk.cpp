/** @file mrk.cpp
*
*  @brief Client interface implementation
*
*  @author Juan Ramos 
*/
#include "mrk.h"
#include "MRKGraphicsSystem.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

namespace mrk
{

    GraphicsSystem g_graphicsSystemSingleton;

    void initializeGraphicsSystem(GraphicsSystemCreateInfo  const & createInfo)
    {
        // Makes code easier to read
        auto & windowSystem = g_graphicsSystemSingleton.windowSystem;
        auto & instance = g_graphicsSystemSingleton.instance;
        auto & surface = g_graphicsSystemSingleton.surface;
        auto & device = g_graphicsSystemSingleton.device;
        auto & graphicsQueue = g_graphicsSystemSingleton.graphicsQueue;
        auto & presentQueue = g_graphicsSystemSingleton.presentQueue;
        auto & graphicsPool = g_graphicsSystemSingleton.graphicsPool;
        auto & swapChain = g_graphicsSystemSingleton.swapChain;

        // As of now the default constructor does nothing for the resource manager or pipeline
        //auto & resourceManager = g_graphicsSystemSingleton.resourceManager;
        // auto & pipeline = g_graphicsSystemSingleton.pipeline;

        // Initialization
        WindowSystem::init();
        windowSystem = WindowSystem(createInfo.winCreateInfo);
        instance = Instance(
            mrk::Instance::createInfo{ createInfo.applicationName, createInfo.engineName, windowSystem });
        surface = vk::SurfaceKHR(
            windowSystem.createSurface(static_cast<vk::Instance>(instance)));
        device = mrk::Device(mrk::Device::createInfo(static_cast<vk::Instance>(instance), surface));
        graphicsQueue = device.logicalDevice_.getQueue(device.queueFamilyIndices_.graphicsFamilyIndex, 0);
        presentQueue  = device.logicalDevice_.getQueue(device.queueFamilyIndices_.presentFamilyIndex, 0);
        graphicsPool = device.createCommandPool(device.queueFamilyIndices_.graphicsFamilyIndex);
        swapChain = mrk::Swapchain(mrk::Swapchain::createInfo{});

        // As of now the default constructor does nothing for the resource manager or pipeline
        //resourceManager = mrk::ResourceManager();
        //pipeline = mrk::Pipeline();
    }

   void LoadResources()
    {
        char const * const MODEL_PATH = "Assets/models/chalet.obj";
        char const * const TEXTURE_PATH = "Assets/textures/chalet.jpg";
        char const * const VERTEX_SHADER_PATH = "Assets/shaders/vert.spv";
        char const * const FRAGMENT_SHADER_PATH = "Assets/shaders/frag.spv";
		
        g_graphicsSystemSingleton.resourceManager.load(ResourceManager::loadInfo{ MODEL_PATH, TEXTURE_PATH, VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH });

		g_graphicsSystemSingleton.pipeline.load();
    }

    void Draw()
    {
        WindowSystem::update();

        // UPDATING UNIFORM BUFFER

        // Rotation Timing Logic
        using namespace std::chrono;
        static auto startTime = high_resolution_clock::now();

        auto currentTime = high_resolution_clock::now();
        float time = duration_cast<milliseconds>(currentTime - startTime).count() / 1000.0f;

        UniformBufferObject ubo;
        // Third parameter: Rotating about z axis
        ubo.model = glm::rotate(glm::mat4(), time * glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), // cameraPosition
            glm::vec3(0.0f, 0.0f, 0.0f), // cameraTarget
            glm::vec3(0.0f, 0.0f, 1.0f) // cameraUp
        );

        vk::Extent2D extent = g_graphicsSystemSingleton.swapChain.getExtent();
        const float nearPlane = 0.1f;
        const float farPlane = 10.0f;

        ubo.proj = glm::perspective(glm::radians(45.0f),
            static_cast<float>(extent.width) / static_cast<float>(extent.height),
            nearPlane, farPlane);
        // GLM was originally designed for opengl where the y coordinate of the clip coordinates is inverted
        ubo.proj[1][1] *= -1;

        void *data;
        g_graphicsSystemSingleton.resourceManager.getUniformBuffer().map(data);
        memcpy(data, &ubo, sizeof(ubo));
        g_graphicsSystemSingleton.resourceManager.getUniformBuffer().unmap();

        // Drawing Logic

		vk::ResultValue<uint32_t> result = 
            g_graphicsSystemSingleton.device.logicalDevice_.acquireNextImageKHR(
                g_graphicsSystemSingleton.swapChain.getSwapChain(), std::numeric_limits<uint64_t>::max(), g_graphicsSystemSingleton.pipeline.imageAvailable, {}
        );

		if (result.result == vk::Result::eErrorOutOfDateKHR)
		{
			g_graphicsSystemSingleton.swapChain.recreate();
			return;
		}
		else if (result.result != vk::Result::eSuccess && result.result != vk::Result::eSuboptimalKHR)
		{
			throw_line("Failed to acquire swap chain image!");
		}

		uint32_t imageIndex = result.value;

		std::array<vk::Semaphore, 1> waitSemaphores = { g_graphicsSystemSingleton.pipeline.imageAvailable };
		std::array<vk::PipelineStageFlags, 1> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

		std::array<vk::Semaphore, 1> signalSemaphores = { g_graphicsSystemSingleton.pipeline.renderFinished };

		vk::SubmitInfo submitInfo = vk::SubmitInfo()
			.setWaitSemaphoreCount(static_cast<uint32_t>(waitSemaphores.size()))
			.setPWaitSemaphores(waitSemaphores.data())
			.setSignalSemaphoreCount(static_cast<uint32_t>(signalSemaphores.size()))
			.setPSignalSemaphores(signalSemaphores.data())
			.setPWaitDstStageMask(waitStages.data())
			.setCommandBufferCount(1)
			.setPCommandBuffers(&g_graphicsSystemSingleton.pipeline.commandBuffers_[imageIndex]);

		MRK_CATCH(g_graphicsSystemSingleton.graphicsQueue.submit({submitInfo},vk::Fence()));

		std::array<vk::SwapchainKHR, 1> swapchains = { g_graphicsSystemSingleton.swapChain.getSwapChain() };

		vk::PresentInfoKHR presentInfo = vk::PresentInfoKHR()
			.setWaitSemaphoreCount(static_cast<uint32_t>(signalSemaphores.size()))
			.setPWaitSemaphores(signalSemaphores.data())
			.setSwapchainCount(static_cast<uint32_t>(swapchains.size()))
			.setPSwapchains(swapchains.data())
			.setPImageIndices(&imageIndex);

		result.result = g_graphicsSystemSingleton.presentQueue.presentKHR(presentInfo);

		if (result.result == vk::Result::eErrorOutOfDateKHR || result.result == vk::Result::eSuboptimalKHR)
		{
			g_graphicsSystemSingleton.swapChain.recreate();
		}
		else if (result.result != vk::Result::eSuccess)
		{
			throw_line("Failed to present swap chain image!");
		}

		 g_graphicsSystemSingleton.presentQueue.waitIdle();
    }

    void StopDrawing()
    {
        g_graphicsSystemSingleton.device.logicalDevice_.waitIdle();
    }

    void CleanUp()
    {
        WindowSystem::clean();
    }

	GLFWwindow * getWindow()
	{
		return g_graphicsSystemSingleton.windowSystem.getWindow();
	}

}
