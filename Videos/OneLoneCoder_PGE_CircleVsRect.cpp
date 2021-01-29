/*
	Easy Circle Vs Rectangle Collision Resolution
	"Everything's just damp, the walls, the roof, everything!" - javidx9

	Video: https://youtu.be/D2a5fHX-Qrs

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

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"


class CircleVsRect : public olc::PixelGameEngine
{
public:
	CircleVsRect()
	{
		sAppName = "Circle Vs Rectangle";
	}

private:
	olc::TileTransformedView tv;

	struct sWorldObject
	{
		olc::vf2d vPos;
		olc::vf2d vVel;
		float fRadius = 0.5f;
	};

	sWorldObject object;
	
	std::string sWorldMap =
		"################################"
		"#..............................#"
		"#.......#####.#.....#####......#"
		"#.......#...#.#.....#..........#"
		"#.......#...#.#.....#..........#"
		"#.......#####.#####.#####......#"
		"#..............................#"
		"#.....#####.#####.#####..##....#"
		"#.........#.#...#.....#.#.#....#"
		"#.....#####.#...#.#####...#....#"
		"#.....#.....#...#.#.......#....#"
		"#.....#####.#####.#####.#####..#"
		"#..............................#"
		"#..............................#"
		"#..#.#..........#....#.........#"
		"#..#.#..........#....#.........#"
		"#..#.#.......#####.#######.....#"
		"#..#.#..........#....#.........#"
		"#..#.#.............###.#.#.....#"
		"#..#.##########................#"
		"#..#..........#....#.#.#.#.....#"
		"#..#.####.###.#................#"
		"#..#.#......#.#................#"
		"#..#.#.####.#.#....###..###....#"
		"#..#.#......#.#....#......#....#"
		"#..#.########.#....#......#....#"
		"#..#..........#....#......#....#"
		"#..############....#......#....#"
		"#..................########....#"
		"#..............................#"
		"#..............................#"
		"################################";

	olc::vi2d vWorldSize = { 32, 32 };

	bool bFollowObject = false;

public:
	bool OnUserCreate() override
	{
		// Create "Tiled World", where each tile is 32x32 screen pixels. Coordinates
		// for drawing will exist in unit-tile space from now on...
		tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 32, 32 });		
		object.vPos = { 3.0f, 3.0f };
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Control of Player Object
		object.vVel = { 0.0f, 0.0f };
		if (GetKey(olc::Key::W).bHeld) object.vVel += { 0.0f, -1.0f };
		if (GetKey(olc::Key::S).bHeld) object.vVel += { 0.0f, +1.0f };
		if (GetKey(olc::Key::A).bHeld) object.vVel += { -1.0f, 0.0f };
		if (GetKey(olc::Key::D).bHeld) object.vVel += { +1.0f, 0.0f };

		if (object.vVel.mag2() > 0)
			object.vVel = object.vVel.norm() * (GetKey(olc::Key::SHIFT).bHeld ? 5.0f : 2.0f);
		
		if (GetKey(olc::Key::SPACE).bReleased) bFollowObject = !bFollowObject;

		
		// Where will object be worst case?
		olc::vf2d vPotentialPosition = object.vPos + object.vVel * fElapsedTime;

		// Extract region of world cells that could have collision this frame
		olc::vi2d vCurrentCell = object.vPos.floor();
		olc::vi2d vTargetCell = vPotentialPosition;
		olc::vi2d vAreaTL = (vCurrentCell.min(vTargetCell) - olc::vi2d(1, 1)).max({ 0,0 });
		olc::vi2d vAreaBR = (vCurrentCell.max(vTargetCell) + olc::vi2d(1, 1)).min(vWorldSize);

		olc::vf2d vRayToNearest;
		 
		// Iterate through each cell in test area
		olc::vi2d vCell;
		for (vCell.y = vAreaTL.y; vCell.y <= vAreaBR.y; vCell.y++)
		{
			for (vCell.x = vAreaTL.x; vCell.x <= vAreaBR.x; vCell.x++)
			{
				// Check if the cell is actually solid...
				if (sWorldMap[vCell.y * vWorldSize.x + vCell.x] == '#')
				{
					// ...it is! So work out nearest point to future player position, around perimeter
					// of cell rectangle. We can test the distance to this point to see if we have
					// collided. 

					olc::vf2d vNearestPoint;
					vNearestPoint.x = std::max(float(vCell.x), std::min(vPotentialPosition.x, float(vCell.x + 1)));
					vNearestPoint.y = std::max(float(vCell.y), std::min(vPotentialPosition.y, float(vCell.y + 1)));

					olc::vf2d vRayToNearest = vNearestPoint - vPotentialPosition;
					float fOverlap = object.fRadius - vRayToNearest.mag();
					if (std::isnan(fOverlap)) fOverlap = 0;

					// If overlap is positive, then a collision has occurred, so we displace backwards by the 
					// overlap amount. The potential position is then tested against other tiles in the area
					// therefore "statically" resolving the collision
					if (fOverlap > 0)
					{
						// Statically resolve the collision
						vPotentialPosition = vPotentialPosition - vRayToNearest.norm() * fOverlap;
					}
				}
			}
		}

		// Set the objects new position to the allowed potential position
		object.vPos = vPotentialPosition;


		// Clear World
		Clear(olc::VERY_DARK_BLUE);
		
		if (bFollowObject)
		{
			tv.SetWorldOffset(object.vPos - tv.ScaleToWorld(olc::vf2d(ScreenWidth()/2.0f, ScreenHeight()/2.0f)));
			DrawString({ 10,10 }, "Following Object");
		}

		// Handle Pan & Zoom
		if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
		if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
		if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
		if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(2.0f, GetMousePos());
		if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.5f, GetMousePos());
		
		// Draw World
		olc::vi2d vTL = tv.GetTopLeftTile().max({ 0,0 });
		olc::vi2d vBR = tv.GetBottomRightTile().min(vWorldSize);
		olc::vi2d vTile;
		for (vTile.y = vTL.y; vTile.y < vBR.y; vTile.y++)
			for (vTile.x = vTL.x; vTile.x < vBR.x; vTile.x++)
			{
				if (sWorldMap[vTile.y * vWorldSize.x + vTile.x] == '#')
				{
					tv.DrawRect(vTile, { 1.0f, 1.0f }, olc::WHITE);
					tv.DrawLine(vTile, vTile + olc::vf2d(1.0f, 1.0f), olc::WHITE);
					tv.DrawLine(vTile + olc::vf2d(0.0f, 1.0f), vTile + olc::vf2d(1.0f, 0.0f), olc::WHITE);
				}
			}

		tv.FillRectDecal(vAreaTL, vAreaBR - vAreaTL + olc::vi2d(1,1), olc::Pixel(0,255,255,32));

		// Draw Boundary
		tv.DrawCircle(object.vPos, object.fRadius, olc::WHITE);

		// Draw Velocity
		if (object.vVel.mag2() > 0)
		{
			tv.DrawLine(object.vPos, object.vPos + object.vVel.norm() * object.fRadius, olc::MAGENTA);
		}

		return true;
	}
};

int main()
{
	CircleVsRect demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();
	return 0;
}