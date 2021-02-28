/*
	Fast Ray Casting Using DDA
	"Itchy Eyes... Not blinking enough..." - javidx9

	Video: https://youtu.be/NbSee-XM7WA

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2021 OneLoneCoder.com

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
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "RayCast With DDA Algorithm";
	}

	olc::vf2d vPlayer = { 0,0 };
	olc::vi2d vMapSize = { 32, 30 };
	olc::vi2d vCellSize = { 16, 16 };
	std::vector<int> vecMap;


public:
	bool OnUserCreate() override
	{
		// Construct Map
		vecMap.resize(vMapSize.x * vMapSize.y);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		olc::vf2d vMouse = { float(GetMouseX()), float(GetMouseY()) };
		olc::vf2d vMouseCell = vMouse / vCellSize;
		olc::vi2d vCell = vMouseCell; // implicit cast to integer, rounds down

		// Paint with right mouse button "solid" tiles
		if (GetMouse(1).bHeld) vecMap[vCell.y * vMapSize.x + vCell.x] = 1;

		// Move "player" position
		if (GetKey(olc::Key::W).bHeld) vPlayer.y -= 25.0f * fElapsedTime;
		if (GetKey(olc::Key::S).bHeld) vPlayer.y += 25.0f * fElapsedTime;
		if (GetKey(olc::Key::A).bHeld) vPlayer.x -= 25.0f * fElapsedTime;
		if (GetKey(olc::Key::D).bHeld) vPlayer.x += 25.0f * fElapsedTime;

		// DDA Algorithm ==============================================
		// https://lodev.org/cgtutor/raycasting.html


		// Form ray cast from player into scene
		olc::vf2d vRayStart = vPlayer;
		olc::vf2d vRayDir = (vMouseCell - vPlayer).norm();
				
		// Lodev.org also explains this additional optimistaion (but it's beyond scope of video)
		// olc::vf2d vRayUnitStepSize = { abs(1.0f / vRayDir.x), abs(1.0f / vRayDir.y) };

		olc::vf2d vRayUnitStepSize = { sqrt(1 + (vRayDir.y / vRayDir.x) * (vRayDir.y / vRayDir.x)), sqrt(1 + (vRayDir.x / vRayDir.y) * (vRayDir.x / vRayDir.y)) };
		olc::vi2d vMapCheck = vRayStart;
		olc::vf2d vRayLength1D;
		olc::vi2d vStep;

		// Establish Starting Conditions
		if (vRayDir.x < 0)
		{
			vStep.x = -1;
			vRayLength1D.x = (vRayStart.x - float(vMapCheck.x)) * vRayUnitStepSize.x;
		}
		else
		{
			vStep.x = 1;
			vRayLength1D.x = (float(vMapCheck.x + 1) - vRayStart.x) * vRayUnitStepSize.x;
		}

		if (vRayDir.y < 0)
		{
			vStep.y = -1;
			vRayLength1D.y = (vRayStart.y - float(vMapCheck.y)) * vRayUnitStepSize.y;
		}
		else
		{
			vStep.y = 1;
			vRayLength1D.y = (float(vMapCheck.y + 1) - vRayStart.y) * vRayUnitStepSize.y;
		}

		// Perform "Walk" until collision or range check
		bool bTileFound = false;
		float fMaxDistance = 100.0f;
		float fDistance = 0.0f;
		while (!bTileFound && fDistance < fMaxDistance)
		{
			// Walk along shortest path
			if (vRayLength1D.x < vRayLength1D.y)
			{
				vMapCheck.x += vStep.x;
				fDistance = vRayLength1D.x;
				vRayLength1D.x += vRayUnitStepSize.x;
			}
			else
			{
				vMapCheck.y += vStep.y;
				fDistance = vRayLength1D.y;
				vRayLength1D.y += vRayUnitStepSize.y;
			}

			// Test tile at new test point
			if (vMapCheck.x >= 0 && vMapCheck.x < vMapSize.x && vMapCheck.y >= 0 && vMapCheck.y < vMapSize.y)
			{
				if (vecMap[vMapCheck.y * vMapSize.x + vMapCheck.x] == 1)
				{
					bTileFound = true;
				}
			}
		}

		// Calculate intersection location
		olc::vf2d vIntersection;
		if (bTileFound)
		{
			vIntersection = vRayStart + vRayDir * fDistance;
		}



		Clear(olc::BLACK);

		// Draw Map
		for (int y = 0; y < vMapSize.y; y++)
		{
			for (int x = 0; x < vMapSize.x; x++)
			{
				int cell = vecMap[y * vMapSize.x + x];
				if (cell == 1)
					FillRect(olc::vi2d(x, y) * vCellSize, vCellSize, olc::BLUE);

				// Draw Cell border
				DrawRect(olc::vi2d(x, y) * vCellSize, vCellSize, olc::DARK_GREY);
			}
		}

		// Draw ray between player and mouse if left mouse button held
		if (GetMouse(0).bHeld)
		{
			DrawLine(vPlayer * vCellSize, vMouse, olc::WHITE, 0xF0F0F0F0);

			if (bTileFound)
			{
				DrawCircle(vIntersection * vCellSize, 4.0f, olc::YELLOW);
			}
		}

		// Draw Player
		FillCircle(vPlayer * vCellSize, 4.0f, olc::RED);

		// Draw Mouse
		FillCircle(vMouse, 4.0f, olc::GREEN);
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}
