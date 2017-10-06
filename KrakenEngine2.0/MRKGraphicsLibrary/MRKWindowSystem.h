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
    GLFWwindow * window_;

    static GLFWwindow * createWindow(createInfo const& info);
    static std::vector<char const *> getExtensions();

public:
    WindowSystem() = default;
    explicit WindowSystem(createInfo const &info);
    WindowSystem & operator=(WindowSystem && other) noexcept;
    ~WindowSystem();

    static void init();
    static void clean();

    /**
     * \brief 
     * Contains the strings needed to display a surface
     * EX: VK_KHR_surface, VK_KHR_win32_surface
     */
    std::vector<char const *> mRequiredExtensions;

    /**
     * \brief 
     * Should be called before any drawing
     */
    static void update();

    glm::ivec2 getWindowSize() const;
	GLFWwindow * getWindow() const;
	vk::SurfaceKHR createSurface(vk::Instance const& instance) const;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
    WindowSystem(WindowSystem const &) = delete;
    WindowSystem& operator=(const WindowSystem&) = delete;
    WindowSystem(WindowSystem &&) = delete;
};

}


