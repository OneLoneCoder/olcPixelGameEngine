/*
	Convex Polygon Collision Detection
	"Don't you dare try concave ones..." - javidx9

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
	Use arrow keys to control pentagon
	Use WASD to control triangle
	F1..F4 selects algorithm

	Relevant Video: https://youtu.be/7Ik2vowGcU0

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

#include <vector>
#include <algorithm>

// Override base class with your custom functionality
class PolygonCollisions : public olc::PixelGameEngine
{
public:
	PolygonCollisions()
	{
		sAppName = "Polygon Collisions";
	}

	struct vec2d
	{
		float x;
		float y;
	};

	struct polygon
	{
		std::vector<vec2d> p;	// Transformed Points
		vec2d pos;				// Position of shape
		float angle;			// Direction of shape
		std::vector<vec2d> o;	// "Model" of shape							
		bool overlap = false;	// Flag to indicate if overlap has occurred
	};

	std::vector<polygon> vecShapes;

	int nMode = 0;

public:
	bool OnUserCreate() override
	{		
		// Create Pentagon
		polygon s1;
		float fTheta = 3.14159f * 2.0f / 5.0f;
		s1.pos = { 100, 100 };
		s1.angle = 0.0f;
		for (int i = 0; i < 5; i++)
		{
			s1.o.push_back({ 30.0f * cosf(fTheta * i), 30.0f * sinf(fTheta * i) });
			s1.p.push_back({ 30.0f * cosf(fTheta * i), 30.0f * sinf(fTheta * i) });
		}

		// Create Triangle
		polygon s2;
		fTheta = 3.14159f * 2.0f / 3.0f;
		s2.pos = { 200, 150 };
		s2.angle = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			s2.o.push_back({ 20.0f * cosf(fTheta * i), 20.0f * sinf(fTheta * i) });
			s2.p.push_back({ 20.0f * cosf(fTheta * i), 20.0f * sinf(fTheta * i) });
		}

		// Create Quad
		polygon s3;
		s3.pos = { 50, 200 };
		s3.angle = 0.0f;
		s3.o.push_back({ -30, -30 });
		s3.o.push_back({ -30, +30 });
		s3.o.push_back({ +30, +30 });
		s3.o.push_back({ +30, -30 });
		s3.p.resize(4);
		
			
		vecShapes.push_back(s1);
		vecShapes.push_back(s2);
		vecShapes.push_back(s3);
		return true;
	}



	bool ShapeOverlap_SAT(polygon &r1, polygon &r2)
	{
		polygon *poly1 = &r1;
		polygon *poly2 = &r2;

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}
		
			for (int a = 0; a < poly1->p.size(); a++)
			{
				int b = (a + 1) % poly1->p.size();
				vec2d axisProj = { -(poly1->p[b].y - poly1->p[a].y), poly1->p[b].x - poly1->p[a].x };
				float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
				axisProj = { axisProj.x / d, axisProj.y / d };

				// Work out min and max 1D points for r1
				float min_r1 = INFINITY, max_r1 = -INFINITY;
				for (int p = 0; p < poly1->p.size(); p++)
				{
					float q = (poly1->p[p].x * axisProj.x + poly1->p[p].y * axisProj.y);
					min_r1 = std::min(min_r1, q);
					max_r1 = std::max(max_r1, q);
				}

				// Work out min and max 1D points for r2
				float min_r2 = INFINITY, max_r2 = -INFINITY;
				for (int p = 0; p < poly2->p.size(); p++)
				{
					float q = (poly2->p[p].x * axisProj.x + poly2->p[p].y * axisProj.y);
					min_r2 = std::min(min_r2, q);
					max_r2 = std::max(max_r2, q);
				}

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return false;
			}
		}

		return true;
	}

	bool ShapeOverlap_SAT_STATIC(polygon &r1, polygon &r2)
	{
		polygon *poly1 = &r1;
		polygon *poly2 = &r2;

		float overlap = INFINITY;
		
		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}

			for (int a = 0; a < poly1->p.size(); a++)
			{
				int b = (a + 1) % poly1->p.size();
				vec2d axisProj = { -(poly1->p[b].y - poly1->p[a].y), poly1->p[b].x - poly1->p[a].x };
				
				// Optional normalisation of projection axis enhances stability slightly
				//float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
				//axisProj = { axisProj.x / d, axisProj.y / d };

				// Work out min and max 1D points for r1
				float min_r1 = INFINITY, max_r1 = -INFINITY;
				for (int p = 0; p < poly1->p.size(); p++)
				{
					float q = (poly1->p[p].x * axisProj.x + poly1->p[p].y * axisProj.y);
					min_r1 = std::min(min_r1, q);
					max_r1 = std::max(max_r1, q);
				}

				// Work out min and max 1D points for r2
				float min_r2 = INFINITY, max_r2 = -INFINITY;
				for (int p = 0; p < poly2->p.size(); p++)
				{
					float q = (poly2->p[p].x * axisProj.x + poly2->p[p].y * axisProj.y);
					min_r2 = std::min(min_r2, q);
					max_r2 = std::max(max_r2, q);
				}

				// Calculate actual overlap along projected axis, and store the minimum
				overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return false;
			}
		}

		// If we got here, the objects have collided, we will displace r1
		// by overlap along the vector between the two object centers
		vec2d d = { r2.pos.x - r1.pos.x, r2.pos.y - r1.pos.y };
		float s = sqrtf(d.x*d.x + d.y*d.y);
		r1.pos.x -= overlap * d.x / s;
		r1.pos.y -= overlap * d.y / s;
		return false;
	}

	// Use edge/diagonal intersections.
	bool ShapeOverlap_DIAGS(polygon &r1, polygon &r2)
	{
		polygon *poly1 = &r1;
		polygon *poly2 = &r2;

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}
			
			// Check diagonals of polygon...
			for (int p = 0; p < poly1->p.size(); p++)
			{
				vec2d line_r1s = poly1->pos;
				vec2d line_r1e = poly1->p[p];

				// ...against edges of the other
				for (int q = 0; q < poly2->p.size(); q++)
				{
					vec2d line_r2s = poly2->p[q];
					vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{
						return true;
					}
				}
			}		
		}
		return false;
	}

	// Use edge/diagonal intersections.
	bool ShapeOverlap_DIAGS_STATIC(polygon &r1, polygon &r2)
	{
		polygon *poly1 = &r1;
		polygon *poly2 = &r2;

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = &r2;
				poly2 = &r1;
			}

			// Check diagonals of this polygon...
			for (int p = 0; p < poly1->p.size(); p++)
			{
				vec2d line_r1s = poly1->pos;
				vec2d line_r1e = poly1->p[p];

				vec2d displacement = { 0,0 };

				// ...against edges of this polygon
				for (int q = 0; q < poly2->p.size(); q++)
				{
					vec2d line_r2s = poly2->p[q];
					vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{						
						displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
						displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);					
					}
				}

				r1.pos.x += displacement.x * (shape == 0 ? -1 : +1);
				r1.pos.y += displacement.y * (shape == 0 ? -1 : +1);
			}
		}

		// Cant overlap if static collision is resolved
		return false;
	}

	

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetKey(olc::Key::F1).bReleased) nMode = 0;
		if (GetKey(olc::Key::F2).bReleased) nMode = 1;
		if (GetKey(olc::Key::F3).bReleased) nMode = 2;
		if (GetKey(olc::Key::F4).bReleased) nMode = 3;
		
		// Shape 1
		if (GetKey(olc::Key::LEFT).bHeld) vecShapes[0].angle -= 2.0f * fElapsedTime;
		if (GetKey(olc::Key::RIGHT).bHeld) vecShapes[0].angle += 2.0f * fElapsedTime;

		if (GetKey(olc::Key::UP).bHeld)
		{
			vecShapes[0].pos.x += cosf(vecShapes[0].angle) * 60.0f * fElapsedTime;
			vecShapes[0].pos.y += sinf(vecShapes[0].angle) * 60.0f * fElapsedTime;
		}

		if (GetKey(olc::Key::DOWN).bHeld)
		{
			vecShapes[0].pos.x -= cosf(vecShapes[0].angle) * 60.0f * fElapsedTime;
			vecShapes[0].pos.y -= sinf(vecShapes[0].angle) * 60.0f * fElapsedTime;
		}

		// Shape 2
		if (GetKey(olc::Key::A).bHeld) vecShapes[1].angle -= 2.0f * fElapsedTime;
		if (GetKey(olc::Key::D).bHeld) vecShapes[1].angle += 2.0f * fElapsedTime;

		if (GetKey(olc::Key::W).bHeld)
		{
			vecShapes[1].pos.x += cosf(vecShapes[1].angle) * 60.0f * fElapsedTime;
			vecShapes[1].pos.y += sinf(vecShapes[1].angle) * 60.0f * fElapsedTime;
		}

		if (GetKey(olc::Key::S).bHeld)
		{
			vecShapes[1].pos.x -= cosf(vecShapes[1].angle) * 60.0f * fElapsedTime;
			vecShapes[1].pos.y -= sinf(vecShapes[1].angle) * 60.0f * fElapsedTime;
		}

		// Update Shapes and reset flags
		for (auto &r : vecShapes)
		{
			for (int i = 0; i < r.o.size(); i++)
				r.p[i] =
				{	// 2D Rotation Transform + 2D Translation
					(r.o[i].x * cosf(r.angle)) - (r.o[i].y * sinf(r.angle)) + r.pos.x,
					(r.o[i].x * sinf(r.angle)) + (r.o[i].y * cosf(r.angle)) + r.pos.y,
				};

			r.overlap = false;
		}

		// Check for overlap
		for (int m = 0; m < vecShapes.size(); m++)
			for (int n = m + 1; n < vecShapes.size(); n++)
			{
				switch (nMode)
				{
				case 0: vecShapes[m].overlap |= ShapeOverlap_SAT(vecShapes[m], vecShapes[n]); break;
				case 1: vecShapes[m].overlap |= ShapeOverlap_SAT_STATIC(vecShapes[m], vecShapes[n]); break;
				case 2: vecShapes[m].overlap |= ShapeOverlap_DIAGS(vecShapes[m], vecShapes[n]); break;
				case 3: vecShapes[m].overlap |= ShapeOverlap_DIAGS_STATIC(vecShapes[m], vecShapes[n]); break;
				}						
			}
		
		// === Render Display ===
		Clear(olc::BLUE);

		// Draw Shapes
		for (auto &r : vecShapes)
		{
			// Draw Boundary
			for (int i = 0; i < r.p.size(); i++)
				DrawLine(r.p[i].x, r.p[i].y, r.p[(i + 1) % r.p.size()].x, r.p[(i + 1) % r.p.size()].y, (r.overlap ? olc::RED : olc::WHITE));

			// Draw Direction
			DrawLine(r.p[0].x, r.p[0].y, r.pos.x, r.pos.y, (r.overlap ? olc::RED : olc::WHITE));
		}

		// Draw HUD
		DrawString(8, 10, "F1: SAT", (nMode == 0 ? olc::RED : olc::YELLOW));
		DrawString(8, 20, "F2: SAT/STATIC", (nMode == 1 ? olc::RED : olc::YELLOW));
		DrawString(8, 30, "F3: DIAG", (nMode == 2 ? olc::RED : olc::YELLOW));
		DrawString(8, 40, "F4: DIAG/STATIC", (nMode == 3 ? olc::RED : olc::YELLOW));

		return true;
	}
};



int main()
{
	PolygonCollisions demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}