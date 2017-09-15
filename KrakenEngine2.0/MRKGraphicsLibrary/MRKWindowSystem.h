/** @file MRKWindowSystem.h
*
*  @brief Window system interface to actually display graphics and get player input
*
*  @author Juan Ramos
*/
#pragma once

// Libs
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.hpp"
#include "glm/glm.hpp"

// Standard
#include <vector>

// Project

namespace mrk
{

struct WindowSystemCreateInfo;
typedef WindowSystemCreateInfo createInfo;

// Forward declaration for Graphics System
class WindowSystem
{
private:
    friend class GraphicsSystem;
    GLFWwindow * const _window;

    explicit WindowSystem(createInfo const &info);

    static GLFWwindow * createWindow(createInfo const& info);
    static std::vector<char const *> getExtensions();

public:
    /**
     * \brief 
     * Contains the strings needed to display a surface
     * EX: VK_KHR_surface, VK_KHR_win32_surface
     */
    std::vector<char const *> const requiredExtensions_;

    /**
     * \brief 
     * Should be called after the drawing in order to poll events for the next frame
     */
    static void update();

    glm::ivec2 getWindowSize() const;

	vk::SurfaceKHR createSurface(vk::Instance const& instance) const;

    ~WindowSystem();

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
    WindowSystem(WindowSystem const &) = delete;
    WindowSystem& operator=(const WindowSystem&) = delete;
    WindowSystem(WindowSystem &&) = delete;
    WindowSystem & operator=(WindowSystem &&) = delete;
};

}


