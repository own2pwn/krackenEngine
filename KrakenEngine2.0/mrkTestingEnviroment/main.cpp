#include "../MRKGraphicsLibrary/mrk.h"
// #include "../MRKGraphicsLibrary/MRKGraphicsSystem.h" later

int main()
{
    mrk::WindowSystemCreateInfo wInfo = {"Testing Window", nullptr, nullptr, 800, 600};
    mrk::GraphicsSystemCreateInfo gInfo = {"Testing Environment", "Kracken Engine", wInfo };
	mrk::initializeGraphicsSystem(gInfo);

	mrk::LoadResources();

    int i = 10000;
	while (i)
	{
		mrk::Draw();
        --i;
	}
    mrk::StopDrawing();

    mrk::CleanUp();
}
