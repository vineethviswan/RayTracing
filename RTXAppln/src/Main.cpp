
/* ---------------------------------------------------------------------------------
 * RAY TRACING
 * Started  : 02-Feb-2025
 * Finished : 
 * References : https://raytracing.github.io/books/RayTracingInOneWeekend.html
 * ---------------------------------------------------------------------------------*/

#include <iostream>
#include <Application.h>

int main()
{
	std::cout << "RAY TRACING !!!\n";

	// Appliation
	{
		Application *app = new Application(L"Ray Tracing", 1309, 605);
		app->Run();

		// Delete the app before exiting
		if (app)
			delete app;
	}

	return 0;
}