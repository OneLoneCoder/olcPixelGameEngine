/*
	One-Size-Fits-All Rectangle Vs Rectangle Collisions
	"Stupid scanners... making me miss at archery..." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2020 OneLoneCoder.com

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

	Relevant Video: https://www.youtube.com/watch?v=8JJ-4JgR7Dg

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

	Community Blog: https://community.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018, 2019, 2020
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <algorithm>
#include <functional>
#undef min
#undef max

namespace olc
{
	namespace aabb
	{
		struct rect
		{
			olc::vf2d pos;
			olc::vf2d size;
			olc::vf2d vel;

			std::array<olc::aabb::rect*, 4> contact;
		};

		bool PointVsRect(const olc::vf2d& p, const olc::aabb::rect* r)
		{
			return (p.x >= r->pos.x && p.y >= r->pos.y && p.x < r->pos.x + r->size.x && p.y < r->pos.y + r->size.y);
		}

		bool RectVsRect(const olc::aabb::rect* r1, const olc::aabb::rect* r2)
		{
			return (r1->pos.x < r2->pos.x + r2->size.x && r1->pos.x + r1->size.x > r2->pos.x && r1->pos.y < r2->pos.y + r2->size.y && r1->pos.y + r1->size.y > r2->pos.y);
		}

		bool RayVsRect(const olc::vf2d& ray_origin, const olc::vf2d& ray_dir, const rect* target, olc::vf2d& contact_point, olc::vf2d& contact_normal, float& t_hit_near)
		{
			contact_normal = { 0,0 };
			contact_point = { 0,0 };

			// Cache division
			olc::vf2d invdir = 1.0f / ray_dir;

			// Calculate intersections with rectangle bounding axes
			olc::vf2d t_near = (target->pos - ray_origin) * invdir;
			olc::vf2d t_far = (target->pos + target->size - ray_origin) * invdir;

			if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
			if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

			// Sort distances
			if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
			if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

			// Early rejection		
			if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

			// Closest 'time' will be the first contact
			t_hit_near = std::max(t_near.x, t_near.y);

			// Furthest 'time' is contact on opposite side of target
			float t_hit_far = std::min(t_far.x, t_far.y);

			// Reject if ray direction is pointing away from object
			if (t_hit_far < 0)
				return false;

			// Contact point of collision from parametric line equation
			contact_point = ray_origin + t_hit_near * ray_dir;

			if (t_near.x > t_near.y)
				if (invdir.x < 0)
					contact_normal = { 1, 0 };
				else
					contact_normal = { -1, 0 };
			else if (t_near.x < t_near.y)
				if (invdir.y < 0)
					contact_normal = { 0, 1 };
				else
					contact_normal = { 0, -1 };

			// Note if t_near == t_far, collision is principly in a diagonal
			// so pointless to resolve. By returning a CN={0,0} even though its
			// considered a hit, the resolver wont change anything.
			return true;
		}

		bool DynamicRectVsRect(const olc::aabb::rect* r_dynamic, const float fTimeStep, const olc::aabb::rect& r_static,
			olc::vf2d& contact_point, olc::vf2d& contact_normal, float& contact_time)
		{
			// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
			if (r_dynamic->vel.x == 0 && r_dynamic->vel.y == 0)
				return false;

			// Expand target rectangle by source dimensions
			olc::aabb::rect expanded_target;
			expanded_target.pos = r_static.pos - r_dynamic->size / 2;
			expanded_target.size = r_static.size + r_dynamic->size;

			if (RayVsRect(r_dynamic->pos + r_dynamic->size / 2, r_dynamic->vel * fTimeStep, &expanded_target, contact_point, contact_normal, contact_time))
				return (contact_time >= 0.0f && contact_time < 1.0f);
			else
				return false;
		}



		bool ResolveDynamicRectVsRect(olc::aabb::rect* r_dynamic, const float fTimeStep, olc::aabb::rect* r_static)
		{
			olc::vf2d contact_point, contact_normal;
			float contact_time = 0.0f;
			if (DynamicRectVsRect(r_dynamic, fTimeStep, *r_static, contact_point, contact_normal, contact_time))
			{
				if (contact_normal.y > 0) r_dynamic->contact[0] = r_static; else nullptr;
				if (contact_normal.x < 0) r_dynamic->contact[1] = r_static; else nullptr;
				if (contact_normal.y < 0) r_dynamic->contact[2] = r_static; else nullptr;
				if (contact_normal.x > 0) r_dynamic->contact[3] = r_static; else nullptr;

				r_dynamic->vel += contact_normal * olc::vf2d(std::abs(r_dynamic->vel.x), std::abs(r_dynamic->vel.y)) * (1 - contact_time);
				return true;
			}

			return false;
		}
	}
}


class RectangleCollisions : public olc::PixelGameEngine
{
public:
	RectangleCollisions()
	{
		sAppName = "Rectangles!";
	}

	std::vector<olc::aabb::rect> vRects;

public:
	bool OnUserCreate() override
	{
		vRects.push_back({ {170.0f, 70.0f}, {10.0f, 40.0f} });
		vRects.push_back({ {150.0f, 50.0f}, {20.0f, 20.0f} });
		vRects.push_back({ {150.0f, 150.0f}, {75.0f, 20.0f} });
		vRects.push_back({ {170.0f, 50.0f}, {20.0f, 20.0f} });
		vRects.push_back({ {190.0f, 50.0f}, {20.0f, 20.0f} });
		vRects.push_back({ {110.0f, 50.0f}, {20.0f, 20.0f} });
		vRects.push_back({ {50.0f, 130.0f}, {20.0f, 20.0f} });
		vRects.push_back({ {50.0f, 150.0f}, {20.0f, 20.0f} });
		vRects.push_back({ {50.0f, 170.0f}, {20.0f, 20.0f} });
		vRects.push_back({ {150.0f, 100.0f}, {10.0f, 1.0f} });
		vRects.push_back({ {200.0f, 100.0f}, {20.0f, 60.0f} });


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::DARK_BLUE);

		olc::vf2d vMouse = { float(GetMouseX()), float(GetMouseY()) };
		olc::vf2d vPoint = { 128.0f, 120.0f };

		if (GetKey(olc::Key::W).bHeld) vRects[0].vel.y = -100.0f;
		if (GetKey(olc::Key::S).bHeld) vRects[0].vel.y = +100.0f;
		if (GetKey(olc::Key::A).bHeld) vRects[0].vel.x = -100.0f;
		if (GetKey(olc::Key::D).bHeld) vRects[0].vel.x = +100.0f;

		if (GetMouse(0).bHeld)
			vRects[0].vel += (vMouse - vRects[0].pos).norm() * 100.0f * fElapsedTime;


		// Draw all rectangles
		for (const auto& r : vRects)
			DrawRect(r.pos, r.size, olc::WHITE);


		// Sort collisions in order of distance
		olc::vf2d cp, cn;
		float t = 0, min_t = INFINITY;
		std::vector<std::pair<int, float>> z;
 
		// Work out collision point, add it to vector along with rect ID
		for (size_t i = 1; i < vRects.size(); i++)
		{
			if (olc::aabb::DynamicRectVsRect(&vRects[0], fElapsedTime, vRects[i], cp, cn, t))
			{
				z.push_back({ i, t });
			}
		}

		// Do the sort
		std::sort(z.begin(), z.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
			{
				return a.second < b.second;
			});

		// Now resolve the collision in correct order 
		for (auto j : z)
			olc::aabb::ResolveDynamicRectVsRect(&vRects[0], fElapsedTime, &vRects[j.first]);

		// Embellish the "in contact" rectangles in yellow
		for (int i = 0; i < 4; i++)
		{
			if (vRects[0].contact[i])
				DrawRect(vRects[0].contact[i]->pos, vRects[0].contact[i]->size, olc::YELLOW);
			vRects[0].contact[i] = nullptr;
		}

		// UPdate the player rectangles position, with its modified velocity
		vRects[0].pos += vRects[0].vel * fElapsedTime;
		
		// Draw players velocity vector
		if (vRects[0].vel.mag2() > 0)
			DrawLine(vRects[0].pos + vRects[0].size / 2, vRects[0].pos + vRects[0].size / 2 + vRects[0].vel.norm() * 20, olc::RED);

		return true;
	}
};


int main()
{
	RectangleCollisions demo;
	if (demo.Construct(256, 240, 4, 4, false, false))
		demo.Start();
	return 0;
}