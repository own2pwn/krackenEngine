/** @file mrk.cpp
*
*  @brief Client interface implementation
*
*  @author Juan Ramos 
*/
#include "mrk.h"
#include "MRKGraphicsSystem.h"
#include <iostream>

namespace
{
	mrk::WindowSystemCreateInfo winInfo = { "Hello Window", nullptr, nullptr, 800, 600 };

	mrk::GraphicsSystemCreateInfo g_CreateInfo = { "mrkTestingEnviroment", nullptr, winInfo };
}
namespace mrk
{
    GraphicsSystem g_graphicsSystemSingleton(g_CreateInfo);
    
    void initializeGraphicsSystem()
    {
        //g_CreateInfo = createInfo;
    }

   void LoadResources()
    {
        g_graphicsSystemSingleton.loadResources();
    }

    void Draw()
    {
        g_graphicsSystemSingleton.draw();
    }

    void StopDrawing()
    {
        g_graphicsSystemSingleton.device_.logicalDevice_.waitIdle();
    }

	GLFWwindow * getWindow()
	{
		
	}
}
