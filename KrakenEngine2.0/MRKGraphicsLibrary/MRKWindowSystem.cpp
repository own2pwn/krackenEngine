/** 
 * @file MRKWindowSystem.cpp
*
*  @brief Window system interface to actually display graphics and get player input
*
*  @author Juan Ramos
*/
#include <stdexcept>
#include <iostream>

#include "mrk.h"
#include "MRKWindowSystem.h"
#include "MRKVulkanTools.h"
#include "MRKInstance.h"
#include "MRKGraphicsSystem.h"

namespace Callbacks // callbacks
{
    void recreateWindowDependentResources(GLFWwindow* /*window*/, int width, int height)
    {
        if (width == 0 || height == 0) { return; }

        mrk::g_graphicsSystemSingleton.swapChain.recreate();
        mrk::g_graphicsSystemSingleton.pipeline.recreate();
    }
}

namespace mrk
{
    WindowSystem::WindowSystem(createInfo const& info) :
        window_(createWindow(info)),
        mRequiredExtensions(getExtensions())
    {
        glfwSetWindowSizeCallback(window_, Callbacks::recreateWindowDependentResources);
    }

    GLFWwindow* WindowSystem::createWindow(createInfo const& info)
    {
        // Tells GLFW that the application is using Vulkan
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        GLFWwindow *window = glfwCreateWindow(info.width, info.height, info.title, nullptr, nullptr);
        if (window == nullptr)
        {
            throw_line("Could not create glfwWindow")
        }

        return window;
    }

    std::vector<char const*> WindowSystem::getExtensions()
    {
        std::vector<char const *> requiredExtensions;

        // Get the required extensions for vulkan to render
        uint32_t extensionCount;
        char const ** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
        for (uint32_t i = 0; i < extensionCount; ++i)
        {
            requiredExtensions.push_back(extensions[i]);
        }

#ifdef _DEBUG
        std::cout << "Extensions GLFW requires" << std::endl;
        for (auto const & extension : requiredExtensions)
        {
            std::cout << extension << std::endl;
        }
#endif

        return requiredExtensions;
    }

    void WindowSystem::init()
    {
        glfwInit();
    }

    void WindowSystem::clean()
    {
        glfwTerminate();
    }

    void WindowSystem::update()
    {
        glfwPollEvents();
    }

    glm::ivec2 WindowSystem::getWindowSize() const
    {
        glm::ivec2 windowSize;

        glfwGetWindowSize(window_, &windowSize.x, &windowSize.y);

        return windowSize;
    }

	GLFWwindow * WindowSystem::getWindow() const
	{
		return _window;
	}

	vk::SurfaceKHR WindowSystem::createSurface(vk::Instance const& instance) const
	{
		VkSurfaceKHR surf = {};

		if (glfwCreateWindowSurface(static_cast<VkInstance>(instance), window_, nullptr, &surf) != VK_SUCCESS)
		{
			throw_line("Failed to create Surface!");
		}

		return surf;
	}

	WindowSystem::~WindowSystem()
    {
        if (window_)
        {
            glfwDestroyWindow(window_);
        }
    }

    WindowSystem& WindowSystem::operator=(WindowSystem&& other) noexcept
    {
        window_ = other.window_;
        other.window_ = nullptr;

        mRequiredExtensions = std::move(other.mRequiredExtensions);

        return *this;
    }
}



    }

	GLFWwindow * WindowSystem::getWindow() const
	{
		return window_;