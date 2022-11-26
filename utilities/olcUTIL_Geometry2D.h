/*
	OneLoneCoder - Geometry 2D v1.01
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	A collection of 2D Geometric primitives and functions to work with
	and between them.


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

	Changes:
	v1.01:		+Made constants inline
				+Header guards (lol... sigh...)

*/

#pragma once
#include "olcPixelGameEngine.h"

namespace olc::utils::geom2d
{
	// Lemon Meringue
	inline const double pi = 3.141592653589793238462643383279502884;

	// Floating point error margin
	inline const double epsilon = 0.001;

	//https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
	template <typename T>
	constexpr int sgn(T val) { return (T(0) < val) - (val < T(0)); }

	// Defines a line segment
	template<typename T>
	struct line
	{
		olc::v2d_generic<T> start;
		olc::v2d_generic<T> end;

		inline line(const olc::v2d_generic<T>& s = { T(0), T(0) },
			const olc::v2d_generic<T>& e = { T(0), T(0) })
			: start(s), end(e)
		{ }


		// Get length of line
		inline constexpr T length()
		{
			return (end - start).mag();
		}

		// Get length of line^2
		inline constexpr T length2()
		{
			return (end - start).mag2();
		}

		inline constexpr olc::v2d_generic<T> vector() const
		{
			return (end - start);
		}

		// Given a real distance, get point along line
		inline constexpr olc::v2d_generic<T> rpoint(const T& distance) const
		{
			return start + (end - start).norm() * distance;
		}

		// Given a unit distance, get point along line
		inline constexpr olc::v2d_generic<T> upoint(const T& distance) const
		{
			return start + (end - start) * distance;
		}

		// Return which side of the line does a point lie
		inline constexpr int32_t side(const olc::v2d_generic<T>& point) const
		{
			double d = (end - start).cross(point - start);
			if (d < 0)
				return -1;
			else
				if (d > 0)
					return 1;
				else
					return 0;
		}
	};

	template<typename T>
	struct ray
	{
		olc::v2d_generic<T> origin;
		olc::v2d_generic<T> direction;
	};

	template<typename T>
	struct rect
	{
		olc::v2d_generic<T> pos;
		olc::v2d_generic<T> size;

		inline rect(const olc::v2d_generic<T>& p = { T(0), T(0) },
			const olc::v2d_generic<T>& s = { T(1), T(1) })
			: pos(p), size(s)
		{ }

		inline olc::v2d_generic<T> middle() const
		{
			return pos + (size * double(0.5));
		}

		// Get line segment from top side of rectangle
		inline line<T> top() const
		{
			return { pos, {pos.x + size.x, pos.y } };
		}

		// Get line segment from bottom side of rectangle
		inline line<T> bottom() const
		{
			return { {pos.x, pos.y + size.y}, pos + size };
		}

		// Get line segment from left side of rectangle
		inline line<T> left() const
		{
			return { pos, {pos.x, pos.y + size.y} };
		}

		// Get line segment from right side of rectangle
		inline line<T> right() const
		{
			return { {pos.x + size.x, pos.y }, pos + size };
		}

		// Get a line from an indexed side, starting top, going clockwise
		inline line<T> side(const size_t i) const
		{
			if (i & 0b11 == 0) return top();
			if (i & 0b11 == 1) return right();
			if (i & 0b11 == 2) return bottom();
			if (i & 0b11 == 3) return left();
		}

		// Get area of rectangle
		inline constexpr T area() const
		{
			return size.x * size.y;
		}

		// Get perimeter of rectangle
		inline constexpr T perimeter() const
		{
			return T(2) * (size.x + size.y);
		}
	};


	template<typename T>
	struct circle
	{
		olc::v2d_generic<T> pos;
		T radius = T(0);

		inline circle(const olc::v2d_generic<T>& p = { T(0), T(0) }, const T r = T(0))
			: pos(p), radius(r)
		{ }

		// Get area of circle
		inline constexpr T area() const
		{
			return T(pi) * radius * radius;
		}

		// Get circumference of circle
		inline constexpr T perimeter() const
		{
			return T(2.0 * pi) * radius;
		}

		// Get circumference of circle
		inline constexpr T circumference() const
		{
			return perimeter();
		}
	};


	template<typename T>
	struct triangle
	{
		std::array<olc::v2d_generic<T>, 3> pos;

		inline triangle(
			const olc::v2d_generic<T>& p0 = { T(0), T(0) },
			const olc::v2d_generic<T>& p1 = { T(0), T(0) },
			const olc::v2d_generic<T>& p2 = { T(0), T(0) })
			: pos{ p0,p1,p2 }
		{ }

		// Get a line from an indexed side, starting top, going clockwise
		inline line<T> side(const size_t i) const
		{
			return line(pos[i % 3], pos[(i + 1) % 3]);
		}

		// Get area of triangle
		inline constexpr T area() const
		{
			return double(0.5) * std::abs(
				(pos[0].x * (pos[1].y - pos[2].y)) +
				(pos[1].x * (pos[2].y - pos[0].y)) +
				(pos[2].x * (pos[0].y - pos[1].y)));
		}

		// Get perimeter of triangle
		inline constexpr T perimeter() const
		{
			return line(pos[0], pos[1]).length()
				+ line(pos[1], pos[2]).length()
				+ line(pos[2], pos[0]).length();
		}
	};


	template<typename T>
	struct polygon
	{
		std::vector<olc::v2d_generic<T>> vPoints;
	};


	// =========================================================================================================================
	// Closest(shape, point) ===================================================================================================

	// Returns closest point to point
	template<typename T1, typename T2>
	inline olc::v2d_generic<T2> closest(const olc::v2d_generic<T1>& p1, const olc::v2d_generic<T2>& p2)
	{
		return p1;
	}

	// Returns closest point on line to point
	template<typename T1, typename T2>
	inline olc::v2d_generic<T2> closest(const line<T1>& l, const olc::v2d_generic<T2>& p)
	{		
		auto d = l.vector();
		double u = std::clamp(double(d.dot(p - l.start) / d.mag2()), 0.0, 1.0);
		return l.start + d * u;
	}

	// Returns closest point on circle to point
	template<typename T1, typename T2>
	inline olc::v2d_generic<T2> closest(const circle<T1>& c, const olc::v2d_generic<T2>& p)
	{		
		return c.pos + (p - c.pos).norm() * c.radius;
	}

	// Returns closest point on rectangle to point
	template<typename T1, typename T2>
	inline olc::v2d_generic<T2> closest(const rect<T1>& r, const olc::v2d_generic<T2>& p)
	{
		// This could be a "constrain" function hmmmm
		// TODO: Not quite what i wanted, should restrain to boundary
		return olc::v2d_generic<T2>{ std::clamp(p.x, r.pos.x, r.pos.x + r.size.x), std::clamp(p.y, r.pos.y, r.pos.y + r.size.y) };
		
	}

	// Returns closest point on triangle to point
	template<typename T1, typename T2>
	inline olc::v2d_generic<T2> closest(const triangle<T1>& t, const olc::v2d_generic<T2>& p)
	{
		// TODO:
		return olc::v2d_generic<T2>();
	}











	// ================================================================================================================
	// POINT ==========================================================================================================

	// Checks if point contains point
	template<typename T1, typename T2>
	inline constexpr bool contains(const olc::v2d_generic<T1>& p1, const olc::v2d_generic<T2>& p2)
	{
		return (p1 - p2).mag2() < epsilon;
	}

	// Checks if line contains point
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l, const olc::v2d_generic<T2>& p)
	{
		double d = ((p.x - l.start.x) * (l.end.y - l.start.y) - (p.y - l.start.y) * (l.end.x - l.start.x));
		if (std::abs(d) < epsilon)
		{
			// point is on line
			double u = l.vector().dot(p - l.start) / l.vector().mag2();
			return (u >= double(0.0) && u <= double(1.0));
		}

		return false;
	}

	// Checks if rectangle contains point
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const olc::v2d_generic<T2>& p)
	{
		return !(p.x < r.pos.x || p.y < r.pos.y ||
			p.x > (r.pos.x + r.size.x) || p.y > (r.pos.y + r.size.y));
	}

	// Checks if circle contains a point
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const olc::v2d_generic<T2>& p)
	{
		return (c.pos - p).mag2() < (c.radius * c.radius);
	}

	// Checks if triangle contains a point
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const olc::v2d_generic<T2>& p)
	{
		// http://jsfiddle.net/PerroAZUL/zdaY8/1/
		T2 A = T2(0.5) * (-t.pos[1].y * t.pos[2].x + t.pos[0].y * (-t.pos[1].x + t.pos[2].x) + t.pos[0].x * (t.pos[1].y - t.pos[2].y) + t.pos[1].x * t.pos[2].y);
		T2 sign = A < T2(0) ? T2(-1) : T2(1);
		T2 s = (t.pos[0].y * t.pos[2].x - t.pos[0].x * t.pos[2].y + (t.pos[2].y - t.pos[0].y) * p.x + (t.pos[0].x - t.pos[2].x) * p.y) * sign;
		T2 v = (t.pos[0].x * t.pos[1].y - t.pos[0].y * t.pos[1].x + (t.pos[0].y - t.pos[1].y) * p.x + (t.pos[1].x - t.pos[0].x) * p.y) * sign;
		return s > T2(0) && v > T2(0) && (s + v) < T2(2) * A * sign;
	}




	// Check if point overlaps with point (analagous to contains())
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v2d_generic<T1>& p1, const olc::v2d_generic<T2>& p2)
	{
		return contains(p1, p2);
	}

	// Checks if line segment overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const olc::v2d_generic<T2>& p)
	{
		return contains(l, p);
	}

	// Checks if rectangle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const olc::v2d_generic<T2>& p)
	{
		return contains(r, p);
	}

	// Checks if circle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const olc::v2d_generic<T2>& p)
	{
		return contains(c, p);
	}

	// Checks if triangle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const olc::v2d_generic<T2>& p)
	{
		return contains(t, p);
	}




	// Get intersection points where point intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const olc::v2d_generic<T1>& p1, const olc::v2d_generic<T2>& p2)
	{
		if (contains(p1, p2))
			return { p1 };
		else
			return {};
	}

	// Get intersection points where line segment intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const line<T1>& l, const olc::v2d_generic<T2>& p)
	{
		if (contains(l, p))
			return { p };
		else
			return {};
	}

	// Get intersection points where rectangle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const rect<T1>& r, const olc::v2d_generic<T2>& p)
	{
		std::vector<olc::v2d_generic<T2>> vPoints;
		if (contains(r.top(), p)) vPoints.push_back(p);
		if (contains(r.bottom(), p)) vPoints.push_back(p);
		if (contains(r.left(), p)) vPoints.push_back(p);
		if (contains(r.right(), p)) vPoints.push_back(p);
		return vPoints;
	}

	// Get intersection points where circle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const circle<T1>& c, const olc::v2d_generic<T2>& p)
	{
		if (std::abs((p - c.pos).mag2() - (c.radius * c.radius)) <= epsilon)
			return { p };
		else
			return {};
	}

	// Get intersection points where triangle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const triangle<T1>& r, const olc::v2d_generic<T2>& p)
	{
		// TODO:
		return {};
	}












	// ================================================================================================================
	// LINE ===========================================================================================================

	// Check if point contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const olc::v2d_generic<T1>& p, const line<T2>& l)
	{
		return false; // It can't!
	}

	// Check if line segment contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l1, const line<T2>& l2)
	{
		// TODO: Check if segments are colinear, and l1 exists within bounds of l2
		return false;
	}

	// Check if rectangle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const line<T2>& l)
	{
		return contains(r, l.start) && contains(r, l.end);
	}

	// Check if circle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c1, const line<T2>& l)
	{
		return contains(c1, l.start) && contains(c1, l.end);
	}

	// Check if triangle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const line<T2>& l)
	{
		return contains(t, l.start) && contains(t, l.end);
	}




	// Check if point overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v2d_generic<T1>& p, const line<T2>& l)
	{
		return contains(l, p);
	}

	// Check if line segment overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l1, const line<T2>& l2)
	{
		// TODO: 
		return false;
	}

	// Check if rectangle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const line<T2>& l)
	{
		return contains(r, l.start)
			|| contains(r, l.end);

		// TODO: This method is no good, it cant detect lines whose start and end
		// points are outside the rectangle
	}

	// Check if circle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const line<T2>& l)
	{
		// TODO:
		return false;
	}

	// Check if triangle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const line<T2>& l)
	{
		return overlaps(t, l.start) || overlaps(t, l.end);

		// TODO: This method is no good, it cant detect lines whose start and end
		// points are outside the triangle
	}




	// Get intersection points where point intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const olc::v2d_generic<T1>& p, const line<T2>& l)
	{
		// TODO:
		return {};
	}

	// Get intersection points where line segment intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const line<T1>& l1, const line<T2>& l2)
	{
		// TODO:
		return {};
	}

	// Get intersection points where rectangle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const rect<T1>& r, const line<T2>& l)
	{
		// TODO:
		return {};
	}

	// Get intersection points where circle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const circle<T1>& c, const line<T2>& l)
	{
		// TODO:
		return {};
	}

	// Get intersection points where triangle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const triangle<T1>& t, const line<T2>& l)
	{
		// TODO:
		return {};
	}












	// ================================================================================================================
	// RECTANGLE ======================================================================================================

	// Check if point contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const olc::v2d_generic<T1>& p, const rect<T2>& r)
	{
		return false; // It can't!
	}

	// Check if line segment contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l, const rect<T2>& r)
	{
		return false; // It can't
	}

	// Check if rectangle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r1, const rect<T2>& r2)
	{
		return (r2.pos.x >= r1.pos.x) && (r2.pos.x + r2.size.x < r1.pos.x + r1.size.x) &&
			(r2.pos.y >= r1.pos.y) && (r2.pos.y + r2.size.y < r1.pos.y + r1.size.y);
	}

	// Check if circle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const rect<T2>& r)
	{
		return contains(c, r.pos) 
			&& contains(c, olc::v2d_generic<T2>{ r.pos.x + r.size.x, r.pos.y })
			&& contains(c, olc::v2d_generic<T2>{ r.pos.x, r.pos.y + r.size.y })
			&& contains(c, r.pos + r.size);
	}

	// Check if triangle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const rect<T2>& r)
	{
		return contains(t, r.pos) 
			&& contains(t, r.pos + r.size)
			&& contains(t, olc::v2d_generic<T2>{ r.pos.x + r.size.x,r.pos.y })
			&& contains(t, olc::v2d_generic<T2>{ r.pos.x, r.pos.y + r.size.y });
	}




	// Check if point overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v2d_generic<T1>& p, const rect<T2>& r)
	{
		return overlaps(r, p);
	}

	// Check if line segment overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const rect<T2>& r)
	{
		return overlaps(r, l);
	}

	// Check if rectangle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r1, const rect<T2>& r2)
	{
		return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x >= r2.pos.x &&
			r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y >= r2.pos.y);
	}

	// Check if circle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const rect<T2>& r)
	{
		// Inspired by this (very clever btw) 
		// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response
		// But modified to work :P
		T2 overlap = (olc::v2d_generic<T2>{ std::clamp(c.pos.x, r.pos.x, r.pos.x + r.size.x), std::clamp(c.pos.y, r.pos.y, r.pos.y + r.size.y) } - c.pos).mag2();
		if (std::isnan(overlap)) overlap = T2(0);
		return (overlap - (c.radius * c.radius)) < T2(0);
	}

	// Check if triangle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const rect<T2>& r)
	{
		return contains(t, r.pos) 
			|| contains(t, r.pos + r.size)
			|| contains(t, olc::v2d_generic<T2>{ r.pos.x + r.size.x, r.pos.y }) 
			|| contains(t, olc::v2d_generic<T2>{ r.pos.x, r.pos.y + r.size.y });

		// TODO: This method is no good, consider rectangle with all vertices
		// outside of triangle, but edges still crossing
	}




	// Get intersection points where point intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const olc::v2d_generic<T1>& p, const rect<T2>& r)
	{
		return intersects(r, p);
	}

	// Get intersection points where line segment intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const line<T1>& l, const rect<T2>& r)
	{
		// TODO:
		return {};
	}

	// Get intersection points where rectangle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const rect<T1>& r1, const rect<T2>& r2)
	{
		// TODO:
		return {};
	}

	// Get intersection points where circle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const circle<T1>& c, const rect<T2>& r)
	{
		// TODO:
		return {};
	}

	// Get intersection points where triangle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const triangle<T1>& t, const rect<T2>& r)
	{
		// TODO:
		return {};
	}













	// ================================================================================================================
	// CIRCLE =========================================================================================================

	// Check if point contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const olc::v2d_generic<T1>& p, const circle<T2>& c)
	{
		return false; // It can't!
	}

	// Check if line segment contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l, const circle<T2>& c)
	{
		return false; // It can't!
	}

	// Check if rectangle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const circle<T2>& c)
	{
		// TODO:
		return false;
	}

	// Check if circle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c1, const circle<T2>& c2)
	{
		return (c1.pos - c2.pos).mag2() <= (c1.radius - c2.radius) * (c1.radius - c2.radius);
	}

	// Check if triangle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const circle<T2>& c)
	{
		// TODO:
		return false;
	}




	// Check if point overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v2d_generic<T1>& p, const circle<T2>& c)
	{
		return overlaps(c, p);
	}

	// Check if line segment overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const circle<T2>& c)
	{
		return overlaps(c, l);
	}

	// Check if rectangle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const circle<T2>& c)
	{
		return overlaps(c, r);
	}

	// Check if circle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c1, const circle<T2>& c2)
	{
		return (c1.pos - c2.pos).mag2() <= (c1.radius + c2.radius) * (c1.radius + c2.radius);
	}

	// Check if triangle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const circle<T2>& c)
	{
		// TODO:
		return false;
	}




	// Get intersection points where point intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const olc::v2d_generic<T1>& p, const circle<T2>& c)
	{
		// TODO:
		return {};
	}

	// Get intersection points where line segment intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const line<T1>& l, const circle<T2>& c)
	{
		// TODO:
		return {};
	}

	// Get intersection points where rectangle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const rect<T1>& r, const circle<T2>& c)
	{
		// TODO:
		return {};
	}

	// Get intersection points where circle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const circle<T1>& c1, const circle<T2>& c2)
	{
		// TODO:
		return {};
	}

	// Get intersection points where triangle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const triangle<T1>& t, const circle<T2>& c)
	{
		// TODO:
		return {};
	}













	// ================================================================================================================
	// TRIANGLE =======================================================================================================

	// Check if point contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const olc::v2d_generic<T1>& p, const triangle<T2>& t)
	{
		return false; // It can't!
	}

	// Check if line segment contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l, const triangle<T2>& t)
	{
		return false; // It can't
	}

	// Check if rectangle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const triangle<T2>& t)
	{
		// TODO:
		return false;
	}

	// Check if circle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const triangle<T2>& t)
	{
		// TODO:
		return false;
	}

	// Check if triangle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		// TODO:
		return false;
	}




	// Check if point overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v2d_generic<T1>& p, const triangle<T2>& t)
	{
		return overlaps(t, p);
	}

	// Check if line segment overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const triangle<T2>& t)
	{
		return overlaps(t, l);
	}

	// Check if rectangle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const triangle<T2>& t)
	{
		return overlaps(t, r);
	}

	// Check if circle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const triangle<T2>& t)
	{
		return overlaps(t, c);
	}

	// Check if triangle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		// TODO:
		return false;
	}




	// Get intersection points where point intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const olc::v2d_generic<T1>& p, const triangle<T2>& t)
	{
		// TODO:
		return {};
	}

	// Get intersection points where line segment intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const line<T1>& l, const triangle<T2>& t)
	{
		// TODO:
		return {};
	}

	// Get intersection points where rectangle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const rect<T1>& r, const triangle<T2>& t)
	{
		// TODO:
		return {};
	}

	// Get intersection points where circle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const circle<T1>& c, const triangle<T2>& t)
	{
		// TODO:
		return {};
	}

	// Get intersection points where triangle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v2d_generic<T2>> intersects(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		// TODO:
		return {};
	}

}