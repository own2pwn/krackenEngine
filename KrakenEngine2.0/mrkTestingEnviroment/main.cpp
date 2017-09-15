#include "mrk.h"

int main()
{
	mrk::initializeGraphicsSystem();

	mrk::LoadResources();

	while (true)
		mrk::Draw();

    return 0;
}
