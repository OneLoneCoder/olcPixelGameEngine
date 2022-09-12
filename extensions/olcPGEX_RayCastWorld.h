/*
	olcPGEX_RayCastWorld.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                   Ray Cast World v1.02                      |
	+-------------------------------------------------------------+

	NOTE: UNDER ACTIVE DEVELOPMENT - THERE ARE BUGS/GLITCHES

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	a quick and easy to use, flexible, skinnable, ray-cast 3D world
	engine, which handles all graphics and collisions within a
	pseudo 3D world. 

	It is designed to be implementation independent. Please see example
	files for usage instructions.

	Video: https://youtu.be/Vij_obgv9h4

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2020 OneLoneCoder.com

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
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020

	Revisions:
	1.00:	Initial Release
	1.01:	Fix NaN check on overlap distance (Thanks Dandistine)
	1.02:	Added dynamic step size for collisions
*/

#ifndef OLC_PGEX_RAYCASTWORLD_H
#define OLC_PGEX_RAYCASTWORLD_H

#include <unordered_map>
#include <algorithm>

namespace olc
{
	namespace rcw
	{
		// Base class for objects that exist in world
		class Object 
		{
		public:
			// Linkage to user object description
			uint32_t nGenericID = 0;
			// Position in tile/world space
			olc::vf2d pos;
			// Velocity in tile/world space
			olc::vf2d vel;
			// Speed of object
			float fSpeed = 0.0f;
			// Angular direction of object
			float fHeading = 0.0f;
			// Collision radius of object
			float fRadius = 0.5f;
			// Is drawn?
			bool bVisible = true;
			// Flag to be removed form world
			bool bRemove = false;
			// Can collide with scenery?
			bool bCollideWithScenery = true;
			// Notify scenery collision?
			bool bNotifySceneryCollision = false;
			// Can collide with other objects?
			bool bCollideWithObjects = false;
			// Notify object collisions?
			bool bNotifyObjectCollision = false;
			// Can this object be moved by another object?
			bool bCanBeMoved = true;
			// Has physics/collisions applied
			bool bIsActive = true;

			void Walk(const float fWalkSpeed);
			void Strafe(const float fStrafeSpeed);
			void Turn(const float fTurnSpeed);
			void Stop();
		};

		// The RayCastWorld Engine - Inherit from this, implement abstract
		// methods, call Update() and Render() when required
		class Engine : public olc::PGEX
		{
		public:
			// Identifies side of cell
			enum class CellSide
			{
				North,
				East,
				South,
				West,
				Top,
				Bottom,
			};

		public:
			// Construct world rednering parameters
			Engine(const int screen_w, const int screen_h, const float fov);

		protected:
			// ABSTRACT - User must return a suitable olc::Pixel depending on world location information provided
			virtual olc::Pixel SelectSceneryPixel(const int tile_x, const int tile_y, const olc::rcw::Engine::CellSide side, const float sample_x, const float sample_y, const float distance) = 0;

			// ABSTRACT - User must return a boolean indicating if the tile is solid or not
			virtual bool IsLocationSolid(const float tile_x, const float tile_y) = 0;

			// ABSTRACT - User must return sizes of requested objects in Unit Cell Size
			virtual float GetObjectWidth(const uint32_t id) = 0;
			virtual float GetObjectHeight(const uint32_t id) = 0;

			// ABSTRACT - User must return suitable olc::Pixel for object sprite sample location
			virtual olc::Pixel SelectObjectPixel(const uint32_t id, const float sample_x, const float sample_y, const float distance, const float angle) = 0;
	
			// OPTIONAL - User can handle collsiion response with scenery should they choose to
			virtual void HandleObjectVsScenery(std::shared_ptr<olc::rcw::Object> object, const int tile_x, const int tile_y, const olc::rcw::Engine::CellSide side, const float offset_x, const float offset_y);

			// OPTIONAL - User can handle collsiion response with objects should they choose to
			virtual void HandleObjectVsObject(std::shared_ptr<olc::rcw::Object> object1, std::shared_ptr<olc::rcw::Object> object2);



		public:
			// Sets In-Game Camera position
			void SetCamera(const olc::vf2d& pos, const float heading);

			// Called to update world state
			virtual void Update(float fElapsedTime);

			// Called to draw the world and its contents
			void Render();

		public:
			std::unordered_map<uint32_t, std::shared_ptr<olc::rcw::Object>> mapObjects;

		private:
			// A convenient utility struct to store all the info required to understand how a ray
			// has hit a specific tile
			struct sTileHit
			{
				olc::vi2d vTilePos = { 0,0 };
				olc::vf2d vHitPos = { 0,0 };
				float fLength = 0.0f;
				float fSampleX = 0.0f;
				Engine::CellSide eSide = Engine::CellSide::North;
			};

			// Cast ray into tile world, and return info about what it hits (if anything)
			bool CastRayDDA(const olc::vf2d& vOrigin, const olc::vf2d& vDirection, sTileHit& hit);
			
			// Convenient constants in algorithms
			const olc::vi2d vScreenSize;
			const olc::vi2d vHalfScreenSize;
			const olc::vf2d vFloatScreenSize;
			float fFieldOfView = 0.0f;

			// A depth buffer used to sort pixels in Z-Axis
			std::unique_ptr<float[]> pDepthBuffer;

			// Local store of camera position and direction
			olc::vf2d vCameraPos = { 5.0f, 5.0f };
			float fCameraHeading = 0.0f;
		};		
	}
}


#ifdef OLC_PGEX_RAYCASTWORLD
#undef OLC_PGEX_RAYCASTWORLD

#undef min
#undef max

void olc::rcw::Object::Walk(const float fWalkSpeed)
{
	fSpeed = fWalkSpeed;
	vel = olc::vf2d(std::cos(fHeading), std::sin(fHeading)) * fSpeed;
}

void olc::rcw::Object::Strafe(const float fStrafeSpeed)
{
	fSpeed = fStrafeSpeed;
	vel = olc::vf2d(std::cos(fHeading), std::sin(fHeading)).perp() * fSpeed;
}

void olc::rcw::Object::Turn(const float fTurnSpeed)
{
	fHeading += fTurnSpeed;

	// Wrap heading to sensible angle
	if (fHeading < -3.14159f) fHeading += 2.0f * 3.14159f;
	if (fHeading > 3.14159f) fHeading -= 2.0f * 3.14159f;
}


void olc::rcw::Object::Stop()
{
	fSpeed = 0;
	vel = { 0,0 };
}

olc::rcw::Engine::Engine(const int screen_w, const int screen_h, const float fov) :
	vScreenSize(screen_w, screen_h),
	vHalfScreenSize(screen_w / 2, screen_h / 2),
	vFloatScreenSize(float(screen_w), float(screen_h))
{
	fFieldOfView = fov;
	pDepthBuffer.reset(new float[vScreenSize.x * vScreenSize.y]);
}


void olc::rcw::Engine::SetCamera(const olc::vf2d& pos, const float heading)
{
	vCameraPos = pos;
	fCameraHeading = heading;
}


void olc::rcw::Engine::Update(float fElapsedTime)
{
	// Update the position and statically resolve for collisions against the map
	for (auto& ob : mapObjects)
	{
		std::shared_ptr<olc::rcw::Object> object = ob.second;
		if (!object->bIsActive) continue;

		int nSteps = 1;
		float fDelta = fElapsedTime;
		float fTotalTravel = (object->vel * fElapsedTime).mag2();
		float fTotalRadius = (object->fRadius * object->fRadius);

		if(fTotalTravel >= fTotalRadius)
		{
			float fSteps = std::ceil(fTotalTravel / fTotalRadius);
			nSteps = int(fSteps);
			fDelta = fElapsedTime / fSteps;
		}

		for (int nStep = 0; nStep < nSteps; nStep++)
		{
			// Determine where object is trying to be
			olc::vf2d vPotentialPosition = object->pos + object->vel * fDelta;

			// If the object can collide with other objects
			if (object->bCollideWithObjects)
			{
				// Iterate through all other objects (this can be costly)
				for (auto& ob2 : mapObjects)
				{
					std::shared_ptr<olc::rcw::Object> target = ob2.second;

					// Ignore if target object cant interact
					if (!target->bCollideWithObjects) continue;

					// Don't test against self
					if (target == object) continue;

					// Quick check to see if objects overlap...
					if ((target->pos - object->pos).mag2() <= (target->fRadius + object->fRadius) * (target->fRadius + object->fRadius))
					{
						// ..they do. Calculate displacement required
						float fDistance = (target->pos - object->pos).mag();
						float fOverlap = 1.0f * (fDistance - object->fRadius - target->fRadius);

						// Object will always give way to target
						vPotentialPosition -= (object->pos - target->pos) / fDistance * fOverlap;

						if (target->bCanBeMoved)
							target->pos += (object->pos - target->pos) / fDistance * fOverlap;

						if (object->bNotifyObjectCollision)
							HandleObjectVsObject(object, target);
					}


				}
			}

			// If the object can collide with scenery...
			if (object->bCollideWithScenery)
			{
				// ...Determine an area of cells to check for collision. We use a region
				// to account for diagonal collisions, and corner collisions.
				olc::vi2d vCurrentCell = object->pos;
				olc::vi2d vTargetCell = vPotentialPosition;
				olc::vi2d vAreaTL = { std::min(vCurrentCell.x, vTargetCell.x) - 1, std::min(vCurrentCell.y, vTargetCell.y) - 1 };
				olc::vi2d vAreaBR = { std::max(vCurrentCell.x, vTargetCell.x) + 1, std::max(vCurrentCell.y, vTargetCell.y) + 1 };




				// Iterate through each cell in test area
				olc::vi2d vCell;
				for (vCell.y = vAreaTL.y; vCell.y <= vAreaBR.y; vCell.y++)
				{
					for (vCell.x = vAreaTL.x; vCell.x <= vAreaBR.x; vCell.x++)
					{
						// Check if the cell is actually solid...
						olc::vf2d vCellMiddle = olc::vf2d(float(vCell.x) + 0.5f, float(vCell.y) + 0.5f);
						if (IsLocationSolid(vCellMiddle.x, vCellMiddle.y))
						{
							// ...it is! So work out nearest point to future player position, around perimeter
							// of cell rectangle. We can test the distance to this point to see if we have
							// collided.

							olc::vf2d vNearestPoint;
							// Inspired by this (very clever btw) 
							// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response
							vNearestPoint.x = std::max(float(vCell.x), std::min(vPotentialPosition.x, float(vCell.x + 1)));
							vNearestPoint.y = std::max(float(vCell.y), std::min(vPotentialPosition.y, float(vCell.y + 1)));

							// But modified to work :P
							olc::vf2d vRayToNearest = vNearestPoint - vPotentialPosition;
							float fOverlap = object->fRadius - vRayToNearest.mag();
							if (std::isnan(fOverlap)) fOverlap = 0;// Thanks Dandistine!

							// If overlap is positive, then a collision has occurred, so we displace backwards by the 
							// overlap amount. The potential position is then tested against other tiles in the area
							// therefore "statically" resolving the collision
							if (fOverlap > 0)
							{
								// Statically resolve the collision
								vPotentialPosition = vPotentialPosition - vRayToNearest.norm() * fOverlap;

								// Notify system that a collision has occurred
								if (object->bNotifySceneryCollision)
								{
									olc::rcw::Engine::CellSide side = olc::rcw::Engine::CellSide::Bottom;
									if (vNearestPoint.x == float(vCell.x)) side = olc::rcw::Engine::CellSide::West;
									if (vNearestPoint.x == float(vCell.x + 1)) side = olc::rcw::Engine::CellSide::East;
									if (vNearestPoint.y == float(vCell.y)) side = olc::rcw::Engine::CellSide::North;
									if (vNearestPoint.y == float(vCell.y + 1)) side = olc::rcw::Engine::CellSide::South;

									HandleObjectVsScenery(object, vCell.x, vCell.y, side, vNearestPoint.x - float(vCell.x), vNearestPoint.y - float(vCell.y));
								}
							}
						}
					}
				}
			}

			// Set the objects new position to the allowed potential position
			object->pos = vPotentialPosition;
		}
	}
}

void olc::rcw::Engine::Render()
{
	// Utility lambda to draw to screen and depth buffer
	auto DepthDraw = [&](int x, int y, float z, olc::Pixel p)
	{
		if (z <= pDepthBuffer[y * vScreenSize.x + x])
		{			
			pge->Draw(x, y, p);
			pDepthBuffer[y * vScreenSize.x + x] = z;
		}
	};

	
	// Clear screen and depth buffer ========================================
	// pge->Clear(olc::BLACK); <- Left to user to decide
	for (int i = 0; i < vScreenSize.x * vScreenSize.y; i++)
		pDepthBuffer[i] = INFINITY;

	// Draw World ===========================================================

	// For each column on screen...
	for (int x = 0; x < vScreenSize.x; x++)
	{
		// ...create a ray eminating from player position into world...
		float fRayAngle = (fCameraHeading - (fFieldOfView / 2.0f)) + (float(x) / vFloatScreenSize.x) * fFieldOfView;

		// ...create unit vector for that ray...
		olc::vf2d vRayDirection = { std::cos(fRayAngle), std::sin(fRayAngle) };

		// ... and cast ray into world, see what it hits (if anything)
		sTileHit hit;

		// Assuming it hits nothing, then we draw to the middle of the screen (far far away)				
		float fRayLength = INFINITY;

		// Otherwise...
		if (CastRayDDA(vCameraPos, vRayDirection, hit))
		{
			// It has hit something, so extract information to draw column
			olc::vf2d vRay = hit.vHitPos - vCameraPos;

			// Length of ray is vital for pseudo-depth, but we'll also cosine correct to remove fisheye
			fRayLength = vRay.mag() * std::cos(fRayAngle - fCameraHeading);
		}

		// Calculate locations in column that divides ceiling, wall and floor
		float fCeiling = (vFloatScreenSize.y / 2.0f) - (vFloatScreenSize.y / fRayLength);
		float fFloor = vFloatScreenSize.y - fCeiling;
		float fWallHeight = fFloor - fCeiling;
		float fFloorHeight = vFloatScreenSize.y - fFloor;

		// Now draw the column from top to bottom
		for (int y = 0; y < vScreenSize.y; y++)
		{
			if (y <= int(fCeiling))
			{
				// For floors and ceilings, we don't use the ray, instead we just pseudo-project
				// a plane, a la Mode 7. First calculate depth into screen...
				float fPlaneZ = (vFloatScreenSize.y / 2.0f) / ((vFloatScreenSize.y / 2.0f) - float(y));

				// ... then project polar coordinate (r, theta) from camera into screen (x, y), again
				// compensating with cosine to remove fisheye
				olc::vf2d vPlanePoint = vCameraPos + vRayDirection * fPlaneZ * 2.0f / std::cos(fRayAngle - fCameraHeading);

				// Work out which planar tile we are in
				int nPlaneTileX = int(vPlanePoint.x);
				int nPlaneTileY = int(vPlanePoint.y);

				// Work out normalised offset into planar tile
				float fPlaneSampleX = vPlanePoint.x - nPlaneTileX;
				float fPlaneSampleY = vPlanePoint.y - nPlaneTileY;

				// Location is marked as ceiling					
				olc::Pixel pixel = SelectSceneryPixel(nPlaneTileX, nPlaneTileY, olc::rcw::Engine::CellSide::Top, fPlaneSampleX, fPlaneSampleY, fPlaneZ);

				// Draw ceiling pixel - no depth buffer required
				pge->Draw(x, y, pixel);
			}
			else if (y > int(fCeiling) && y <= int(fFloor))
			{
				// Location is marked as wall. Here we will sample the appropriate
				// texture at the hit location. The U sample coordinate is provided
				// by the "hit" structure, though we will need to scan the V sample
				// coordinate based upon the height of the wall in screen space

				// Create normalised "V" based on height of wall --> (0.0 to 1.0)
				float fSampleY = (float(y) - fCeiling) / fWallHeight;

				// Select appropriate texture of that wall
				olc::Pixel pixel = SelectSceneryPixel(hit.vTilePos.x, hit.vTilePos.y, hit.eSide, hit.fSampleX, fSampleY, fRayLength);

				// Finally draw the screen pixel doing a depth test too
				DepthDraw(x, y, fRayLength, pixel);
			}
			else
			{
				// For floors and ceilings, we don't use the ray, instead we just pseudo-project
				// a plane, a la Mode 7. First calculate depth into screen...
				float fPlaneZ = (vFloatScreenSize.y / 2.0f) / (float(y) - (vFloatScreenSize.y / 2.0f));

				// ... then project polar coordinate (r, theta) from camera into screen (x, y), again
				// compensating with cosine to remove fisheye
				olc::vf2d vPlanePoint = vCameraPos + vRayDirection * fPlaneZ * 2.0f / std::cos(fRayAngle - fCameraHeading);

				// Work out which planar tile we are in
				int nPlaneTileX = int(vPlanePoint.x);
				int nPlaneTileY = int(vPlanePoint.y);

				// Work out normalised offset into planar tile
				float fPlaneSampleX = vPlanePoint.x - nPlaneTileX;
				float fPlaneSampleY = vPlanePoint.y - nPlaneTileY;

				// Location is marked as floor
				olc::Pixel pixel = SelectSceneryPixel(nPlaneTileX, nPlaneTileY, olc::rcw::Engine::CellSide::Bottom, fPlaneSampleX, fPlaneSampleY, fPlaneZ);

				// Draw floor pixel - no depth buffer required
				pge->Draw(x, y, pixel);
			}
		}
	}

	// Scenery is now drawn, and depth buffer is filled. We can now draw
	// the ingame objects. Assuming binary transparency, we've no need to
	// sort objects

	// Iterate through all in game objects
	for (const auto& ob : mapObjects)
	{
		const std::shared_ptr<olc::rcw::Object> object = ob.second;

		// If object is invisible, nothing to do - this is useful
		// for both effects, and making sure we dont render the
		// "player" at the camera location perhaps
		if (!object->bVisible) continue;

		// Create vector from camera to object
		olc::vf2d vObject = object->pos - vCameraPos;

		// Calculate distance object is away from camera
		float fDistanceToObject = vObject.mag();

		// Check if object center is within camera FOV...
		float fObjectAngle = atan2f(vObject.y, vObject.x) - fCameraHeading;
		if (fObjectAngle < -3.14159f) fObjectAngle += 2.0f * 3.14159f;
		if (fObjectAngle > 3.14159f) fObjectAngle -= 2.0f * 3.14159f;

		// ...with a bias based upon distance - allows us to have object centers offscreen
		bool bInPlayerFOV = fabs(fObjectAngle) < (fFieldOfView + (1.0f / fDistanceToObject)) / 2.0f;

		// If object is within view, and not too close to camera, draw it!
		if (bInPlayerFOV && vObject.mag() >= 0.5f)
		{
			// Work out its position on the floor...
			olc::vf2d vFloorPoint;

			// Horizontal screen location is determined based on object angle relative to camera heading
			vFloorPoint.x = (0.5f * ((fObjectAngle / (fFieldOfView * 0.5f))) + 0.5f) * vFloatScreenSize.x;

			// Vertical screen location is projected distance
			vFloorPoint.y = (vFloatScreenSize.y / 2.0f) + (vFloatScreenSize.y / fDistanceToObject) / std::cos(fObjectAngle / 2.0f);

			// First we need the objects size...
			olc::vf2d vObjectSize = { float(GetObjectWidth(object->nGenericID)), float(GetObjectHeight(object->nGenericID)) };

			// ...which we can scale into world space (maintaining aspect ratio)...
			vObjectSize *= 2.0f * vFloatScreenSize.y;

			// ...then project into screen space
			vObjectSize /= fDistanceToObject;

			// Second we need the objects top left position in screen space...
			olc::vf2d vObjectTopLeft;

			// ...which is relative to the objects size and assumes the middle of the object is
			// the location in world space
			vObjectTopLeft = { vFloorPoint.x - vObjectSize.x / 2.0f, vFloorPoint.y - vObjectSize.y };

			// Now iterate through the objects screen pixels
			for (float y = 0; y < vObjectSize.y; y++)
			{
				for (float x = 0; x < vObjectSize.x; x++)
				{
					// Create a normalised sample coordinate
					float fSampleX = x / vObjectSize.x;
					float fSampleY = y / vObjectSize.y;

					// Get pixel from a suitable texture
					float fNiceAngle = fCameraHeading - object->fHeading + 3.14159f / 4.0f;
					if (fNiceAngle < 0) fNiceAngle += 2.0f * 3.14159f;
					if (fNiceAngle > 2.0f * 3.14159f) fNiceAngle -= 2.0f * 3.14159f;
					olc::Pixel p = SelectObjectPixel(object->nGenericID, fSampleX, fSampleY, fDistanceToObject, fNiceAngle);

					// Calculate screen pixel location
					olc::vi2d a = { int(vObjectTopLeft.x + x), int(vObjectTopLeft.y + y) };

					// Check if location is actually on screen (to not go OOB on depth buffer)
					// and if the pixel is indeed visible (has no transparency component)
					if (a.x >= 0 && a.x < vScreenSize.x && a.y >= 0 && a.y < vScreenSize.y && p.a == 255)
					{
						// Draw the pixel taking into account the depth buffer
						DepthDraw(a.x, a.y, fDistanceToObject, p);
					}
				}
			}
		}
	}
}

void olc::rcw::Engine::HandleObjectVsScenery(std::shared_ptr<olc::rcw::Object> object, const int tile_x, const int tile_y, const olc::rcw::Engine::CellSide side, const float offset_x, const float offset_y)
{}

void olc::rcw::Engine::HandleObjectVsObject(std::shared_ptr<olc::rcw::Object> object1, std::shared_ptr<olc::rcw::Object> object2)
{}

// Will be explained in upcoming video...
bool olc::rcw::Engine::CastRayDDA(const olc::vf2d& vOrigin, const olc::vf2d& vDirection, sTileHit& hit)
{
	olc::vf2d vRayDelta = { sqrt(1 + (vDirection.y / vDirection.x) * (vDirection.y / vDirection.x)), sqrt(1 + (vDirection.x / vDirection.y) * (vDirection.x / vDirection.y)) };

	olc::vi2d vMapCheck = vOrigin;
	olc::vf2d vSideDistance;
	olc::vi2d vStepDistance;

	if (vDirection.x < 0)
	{
		vStepDistance.x = -1;
		vSideDistance.x = (vOrigin.x - (float)vMapCheck.x) * vRayDelta.x;
	}
	else
	{
		vStepDistance.x = 1;
		vSideDistance.x = ((float)vMapCheck.x + 1.0f - vOrigin.x) * vRayDelta.x;
	}

	if (vDirection.y < 0)
	{
		vStepDistance.y = -1;
		vSideDistance.y = (vOrigin.y - (float)vMapCheck.y) * vRayDelta.y;
	}
	else
	{
		vStepDistance.y = 1;
		vSideDistance.y = ((float)vMapCheck.y + 1.0f - vOrigin.y) * vRayDelta.y;
	}

	olc::vf2d vIntersection;
	olc::vi2d vHitTile;
	float fMaxDistance = 100.0f;
	float fDistance = 0.0f;
	bool bTileFound = false;
	while (!bTileFound && fDistance < fMaxDistance)
	{
		if (vSideDistance.x < vSideDistance.y)
		{
			vSideDistance.x += vRayDelta.x;
			vMapCheck.x += vStepDistance.x;
		}
		else
		{
			vSideDistance.y += vRayDelta.y;
			vMapCheck.y += vStepDistance.y;
		}

		olc::vf2d rayDist = { (float)vMapCheck.x - vOrigin.x, (float)vMapCheck.y - vOrigin.y };
		fDistance = rayDist.mag();


		if (IsLocationSolid(float(vMapCheck.x), float(vMapCheck.y)))
		{
			vHitTile = vMapCheck;
			bTileFound = true;

			hit.vTilePos = vMapCheck;


			// Find accurate Hit Location

			float m = vDirection.y / vDirection.x;


			// From Top Left

			if (vOrigin.y <= vMapCheck.y)
			{
				if (vOrigin.x <= vMapCheck.x)
				{
					hit.eSide = olc::rcw::Engine::CellSide::West;
					vIntersection.y = m * (vMapCheck.x - vOrigin.x) + vOrigin.y;
					vIntersection.x = float(vMapCheck.x);
					hit.fSampleX = vIntersection.y - std::floor(vIntersection.y);
				}
				else if (vOrigin.x >= (vMapCheck.x + 1))
				{
					hit.eSide = olc::rcw::Engine::CellSide::East;
					vIntersection.y = m * ((vMapCheck.x + 1) - vOrigin.x) + vOrigin.y;
					vIntersection.x = float(vMapCheck.x + 1);
					hit.fSampleX = vIntersection.y - std::floor(vIntersection.y);
				}
				else
				{
					hit.eSide = olc::rcw::Engine::CellSide::North;
					vIntersection.y = float(vMapCheck.y);
					vIntersection.x = (vMapCheck.y - vOrigin.y) / m + vOrigin.x;
					hit.fSampleX = vIntersection.x - std::floor(vIntersection.x);
				}


				if (vIntersection.y < vMapCheck.y)
				{
					hit.eSide = olc::rcw::Engine::CellSide::North;
					vIntersection.y = float(vMapCheck.y);
					vIntersection.x = (vMapCheck.y - vOrigin.y) / m + vOrigin.x;
					hit.fSampleX = vIntersection.x - std::floor(vIntersection.x);
				}
			}
			else if (vOrigin.y >= vMapCheck.y + 1)
			{
				if (vOrigin.x <= vMapCheck.x)
				{
					hit.eSide = olc::rcw::Engine::CellSide::West;
					vIntersection.y = m * (vMapCheck.x - vOrigin.x) + vOrigin.y;
					vIntersection.x = float(vMapCheck.x);
					hit.fSampleX = vIntersection.y - std::floor(vIntersection.y);
				}
				else if (vOrigin.x >= (vMapCheck.x + 1))
				{
					hit.eSide = olc::rcw::Engine::CellSide::East;
					vIntersection.y = m * ((vMapCheck.x + 1) - vOrigin.x) + vOrigin.y;
					vIntersection.x = float(vMapCheck.x + 1);
					hit.fSampleX = vIntersection.y - std::floor(vIntersection.y);
				}
				else
				{
					hit.eSide = olc::rcw::Engine::CellSide::South;
					vIntersection.y = float(vMapCheck.y + 1);
					vIntersection.x = ((vMapCheck.y + 1) - vOrigin.y) / m + vOrigin.x;
					hit.fSampleX = vIntersection.x - std::floor(vIntersection.x);
				}

				if (vIntersection.y > (vMapCheck.y + 1))
				{
					hit.eSide = olc::rcw::Engine::CellSide::South;
					vIntersection.y = float(vMapCheck.y + 1);
					vIntersection.x = ((vMapCheck.y + 1) - vOrigin.y) / m + vOrigin.x;
					hit.fSampleX = vIntersection.x - std::floor(vIntersection.x);
				}
			}
			else
			{
				if (vOrigin.x <= vMapCheck.x)
				{
					hit.eSide = olc::rcw::Engine::CellSide::West;
					vIntersection.y = m * (vMapCheck.x - vOrigin.x) + vOrigin.y;
					vIntersection.x = float(vMapCheck.x);
					hit.fSampleX = vIntersection.y - std::floor(vIntersection.y);
				}
				else if (vOrigin.x >= (vMapCheck.x + 1))
				{
					hit.eSide = olc::rcw::Engine::CellSide::East;
					vIntersection.y = m * ((vMapCheck.x + 1) - vOrigin.x) + vOrigin.y;
					vIntersection.x = float(vMapCheck.x + 1);
					hit.fSampleX = vIntersection.y - std::floor(vIntersection.y);
				}
			}

			hit.vHitPos = vIntersection;
		}
	}

	return bTileFound;
}


#endif // OLC_PGEX_RAYCASTWORLD
#endif // OLC_PGEX_RAYCASTWORLD_H
