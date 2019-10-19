/*
	Coding Quickie: Isometric Tiles
	"Owww... My insides hurt :(" - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

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

	Relevant Video: https://youtu.be/ukkbNKTgf5U

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class IsometricDemo : public olc::PixelGameEngine
{
public:
	IsometricDemo()
	{
		sAppName = "Coding Quickie: Isometric Tiles";
	}

private:
	// Number of tiles in world
	olc::vi2d vWorldSize = { 14, 10 };

	// Size of single tile graphic
	olc::vi2d vTileSize = { 40, 20 };

	// Where to place tile (0,0) on screen (in tile size steps)
	olc::vi2d vOrigin = { 5, 1 };

	// Sprite that holds all imagery
	olc::Sprite *sprIsom = nullptr;

	// Pointer to create 2D world array
	int *pWorld = nullptr;

public:
	bool OnUserCreate() override
	{
		// Load sprites used in demonstration
		sprIsom = new olc::Sprite("isometric_demo.png");

		// Create empty world
		pWorld = new int[vWorldSize.x * vWorldSize.y]{ 0 };
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::WHITE);

		// Get Mouse in world
		olc::vi2d vMouse = { GetMouseX(), GetMouseY() };
		
		// Work out active cell
		olc::vi2d vCell = { vMouse.x / vTileSize.x, vMouse.y / vTileSize.y };

		// Work out mouse offset into cell
		olc::vi2d vOffset = { vMouse.x % vTileSize.x, vMouse.y % vTileSize.y };

		// Sample into cell offset colour
		olc::Pixel col = sprIsom->GetPixel(3 * vTileSize.x + vOffset.x, vOffset.y);

		// Work out selected cell by transforming screen cell
		olc::vi2d vSelected = 
		{
			(vCell.y - vOrigin.y) + (vCell.x - vOrigin.x),
			(vCell.y - vOrigin.y) - (vCell.x - vOrigin.x) 
		};

		// "Bodge" selected cell by sampling corners
		if (col == olc::RED) vSelected += {-1, +0};
		if (col == olc::BLUE) vSelected += {+0, -1};
		if (col == olc::GREEN) vSelected += {+0, +1};
		if (col == olc::YELLOW) vSelected += {+1, +0};

		// Handle mouse click to toggle if a tile is visible or not
		if (GetMouse(0).bPressed)
		{
			// Guard array boundary
			if (vSelected.x >= 0 && vSelected.x < vWorldSize.x && vSelected.y >= 0 && vSelected.y < vWorldSize.y)
				++pWorld[vSelected.y * vWorldSize.x + vSelected.x] %= 6;
		}
						
		// Labmda function to convert "world" coordinate into screen space
		auto ToScreen = [&](int x, int y)
		{			
			return olc::vi2d
			{
				(vOrigin.x * vTileSize.x) + (x - y) * (vTileSize.x / 2),
				(vOrigin.y * vTileSize.y) + (x + y) * (vTileSize.y / 2)
			};
		};
		
		// Draw World - has binary transparancy so enable masking
		SetPixelMode(olc::Pixel::MASK);

		// (0,0) is at top, defined by vOrigin, so draw from top to bottom
		// to ensure tiles closest to camera are drawn last
		for (int y = 0; y < vWorldSize.y; y++)
		{
			for (int x = 0; x < vWorldSize.x; x++)
			{
				// Convert cell coordinate to world space
				olc::vi2d vWorld = ToScreen(x, y);
				
				switch(pWorld[y*vWorldSize.x + x])
				{
				case 0:
					// Invisble Tile
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 1:
					// Visible Tile
					DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 2 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
					break;
				case 2:
					// Tree
					DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 0 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
					break;
				case 3:
					// Spooky Tree
					DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 1 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
					break;
				case 4:
					// Beach
					DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 2 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
					break;
				case 5:
					// Water
					DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 3 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, vTileSize.y * 2);
					break;
				}			
			}
		}

		// Draw Selected Cell - Has varying alpha components
		SetPixelMode(olc::Pixel::ALPHA);

		// Convert selected cell coordinate to world space
		olc::vi2d vSelectedWorld = ToScreen(vSelected.x, vSelected.y);

		// Draw "highlight" tile
		DrawPartialSprite(vSelectedWorld.x, vSelectedWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);

		// Go back to normal drawing with no expected transparency
		SetPixelMode(olc::Pixel::NORMAL);

		// Draw Hovered Cell Boundary
		//DrawRect(vCell.x * vTileSize.x, vCell.y * vTileSize.y, vTileSize.x, vTileSize.y, olc::RED);
				
		// Draw Debug Info
		DrawString(4, 4, "Mouse   : " + std::to_string(vMouse.x) + ", " + std::to_string(vMouse.y), olc::BLACK);
		DrawString(4, 14, "Cell    : " + std::to_string(vCell.x) + ", " + std::to_string(vCell.y), olc::BLACK);
		DrawString(4, 24, "Selected: " + std::to_string(vSelected.x) + ", " + std::to_string(vSelected.y), olc::BLACK);
		return true;
	}
};


int main()
{
	IsometricDemo demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}