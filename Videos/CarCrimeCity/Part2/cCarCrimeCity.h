/*
	Top Down City Based Car Crime Game - Part #2
	"Colin, I hope you're shooting 600+ wherever you are buddy. RIP." - javidx9

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
	Scroll with middle mouse wheel, TAB toggle edit mode, R to place road
	P to place pavement, Q to place building, Arrow keys to drive car

	Relevant Video: https://youtu.be/fIV6P1W-wuo

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


#pragma once

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"

#include "cGameSettings.h"
#include "cCityMap.h"

#include <vector>
#include <unordered_set>

struct sSmokeDecal
{
	float fLifetime = 0.1f;
	olc::GFX3D::vec3d pos;
};

class cCarCrimeCity : public olc::PixelGameEngine
{
public:
	cCarCrimeCity();
	~cCarCrimeCity();

private:
	bool OnUserCreate()                   override;
	bool OnUserUpdate(float fElapsedTime) override;
	bool OnUserDestroy()                  override;

private:

	class cGameObjectQuad
	{
	public:
		cGameObjectQuad(float w, float h)
		{
			fWidth = w;
			fHeight = h;
			fAngle = 0.0f;

			// Construct Model Quad Geometry
			vecPointsModel = { {-fWidth / 2.0f, -fHeight / 2.0f, -0.01f, 1.0f},
							   {-fWidth / 2.0f, +fHeight / 2.0f, -0.01f, 1.0f},
							   {+fWidth / 2.0f, +fHeight / 2.0f, -0.01f, 1.0f},
							   {+fWidth / 2.0f, -fHeight / 2.0f, -0.01f, 1.0f} };

			vecPointsWorld.resize(vecPointsModel.size());
			TransformModelToWorld();
		}

		void TransformModelToWorld()
		{
			for (size_t i = 0; i < vecPointsModel.size(); ++i)
			{
				vecPointsWorld[i] = {
					(vecPointsModel[i].x * cosf(fAngle)) - (vecPointsModel[i].y * sinf(fAngle)) + pos.x,
					(vecPointsModel[i].x * sinf(fAngle)) + (vecPointsModel[i].y * cosf(fAngle)) + pos.y,
					vecPointsModel[i].z,
					vecPointsModel[i].w
				};
			}
		}

		std::vector<olc::GFX3D::triangle> GetTriangles()
		{
			// Return triangles based upon this quad
			return
			{
				{vecPointsWorld[0], vecPointsWorld[1], vecPointsWorld[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, olc::RED},
				{vecPointsWorld[0], vecPointsWorld[2], vecPointsWorld[3], 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, olc::RED},
			};
		}

		// Use rectangle edge intersections.
		bool StaticCollisionWith(cGameObjectQuad &r2, bool bResolveStatic = false)
		{
			struct vec2d { float x; float y; };

			bool bCollision = false;

			// Check diagonals of R1 against edges of R2
			for (size_t p = 0; p < vecPointsWorld.size(); p++)
			{
				vec2d line_r1s = { pos.x, pos.y };
				vec2d line_r1e = { vecPointsWorld[p].x, vecPointsWorld[p].y };

				vec2d displacement = { 0,0 };

				for (size_t q = 0; q < r2.vecPointsWorld.size(); q++)
				{
					vec2d line_r2s = { r2.vecPointsWorld[q].x, r2.vecPointsWorld[q].y };
					vec2d line_r2e = { r2.vecPointsWorld[(q + 1) % r2.vecPointsWorld.size()].x, r2.vecPointsWorld[(q + 1) % r2.vecPointsWorld.size()].y };

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					if (t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f)
					{
						if (bResolveStatic)
						{
							displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
							displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
							bCollision = true;
						}
						else
							return true;
					}
				}

				pos.x -= displacement.x;
				pos.y -= displacement.y;
			}

			// Check diagonals of R2 against edges of R1
			for (size_t p = 0; p < r2.vecPointsWorld.size(); p++)
			{
				vec2d line_r1s = { r2.pos.x, r2.pos.y };
				vec2d line_r1e = { r2.vecPointsWorld[p].x, r2.vecPointsWorld[p].y };

				vec2d displacement = { 0,0 };

				for (size_t q = 0; q < vecPointsWorld.size(); q++)
				{
					vec2d line_r2s = { vecPointsWorld[q].x, vecPointsWorld[q].y };
					vec2d line_r2e = { vecPointsWorld[(q + 1) % vecPointsWorld.size()].x, vecPointsWorld[(q + 1) % vecPointsWorld.size()].y };

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					if (t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f)
					{
						if (bResolveStatic)
						{
							displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
							displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
							bCollision = true;
						}
						else
							return true;
					}
				}


				pos.x += displacement.x;
				pos.y += displacement.y;
			}

			return bCollision;
		}

		std::vector<olc::GFX3D::triangle> meshTris;
		std::vector<olc::GFX3D::vec3d> vecPointsModel;
		std::vector<olc::GFX3D::vec3d> vecPointsWorld;
		olc::GFX3D::vec3d pos;

		float fWidth;
		float fHeight;
		float fOriginX;
		float fOriginY;
		float fAngle;
	};

	bool LoadAssets();
	
	std::map<std::string, olc::Sprite*> mapAssetTextures;
	std::map<std::string, olc::GFX3D::mesh*> mapAssetMeshes;
	std::map<std::string, olc::GFX3D::mat4x4> mapAssetTransform;

	// Camera variables
	olc::GFX3D::vec3d vCamera = { 0.0f, 0.0f, -3.0f };
	olc::GFX3D::vec3d vUp = { 0.0f, 1.0f, 0.0f };
	olc::GFX3D::vec3d vEye = { 0.0f, 0.0f, -3.0f };
	olc::GFX3D::vec3d vLookDir = { 0.0f, 0.0f, 1.0f };

	// Ray Casting Parameters
	olc::vf2d viewWorldTopLeft;
	olc::vf2d viewWorldBottomRight;

	// Cloud movement variables
	float fCloudOffsetX = 0.0f;
	float fCloudOffsetY = 0.0f;

	// Mouse Control
	olc::vf2d vOffset = { 0.0f, 0.0f };
	olc::vf2d vStartPan = { 0.0f, 0.0f };
	olc::vf2d vMouseOnGround = { 0.0f, 0.0f };
	float fScale = 1.0f;

	olc::vf2d GetMouseOnGround(const olc::vf2d &vMouseScreen);

	//cVehicle car;
	olc::vf2d carvel;
	olc::vf2d carpos;
	float fSpeed = 0.0f;
	float fAngle = 0.0f;

	std::list<cAuto_Body*> listAutomata; // Holds all automata, note its a pointer because we use polymorphism

	void SpawnPedestrian(int x, int y);
	void SpawnVehicle(int x, int y);

	//cGameObjectQuad *goCar = nullptr;
	//cGameObjectQuad *goObstacle = nullptr;

	//std::vector<cGameObjectQuad> vecObstacles;

	cCityMap *pCity = nullptr;

	float fGlobalTime = 0.0f;

	// Editing Utilities
	bool bEditMode = true;
	int nMouseX = 0;
	int nMouseY = 0;

	struct sCellLoc { int x, y; };
	std::unordered_set<int> setSelectedCells;

	//std::list<sSmokeDecal> listDecalSmoke;

	//int nTrafficState = 0;

	void DoEditMode(float fElapsedTime);
};

