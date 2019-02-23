/*
	OneLoneCoder_PGE_ShadowCasting2D.cpp

	What is this?
	~~~~~~~~~~~~~
	This is an implementation of two algorithms that work together
	to produuce a "line-of-sight" or "shadow casting" effect depending
	upon your perspective. To use it compile with olcPixelGameEngine.h,
	place/remove blocks with left click and hold down right mouse button
	to illuminate the scene from the mouse cursor.

	The first algorithm converts a tile map into a set of geometric 
	primitives whcih are more conveninet to use in geometric programs
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
#include <cassert>
using namespace std;

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

/*
 * atan2f() chart;  float dx = ex - sx, dy...
 *
 *       Name == (dx, dy) ==  deg ==   rad ==     CPP equiv
 * ￼
 *       East == ( 1,  0) ==    0 ==  0.00 ==     0 == 0.0f
 * North East == ( 1, -1) ==  -45 == -0.79 ==       -M_PI_4
 *      North == ( 0, -1) ==  -90 == -1.57 ==       -M_PI_2
 * North West == (-1, -1) == -135 == -2.36 == M_PI_4 - M_PI
 *       West == (-1,  0) ==  180 ==  3.14 ==	       M_PI
 * South West == (-1,  1) ==  135 ==  2.36 == M_PI - M_PI_4
 *      South == ( 0,  1) ==   90 ==  1.57 ==	     M_PI_2
 * South East == ( 1,  1) ==   45 ==  0.79 ==	     M_PI_4
 *
 * Notice: As the Y-Axis is flipped(Quadrant III) atan is also flipped to CC roatation.
 * 
 */

class cATan
{
      public:
	float aTan;
	// operator float&() { return this->aTan; }

	cATan() = default;
	cATan(float _tan) : aTan(_tan) {}
	cATan(const cATan& t) : aTan(t.aTan) {}
	cATan(float dy, float dx): aTan(atan2f(dy, dx)) {}

	cATan add(float rhs)
	{

		float rad;
		rad = fmod(rhs, M_PI);

		cATan ret;
		ret.aTan = aTan + rad;

		if(ret.aTan > M_PI)
			ret.aTan -= M_PI * 2;

		return ret;
	}

	cATan subtract(float rhs)
	{

		float rad;
		rad = fmod(rhs, M_PI);

		cATan ret;
		ret.aTan = aTan - rad;

		if(ret.aTan <= -M_PI)
			ret.aTan += M_PI * 2;

		return ret;
	}

	// Exclusive.
	bool inRange(float start, float end)
	{
                // assert(start != end);

                if (start > end)
                // I ordered these from least to gratest when true.
			return aTan < end || start < aTan;
		return start < aTan && aTan < end;
	}

};

struct sEdge
{
	// Counter clockwise winding, like OpenGL/Vulkan.
	float sx, sy; // Start coordinate
	float ex, ey; // End coordinate
	// With this we can reduce the work by 1/3.
	cATan normal; // Direction edge is facing
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
							vecEdges[world[n].edge_id[WEST]].sy += fBlockWidth;
							world[i].edge_id[WEST] = world[n].edge_id[WEST];
							world[i].edge_exist[WEST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							sEdge edge;
							edge.ex = (sx + x) * fBlockWidth; edge.ey = (sy + y) * fBlockWidth;
							edge.sx = edge.ex; edge.sy = edge.ey + fBlockWidth;
							edge.normal = cATan(edge.ey - edge.sy, edge.ex - edge.sx).subtract(M_PI_2);

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
							edge.normal = cATan(edge.ey - edge.sy, edge.ex - edge.sx).subtract(M_PI_2);

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
							edge.normal = cATan(edge.ey - edge.sy, edge.ex - edge.sx).subtract(M_PI_2);

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
							vecEdges[world[w].edge_id[SOUTH]].sx += fBlockWidth;
							world[i].edge_id[SOUTH] = world[w].edge_id[SOUTH];
							world[i].edge_exist[SOUTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so I need to create one
							sEdge edge;
							edge.ex = (sx + x) * fBlockWidth; edge.ey = (sy + y + 1) * fBlockWidth;
							edge.sx = edge.ex + fBlockWidth; edge.sy = edge.ey;
							edge.normal = cATan(edge.ey - edge.sy, edge.ex - edge.sx).subtract(M_PI_2);

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

		// For each edge in PolyMap, final check for CCW.
                // Once again ordered for <
		for (auto &e : vecEdges)
		{
		       if (e.sy == e.ey) {
				assert(e.sx != e.ex && "Line is a point");
                                // e.normal.aTan == M_PI_2
				if (0.0f < e.normal.aTan) {
					if (e.sx < e.ex) {
						cerr << "opps: e.sx(" << e.sx << ") < e.ex(" << e.ex << "): " << e.normal.aTan << endl;
					}
				} else if(e.ex < e.sx) {
					cerr << "opps: e.ex(" << e.ex << ") < e.sx(" << e.sx << "): " << e.normal.aTan << endl;
				}
		       } else {
				if (e.normal.inRange(-M_PI_2, M_PI_2)) {
					if (e.ey < e.sy) {
						cerr << "opps: e.ey(" << e.ey << ") < e.sy(" << e.sy << "): " << e.normal.aTan << endl;
					}
				} else if (e.sy < e.ey) {
					cerr << "opps: e.sy(" << e.sy << ") < e.ey(" << e.ey << "): " << e.normal.aTan << endl;
				}
		       }
		}
	}

	struct sCollision
	{
		float min_t1 = INFINITY;
		float min_px = 0, min_py = 0, min_ang = 0;
		bool bValid = false;

		float rdx, rdy;
		float ox, oy;

		void digest(sEdge& e2)
		{
			// Create line segment vector
			float sdx = e2.ex - e2.sx;
			float sdy = e2.ey - e2.sy;

			// Just at the endge of our vision?
			if(!(fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f))
				return;

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
	};

	void CalculateVisibilityPolygon(float ox, float oy, float radius)
	{
		// Get rid of existing polygon
		vecVisibilityPolygonPoints.clear();

		// For each edge in PolyMap
		vector<tuple<sEdge, cATan>> vecInterestingEdges;
		for (auto &e : vecEdges)
		{
			tuple<sEdge, cATan> canidate(e, cATan(e.sy - oy, e.sx - ox));

			// This should skip almost half of the Edges.
			if (get<cATan>(canidate).inRange(e.normal.subtract(M_PI_2).aTan, e.normal.add(M_PI_2).aTan))
				continue;

			vecInterestingEdges.push_back(canidate);
		}

		for (auto &i1 : vecInterestingEdges)
		{
			auto &e1 = get<sEdge>(i1);
			auto &a = get<cATan>(i1);

			// Take the start point, then the end point (we could use a pool of
			// non-duplicated points here, it would be more optimal)
			for (int i = 0; i < 2; i++)
			{
				cATan ang;
				if (i == 0) { ang = get<cATan>(i1); } else
					ang = cATan(e1.ey - oy, e1.ex - ox);

				float ang_adjust = i == 0 ? 0.0001f : -0.0001f;

				sCollision hit;
				hit.rdx = radius * cosf(ang.subtract(ang_adjust).aTan);
				hit.rdy = radius * sinf(ang.subtract(ang_adjust).aTan);

				sCollision miss;
				miss.rdx = radius * cosf(ang.add(ang_adjust).aTan);
				miss.rdy = radius * sinf(ang.add(ang_adjust).aTan);

				hit.ox = miss.ox = ox;
				hit.oy = miss.oy = oy;

				// Check for ray intersection with all edges
				for (auto &i2 : vecInterestingEdges)
				{
					hit.digest(get<sEdge>(i2));
					miss.digest(get<sEdge>(i2));
				}

				if(hit.bValid)// Add intersection point to visibility polygon perimeter
					vecVisibilityPolygonPoints.push_back({ hit.min_ang, hit.min_px, hit.min_py });
				if(miss.bValid)// Add intersection point to visibility polygon perimeter
					vecVisibilityPolygonPoints.push_back({ miss.min_ang, miss.min_px, miss.min_py });

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

		// Right click didn't do anything on Linux/X11
#define SHINE_LIGHT (GetMouse(1).bHeld || GetKey(olc::Key::Z).bHeld)
		if (SHINE_LIGHT)
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
		if (SHINE_LIGHT && vecVisibilityPolygonPoints.size() > 1)
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
