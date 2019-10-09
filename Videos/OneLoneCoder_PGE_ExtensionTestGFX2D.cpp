/*
	OneLoneCoder_PGE_ExtensionTestGFX2D.cpp


	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018
*/

// Include the olcPixelGameEngine
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// To use an extension, just include it
#define OLC_PGE_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"

class TestExtension : public olc::PixelGameEngine
{
public:
	TestExtension()
	{
		sAppName = "Testing Graphics2D";
	}

public:
	bool OnUserCreate() override
	{
		for (int i = 0; i < 16; i++)
			listEvents.push_back("");

		spr = new olc::Sprite("new_piskel.png");

		return true;
	}

	std::list<std::string> listEvents;
	float fTotalTime = 0.0f;
	olc::Sprite *spr;

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Clear Screen
		SetPixelMode(olc::Pixel::NORMAL);
		Clear(olc::BLUE);		

		// Draw Primitives
		DrawCircle(32, 32, 30);		// Circle
		DrawCircle(96, 32, 30);		// Circle


		float mx = (float)GetMouseX();
		float my = (float)GetMouseY();

		float px1 = mx - 32, px2 = mx - 96;
		float py1 = my - 32, py2 = my - 32;
		float pr1 = 1.0f / sqrtf(px1*px1 + py1*py1);
		float pr2 = 1.0f / sqrtf(px2*px2 + py2*py2);
		px1 = 22.0f * (px1 * pr1) + 32.0f;
		py1 = 22.0f * (py1 * pr1) + 32.0f;
		px2 = 22.0f * (px2 * pr2) + 96.0f;
		py2 = 22.0f * (py2 * pr2) + 32.0f;
		FillCircle((int32_t)px1, (int32_t)py1, 8, olc::CYAN);
		FillCircle((int32_t)px2, (int32_t)py2, 8, olc::CYAN);
				
		DrawLine(10, 70, 54, 70);	// Lines
		DrawLine(54, 70, 70, 54);

		DrawRect(10, 80, 54, 30);
		FillRect(10, 80, 54, 30);

		// Multiline Text
		std::string mpos = "Your Mouse Position is:\nX=" + std::to_string(mx) + "\nY=" + std::to_string(my);
		DrawString(10, 130, mpos);

		auto AddEvent = [&](std::string s)
		{
			listEvents.push_back(s);
			listEvents.pop_front();
		};

		if (GetMouse(0).bPressed)	AddEvent("Mouse Button 0 Down");
		if (GetMouse(0).bReleased)	AddEvent("Mouse Button 0 Up");
		if (GetMouse(1).bPressed)	AddEvent("Mouse Button 1 Down");
		if (GetMouse(1).bReleased)	AddEvent("Mouse Button 1 Up");
		if (GetMouse(2).bPressed)	AddEvent("Mouse Button 2 Down");
		if (GetMouse(2).bReleased)	AddEvent("Mouse Button 2 Up");


		// Draw Event Log
		int nLog = 0;
		for (auto &s : listEvents)
		{
			DrawString(200, nLog * 8 + 20, s, olc::Pixel(nLog * 16, nLog * 16, nLog * 16));
			nLog++;
		}

		std::string notes = "CDEFGAB";


		// Test Text scaling and colours
		DrawString(0, 360, "Text Scale = 1", olc::WHITE, 1);
		DrawString(0, 368, "Text Scale = 2", olc::BLUE, 2);
		DrawString(0, 384, "Text Scale = 3", olc::RED, 3);
		DrawString(0, 408, "Text Scale = 4", olc::YELLOW, 4);
		DrawString(0, 440, "Text Scale = 5", olc::GREEN, 5);

		fTotalTime += fElapsedTime;

		float fAngle = fTotalTime;

		// Draw Sprite using extension, first create a transformation stack
		olc::GFX2D::Transform2D t1;

		// Traslate sprite so center of image is at 0,0
		t1.Translate(-250, -35);
		// Scale the sprite
		t1.Scale(1 * sinf(fAngle) + 1, 1 * sinf(fAngle) + 1);
		// Rotate it
		t1.Rotate(fAngle*2.0f);
		// Translate to 0,100
		t1.Translate(0, 100);
		// Rotate different speed
		t1.Rotate(fAngle / 3);
		// Translate to centre of screen
		t1.Translate(320, 240);

		SetPixelMode(olc::Pixel::ALPHA);

		// Use extension to draw sprite with transform applied
		olc::GFX2D::DrawSprite(spr, t1);

		DrawSprite((int32_t)mx, (int32_t)my, spr, 4);
		
		return true;
	}
};


int main()
{
	TestExtension demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();

	return 0;
}