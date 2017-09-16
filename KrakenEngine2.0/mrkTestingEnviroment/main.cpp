#include "../MRKGraphicsLibrary/mrk.h"
// #include "../MRKGraphicsLibrary/MRKGraphicsSystem.h" later

int main()
{
	mrk::initializeGraphicsSystem();

	mrk::LoadResources();

	while (true)
		mrk::Draw();
}
