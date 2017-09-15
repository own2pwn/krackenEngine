#include <vector>
#include <array>
#include "MRKDevice.h"
#include "MRKSwapchain.h"
#include "mrk.h"
#include "MRKGraphicsSystem.h"
#include "MRKVulkanTools.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace mrk
{
	/**
	 * \brief Construct a GraphicsSystem from a CreateInfo
	 * \param createInfo
	 */
	GraphicsSystem::GraphicsSystem(CreateInfo const& createInfo) :
		windowSystem_(createInfo.winCreateInfo),
		instance_(mrk::Instance::createInfo{createInfo.applicationName, createInfo.engineName, windowSystem_}),
		surface_(windowSystem_.createSurface(static_cast<vk::Instance>(instance_))),
		device_(mrk::Device::createInfo{static_cast<vk::Instance>(instance_), surface_}),
        graphicsQueue_(device_.logicalDevice_.getQueue(device_.queueFamilyIndices_.graphicsFamilyIndex, 0)),
        presentQueue_(device_.logicalDevice_.getQueue(device_.queueFamilyIndices_.presentFamilyIndex, 0)),
        graphicsPool_(device_.createCommandPool(device_.queueFamilyIndices_.graphicsFamilyIndex)),
		swapchain_({}),
		resourceManager_(),
		pipeline_()
	{
	}

    void GraphicsSystem::loadResources()
    {
        char const * const MODEL_PATH = "Assets/models/chalet.obj";
        char const * const TEXTURE_PATH = "Assets/textures/chalet.jpg";
        char const * const VERTEX_SHADER_PATH = "Assets/shaders/vert.spv";
        char const * const FRAGMENT_SHADER_PATH = "Assets/shaders/frag.spv";
		
        resourceManager_.load(ResourceManager::loadInfo{ MODEL_PATH, TEXTURE_PATH, VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH });

		pipeline_.load();
    }

	void GraphicsSystem::draw()
	{
		vk::ResultValue<uint32_t> result = device_.logicalDevice_.acquireNextImageKHR(swapchain_.getSwapChain(), std::numeric_limits<uint64_t>::max(), pipeline_.imageAvailable, {});

		if (result.result == vk::Result::eErrorOutOfDateKHR)
		{
			swapchain_.reCreateSwapChain();
			return;
		}
		else if (result.result != vk::Result::eSuccess && result.result != vk::Result::eSuboptimalKHR)
		{
			throw_line("Failed to acquire swap chain image!");
		}

		uint32_t imageIndex = result.value;

		std::array<vk::Semaphore, 1> waitSemaphores = { pipeline_.imageAvailable };
		std::array<vk::PipelineStageFlags, 1> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

		std::array<vk::Semaphore, 1> signalSemaphores = { pipeline_.renderFinished };

		vk::SubmitInfo submitInfo = vk::SubmitInfo()
			.setWaitSemaphoreCount(static_cast<uint32_t>(waitSemaphores.size()))
			.setPWaitSemaphores(waitSemaphores.data())
			.setSignalSemaphoreCount(static_cast<uint32_t>(signalSemaphores.size()))
			.setPSignalSemaphores(signalSemaphores.data())
			.setPWaitDstStageMask(waitStages.data())
			.setCommandBufferCount(1)
			.setPCommandBuffers(&pipeline_.commandBuffers_[imageIndex]);

		MRK_CATCH(graphicsQueue_.submit({submitInfo},vk::Fence()));

		std::array<vk::SwapchainKHR, 1> swapchains = { swapchain_.getSwapChain() };

		vk::PresentInfoKHR presentInfo = vk::PresentInfoKHR()
			.setWaitSemaphoreCount(static_cast<uint32_t>(signalSemaphores.size()))
			.setPWaitSemaphores(signalSemaphores.data())
			.setSwapchainCount(static_cast<uint32_t>(swapchains.size()))
			.setPSwapchains(swapchains.data())
			.setPImageIndices(&imageIndex);

		result.result = presentQueue_.presentKHR(presentInfo);

		if (result.result == vk::Result::eErrorOutOfDateKHR || result.result == vk::Result::eSuboptimalKHR)
		{
			swapchain_.reCreateSwapChain();
		}
		else if (result.result != vk::Result::eSuccess)
		{
			throw_line("Failed to present swap chain image!");
		}

		presentQueue_.waitIdle();

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

        vk::Extent2D extent = swapchain_.getExtent();
        const float nearPlane = 0.1f;
        const float farPlane = 10.0f;

        ubo.proj = glm::perspective(glm::radians(45.0f),
            static_cast<float>(extent.width) / static_cast<float>(extent.height),
            nearPlane, farPlane);
        // GLM was originally designed for opengl where the y coordinate of the clip coordinates is inverted
        ubo.proj[1][1] *= -1;

        void *data;
        resourceManager_.getUniformBuffer().map(data);
        memcpy(data, &ubo, sizeof(ubo));
        resourceManager_.getUniformBuffer().unmap();
	}

}
