/*
	OneLoneCoder_PGE_ShadowCasting2D.cpp

	What is this?
	~~~~~~~~~~~~~
	This is an implementation of two algorithms that work together
	to produce a "line-of-sight" or "shadow casting" effect depending
	upon your perspective. To use it compile with olcPixelGameEngine.h,
	place/remove blocks with left click and hold down right mouse button
	to illuminate the scene from the mouse cursor.

	The first algorithm converts a tile map into a set of geometric
	primitives which are more convenient to use in geometric programs
	such as this. The second algorithm casts rays to create a triangle
	fan that represents the visible area form the source.

	Thanks to these resources for ideas
	https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	https://www.redblobgames.com/articles/visibility/
	https://ncase.me/sight-and-light/


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

	Relevant Videos
	~~~~~~~~~~~~~~~
	https://youtu.be/kRH6oJLFYxY Introducing olcPixelGameEngine
	--> https://youtu.be/fc3nnG2CG8U Video about this very code!

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018
*/
#include <iostream>
#include <algorithm>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"


struct sEdge
{
	float sx, sy; // Start coordinate
	float ex, ey; // End coordinate
};

struct sCell
{
	int edge_id[4];
	bool edge_exist[4];
	bool exist = false;
};

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

class ShadowCasting2D : public olc::PixelGameEngine
{
public:
	ShadowCasting2D()
	{
		sAppName = "ShadowCasting2D";
	}

private:
	sCell* world;
	int nWorldWidth = 40;
	int nWorldHeight = 30;

	olc::Sprite *sprLightCast;
	olc::Sprite *buffLightRay;
	olc::Sprite *buffLightTex;

	vector<sEdge> vecEdges;

	//				angle	x	y
	vector<tuple<float, float, float>> vecVisibilityPolygonPoints;

	void ConvertTileMapToPolyMap(int sx, int sy, int w, int h, float fBlockWidth, int pitch)
	{
		// Clear "PolyMap"
		vecEdges.clear();

		for (int x = 0; x < w; x++)
			for (int y = 0; y < h; y++)
				for (int j = 0; j < 4; j++)
				{
					world[(y + sy) * pitch + (x + sx)].edge_exist[j] = false;
					world[(y + sy) * pitch + (x + sx)].edge_id[j] = 0;
				}

		// Iterate through region from top left to bottom right
		for (int x = 1; x < w - 1; x++)
			for (int y = 1; y < h - 1; y++)
			{
				// Create some convenient indices
				int i = (y + sy) * pitch + (x + sx);			// This
				int n = (y + sy - 1) * pitch + (x + sx);		// Northern Neighbour
				int s = (y + sy + 1) * pitch + (x + sx);		// Southern Neighbour
				int w = (y + sy) * pitch + (x + sx - 1);	// Western Neighbour
				int e = (y + sy) * pitch + (x + sx + 1);	// Eastern Neighbour

				// If this cell exists, check if it needs edges
				if (world[i].exist)
				{
					// If this cell has no western neighbour, it needs a western edge
					if (!world[w].exist)
					{
						// It can either extend it from its northern neighbour if they have
						// one, or It can start a new one.
						if (world[n].edge_exist[WEST])
						{
							// Northern neighbour has a western edge, so grow it downwards
							vecEdges[world[n].edge_id[WEST]].ey += fBlockWidth;
							world[i].edge_id[WEST] = world[n].edge_id[WEST];
							world[i].edge_exist[WEST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							sEdge edge;
							edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
							edge.ex = edge.sx; edge.ey = edge.sy + fBlockWidth;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_id[WEST] = edge_id;
							world[i].edge_exist[WEST] = true;
						}
					}

					// If this cell dont have an eastern neignbour, It needs a eastern edge
					if (!world[e].exist)
					{
						// It can either extend it from its northern neighbour if they have
						// one, or It can start a new one.
						if (world[n].edge_exist[EAST])
						{
							// Northern neighbour has one, so grow it downwards
							vecEdges[world[n].edge_id[EAST]].ey += fBlockWidth;
							world[i].edge_id[EAST] = world[n].edge_id[EAST];
							world[i].edge_exist[EAST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							sEdge edge;
							edge.sx = (sx + x + 1) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
							edge.ex = edge.sx; edge.ey = edge.sy + fBlockWidth;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_id[EAST] = edge_id;
							world[i].edge_exist[EAST] = true;
						}
					}

					// If this cell doesnt have a northern neignbour, It needs a northern edge
					if (!world[n].exist)
					{
						// It can either extend it from its western neighbour if they have
						// one, or It can start a new one.
						if (world[w].edge_exist[NORTH])
						{
							// Western neighbour has one, so grow it eastwards
							vecEdges[world[w].edge_id[NORTH]].ex += fBlockWidth;
							world[i].edge_id[NORTH] = world[w].edge_id[NORTH];
							world[i].edge_exist[NORTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so create one
							sEdge edge;
							edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y) * fBlockWidth;
							edge.ex = edge.sx + fBlockWidth; edge.ey = edge.sy;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_id[NORTH] = edge_id;
							world[i].edge_exist[NORTH] = true;
						}
					}

					// If this cell doesnt have a southern neignbour, It needs a southern edge
					if (!world[s].exist)
					{
						// It can either extend it from its western neighbour if they have
						// one, or It can start a new one.
						if (world[w].edge_exist[SOUTH])
						{
							// Western neighbour has one, so grow it eastwards
							vecEdges[world[w].edge_id[SOUTH]].ex += fBlockWidth;
							world[i].edge_id[SOUTH] = world[w].edge_id[SOUTH];
							world[i].edge_exist[SOUTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so I need to create one
							sEdge edge;
							edge.sx = (sx + x) * fBlockWidth; edge.sy = (sy + y + 1) * fBlockWidth;
							edge.ex = edge.sx + fBlockWidth; edge.ey = edge.sy;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							world[i].edge_id[SOUTH] = edge_id;
							world[i].edge_exist[SOUTH] = true;
						}
					}

				}

			}
	}

	void CalculateVisibilityPolygon(float ox, float oy, float radius)
	{
		// Get rid of existing polygon
		vecVisibilityPolygonPoints.clear();

		// For each edge in PolyMap
		for (auto &e1 : vecEdges)
		{
			// Take the start point, then the end point (we could use a pool of
			// non-duplicated points here, it would be more optimal)
			for (int i = 0; i < 2; i++)
			{
				float rdx, rdy;
				rdx = (i == 0 ? e1.sx : e1.ex) - ox;
				rdy = (i == 0 ? e1.sy : e1.ey) - oy;

				float base_ang = atan2f(rdy, rdx);

				float ang = 0;
				// For each point, cast 3 rays, 1 directly at point
				// and 1 a little bit either side
				for (int j = 0; j < 3; j++)
				{
					if (j == 0)	ang = base_ang - 0.0001f;
					if (j == 1)	ang = base_ang;
					if (j == 2)	ang = base_ang + 0.0001f;

					// Create ray along angle for required distance
					rdx = radius * cosf(ang);
					rdy = radius * sinf(ang);

					float min_t1 = INFINITY;
					float min_px = 0, min_py = 0, min_ang = 0;
					bool bValid = false;

					// Check for ray intersection with all edges
					for (auto &e2 : vecEdges)
					{
						// Create line segment vector
						float sdx = e2.ex - e2.sx;
						float sdy = e2.ey - e2.sy;

						if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f)
						{
							// t2 is normalised distance from line segment start to line segment end of intersect point
							float t2 = (rdx * (e2.sy - oy) + (rdy * (ox - e2.sx))) / (sdx * rdy - sdy * rdx);
							// t1 is normalised distance from source along ray to ray length of intersect point
							float t1 = (e2.sx + sdx * t2 - ox) / rdx;

							// If intersect point exists along ray, and along line
							// segment then intersect point is valid
							if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
							{
								// Check if this intersect point is closest to source. If
								// it is, then store this point and reject others
								if (t1 < min_t1)
								{
									min_t1 = t1;
									min_px = ox + rdx * t1;
									min_py = oy + rdy * t1;
									min_ang = atan2f(min_py - oy, min_px - ox);
									bValid = true;
								}
							}
						}
					}

					if(bValid)// Add intersection point to visibility polygon perimeter
						vecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
				}
			}
		}

		// Sort perimeter points by angle from source. This will allow
		// us to draw a triangle fan.
		sort(
			vecVisibilityPolygonPoints.begin(),
			vecVisibilityPolygonPoints.end(),
			[&](const tuple<float, float, float> &t1, const tuple<float, float, float> &t2)
			{
				return get<0>(t1) < get<0>(t2);
			});

	}


public:
	bool OnUserCreate() override
	{
		world = new sCell[nWorldWidth * nWorldHeight];

		// Add a boundary to the world
		for (int x = 1; x < (nWorldWidth - 1); x++)
		{
			world[1 * nWorldWidth + x].exist = true;
			world[(nWorldHeight - 2) * nWorldWidth + x].exist = true;
		}

		for (int x = 1; x < (nWorldHeight - 1); x++)
		{
			world[x * nWorldWidth + 1].exist = true;
			world[x * nWorldWidth + (nWorldWidth - 2)].exist = true;
		}

		sprLightCast = new olc::Sprite("light_cast.png");

		// Create some screen-sized off-screen buffers for lighting effect
		buffLightTex = new olc::Sprite(ScreenWidth(), ScreenHeight());
		buffLightRay = new olc::Sprite(ScreenWidth(), ScreenHeight());
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		float fBlockWidth = 16.0f;
		float fSourceX = GetMouseX();
		float fSourceY = GetMouseY();

		// Set tile map blocks to on or off
		if (GetMouse(0).bReleased)
		{
			// i = y * width + x
			int i = ((int)fSourceY / (int)fBlockWidth) * nWorldWidth + ((int)fSourceX / (int)fBlockWidth);
			world[i].exist = !world[i].exist;
		}

		// Take a region of "TileMap" and convert it to "PolyMap" - This is done
		// every frame here, but could be a pre-processing stage depending on
		// how your final application interacts with tilemaps
		ConvertTileMapToPolyMap(0, 0, 40, 30, fBlockWidth, nWorldWidth);


		if (GetMouse(1).bHeld)
		{
			CalculateVisibilityPolygon(fSourceX, fSourceY, 1000.0f);
		}



		// Drawing
		SetDrawTarget(nullptr);
		Clear(olc::BLACK);


		int nRaysCast = vecVisibilityPolygonPoints.size();

		// Remove duplicate (or simply similar) points from polygon
		auto it = unique(
			vecVisibilityPolygonPoints.begin(),
			vecVisibilityPolygonPoints.end(),
			[&](const tuple<float, float, float> &t1, const tuple<float, float, float> &t2)
			{
				return fabs(get<1>(t1) - get<1>(t2)) < 0.1f && fabs(get<2>(t1) - get<2>(t2)) < 0.1f;
			});

		vecVisibilityPolygonPoints.resize(distance(vecVisibilityPolygonPoints.begin(), it));

		int nRaysCast2 = vecVisibilityPolygonPoints.size();
		DrawString(4, 4, "Rays Cast: " + to_string(nRaysCast) + " Rays Drawn: " + to_string(nRaysCast2));


		// If drawing rays, set an offscreen texture as our target buffer
		if (GetMouse(1).bHeld && vecVisibilityPolygonPoints.size() > 1)
		{
			// Clear offscreen buffer for sprite
			SetDrawTarget(buffLightTex);
			Clear(olc::BLACK);

			// Draw "Radial Light" sprite to offscreen buffer, centered around
			// source location (the mouse coordinates, buffer is 512x512)
			DrawSprite(fSourceX - 255, fSourceY - 255, sprLightCast);

			// Clear offsecreen buffer for rays
			SetDrawTarget(buffLightRay);
			Clear(olc::BLANK);

			// Draw each triangle in fan
			for (int i = 0; i < vecVisibilityPolygonPoints.size() - 1; i++)
			{
				FillTriangle(
					fSourceX,
					fSourceY,

					get<1>(vecVisibilityPolygonPoints[i]),
					get<2>(vecVisibilityPolygonPoints[i]),

					get<1>(vecVisibilityPolygonPoints[i + 1]),
					get<2>(vecVisibilityPolygonPoints[i + 1]));

			}

			// Fan will have one open edge, so draw last point of fan to first
			FillTriangle(
				fSourceX,
				fSourceY,

				get<1>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),
				get<2>(vecVisibilityPolygonPoints[vecVisibilityPolygonPoints.size() - 1]),

				get<1>(vecVisibilityPolygonPoints[0]),
				get<2>(vecVisibilityPolygonPoints[0]));

			// Wherever rays exist in ray sprite, copy over radial light sprite pixels
			SetDrawTarget(nullptr);
			for (int x = 0; x < ScreenWidth(); x++)
				for (int y = 0; y < ScreenHeight(); y++)
					if (buffLightRay->GetPixel(x, y).r > 0)
						Draw(x, y, buffLightTex->GetPixel(x, y));
		}



		// Draw Blocks from TileMap
		for (int x = 0; x < nWorldWidth; x++)
			for (int y = 0; y < nWorldHeight; y++)
			{
				if (world[y * nWorldWidth + x].exist)
					FillRect(x * fBlockWidth, y * fBlockWidth, fBlockWidth, fBlockWidth, olc::BLUE);
			}

		// Draw Edges from PolyMap
		for (auto &e : vecEdges)
		{
			DrawLine(e.sx, e.sy, e.ex, e.ey);
			FillCircle(e.sx, e.sy, 3, olc::RED);
			FillCircle(e.ex, e.ey, 3, olc::RED);
		}

		return true;
	}
};





int main()
{
	ShadowCasting2D demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();
}
