/** @file mrk.h
*
*  @brief Client interface 
*
*  @author Juan Ramos 
*/
#pragma once
#include "MRKWindowSystem.h"

namespace mrk
{

    struct WindowSystemCreateInfo
    {
        char *title;
        char *iconPath;
        char *cursorIcon;
        unsigned width;
        unsigned height;
    };

	struct GraphicsSystemCreateInfo // create info structure for the entire library
	{
        char *applicationName;
        char *engineName;
        WindowSystemCreateInfo winCreateInfo;
    };

    void initializeGraphicsSystem(); // fix these conflicting names
    void LoadResources();
    void Draw();
    void StopDrawing();
	GLFWwindow * getWindow();
}
