#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC

#include "Application.h"
#include <stdexcept>
#include <iostream>

int main()
{
	try
	{
		Application application;
		application.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nException: " << e.what() << std::endl;
	}
	return 0;
}