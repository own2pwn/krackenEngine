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

    void initializeGraphicsSystem(GraphicsSystemCreateInfo  const & createInfo); 

    struct ModelInfoCreateInfo
    {
        char const * modelPath;
        std::vector<char const *> texturePath;
        unsigned modelID;
    };

    enum class ShaderType
    {
        DEFAULT, // Only uses textures and vertices, doesn't use normals for lighting
        SHADER_COUNT
    };

    struct LoadResourcesCreateInfo
    {
        std::vector<ModelInfoCreateInfo> modelsToLoad;
        ShaderType shaderType;
    };

    void LoadResources(LoadResourcesCreateInfo const & loadInfo);

    void Draw();

    void StopDrawing();

    void CleanUp();

	GLFWwindow * getWindow();
}
