#include "../MRKGraphicsLibrary/mrk.h"
// #include "../MRKGraphicsLibrary/MRKGraphicsSystem.h" later

int main()
{
    mrk::WindowSystemCreateInfo wInfo = {"Testing Window", nullptr, nullptr, 800, 600};
    mrk::GraphicsSystemCreateInfo gInfo = {"Testing Environment", "Kracken Engine", wInfo };
	mrk::initializeGraphicsSystem(gInfo);

    char const * const MODEL_PATH = "Assets/models/chalet.obj";
    char const * const TEXTURE_PATH = "Assets/textures/chalet.jpg";
    const int MODEL_ID = 0;
    std::vector<mrk::ModelInfoCreateInfo> modelsToLoad = { {MODEL_PATH, {TEXTURE_PATH}, MODEL_ID} };
    mrk::LoadResourcesCreateInfo loadInfo = {modelsToLoad, mrk::ShaderType::DEFAULT};
	mrk::LoadResources(loadInfo);

    int i = 10000;
	while (i)
	{
		mrk::Draw();
        --i;
	}
    mrk::StopDrawing();

    mrk::CleanUp();
}
