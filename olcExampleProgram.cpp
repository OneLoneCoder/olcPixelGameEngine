#include <iostream>

#define __GLFW__
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name you application
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame, draws random coloured pixels
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		return true;
	}
};

int main()
{

	float xScale = 1.0f, yScale = 1.0f;

#if defined(__GLFW__)
	glfwInit();
	glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xScale, &yScale);
	glfwTerminate();
#endif

	Example demo;
	if (demo.Construct(256, 240, xScale*3, yScale*3))
		demo.Start();
	return 0;

}

