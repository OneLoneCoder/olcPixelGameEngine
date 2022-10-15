/*
	Example file for olcUTIL_Camera2D.h

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2022 OneLoneCoder.com

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
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021, 2022

*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "extensions/olcPGEX_TransformedView.h"

#include "utilities/olcUTIL_Camera2D.h"

/*
	To demonstrate the camera, we need a world. In this case its a simle tile
	world of 80x75 tiles, and each tile is 32x32 screen pixels.

	A transformed view is used to navigate the world manually via the middle
	mouse button in "free roam" mode, or controlled by the camera. 

	Specifically a Tile Transformed View is used, which means all units for
	drawing and for the camera are specified in tile space, i.e. 1 tile is 
	1x1 units (regardless of pixel size)

	No assets are used for this application, so the world is constructed
	out of coloured squares so you can see you are moving through it.

	Pressing "TAB" key will swap between "free roam" and "play" modes. In
	free roam mode, you can use middle mouse button to pan and zoom around 
	the world. The camera's visible area to the player is highlighted in red.
	In play mode, the camera behaves as it would in a 2D game, depending upon 
	the selected mode.
*/

class TEST_Camera2D : public olc::PixelGameEngine
{
public:
	TEST_Camera2D()
	{
		sAppName = "Camera2D Utility Test";
	}

	// Transformed view object to make world offsetting simple
	olc::TileTransformedView tv;

	// Conveninet constants to define tile map world
	olc::vi2d m_vWorldSize = { 80, 75 };
	olc::vi2d m_vTileSize = { 32, 32 };

	// The camera!
	olc::utils::Camera2D camera;

	// The point that represents the player, it is "tracked"
	// by the camera
	olc::vf2d vTrackedPoint;

	// Flag whether we are in "free roam" or "play" mode
	bool bFreeRoam = false;

	// The world map, stored as a 1D array
	std::vector<uint8_t> vWorldMap;

public:
	bool OnUserCreate() override
	{
		// Construct transform view
		tv = olc::TileTransformedView(GetScreenSize(), m_vTileSize);
		
		// Construct Camera
		vTrackedPoint = { 20.0f, 20.0f };		
		camera = olc::utils::Camera2D(GetScreenSize() / m_vTileSize, vTrackedPoint);

		// Configure Camera
		camera.SetTarget(vTrackedPoint);
		camera.SetMode(olc::utils::Camera2D::Mode::Simple);
		camera.SetWorldBoundary({ 0.0f, 0.0f }, m_vWorldSize);
		camera.EnableWorldBoundary(true);

		// Create "tile map" world with just two tiles
		vWorldMap.resize(m_vWorldSize.x * m_vWorldSize.y);
		for (int i = 0; i < vWorldMap.size(); i++)
			vWorldMap[i] = ((rand() % 20) == 1) ? 1 : 0;		

		// Set background colour
		Clear(olc::CYAN);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// In free roam, middle mouse button pans & zooms
		if (bFreeRoam)
			tv.HandlePanAndZoom();

		// Handle player "physics" in response to key presses
		olc::vf2d vVel = { 0.0f, 0.0f };
		if (GetKey(olc::Key::W).bHeld) vVel += {0, -1};
		if (GetKey(olc::Key::S).bHeld) vVel += {0, +1};
		if (GetKey(olc::Key::A).bHeld) vVel += {-1, 0};
		if (GetKey(olc::Key::D).bHeld) vVel += {+1, 0};
		vTrackedPoint += vVel * 8.0f * fElapsedTime;
		
		// Switch between "free roam" and "play" mode with TAB key
		if (GetKey(olc::Key::TAB).bPressed)
		{
			// Always setup camera to play mode when tab key pressed
			tv.SetWorldOffset(camera.GetViewPosition());
			tv.SetWorldScale(m_vTileSize);
			bFreeRoam = !bFreeRoam;
		}

		// Switch camera mode in operation
		if (GetKey(olc::Key::K1).bPressed) 
			camera.SetMode(olc::utils::Camera2D::Mode::Simple);
		if (GetKey(olc::Key::K2).bPressed) 
			camera.SetMode(olc::utils::Camera2D::Mode::EdgeMove);
		if (GetKey(olc::Key::K3).bPressed) 
			camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);
		if (GetKey(olc::Key::K4).bPressed) 
			camera.SetMode(olc::utils::Camera2D::Mode::FixedScreens);

		// Update the camera, if teh tracked object remains visible, 
		// true is returned
		bool bOnScreen = camera.Update(fElapsedTime);

		// In "play" mode, set the transformed view to that required by
		// the camera
		if (!bFreeRoam)
			tv.SetWorldOffset(camera.GetViewPosition());

		// Render "tile map", by getting visible tiles
		olc::vi2d vTileTL = tv.GetTopLeftTile().max({ 0,0 });
		olc::vi2d vTileBR = tv.GetBottomRightTile().min(m_vWorldSize);
		olc::vi2d vTile;
		// Then looping through them and drawing them
		for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
			for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
			{
				int idx = vTile.y * m_vWorldSize.x + vTile.x;
				
				if (vWorldMap[idx] == 0)
					tv.FillRectDecal(vTile, { 1.0f, 1.0f }, olc::Pixel(40, 40, 40));
				
				if (vWorldMap[idx] == 1)
					tv.FillRectDecal(vTile, { 1.0f, 1.0f }, olc::Pixel(60, 60, 60));				
			}

		// Draw the "player" as a 1x1 cell
		tv.FillRectDecal(vTrackedPoint - olc::vf2d(0.5f, 0.5f), {1.0f, 1.0f}, olc::BLUE);

		// Overlay with information
		if (bFreeRoam)
		{
			tv.FillRectDecal(camera.GetViewPosition(), camera.GetViewSize(), olc::PixelF(1.0f, 0.0f, 0.0f, 0.5f));
			DrawStringPropDecal({ 2, 2 }, "TAB: Free Mode, M-Btn to Pan & Zoom", olc::YELLOW);
		}
		else
			DrawStringPropDecal({ 2,2 }, "TAB: Play Mode", olc::YELLOW);

		DrawStringPropDecal({ 2,12 }, "WASD  : Move", olc::YELLOW);
		DrawStringPropDecal({ 2,22 }, "CAMERA: 1) Simple  2) EdgeMove  3) LazyFollow  4) Screens", olc::YELLOW);		
		DrawStringPropDecal({ 2,42 }, vTrackedPoint.str(), olc::YELLOW);
		return true;
	}
};

int main()
{
	TEST_Camera2D demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}