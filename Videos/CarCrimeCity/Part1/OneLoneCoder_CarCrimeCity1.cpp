/*
	BIG PROJECT - Top Down City Based Car Crime Game Part #1
	"Probably gonna regret starting this one..." - javidx9

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

	Instructions:
	~~~~~~~~~~~~~
	This is the source that accompanies part 1 of the video series which
	can be viewed via the link below. Here you can create and edit a city
	from a top down perspective ad navigate it with the car.

	Using the mouse left click you can select cells. Using right click clears
	all the selected cells.

	"E" - Lowers building height
	"T" - Raises building height
	"R" - Places road
	"Z, X" - Zoom
	"Up, Left, Right" - Control car
	"F5" - Save current city
	"F8" - Load existing city

	A default city is provided for you - "example1.city", please ensure
	you have this file also.

	Relevant Video: https://youtu.be/mD6b_hP17WI

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"

#include <vector>
#include <list>
#include <algorithm>
#include <utility>
#include <string>
#include <unordered_set>
#include <fstream>

// Override base class with your custom functionality
class CarCrimeCity : public olc::PixelGameEngine
{
public:
	CarCrimeCity()
	{
		sAppName = "Car Crime City";
	}

private:

	// Define the cell
	struct sCell
	{
		int nHeight = 0;
		int nWorldX = 0;
		int nWorldY = 0;
		bool bRoad = false;
		bool bBuilding = true;
	};

	// Map variables
	int nMapWidth;
	int nMapHeight;
	sCell *pMap;

	olc::Sprite *sprAll;
	olc::Sprite *sprGround;
	olc::Sprite *sprRoof;
	olc::Sprite *sprFrontage;
	olc::Sprite *sprWindows;
	olc::Sprite *sprRoad[12];
	olc::Sprite *sprCar;

	float fCameraX = 0.0f;
	float fCameraY = 0.0f;
	float fCameraZ = -10.0f;

	olc::GFX3D::mesh meshCube;
	olc::GFX3D::mesh meshFlat;
	olc::GFX3D::mesh meshWallsOut;

	float fCarAngle = 0.0f;
	float fCarSpeed = 2.0f;
	olc::GFX3D::vec3d vecCarVel = { 0,0,0 };
	olc::GFX3D::vec3d vecCarPos = { 0,0,0 };
	

	int nMouseWorldX = 0;
	int nMouseWorldY = 0;
	int nOldMouseWorldX = 0;
	int nOldMouseWorldY = 0;

	bool bMouseDown = false;
	std::unordered_set<sCell*> setSelectedCells;

	olc::GFX3D::PipeLine pipeRender;
	olc::GFX3D::mat4x4 matProj;
	olc::GFX3D::vec3d vUp = { 0,1,0 };
	olc::GFX3D::vec3d vEye = { 0,0,-10 };
	olc::GFX3D::vec3d vLookDir = { 0,0,1 };

	olc::GFX3D::vec3d viewWorldTopLeft, viewWorldBottomRight;


	void SaveCity(std::string sFilename)
	{
		std::ofstream file(sFilename, std::ios::out | std::ios::binary);
		file.write((char*)&nMapWidth, sizeof(int));
		file.write((char*)&nMapHeight, sizeof(int));
		for (int x = 0; x < nMapWidth; x++)
		{
			for (int y = 0; y < nMapHeight; y++)
			{
				file.write((char*)&pMap[y*nMapWidth + x], sizeof(sCell));
			}
		}
	}

	void LoadCity(std::string sFilename)
	{
		std::ifstream file(sFilename, std::ios::in | std::ios::binary);
		file.read((char*)&nMapWidth, sizeof(int));
		file.read((char*)&nMapHeight, sizeof(int));
		delete[] pMap;
		pMap = new sCell[nMapWidth * nMapHeight];
		for (int x = 0; x < nMapWidth; x++)
		{
			for (int y = 0; y < nMapHeight; y++)
			{
				file.read((char*)&pMap[y*nMapWidth + x], sizeof(sCell));
			}
		}
	}

public:
	bool OnUserCreate() override
	{
		// Load Sprite Sheet
		sprAll = new olc::Sprite("City_Roads1_mip0.png");

		// Here we break up the sprite sheet into individual textures. This is more
		// out of convenience than anything else, as it keeps the texture coordinates
		// easy to manipulate

		// Building Lowest Floor
		sprFrontage = new olc::Sprite(32, 96);
		SetDrawTarget(sprFrontage);
		DrawPartialSprite(0, 0, sprAll, 288, 64, 32, 96);

		// Building Windows
		sprWindows = new olc::Sprite(32, 96);
		SetDrawTarget(sprWindows);
		DrawPartialSprite(0, 0, sprAll, 320, 64, 32, 96);

		// Plain Grass Field
		sprGround = new olc::Sprite(96, 96);
		SetDrawTarget(sprGround);
		DrawPartialSprite(0, 0, sprAll, 192, 0, 96, 96);

		// Building Roof
		sprRoof = new olc::Sprite(96, 96);
		SetDrawTarget(sprRoof);
		DrawPartialSprite(0, 0, sprAll, 352, 64, 96, 96);

		// There are 12 Road Textures, aranged in a 3x4 grid
		for (int r = 0; r < 12; r++)
		{
			sprRoad[r] = new olc::Sprite(96, 96);
			SetDrawTarget(sprRoad[r]);
			DrawPartialSprite(0, 0, sprAll, (r%3)*96, (r/3)*96, 96, 96);
		}

		// Don't foregt to set the draw target back to being the main screen (been there... wasted 1.5 hours :| )
		SetDrawTarget(nullptr);

		// The Yellow Car
		sprCar = new olc::Sprite("car_top.png");

		
		
		// Define the city map, a 64x32 array of Cells. Initialise cells
		// to be just grass fields
		nMapWidth = 64;
		nMapHeight = 32;
		pMap = new sCell[nMapWidth * nMapHeight];
		for (int x = 0; x < nMapWidth; x++)
		{
			for (int y = 0; y < nMapHeight; y++)
			{
				pMap[y*nMapWidth + x].bRoad = false;
				pMap[y*nMapWidth + x].nHeight = 0;
				pMap[y*nMapWidth + x].nWorldX = x;
				pMap[y*nMapWidth + x].nWorldY = y;
			}
		}


		// Now we'll hand construct some meshes. These are DELIBERATELY simple and not optimised (see a later video)
		// Here the geometry is unit in size (1x1x1)

		// A Full cube - Always useful for debugging
		meshCube.tris = 
		{
			// SOUTH
			{ 0.0f, 0.0f, 0.0f, 1.0f,	    0.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
			{ 0.0f, 0.0f, 0.0f, 1.0f,  		1.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 0.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

			// EAST
			{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
			{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

			// NORTH
			{ 1.0f, 0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
			{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

			// WEST
			{ 0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
			{ 0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

			// TOP             																 				    
			{ 0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
			{ 0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

			// BOTTOM         																 				   
			{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
			{ 1.0f, 0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

		};

		// A Flat quad
		meshFlat.tris =
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f,	    0.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },
			{ 0.0f, 0.0f, 0.0f, 1.0f,  		1.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
		};

		// The four outer walls of a cell
		meshWallsOut.tris = 
		{
			// EAST
			{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.2f, 1.0f,		1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		0.0f, 0.0f, 0.0f, },
			{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.2f, 1.0f,		1.0f, 0.0f, 0.2f, 1.0f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		0.0f, 1.0f, 0.0f, },

			// WEST
			{ 0.0f, 0.0f, 0.2f, 1.0f,		0.0f, 1.0f, 0.2f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
			{ 0.0f, 0.0f, 0.2f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },

			// TOP             																 				    
			{ 0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.2f, 1.0f,		1.0f, 1.0f, 0.2f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		0.0f, 1.0f, 0.0f,   },
			{ 0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.2f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 		1.0f, 1.0f, 0.0f,   },

			// BOTTOM         																 				   
			{ 1.0f, 0.0f, 0.2f, 1.0f,		0.0f, 0.0f, 0.2f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, },
			{ 1.0f, 0.0f, 0.2f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, },
		};


		// Initialise the 3D Graphics PGE Extension. This is required
		// to setup internal buffers to the same size as the main output
		olc::GFX3D::ConfigureDisplay();

		// Configure the rendering pipeline with projection and viewport properties
		pipeRender.SetProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f, 0.0f, 0.0f, ScreenWidth(), ScreenHeight());

		// Also make a projection matrix, we might need this later
		matProj = olc::GFX3D::Math::Mat_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);
	
		LoadCity("example1.city");

		// Ok, lets go!
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Directly manipulate camera
		//if (GetKey(olc::Key::W).bHeld) fCameraY -= 2.0f * fElapsedTime;
		//if (GetKey(olc::Key::S).bHeld) fCameraY += 2.0f * fElapsedTime;
		//if (GetKey(olc::Key::A).bHeld) fCameraX -= 2.0f * fElapsedTime;
		//if (GetKey(olc::Key::D).bHeld) fCameraX += 2.0f * fElapsedTime;
		if (GetKey(olc::Key::Z).bHeld) fCameraZ += 5.0f * fElapsedTime;
		if (GetKey(olc::Key::X).bHeld) fCameraZ -= 5.0f * fElapsedTime;

		if (GetKey(olc::Key::F5).bReleased) SaveCity("example1.city");
		if (GetKey(olc::Key::F8).bReleased) LoadCity("example1.city");

		// === Handle User Input for Editing ==

		// If there are no selected cells, then only edit the cell under the current mouse cursor
		// otherwise iterate through the set of sleected cells and apply to all of them

		// Check that cell exists in valid 2D map space
		if (nMouseWorldX >= 0 && nMouseWorldX < nMapWidth && nMouseWorldY >= 0 && nMouseWorldY < nMapHeight)
		{
			// Press "R" to toggle Road flag for selected cell(s)
			if (GetKey(olc::Key::R).bPressed)
			{
				if (!setSelectedCells.empty())
				{
					for (auto &cell : setSelectedCells)
					{
						cell->bRoad = !cell->bRoad;
					}
				}
				else
					pMap[nMouseWorldY*nMapWidth + nMouseWorldX].bRoad = !pMap[nMouseWorldY*nMapWidth + nMouseWorldX].bRoad;
			}

			// Press "T" to increase height for selected cell(s)
			if (GetKey(olc::Key::T).bPressed)
			{
				if (!setSelectedCells.empty())
				{
					for (auto &cell : setSelectedCells)
					{
						cell->nHeight++;
					}
				}
				else
					pMap[nMouseWorldY*nMapWidth + nMouseWorldX].nHeight++;
			}

			// Press "E" to decrease height for selected cell(s)
			if (GetKey(olc::Key::E).bPressed)
			{
				if (!setSelectedCells.empty())
				{
					for (auto &cell : setSelectedCells)
					{
						cell->nHeight--;
					}
				}
				else
					pMap[nMouseWorldY*nMapWidth + nMouseWorldX].nHeight--;
			}
		}


		// === Car User Input ===

		if (GetKey(olc::Key::LEFT).bHeld) fCarAngle -= 4.0f * fElapsedTime;
		if (GetKey(olc::Key::RIGHT).bHeld) fCarAngle += 4.0f * fElapsedTime;

		olc::GFX3D::vec3d a = { 1, 0, 0 };
		olc::GFX3D::mat4x4 m = olc::GFX3D::Math::Mat_MakeRotationZ(fCarAngle);
		vecCarVel = olc::GFX3D::Math::Mat_MultiplyVector(m, a);

		if (GetKey(olc::Key::UP).bHeld)
		{
			vecCarPos.x += vecCarVel.x * fCarSpeed * fElapsedTime;
			vecCarPos.y += vecCarVel.y * fCarSpeed * fElapsedTime;
		}

		// === Position Camera ===

		// Our camera currently follows the car, and the car stays in the middle of
		// the screen. We need to know where the camera is before we can work with
		// on screen interactions
		fCameraY = vecCarPos.y;
		fCameraX = vecCarPos.x;
		vEye = { fCameraX,fCameraY,fCameraZ };
		olc::GFX3D::vec3d vLookTarget = olc::GFX3D::Math::Vec_Add(vEye, vLookDir);

		// Setup the camera properties for the pipeline - aka "view" transform
		pipeRender.SetCamera(vEye, vLookTarget, vUp);


		// === Calculate Mouse Position on Ground Plane ===

		// Here we take the screen coordinate of the mouse, transform it into normalised space (-1 --> +1)
		// for both axes. Instead of inverting and multiplying by the projection matrix, we only need the
		// aspect ratio parameters, with which we'll scale the mouse coordinate. This new point is then 
		// multiplied by the inverse of the look at matrix (camera view matrix) aka a point at matrix, to
		// transform the new point into world space.
		//
		// Now, the thing is, a 2D point is no good on its own, our world has depth. If we treat the 2D
		// point as a ray cast from (0, 0)->(mx, my), we can see where this ray intersects with a plane
		// at a specific depth.

		// Create a point at matrix, if you recall, this is the inverse of the look at matrix
		// used by the camera
		olc::GFX3D::mat4x4 matView = olc::GFX3D::Math::Mat_PointAt(vEye, vLookTarget, vUp);

		// Assume the origin of the mouse ray is the middle of the screen...
		olc::GFX3D::vec3d vecMouseOrigin = { 0.0f, 0.0f, 0.0f };

		// ...and that a ray is cast to the mouse location from the origin. Here we translate
		// the mouse coordinates into viewport coordinates
		olc::GFX3D::vec3d vecMouseDir = {
			2.0f * ((GetMouseX() / (float)ScreenWidth()) - 0.5f) / matProj.m[0][0],
			2.0f * ((GetMouseY() / (float)ScreenHeight()) - 0.5f) / matProj.m[1][1],
			1.0f,
			0.0f };

		// Now transform the origin point and ray direction by the inverse of the camera		
		vecMouseOrigin = olc::GFX3D::Math::Mat_MultiplyVector(matView, vecMouseOrigin);
		vecMouseDir = olc::GFX3D::Math::Mat_MultiplyVector(matView, vecMouseDir);

		// Extend the mouse ray to a large length
		vecMouseDir = olc::GFX3D::Math::Vec_Mul(vecMouseDir, 1000.0f);

		// Offset the mouse ray by the mouse origin
		vecMouseDir = olc::GFX3D::Math::Vec_Add(vecMouseOrigin, vecMouseDir);

		// All of our intersections for mouse checks occur in the ground plane (z=0), so
		// define a plane at that location
		olc::GFX3D::vec3d plane_p = { 0.0f, 0.0f, 0.0f };
		olc::GFX3D::vec3d plane_n = { 0.0f, 0.0f, 1.0f };
		
		// Calculate Mouse Location in plane, by doing a line/plane intersection test
		float t = 0.0f;
		olc::GFX3D::vec3d mouse3d = olc::GFX3D::Math::Vec_IntersectPlane(plane_p, plane_n, vecMouseOrigin, vecMouseDir, t);

		
		
		// === Now we have the mouse in 3D! Handle mouse user input ===

		// Left click & left click drag selects cells by adding them to the set of selected cells
		// Here I make sure only to do this if the cell under the mouse has changed from the 
		// previous frame, but the set will also reject duplicate cells being added
		if (GetMouse(0).bHeld && ((nMouseWorldX != nOldMouseWorldX) || (nMouseWorldY != nOldMouseWorldY)))
			setSelectedCells.emplace(&pMap[nMouseWorldY * nMapWidth + nMouseWorldX]);

		// Single clicking cells also adds them
		if (GetMouse(0).bPressed)
			setSelectedCells.emplace(&pMap[nMouseWorldY * nMapWidth + nMouseWorldX]);
	
		// If the user right clicks, the set of selected cells is emptied
		if (GetMouse(1).bReleased)
			setSelectedCells.clear();
		
		// Cache the current mouse position to use during comparison in next frame
		nOldMouseWorldX = nMouseWorldX;
		nOldMouseWorldY = nMouseWorldY;

		nMouseWorldX = (int)mouse3d.x;
		nMouseWorldY = (int)mouse3d.y;

		


		// === Rendering ===

		// Right, now we're gonna render the scene!
		
		// First Clear the screen and the depth buffer
		Clear(olc::BLUE);
		olc::GFX3D::ClearDepth();

		// === Calculate Visible World ===

		// Since we now have the transforms to convert screen space into ground plane space, we
		// can calculate the visible extents of the world, regardless of zoom level! The method is
		// exactly the same for the mouse, but we use fixed screen coordinates that represent the
		// top left, and bottom right of the screen

		// Work out Top Left Ground Cell
		vecMouseDir = { -1.0f / matProj.m[0][0],-1.0f / matProj.m[1][1], 1.0f, 0.0f };
		vecMouseDir = olc::GFX3D::Math::Mat_MultiplyVector(matView, vecMouseDir);
		vecMouseDir = olc::GFX3D::Math::Vec_Mul(vecMouseDir, 1000.0f);
		vecMouseDir = olc::GFX3D::Math::Vec_Add(vecMouseOrigin, vecMouseDir);
		viewWorldTopLeft = olc::GFX3D::Math::Vec_IntersectPlane(plane_p, plane_n, vecMouseOrigin, vecMouseDir, t);

		// Work out Bottom Right Ground Cell
		vecMouseDir = { 1.0f / matProj.m[0][0], 1.0f / matProj.m[1][1], 1.0f, 0.0f };
		vecMouseDir = olc::GFX3D::Math::Mat_MultiplyVector(matView, vecMouseDir);
		vecMouseDir = olc::GFX3D::Math::Vec_Mul(vecMouseDir, 1000.0f);
		vecMouseDir = olc::GFX3D::Math::Vec_Add(vecMouseOrigin, vecMouseDir);
		viewWorldBottomRight = olc::GFX3D::Math::Vec_IntersectPlane(plane_p, plane_n, vecMouseOrigin, vecMouseDir, t);

		// Calculate visible tiles
		//int nStartX = 0;
		//int nEndX = nMapWidth;
		//int nStartY = 0;
		//int nEndY = nMapHeight;

		int nStartX = std::max(0, (int)viewWorldTopLeft.x - 1);
		int nEndX = std::min(nMapWidth, (int)viewWorldBottomRight.x + 1);
		int nStartY = std::max(0, (int)viewWorldTopLeft.y - 1);
		int nEndY = std::min(nMapHeight, (int)viewWorldBottomRight.y + 1);


		// Iterate through all the cells we wish to draw. Each cell is 1x1 and elevates in the Z -Axis
		for (int x = nStartX; x < nEndX; x++)
		{
			for (int y = nStartY; y < nEndY; y++)
			{
				if (pMap[y*nMapWidth + x].bRoad) 
				{
					// Cell is a road, look at neighbouring cells. If they are roads also,
					// then choose the appropriate texture that joins up correctly

					int road = 0;
					auto r = [&](int i, int j)
					{
						return pMap[(y + j) * nMapWidth + (x + i)].bRoad;
					};

					if (r(0, -1) && r(0, +1) && !r(-1, 0) && !r(+1, 0)) road = 0;
					if (!r(0, -1) && !r(0, +1) && r(-1, 0) && r(+1, 0)) road = 1;

					if (!r(0, -1) && r(0, +1) && !r(-1, 0) && r(+1, 0)) road = 3;
					if (!r(0, -1) && r(0, +1) && r(-1, 0) && r(+1, 0)) road = 4;
					if (!r(0, -1) && r(0, +1) && r(-1, 0) && !r(+1, 0)) road = 5;

					if (r(0, -1) && r(0, +1) && !r(-1, 0) && r(+1, 0)) road = 6;
					if (r(0, -1) && r(0, +1) && r(-1, 0) && r(+1, 0)) road = 7;
					if (r(0, -1) && r(0, +1) && r(-1, 0) && !r(+1, 0)) road = 8;

					if (r(0, -1) && !r(0, +1) && !r(-1, 0) && r(+1, 0)) road = 9;
					if (r(0, -1) && !r(0, +1) && r(-1, 0) && r(+1, 0)) road = 10;
					if (r(0, -1) && !r(0, +1) && r(-1, 0) && !r(+1, 0)) road = 11;

					// Create a translation transform to position the cell in the world
					olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeTranslation(x, y, 0.0f);
					pipeRender.SetTransform(matWorld);

					// Set the appropriate texture to use
					pipeRender.SetTexture(sprRoad[road]);

					// Draw a flat quad
					pipeRender.Render(meshFlat.tris);

				}
				else // Not Road
				{
					// If the cell is not considered road, then draw it appropriately

					if (pMap[y*nMapWidth + x].nHeight < 0)
					{
						// Cell is blank - for now ;-P
					}

					if (pMap[y*nMapWidth + x].nHeight == 0)
					{
						// Cell is ground, draw a flat grass quad at height 0
						olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeTranslation(x, y, 0.0f);
						pipeRender.SetTransform(matWorld);
						pipeRender.SetTexture(sprGround);
						pipeRender.Render(meshFlat.tris);
					}

					if (pMap[y*nMapWidth + x].nHeight > 0)
					{
						// Cell is Building, for now, we'll draw each storey as a seperate mesh 
						int h, t;
						t = pMap[y*nMapWidth + x].nHeight;

						for (h = 0; h < t; h++)
						{
							// Create a transform that positions the storey according to its height
							olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeTranslation(x, y, -(h + 1) * 0.2f);
							pipeRender.SetTransform(matWorld);

							// Choose a texture, if its ground level, use the "street level front", otherwise use windows
							pipeRender.SetTexture(h == 0 ? sprFrontage : sprWindows);
							pipeRender.Render(meshWallsOut.tris);
						}

						// Top the building off with a roof
						olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeTranslation(x, y, -(h) * 0.2f);
						pipeRender.SetTransform(matWorld);
						pipeRender.SetTexture(sprRoof);
						pipeRender.Render(meshFlat.tris);
					}
				}
			}
		}

		// Draw Selected Cells, iterate through the set of cells, and draw a wireframe quad at ground level
		// to indicate it is in the selection set
		for (auto &cell : setSelectedCells)
		{
			// Draw CursorCube
			olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeTranslation(cell->nWorldX, cell->nWorldY, 0.0f);
			pipeRender.SetTransform(matWorld);
			pipeRender.SetTexture(sprRoof);
			pipeRender.Render(meshFlat.tris, olc::GFX3D::RENDER_WIRE);
		}

		// Draw Car, a few transforms required for this

		// 1) Offset the car to the middle of the quad
		olc::GFX3D::mat4x4 matCarOffset = olc::GFX3D::Math::Mat_MakeTranslation(-0.5f, -0.5f, -0.0f);
		// 2) The quad is currently unit square, scale it to be more rectangular and smaller than the cells
		olc::GFX3D::mat4x4 matCarScale = olc::GFX3D::Math::Mat_MakeScale(0.4f, 0.2f, 1.0f);
		// 3) Combine into matrix
		olc::GFX3D::mat4x4 matCar = olc::GFX3D::Math::Mat_MultiplyMatrix(matCarOffset, matCarScale);
		// 4) Rotate the car around its offset origin, according to its angle
		olc::GFX3D::mat4x4 matCarRot = olc::GFX3D::Math::Mat_MakeRotationZ(fCarAngle);
		matCar = olc::GFX3D::Math::Mat_MultiplyMatrix(matCar, matCarRot);
		// 5) Translate the car into its position in the world. Give it a little elevation so its baove the ground
		olc::GFX3D::mat4x4 matCarTrans = olc::GFX3D::Math::Mat_MakeTranslation(vecCarPos.x, vecCarPos.y, -0.01f);
		matCar = olc::GFX3D::Math::Mat_MultiplyMatrix(matCar, matCarTrans);
				
		// Set the car texture to the pipeline
		pipeRender.SetTexture(sprCar);
		// Apply "world" transform to pipeline
		pipeRender.SetTransform(matCar);
		
		// The car has transparency, so enable it
		SetPixelMode(olc::Pixel::ALPHA);
		// Render the quad
		pipeRender.Render(meshFlat.tris);
		// Set transparency back to none to optimise drawing other pixels
		SetPixelMode(olc::Pixel::NORMAL);


		// Draw the current camera position for debug information
		//DrawString(10, 30, "CX: " + std::to_string(fCameraX) + " CY: " + std::to_string(fCameraY) + " CZ: " + std::to_string(fCameraZ));
		return true;
	} 
};



int main()
{
	CarCrimeCity demo;
	if (demo.Construct(768, 480, 2, 2))
		demo.Start();
	return 0;
}