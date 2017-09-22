#include "../MRKGraphicsLibrary/mrk.h"
// #include "../MRKGraphicsLibrary/MRKGraphicsSystem.h" later

int main()
{
	mrk::initializeGraphicsSystem();

	mrk::LoadResources();

    int i = 10000;
	while (i)
	{
		mrk::Draw();
        --i;
	}

}
