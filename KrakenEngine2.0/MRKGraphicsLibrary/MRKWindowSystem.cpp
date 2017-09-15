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

namespace mrk
{
    WindowSystem::WindowSystem(createInfo const& info) :
        _window(createWindow(info)),
        requiredExtensions_(getExtensions())
    {
    }

    GLFWwindow* WindowSystem::createWindow(createInfo const& info)
    {
        glfwInit();

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

    void WindowSystem::update()
    {
        glfwPollEvents();
    }

    glm::ivec2 WindowSystem::getWindowSize() const
    {
        glm::ivec2 windowSize;

        glfwGetWindowSize(_window, &windowSize.x, &windowSize.y);

        return windowSize;
    }

	vk::SurfaceKHR WindowSystem::createSurface(vk::Instance const& instance) const
	{
		VkSurfaceKHR surf = {};

		if (glfwCreateWindowSurface(static_cast<VkInstance>(instance), _window, nullptr, &surf) != VK_SUCCESS)
			throw_line("Failed to create Surface!");

		return surf;
	}

	WindowSystem::~WindowSystem()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

}


