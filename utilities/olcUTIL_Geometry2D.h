/*
	OneLoneCoder - Geometry 2D v2.0
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	A collection of 2D Geometric primitives and functions to work with
	and between them.


	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2024 OneLoneCoder.com

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

	Authors
	~~~~~~~
	David Barr, aka javidx9, (c) OneLoneCoder 2019, 2020, 2021, 2022, 2023, 2024

	With significant contributions from:

	Piratux, Gusgo99, Gorbit99, MaGetzUb, Dandistine, Moros1138
	cstdint, sigonasr, bixxy, Qwerasd, starfreakclone, fux

	Changes:
	v1.01:		+Made constants inline
				+Header guards (lol... sigh...)
	v2.0:		+Major file contribution and restructuring

*/

/*
	What Is This?
	~~~~~~~~~~~~~

	I've worked with 2D stuff for decades and I'm tired of reapeatedly researching, deriving
	and implementing geometric analysis routines, so wanted a "one-stop-shop" to collate all
	these mathematics. You don't even need olc::PixelGameEngine, this file will run as a
	standalone 2D vector/geometry utility.

	As well as a robust 2D Vector implementation, this file offers definitions of the following
	shapes:

		point		- A 2D (x,y) vector from (0,0)
		line		- A line segment defined by a start and end point
		circle		- A circle defined by a middle point and a radius
		rectangle	- An axis aligned quad defined by a top left point, and a size
		triangle	- A triangle defined by 3 points
		ray			- A special case "line" with an origin and a direction

	Functions have been provided that yield useful analyses for almost every combination
	of shapes. The function groups all have the same names, and are differentiated via
	argument type:

		point closest(a, b)
			Returns closest point on Shape A to Shape B

		bool overlaps(a, b)
			Returns true if any part of Shape A overlaps any part of Shape B, including boundaries

		bool contains(a, b)
			Returns true if Shape A wholly contains Shape B within and including it's boundary

		vector<point> intersects(a, b)
			Returns a vector of points where Shape A boundary intersects with Shape B boundary

		optional<point> project(a, b, ray)
			Projects Shape A along a ray, until and if it contacts shape B. If it never contacts
			then nothing is returned. If it does contact the closest position Shape A can be to
			Shape B is returned without the shapes overlapping

		rect envelope_r(a) / bounding_box(a)
			Returns the minimum area rectangle that fully encompasses Shape A

		rect envelope_c(a) / bounding_circle(a)
			Returns the minimum area circle that fully encompasses Shape A

		ray reflect(ray, a)
			Returns a ray that is a reflection of supplied incident ray against Shape A

		optional<point, normal> collision(ray, a)
			Returns the point and normal where a ray collides with Shape A

*/

/*
	Quick Navigation
	~~~~~~~~~~~~~~~~

	To jump to an implementation quickly, use your editor to search in the following way:

	f(a, b)

	where:

	f = overlaps, intersects, contains, closest, envelope_r, envelope_b, reflects, collision
	a = p, l, r, c, t, q, pol (point, line, rect, circ, triangle, ray, polygon)

	example:

	"contains(r,c)"		- takes you to implementation for contains(rect, circ)
						- Does the rectangle contain the circle?
*/

/*
	Function Matrix - Function(A, B)
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	A      B>|    POINT     |     LINE     |     RECT     |    CIRCLE    |   TRIANGLE   |      RAY     |
	---------+--------------+--------------+--------------+--------------+--------------+--------------+
	POINT    | contains     | contains     | contains     | contains     | contains     |              |
			 | closest      | closest      | closest      | closest      | closest      | closest      |
			 | overlaps     | overlaps     | overlaps     | overlaps     | overlaps     |              |
			 | intersects   | intersects   | intersects   | intersects   | intersects   |              |
			 |              |              |              |              |              |              |
	---------+--------------+--------------+--------------+--------------+--------------+--------------+
	LINE     | contains     | contains     | contains     | contains     | contains     |              |
			 | closest      |              |              | closest      |              |              |
			 | overlaps     | overlaps     | overlaps     | overlaps     | overlaps     |              |
			 | intersects   | intersects   | intersects   | intersects   | intersects   |              |
			 |              |              |              |              |              |              |
	---------+--------------+--------------+--------------+--------------+--------------+--------------+
	RECT     | contains     | contains     | contains     | contains     | contains     |              |
			 | closest      |              |              |              |              |              |
			 | overlaps     | overlaps     | overlaps     | overlaps     | overlaps     |              |
			 | intersects   | intersects   | intersects   | intersects   | intersects   |              |
			 |              |              |              |              |              |              |
	---------+--------------+--------------+--------------+--------------+--------------+--------------+
	CIRCLE   | contains     | contains     | contains     | contains     | contains     |              |
			 | closest      | closest      |              |              |              |              |
			 | overlaps     | overlaps     | overlaps     | overlaps     | overlaps     |              |
			 | intersects   | intersects   | intersects   | intersects   | intersects   |              |
			 | project      | project      |              | project      |              |              |
	---------+--------------+--------------+--------------+--------------+--------------+--------------+
	TRIANGLE | contains     | contains     | contains     | contains     | contains     |              |
			 | closest      |              |              |              |              |              |
			 | overlaps     | overlaps     | overlaps     | overlaps     | overlaps     |              |
			 | intersects   | intersects   | intersects   | intersects   | intersects   |              |
			 |              |              |              |              |              |              |
	---------+--------------+--------------+--------------+--------------+--------------+--------------+
	RAY      | contains     |              |              |              |              |              |
			 |              |              |              |              |              |              |
			 |              | collision    | collision    | collision    | collision    | collision*   |
			 |              | intersects   | intersects   | intersects   | intersects   | intersects   |
			 |              | reflect      | reflect      | reflect      | reflect      | reflect*     |
	---------+--------------+--------------+--------------+--------------+--------------+--------------+

	* Exists but always fails
*/

#pragma once
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <optional>
#include <cassert>
#include <array>


#ifdef PGE_VER
#error "olcUTIL_Geometry2D.h must be included BEFORE olcPixelGameEngine.h"
#else


#if !defined(OLC_VECTOR2D_DEFINED)
namespace olc
{
	/*
		A complete 2D geometric vector structure, with a variety
		of useful utility functions and operator overloads
	*/
	template<class T>
	struct v_2d
	{
		static_assert(std::is_arithmetic<T>::value, "olc::v_2d<type> must be numeric");

		// x-axis component
		T x = 0;
		// y-axis component
		T y = 0;

		// Default constructor
		inline constexpr v_2d() = default;

		// Specific constructor
		inline constexpr v_2d(T _x, T _y) : x(_x), y(_y)
		{}

		// Copy constructor
		inline constexpr v_2d(const v_2d& v) = default;

		// Assignment operator
		inline constexpr v_2d& operator=(const v_2d& v) = default;


		// Returns rectangular area of vector
		inline constexpr auto area() const
		{
			return x * y;
		}

		// Returns magnitude of vector
		inline auto mag() const
		{
			return std::sqrt(x * x + y * y);
		}

		// Returns magnitude squared of vector (useful for fast comparisons)
		inline constexpr T mag2() const
		{
			return x * x + y * y;
		}

		// Returns normalised version of vector
		inline v_2d norm() const
		{
			auto r = 1 / mag();
			return v_2d(x * r, y * r);
		}

		// Returns vector at 90 degrees to this one
		inline constexpr v_2d perp() const
		{
			return v_2d(-y, x);
		}

		// Rounds both components down
		inline constexpr v_2d floor() const
		{
			return v_2d(std::floor(x), std::floor(y));
		}

		// Rounds both components up
		inline constexpr v_2d ceil() const
		{
			return v_2d(std::ceil(x), std::ceil(y));
		}

		// Returns 'element-wise' max of this and another vector
		inline constexpr v_2d max(const v_2d& v) const
		{
			return v_2d(std::max(x, v.x), std::max(y, v.y));
		}

		// Returns 'element-wise' min of this and another vector
		inline constexpr v_2d min(const v_2d& v) const
		{
			return v_2d(std::min(x, v.x), std::min(y, v.y));
		}

		// Calculates scalar dot product between this and another vector
		inline constexpr auto dot(const v_2d& rhs) const
		{
			return this->x * rhs.x + this->y * rhs.y;
		}

		// Calculates 'scalar' cross product between this and another vector (useful for winding orders)
		inline constexpr auto cross(const v_2d& rhs) const
		{
			return this->x * rhs.y - this->y * rhs.x;
		}

		// Treat this as polar coordinate (R, Theta), return cartesian equivalent (X, Y)
		inline constexpr v_2d cart() const
		{
			return v_2d(std::cos(y) * x, std::sin(y) * x);
		}

		// Treat this as cartesian coordinate (X, Y), return polar equivalent (R, Theta)
		inline constexpr v_2d polar() const
		{
			return v_2d(mag(), std::atan2(y, x));
		}

		// Clamp the components of this vector in between the 'element-wise' minimum and maximum of 2 other vectors
		inline constexpr v_2d clamp(const v_2d& v1, const v_2d& v2) const
		{
			return this->max(v1).min(v2);
		}

		// Linearly interpolate between this vector, and another vector, given normalised parameter 't'
		inline constexpr v_2d lerp(const v_2d& v1, const double t) const
		{
			return (*this) * (T(1.0 - t)) + (v1 * T(t));
		}

		// Compare if this vector is numerically equal to another
		inline constexpr bool operator == (const v_2d& rhs) const
		{
			return (this->x == rhs.x && this->y == rhs.y);
		}

		// Compare if this vector is not numerically equal to another
		inline constexpr bool operator != (const v_2d& rhs) const
		{
			return (this->x != rhs.x || this->y != rhs.y);
		}

		// Return this vector as a std::string, of the form "(x,y)"
		inline std::string str() const
		{
			return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + ")";
		}

		// Assuming this vector is incident, given a normal, return the reflection
		inline constexpr v_2d reflect(const v_2d& n) const
		{
			return (*this) - 2.0 * (this->dot(n) * n);
		}

		// Allow 'casting' from other v_2d types
		template<class F>
		inline constexpr operator v_2d<F>() const
		{
			return { static_cast<F>(this->x), static_cast<F>(this->y) };
		}
	};

	// Multiplication operator overloads between vectors and scalars, and vectors and vectors
	template<class TL, class TR>
	inline constexpr auto operator * (const TL& lhs, const v_2d<TR>& rhs)
	{
		return v_2d(lhs * rhs.x, lhs * rhs.y);
	}

	template<class TL, class TR>
	inline constexpr auto operator * (const v_2d<TL>& lhs, const TR& rhs)
	{
		return v_2d(lhs.x * rhs, lhs.y * rhs);
	}

	template<class TL, class TR>
	inline constexpr auto operator * (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
	{
		return v_2d(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	template<class TL, class TR>
	inline constexpr auto operator *= (v_2d<TL>& lhs, const TR& rhs)
	{
		lhs = lhs * rhs;
		return lhs;
	}

	// Division operator overloads between vectors and scalars, and vectors and vectors
	template<class TL, class TR>
	inline constexpr auto operator / (const TL& lhs, const v_2d<TR>& rhs)
	{
		return v_2d(lhs / rhs.x, lhs / rhs.y);
	}

	template<class TL, class TR>
	inline constexpr auto operator / (const v_2d<TL>& lhs, const TR& rhs)
	{
		return v_2d(lhs.x / rhs, lhs.y / rhs);
	}

	template<class TL, class TR>
	inline constexpr auto operator / (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
	{
		return v_2d(lhs.x / rhs.x, lhs.y / rhs.y);
	}

	template<class TL, class TR>
	inline constexpr auto operator /= (v_2d<TL>& lhs, const TR& rhs)
	{
		lhs = lhs / rhs;
		return lhs;
	}

	// Unary Addition operator (pointless but i like the platinum trophies)
	template<class T>
	inline constexpr auto operator + (const v_2d<T>& lhs)
	{
		return v_2d(+lhs.x, +lhs.y);
	}

	// Addition operator overloads between vectors and scalars, and vectors and vectors
	template<class TL, class TR>
	inline constexpr auto operator + (const TL& lhs, const v_2d<TR>& rhs)
	{
		return v_2d(lhs + rhs.x, lhs + rhs.y);
	}

	template<class TL, class TR>
	inline constexpr auto operator + (const v_2d<TL>& lhs, const TR& rhs)
	{
		return v_2d(lhs.x + rhs, lhs.y + rhs);
	}

	template<class TL, class TR>
	inline constexpr auto operator + (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
	{
		return v_2d(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	template<class TL, class TR>
	inline constexpr auto operator += (v_2d<TL>& lhs, const TR& rhs)
	{
		lhs = lhs + rhs;
		return lhs;
	}

	template<class TL, class TR>
	inline constexpr auto operator += (v_2d<TL>& lhs, const v_2d<TR>& rhs)
	{
		lhs = lhs + rhs;
		return lhs;
	}

	// Unary negation operator overoad for inverting a vector
	template<class T>
	inline constexpr auto operator - (const v_2d<T>& lhs)
	{
		return v_2d(-lhs.x, -lhs.y);
	}

	// Subtraction operator overloads between vectors and scalars, and vectors and vectors
	template<class TL, class TR>
	inline constexpr auto operator - (const TL& lhs, const v_2d<TR>& rhs)
	{
		return v_2d(lhs - rhs.x, lhs - rhs.y);
	}

	template<class TL, class TR>
	inline constexpr auto operator - (const v_2d<TL>& lhs, const TR& rhs)
	{
		return v_2d(lhs.x - rhs, lhs.y - rhs);
	}

	template<class TL, class TR>
	inline constexpr auto operator - (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
	{
		return v_2d(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	template<class TL, class TR>
	inline constexpr auto operator -= (v_2d<TL>& lhs, const TR& rhs)
	{
		lhs = lhs - rhs;
		return lhs;
	}

	// Greater/Less-Than Operator overloads - mathematically useless, but handy for "sorted" container storage
	template<class TL, class TR>
	inline constexpr bool operator < (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
	{
		return (lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x);
	}

	template<class TL, class TR>
	inline constexpr bool operator > (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
	{
		return (lhs.y > rhs.y) || (lhs.y == rhs.y && lhs.x > rhs.x);
	}

	// Allow olc::v_2d to play nicely with std::cout
	template<class T>
	inline constexpr std::ostream& operator << (std::ostream& os, const v_2d<T>& rhs)
	{
		os << rhs.str();
		return os;
	}

	// Convenient types ready-to-go
	typedef v_2d<int32_t> vi2d;
	typedef v_2d<uint32_t> vu2d;
	typedef v_2d<float> vf2d;
	typedef v_2d<double> vd2d;
}
#define OLC_VECTOR2D_DEFINED 1
#endif



namespace olc::utils::geom2d
{
	// Lemon Meringue
	inline const double pi = 3.141592653589793238462643383279502884;

	// Floating point error margin
	inline const double epsilon = 0.001;

	namespace internal
	{
		template<typename T>
		inline std::vector<olc::v_2d<T>> filter_duplicate_points(const std::vector<olc::v_2d<T>>& points) {
			std::vector<olc::v_2d<T>> filtered_points;

			for (const auto& point : points)
			{
				bool is_duplicate = false;

				for (const auto& filtered_point : filtered_points)
				{
					if (std::abs(point.x - filtered_point.x) < epsilon && std::abs(point.y - filtered_point.y) < epsilon)
					{
						is_duplicate = true;
						break;
					}
				}

				if (!is_duplicate)
				{
					filtered_points.push_back(point);
				}
			}

			return filtered_points;
		}
	};

	//https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
	template <typename T>
	constexpr int sgn(T val) { return (T(0) < val) - (val < T(0)); }

	// Defines a line segment
	template<typename T>
	struct line
	{
		olc::v_2d<T> start;
		olc::v_2d<T> end;

		inline line(const olc::v_2d<T>& s = { T(0), T(0) },
			const olc::v_2d<T>& e = { T(0), T(0) })
			: start(s), end(e)
		{ }

		// Get vector pointing from start to end
		inline constexpr olc::v_2d<T> vector() const
		{
			return (end - start);
		}

		// Get length of line
		inline constexpr T length()
		{
			return vector().mag();
		}

		// Get length of line^2
		inline constexpr T length2()
		{
			return vector().mag2();
		}



		// Given a real distance, get point along line
		inline constexpr olc::v_2d<T> rpoint(const T& distance) const
		{
			return start + vector().norm() * distance;
		}

		// Given a unit distance, get point along line
		inline constexpr olc::v_2d<T> upoint(const T& distance) const
		{
			return start + vector() * distance;
		}

		// Return which side of the line does a point lie
		inline constexpr int32_t side(const olc::v_2d<T>& point) const
		{
			double d = vector().cross(point - start);
			if (d < 0)
				return -1;
			else
				if (d > 0)
					return 1;
				else
					return 0;
		}

		// Returns line equation "mx + a" coefficients where:
		// x: m
		// y: a
		// NOTE: Returns {inf, inf} if std::abs(end.x - start.x) < epsilon:
		inline constexpr olc::vd2d coefficients() const
		{
			double x1 = start.x;
			double x2 = end.x;
			double y1 = start.y;
			double y2 = end.y;

			// check if line is vertical or close to vertical
			if (std::abs(x2 - x1) < epsilon) {
				return olc::vd2d{ std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity() };
			}

			double m = (y2 - y1) / (x2 - x1);
			return olc::vd2d{ m, -m * x1 + y1 };
		}
	};

	template<typename T>
	struct ray
	{
		olc::v_2d<T> origin;
		olc::v_2d<T> direction;

		inline ray(const olc::v_2d<T>& o = { T(0), T(0) },
			const olc::v_2d<T>& d = { T(0), T(0) })
			: origin(o), direction(d)
		{ }
	};

	template<typename T>
	struct rect
	{
		olc::v_2d<T> pos;
		olc::v_2d<T> size;

		inline rect(const olc::v_2d<T>& p = { T(0), T(0) },
			const olc::v_2d<T>& s = { T(1), T(1) })
			: pos(p), size(s)
		{ }

		inline olc::v_2d<T> middle() const
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
			if ((i & 0b11) == 0) return top();
			if ((i & 0b11) == 1) return right();
			if ((i & 0b11) == 2) return bottom();
			//if ((i & 0b11) == 3) return left(); // Dumb compilers cant fathom this
			return left();
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

		// Returns side count: 4
		inline constexpr size_t side_count() const {
			return 4;
		}
	};


	template<typename T>
	struct circle
	{
		olc::v_2d<T> pos;
		T radius = T(0);

		inline circle(const olc::v_2d<T>& p = { T(0), T(0) }, const T r = T(0))
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
		std::array<olc::v_2d<T>, 3> pos;

		inline triangle(
			const olc::v_2d<T>& p0 = { T(0), T(0) },
			const olc::v_2d<T>& p1 = { T(0), T(0) },
			const olc::v_2d<T>& p2 = { T(0), T(0) })
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

		// Returns side count: 3
		inline constexpr size_t side_count() const {
			return 3;
		}
	};


	template<typename T>
	struct polygon
	{
		std::vector<olc::v_2d<T>> pos;
	};


	// =========================================================================================================================
	// Closest(shape, point) ===================================================================================================

	// Closest location on [SHAPE] to Point

	// closest(p,p)
	// Returns closest point on point to any shape (aka the original point) :P
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const olc::v_2d<T1>& p, [[maybe_unused]] const T2& anything_who_cares)
	{
		return p;
	}

	// closest(l,p)
	// Returns closest point on line to point
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const line<T1>& l, const olc::v_2d<T2>& p)
	{
		auto d = l.vector();
		double u = std::clamp(double(d.dot(p - l.start)) / d.mag2(), 0.0, 1.0);
		return l.start + u * d;
	}

	// closest(c,p)
	// Returns closest point on circle to point
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const circle<T1>& c, const olc::v_2d<T2>& p)
	{
		return c.pos + olc::vd2d(p - c.pos).norm() * c.radius;
	}

	// closest(r,p)
	// Returns closest point on rectangle to point
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const rect<T1>& r, const olc::v_2d<T2>& p)
	{
		// Note: this algorithm can be reused for polygon
		auto c1 = closest(r.top(), p);
		auto c2 = closest(r.bottom(), p);
		auto c3 = closest(r.left(), p);
		auto c4 = closest(r.right(), p);

		auto d1 = (c1 - p).mag2();
		auto d2 = (c2 - p).mag2();
		auto d3 = (c3 - p).mag2();
		auto d4 = (c4 - p).mag2();

		auto dmin = d1;
		auto cmin = c1;

		if (d2 < dmin) {
			dmin = d2;
			cmin = c2;
		}
		if (d3 < dmin) {
			dmin = d3;
			cmin = c3;
		}
		if (d4 < dmin) {
			dmin = d4;
			cmin = c4;
		}

		return cmin;
	}

	// closest(t,p)
	// Returns closest point on triangle to point
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const triangle<T1>& t, const olc::v_2d<T2>& p)
	{
		olc::utils::geom2d::line<T1> l{ t.pos[0], t.pos[1] };
		auto p0 = closest(l, p);
		auto d0 = (p0 - p).mag2();

		l.end = t.pos[2];
		auto p1 = closest(l, p);
		auto d1 = (p1 - p).mag2();

		l.start = t.pos[1];
		auto p2 = closest(l, p);
		auto d2 = (p2 - p).mag2();

		if ((d0 <= d1) && (d0 <= d2)) {
			return p0;
		}
		else if ((d1 <= d0) && (d1 <= d2)) {
			return p1;
		}
		else {
			return p2;
		}
	}

	// closest(ray,p)
	// Returns closest point on ray to point
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const ray<T1>& r, const olc::v_2d<T2>& p)
	{
		// TODO: implement
		return p;
	}


	// Closest location on [SHAPE] to Line

	// closest(l,l)
	// Returns closest point on line to line
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const line<T1>& l1, const line<T2>& l2)
	{
		// TODO:
		return {};
	}

	// closest(r,l)
	// Returns closest point on rectangle to line
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const rect<T1>& r, const line<T2>& l)
	{
		// TODO:
		return {};
	}

	// closest(c,l)
	// Returns closest point on circle to line
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const circle<T1>& c, const line<T2>& l)
	{
		const auto p1 = closest(l, c.pos);
		return c.pos + olc::vd2d(p1 - c.pos).norm() * c.radius;
	}

	// closest(t,l)
	// Returns closest point on triangle to line
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const triangle<T1>& t, const line<T2>& l)
	{
		// TODO:
		return {};
	}


	// Closest location on [SHAPE] to Circle

	// closest(l,c)
	// Returns closest point on line to circle
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const line<T1>& l, const circle<T2>& c)
	{
		const auto p1 = closest(c, l); // Closest point on circle to line
		return closest(l, p1);
	}

	// closest(r,c)
	// Returns closest point on rectangle to circle
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const rect<T1>& r, const circle<T2>& l)
	{
		// TODO:
		return {};
	}

	// closest(c,c)
	// Returns closest point on circle to circle
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const circle<T1>& c1, const circle<T2>& c2)
	{
		return closest(c1, c2.pos);
	}

	// closest(t,c)
	// Returns closest point on triangle to circle
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const triangle<T1>& r, const circle<T2>& l)
	{
		// TODO:
		return {};
	}


	// Closest location on [SHAPE] to Triangle

	// closest(l,t)
	// Returns closest point on line to triangle
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const line<T1>& l, const triangle<T2>& c)
	{
		// TODO:
		return {};
	}

	// closest(r,t)
	// Returns closest point on rectangle to triangle
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const rect<T1>& r, const triangle<T2>& l)
	{
		// TODO:
		return {};
	}

	// closest(c,t)
	// Returns closest point on circle to triangle
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const circle<T1>& c, const triangle<T2>& l)
	{
		// TODO:
		return {};
	}

	// closest(t,t)
	// Returns closest point on triangle to triangle
	template<typename T1, typename T2>
	inline olc::v_2d<T1> closest(const triangle<T1>& r, const triangle<T2>& l)
	{
		// TODO:
		return {};
	}





	// ================================================================================================================
	// POINT ==========================================================================================================

	// contains(p,p)
	// Checks if point contains point
	template<typename T1, typename T2>
	inline constexpr bool contains(const olc::v_2d<T1>& p1, const olc::v_2d<T2>& p2)
	{
		return (p1 - p2).mag2() < epsilon;
	}

	// contains(l,p)
	// Checks if line contains point
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l, const olc::v_2d<T2>& p)
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

	// contains(r,p)
	// Checks if rectangle contains point
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const olc::v_2d<T2>& p)
	{
		return !(p.x < r.pos.x || p.y < r.pos.y ||
			p.x >(r.pos.x + r.size.x) || p.y >(r.pos.y + r.size.y));
	}

	// contains(c,p)
	// Checks if circle contains a point
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const olc::v_2d<T2>& p)
	{
		return (c.pos - p).mag2() <= (c.radius * c.radius);
	}

	// contains(t,p)
	// Checks if triangle contains a point
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const olc::v_2d<T2>& p)
	{
		// http://jsfiddle.net/PerroAZUL/zdaY8/1/
		T2 A = T2(0.5) * (-t.pos[1].y * t.pos[2].x + t.pos[0].y * (-t.pos[1].x + t.pos[2].x) + t.pos[0].x * (t.pos[1].y - t.pos[2].y) + t.pos[1].x * t.pos[2].y);
		T2 sign = A < T2(0) ? T2(-1) : T2(1);
		T2 s = (t.pos[0].y * t.pos[2].x - t.pos[0].x * t.pos[2].y + (t.pos[2].y - t.pos[0].y) * p.x + (t.pos[0].x - t.pos[2].x) * p.y) * sign;
		T2 v = (t.pos[0].x * t.pos[1].y - t.pos[0].y * t.pos[1].x + (t.pos[0].y - t.pos[1].y) * p.x + (t.pos[1].x - t.pos[0].x) * p.y) * sign;
		return s >= T2(0) && v >= T2(0) && (s + v) <= T2(2) * A * sign;
	}

	template<typename T1, typename T2>
	inline constexpr bool contains(const ray<T1>& r, const olc::v_2d<T2>& p)
	{
		// Calculate the vector from the ray's origin to point p
		olc::v_2d<T2> op = p - r.origin;

		// Calculate the dot product between op and the ray's direction
		// This checks if p is in the direction of the ray and not behind the origin
		T2 dotProduct = op.dot(r.direction);

		if (dotProduct < 0) {
			// p is behind the ray's origin
			return false;
		}

		// Project op onto the ray's direction (which is already normalized)
		olc::v_2d<T2> projection = { r.direction.x * dotProduct, r.direction.y * dotProduct };

		// Check if the projection of op onto the ray's direction is equivalent to op
		// This is true if p lies on the ray

		T2 distance = std::sqrt((projection.x - op.x) * (projection.x - op.x) + (projection.y - op.y) * (projection.y - op.y));

		// Assuming a small threshold for floating point arithmetic issues
		return distance < epsilon;
	}

	// overlaps(p,p)
	// Check if point overlaps with point (analogous to contains())
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p1, const olc::v_2d<T2>& p2)
	{
		return contains(p1, p2);
	}

	// overlaps(l,p)
	// Checks if line segment overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const olc::v_2d<T2>& p)
	{
		return contains(l, p);
	}

	// overlaps(r,p)
	// Checks if rectangle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const olc::v_2d<T2>& p)
	{
		return contains(r, p);
	}

	// overlaps(c,p)
	// Checks if circle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const olc::v_2d<T2>& p)
	{
		return contains(c, p);
	}

	// overlaps(t,p)
	// Checks if triangle overlaps with point
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const olc::v_2d<T2>& p)
	{
		return contains(t, p);
	}




	// intersects(p,p)
	// Get intersection points where point intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p1, const olc::v_2d<T2>& p2)
	{
		if (contains(p1, p2))
			return { p1 };

		return {};
	}

	// intersects(l,p)
	// Get intersection points where line segment intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l, const olc::v_2d<T2>& p)
	{
		if (contains(l, p))
			return { p };

		return {};
	}

	// intersects(r,p)
	// Get intersection points where rectangle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r, const olc::v_2d<T2>& p)
	{
		for (size_t i = 0; i < r.side_count(); i++)
			if (contains(r.side(i), p))
				return { p };

		return {};
	}

	// intersects(c,p)
	// Get intersection points where circle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c, const olc::v_2d<T2>& p)
	{
		if (std::abs((p - c.pos).mag2() - (c.radius * c.radius)) <= epsilon)
			return { p };

		return {};
	}

	// intersects(t,p)
	// Get intersection points where triangle intersects with point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t, const olc::v_2d<T2>& p)
	{
		for (size_t i = 0; i < t.side_count(); i++)
			if (contains(t.side(i), p))
				return { p };

		return {};

	}












	// ================================================================================================================
	// LINE ===========================================================================================================

	// contains(p,l)
	// Check if point contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const olc::v_2d<T1>& p, [[maybe_unused]] const line<T2>& l)
	{
		return false; // It can't!
	}

	// contains(l,l)
	// Check if line segment contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const line<T1>& l1, const line<T2>& l2)
	{
		return overlaps(l1, l2.start) && overlaps(l1, l2.end);
	}

	// contains(r,l)
	// Check if rectangle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const line<T2>& l)
	{
		return contains(r, l.start) && contains(r, l.end);
	}

	// contains(c,l)
	// Check if circle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c1, const line<T2>& l)
	{
		return contains(c1, l.start) && contains(c1, l.end);
	}

	// contains(t,l)
	// Check if triangle contains line segment
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const line<T2>& l)
	{
		return contains(t, l.start) && contains(t, l.end);
	}



	// overlaps(p,l)
	// Check if point overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p, const line<T2>& l)
	{
		return contains(l, p);
	}

	// overlaps(l,l)
	// Check if line segment overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l1, const line<T2>& l2)
	{
		double D = ((l2.end.y - l2.start.y) * (l1.end.x - l1.start.x) - (l2.end.x - l2.start.x) * (l1.end.y - l1.start.y));
		double uA = ((l2.end.x - l2.start.x) * (l1.start.y - l2.start.y) - (l2.end.y - l2.start.y) * (l1.start.x - l2.start.x)) / D;
		double uB = ((l1.end.x - l1.start.x) * (l1.start.y - l2.start.y) - (l1.end.y - l1.start.y) * (l1.start.x - l2.start.x)) / D;
		return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
	}

	// overlaps(r,l)
	// Check if rectangle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const line<T2>& l)
	{
		return contains(r, l.start)
			|| overlaps(r.top(), l)
			|| overlaps(r.bottom(), l)
			|| overlaps(r.left(), l)
			|| overlaps(r.right(), l);
	}

	// overlaps(c,l)
	// Check if circle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const line<T2>& l)
	{
		auto vClosest = closest(l, c.pos);
		return ((c.pos - vClosest).mag2() <= (c.radius * c.radius));
	}

	// overlaps(t,l)
	// Check if triangle overlaps line segment
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const line<T2>& l)
	{
		return overlaps(t, l.start) || overlaps(t.side(0), l) || overlaps(t.side(1), l) || overlaps(t.side(2), l);
	}



	// intersects(p,l)
	// Get intersection points where point intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p, const line<T2>& l)
	{
		return intersects(l, p);
	}

	// intersects(l,l)
	// Get intersection points where line segment intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l1, const line<T2>& l2)
	{
		float rd = l1.vector().cross(l2.vector());
		if (rd == 0) return {}; // Parallel or Colinear TODO: Return two points

		//Inverse rd product
		rd = 1.f / rd;

		//Cross products: 
		//rn = (b1b2 x b1a1)
		float rn = ((l2.end.x - l2.start.x) * (l1.start.y - l2.start.y) - (l2.end.y - l2.start.y) * (l1.start.x - l2.start.x)) * rd;
		//sn = (a1a2 x b1a1)
		float sn = ((l1.end.x - l1.start.x) * (l1.start.y - l2.start.y) - (l1.end.y - l1.start.y) * (l1.start.x - l2.start.x)) * rd;

		//Return the intersection depth
		//if (d) *d = rn;

		if (rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f)
			return {}; // Intersection not within line segment

		return { l1.start + rn * l1.vector() };
	}

	// intersects(r,l)
	// Get intersection points where rectangle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r, const line<T2>& l)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r.side_count(); i++)
		{
			auto v = intersects(r.side(i), l);
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(c,l)
	// Get intersection points where circle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c, const line<T2>& l)
	{
		const auto closestPointToSegment = closest(l, c.pos);
		if (!overlaps(c, closestPointToSegment))
		{
			// Circle is too far away
			return {};
		}

		// Compute point closest to the circle on the line
		const auto d = l.vector();
		const auto uLine = d.dot(c.pos - l.start) / d.mag2();
		const auto closestPointToLine = l.start + uLine * d;
		const auto distToLine = (c.pos - closestPointToLine).mag2();

		if (std::abs(distToLine - c.radius * c.radius) < epsilon)
		{
			// Circle "kisses" the line
			return { closestPointToLine };
		}

		// Circle intersects the line
		const auto length = std::sqrt(c.radius * c.radius - distToLine);
		const auto p1 = closestPointToLine + l.vector().norm() * length;
		const auto p2 = closestPointToLine - l.vector().norm() * length;

		std::vector<olc::v_2d<T2>> intersections;
		intersections.reserve(2);

		if ((p1 - closest(l, p1)).mag2() < epsilon * epsilon)
			intersections.push_back(p1);
		if ((p2 - closest(l, p2)).mag2() < epsilon * epsilon)
			intersections.push_back(p2);

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(t,l)
	// Get intersection points where triangle intersects with line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t, const line<T2>& l)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < t.side_count(); i++)
		{
			auto v = intersects(t.side(i), l);
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}












	// ================================================================================================================
	// RECTANGLE ======================================================================================================

	// contains(p,r)
	// Check if point contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const olc::v_2d<T1>& p, [[maybe_unused]] const rect<T2>& r)
	{
		return false; // It can't!
	}

	// contains(l,r)
	// Check if line segment contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const line<T1>& l, [[maybe_unused]] const rect<T2>& r)
	{
		return false; // It can't
	}

	// contains(r,r)
	// Check if rectangle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r1, const rect<T2>& r2)
	{
		return (r2.pos.x >= r1.pos.x) && (r2.pos.x + r2.size.x <= r1.pos.x + r1.size.x) &&
			(r2.pos.y >= r1.pos.y) && (r2.pos.y + r2.size.y <= r1.pos.y + r1.size.y);
	}

	// contains(c,r)
	// Check if circle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const rect<T2>& r)
	{
		return contains(c, r.pos)
			&& contains(c, olc::v_2d<T2>{ r.pos.x + r.size.x, r.pos.y })
			&& contains(c, olc::v_2d<T2>{ r.pos.x, r.pos.y + r.size.y })
			&& contains(c, r.pos + r.size);
	}

	// contains(t,r)
	// Check if triangle contains rectangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const rect<T2>& r)
	{
		return contains(t, r.pos)
			&& contains(t, r.pos + r.size)
			&& contains(t, olc::v_2d<T2>{ r.pos.x + r.size.x, r.pos.y })
			&& contains(t, olc::v_2d<T2>{ r.pos.x, r.pos.y + r.size.y });
	}



	// overlaps(p,r)
	// Check if point overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p, const rect<T2>& r)
	{
		return overlaps(r, p);
	}

	// overlaps(l,r)
	// Check if line segment overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const rect<T2>& r)
	{
		return overlaps(r, l);
	}

	// overlaps(r,r)
	// Check if rectangle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r1, const rect<T2>& r2)
	{
		return (r1.pos.x <= r2.pos.x + r2.size.x && r1.pos.x + r1.size.x >= r2.pos.x &&
			r1.pos.y <= r2.pos.y + r2.size.y && r1.pos.y + r1.size.y >= r2.pos.y);
	}

	// overlaps(c,r)
	// Check if circle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const rect<T2>& r)
	{
		// Inspired by this (very clever btw) 
		// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response
		// But modified to work :P
		double overlap = (olc::v_2d<T2>{ std::clamp(c.pos.x, r.pos.x, r.pos.x + r.size.x), std::clamp(c.pos.y, r.pos.y, r.pos.y + r.size.y) } - c.pos).mag2();
		if (std::isnan(overlap)) overlap = 0;
		return (overlap - (c.radius * c.radius)) < 0;
	}

	// overlaps(t,r)
	// Check if triangle overlaps rectangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const rect<T2>& r)
	{
		return overlaps(t, r.top())
			|| overlaps(t, r.bottom())
			|| overlaps(t, r.left())
			|| overlaps(t, r.right())
			|| contains(r, t.pos[0]);
	}



	// intersects(p,r)
	// Get intersection points where point intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p, const rect<T2>& r)
	{
		return intersects(r, p);
	}

	// intersects(l,r)
	// Get intersection points where line segment intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l, const rect<T2>& r)
	{
		return intersects(r, l);
	}

	// intersects(r,r)
	// Get intersection points where rectangle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r1, const rect<T2>& r2)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r2.side_count(); i++) {
			auto v = intersects(r1, r2.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(c,r)
	// Get intersection points where circle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c, const rect<T2>& r)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r.side_count(); i++)
		{
			auto v = intersects(c, r.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(t,r)
	// Get intersection points where triangle intersects with rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t, const rect<T2>& r)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r.side_count(); i++) {
			auto v = intersects(t, r.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}













	// ================================================================================================================
	// CIRCLE =========================================================================================================

	// contains(p,c)
	// Check if point contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const olc::v_2d<T1>& p, [[maybe_unused]] const circle<T2>& c)
	{
		return false; // It can't!
	}

	// contains(l,c)
	// Check if line segment contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const line<T1>& l, [[maybe_unused]] const circle<T2>& c)
	{
		return false; // It can't!
	}

	// contains(r,c)
	// Check if rectangle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const circle<T2>& c)
	{
		return r.pos.x + c.radius <= c.pos.x
			&& c.pos.x <= r.pos.x + r.size.x - c.radius
			&& r.pos.y + c.radius <= c.pos.y
			&& c.pos.y <= r.pos.y + r.size.y - c.radius;
	}

	// contains(c,c)
	// Check if circle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c1, const circle<T2>& c2)
	{
		return (std::sqrt(std::pow(c2.pos.x - c1.pos.x, 2) + std::pow(c2.pos.y - c1.pos.y, 2)) + c2.radius) <= c1.radius;

	}

	// contains(t,c)
	// Check if triangle contains circle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t, const circle<T2>& c)
	{
		return contains(t, c.pos) && (c.pos - closest(t, c.pos)).mag2() >= c.radius * c.radius;
	}



	// overlaps(p,c)
	// Check if point overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p, const circle<T2>& c)
	{
		return overlaps(c, p);
	}

	// overlaps(l,c)
	// Check if line segment overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const circle<T2>& c)
	{
		return overlaps(c, l);
	}

	// overlaps(r,c)
	// Check if rectangle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const circle<T2>& c)
	{
		return overlaps(c, r);
	}

	// overlaps(c,c)
	// Check if circle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c1, const circle<T2>& c2)
	{
		return (c1.pos - c2.pos).mag2() <= (c1.radius + c2.radius) * (c1.radius + c2.radius);
	}

	// overlaps(t,c)
	// Check if triangle overlaps circle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t, const circle<T2>& c)
	{
		return contains(t, c.pos) || (c.pos - closest(t, c.pos)).mag2() <= c.radius * c.radius;
	}



	// intersects(p,c)
	// Get intersection points where point intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p, const circle<T2>& c)
	{
		return intersects(c, p);
	}

	// intersects(l,c)
	// Get intersection points where line segment intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l, const circle<T2>& c)
	{
		return intersects(c, l);
	}

	// intersects(r,c)
	// Get intersection points where rectangle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r, const circle<T2>& c)
	{
		return intersects(c, r);
	}

	// intersects(c,c)
	// Get intersection points where circle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c1, const circle<T2>& c2)
	{
		if (c1.pos == c2.pos) return {}; // circles are either within one another so cannot intersect, or are
		// identical so share all points which there's no good way to represent in return value.
		v_2d<T1> between = c2.pos - c1.pos;
		float dist2 = between.mag2();
		float radiusSum = c1.radius + c2.radius;
		if (dist2 > radiusSum * radiusSum) return {}; // circles are too far apart to be touching.
		if (contains(c1, c2) || contains(c2, c1)) return {}; // one circle is inside of the other, they can't be intersecting.
		if (dist2 == radiusSum) return { c1.pos + between.norm() * c1.radius }; // circles are touching at exactly 1 point
		// otherwise they're touching at 2 points.
	  //                                                                                                                   
	  //                ______     ________                                                                                
	  //             .''      ''.X'        ''.      X = intersections                                                      
	  //            /          / |\           \     O = chordCenter                                                        
	  //           |          |  | |           |    In order to find the intersections we first find O.                    
	  //           (      c1--(--O-)--c2       )    To do this, we find the distance c1->O by solving for the              
	  //           |          |  | |           |    two right triangles formed by the chord and c1->c2 (L).                
	  //            \          \ |/           /           .                Pythagorean theorem:                            
	  //             '.        .'X          ,'          .'|\               (L-x)^2 + h^2 = c1.r^2                          
	  //               ''----''   ''------''      c1.r /  | \ c2.radius        x^2 + h^2 = c2.r^2                          
	  //                                             .'  h|  \             Subtract 1 equation from the other and solve:   
	  //                                            /_____|___\            (L-x)^2 + h^2 - (x^2 + h^2) = c1.r^2 - c2.r^2   
	  //                                               x   L-x             (L-x)^2 - x^2 = c1.r^2 - c2.r^2                 
	  //                                                                   L^2 - 2Lx = c1.r^2 - c2.r^2                     
	  //                                                                   2Lx - L^2 = c2.r^2 - c1.r^2                     
	  //       v------------<-----------------<-----------------<-----.    x = (L^2 + c2.r^2 - c1.r^2)/2 -.                
	  //       Next, we have to solve for the height h, and move       '-------<--------------------<-----'                
	  //       that distance from O, perpendicular to c1->c2.                                                              
	  //                                                                                                                   
	  //       Pythagorean theorem: x^2 + h^2 = c1.r^2  ->  c1.r^2 - x^2 = h^2  ->  h = sqrt(c1.r^2 - x^2)                 
	  //                                                                                                                   
	  //       x is ccDist and h is halfChord.                                                                             
	  //                                                                                                                   
		float dist = std::sqrt(dist2);
		float ccDist = (dist2 + c1.radius * c1.radius - c2.radius * c2.radius) / (2 * dist);
		v_2d<T1> chordCenter = c1.pos + between.norm() * ccDist;
		v_2d<T1> halfChord = between.norm().perp() * std::sqrt(c1.radius * c1.radius - ccDist * ccDist);
		return { chordCenter + halfChord, chordCenter - halfChord };
	}

	// intersects(t,c)
	// Get intersection points where triangle intersects with circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t, const circle<T2>& c)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < t.side_count(); i++) {
			auto v = intersects(c, t.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}













	// ================================================================================================================
	// TRIANGLE =======================================================================================================

	// contains(p,t)
	// Check if point contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const olc::v_2d<T1>& p, [[maybe_unused]] const triangle<T2>& t)
	{
		return false; // It can't!
	}

	// contains(l,t)
	// Check if line segment contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains([[maybe_unused]] const line<T1>& l, [[maybe_unused]] const triangle<T2>& t)
	{
		return false; // It can't
	}

	// contains(r,t)
	// Check if rectangle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const rect<T1>& r, const triangle<T2>& t)
	{
		return contains(r, t.side(0))
			&& contains(r, t.side(1))
			&& contains(r, t.side(2));
	}

	// contains(c,t)
	// Check if circle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const circle<T1>& c, const triangle<T2>& t)
	{
		return contains(c, t.pos[0])
			&& contains(c, t.pos[1])
			&& contains(c, t.pos[2]);
	}

	// contains(t,t)
	// Check if triangle contains triangle
	template<typename T1, typename T2>
	inline constexpr bool contains(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		return contains(t1, t2.pos[0])
			&& contains(t1, t2.pos[1])
			&& contains(t1, t2.pos[2]);
	}



	// overlaps(p,t)
	// Check if point overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const olc::v_2d<T1>& p, const triangle<T2>& t)
	{
		return overlaps(t, p);
	}

	// overlaps(l,t)
	// Check if line segment overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const line<T1>& l, const triangle<T2>& t)
	{
		return overlaps(t, l);
	}

	// overlaps(r,t)
	// Check if rectangle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const rect<T1>& r, const triangle<T2>& t)
	{
		return overlaps(t, r);
	}

	// overlaps(c,t)
	// Check if circle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const circle<T1>& c, const triangle<T2>& t)
	{
		return overlaps(t, c);
	}

	// overlaps(t,t)
	// Check if triangle overlaps triangle
	template<typename T1, typename T2>
	inline constexpr bool overlaps(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		return overlaps(t1, t2.side(0))
			|| overlaps(t1, t2.side(1))
			|| overlaps(t1, t2.side(2))
			|| overlaps(t2, t1.pos[0]);
	}



	// intersects(p,t)
	// Get intersection points where point intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const olc::v_2d<T1>& p, const triangle<T2>& t)
	{
		return intersects(t, p);
	}

	// intersects(l,t)
	// Get intersection points where line segment intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const line<T1>& l, const triangle<T2>& t)
	{
		return intersects(t, l);
	}

	// intersects(r,t)
	// Get intersection points where rectangle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const rect<T1>& r, const triangle<T2>& t)
	{
		return intersects(t, r);
	}

	// intersects(c,t)
	// Get intersection points where circle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const circle<T1>& c, const triangle<T2>& t)
	{
		return intersects(t, c);
	}

	// intersects(t,t)
	// Get intersection points where triangle intersects with triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const triangle<T1>& t1, const triangle<T2>& t2)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < t2.side_count(); i++) {
			auto v = intersects(t1, t2.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}


	// envelope_c(c)
	// Return circle that fully encapsulates a point
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const olc::v_2d<T1>& p)
	{
		return circle<T1>(p, 0);
	}



	// envelope_c(l)
	// Return circle that fully encapsulates a line
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const line<T1>& l)
	{
		return { l.upoint(0.5),l.vector().mag() / 2 };
	}

	// envelope_c(r)
	// Return circle that fully encapsulates a rectangle
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const rect<T1>& r)
	{
		// Construct the circle from the rectangle's diagonal
		return envelope_c(line<T1>(r.pos, r.pos + r.size));
	}

	// envelope_c(c)
	// Return circle that fully encapsulates a circle
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const circle<T1>& c)
	{
		return c;
	}

	// envelope_c(t)
	// Return circle that fully encapsulates a triangle
	template<typename T1>
	inline constexpr circle<T1> envelope_c(const triangle<T1>& t)
	{
		olc::v_2d<T1> circumcenter;

		double D = 2 * (t.pos[0].x * (t.pos[1].y - t.pos[2].y) + t.pos[1].x * (t.pos[2].y - t.pos[0].y) + t.pos[2].x * (t.pos[0].y - t.pos[1].y));
		circumcenter.x = T1(double(
			(t.pos[0].x * t.pos[0].x + t.pos[0].y * t.pos[0].y) * (t.pos[1].y - t.pos[2].y) +
			(t.pos[1].x * t.pos[1].x + t.pos[1].y * t.pos[1].y) * (t.pos[2].y - t.pos[0].y) +
			(t.pos[2].x * t.pos[2].x + t.pos[2].y * t.pos[2].y) * (t.pos[0].y - t.pos[1].y)
			) / D);
		circumcenter.y = T1(double(
			(t.pos[0].x * t.pos[0].x + t.pos[0].y * t.pos[0].y) * (t.pos[2].x - t.pos[1].x) +
			(t.pos[1].x * t.pos[1].x + t.pos[1].y * t.pos[1].y) * (t.pos[0].x - t.pos[2].x) +
			(t.pos[2].x * t.pos[2].x + t.pos[2].y * t.pos[2].y) * (t.pos[1].x - t.pos[0].x)
			) / D);

		double r = 0;
		for (auto& point : t.pos) {
			r = std::max(r, double(std::hypot(circumcenter.x - point.x, circumcenter.y - point.y)));
		}

		return { circumcenter, T1(r) };
	}








	// envelope_r(p)
	// Return rectangle that fully encapsulates a point
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const olc::v_2d<T1>& p)
	{
		return rect<T1>(p, { 0,0 });
	}

	// envelope_r(l)
	// Return rectangle that fully encapsulates a line
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const line<T1>& l)
	{
		T1 min_x = std::min(l.start.x, l.end.x);
		T1 min_y = std::min(l.start.y, l.end.y);
		T1 size_x = std::abs(l.start.x - l.end.x);
		T1 size_y = std::abs(l.start.y - l.end.y);
		return { {min_x,min_y},{size_x,size_y} };
	}

	// envelope_r(r)
	// Return rectangle that fully encapsulates a rectangle
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const rect<T1>& r)
	{
		return r;
	}

	// envelope_r(c)
	// Return rectangle that fully encapsulates a circle
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const circle<T1>& c)
	{
		return rect<T1>(c.pos - v_2d<T1>{c.radius, c.radius}, { c.radius * 2, c.radius * 2 });
	}

	// envelope_r(t)
	// Return rectangle that fully encapsulates a triangle
	template<typename T1>
	inline constexpr rect<T1> envelope_r(const triangle<T1>& t)
	{
		auto vMin = t.pos[0].min(t.pos[1].min(t.pos[2]));
		auto vMax = t.pos[0].max(t.pos[1].max(t.pos[2]));
		return rect<T1>(vMin, vMax - vMin);
	}

	template<typename T>
	inline constexpr auto bounding_box(T&& t)
	{
		return envelope_r(std::forward<T>(t));
	}

	template<typename T>
	inline constexpr auto bounding_circle(T&& t)
	{
		return envelope_c(std::forward<T>(t));
	}


	// PROJECTIONS ==========================================================================================================

	// project(c,p)
	// project a circle, onto a point, via a ray (i.e. how far along the ray can the circle travel until it contacts the point?)
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T1>> project(const circle<T1>& c, const olc::v_2d<T2>& p, const ray<T3>& q)
	{
		return project(c, circle<T2>(p, 0), q);
	}


	// project(c,c)
	// project a circle, onto a circle, via a ray (i.e. how far along the ray can the circle travel until it contacts the other circle?)
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T1>> project(const circle<T1>& c1, const circle<T2>& c2, const ray<T3>& q)
	{
		// Inspired by https://math.stackexchange.com/a/929240

		double A = q.direction.mag2();
		double B = 2.0 * (q.origin.dot(q.direction) - c2.pos.dot(q.direction));
		double C = c2.pos.mag2() + q.origin.mag2() - (2.0 * c2.pos.x * q.origin.x) - (2.0 * c2.pos.y * q.origin.y) - ((c1.radius + c2.radius) * (c1.radius + c2.radius));
		double D = B * B - 4.0 * A * C;

		if (D < 0.0)
			return std::nullopt;
		else
		{
			const auto sD = std::sqrt(D);
			const auto s1 = (-B + sD) / (2.0 * A);
			const auto s2 = (-B - sD) / (2.0 * A);

			if (s1 < 0 && s2 < 0)
				return std::nullopt;
			if (s1 < 0)
				return q.origin + q.direction * s2;
			if (s2 < 0)
				return q.origin + q.direction * s1;

			return q.origin + q.direction * std::min(s1, s2);
		}
	}

	// project(c,l)
	// project a circle, onto a line segment, via a ray
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T2>> project(const circle<T1>& c, const line<T2>& l, const ray<T3>& q)
	{
		// Treat line segment as capsule with radius that of the circle
		// and treat the circle as a point

		// First do we hit ends of line segment, inflated to be circles		
		const auto vHitsStartCirc = intersects(q, circle<T2>(l.start, c.radius));
		const auto vHitsEndCirc = intersects(q, circle<T2>(l.end, c.radius));

		// Now create two line segments in parallel to the original, that join
		// up the end circles to form the sides of the capsule
		const auto displace = l.vector().norm().perp() * c.radius;
		const auto vHitsSide1 = intersects(q, line<T2>(l.start + displace, l.end + displace));
		const auto vHitsSide2 = intersects(q, line<T2>(l.start - displace, l.end - displace));

		// Bring the multitude of points to one place
		std::vector<olc::v_2d<T2>> vAllIntersections;
		vAllIntersections.insert(vAllIntersections.end(), vHitsStartCirc.begin(), vHitsStartCirc.end());
		vAllIntersections.insert(vAllIntersections.end(), vHitsEndCirc.begin(), vHitsEndCirc.end());
		vAllIntersections.insert(vAllIntersections.end(), vHitsSide1.begin(), vHitsSide1.end());
		vAllIntersections.insert(vAllIntersections.end(), vHitsSide2.begin(), vHitsSide2.end());

		if (vAllIntersections.size() == 0)
		{
			// No intersections at all, so
			return std::nullopt;
		}

		// Find closest
		double dClosest = std::numeric_limits<double>::max();
		olc::v_2d<T2> vClosest;
		for (const auto& vContact : vAllIntersections)
		{
			double dDistance = (vContact - q.origin).mag2();
			if (dDistance < dClosest)
			{
				dClosest = dDistance;
				vClosest = vContact;
			}
		}

		return vClosest;
	}

	// project(c,r)
	// project a circle, onto a rectangle, via a ray
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T2>> project(const circle<T1>& c, const rect<T2>& r, const ray<T3>& q)
	{
		const auto s1 = project(c, r.top(), q);
		const auto s2 = project(c, r.bottom(), q);
		const auto s3 = project(c, r.left(), q);
		const auto s4 = project(c, r.right(), q);

		std::vector<olc::v_2d<T2>> vAllIntersections;
		if (s1.has_value()) vAllIntersections.push_back(s1.value());
		if (s2.has_value()) vAllIntersections.push_back(s2.value());
		if (s3.has_value()) vAllIntersections.push_back(s3.value());
		if (s4.has_value()) vAllIntersections.push_back(s4.value());

		if (vAllIntersections.size() == 0)
		{
			// No intersections at all, so
			return std::nullopt;
		}

		// Find closest
		double dClosest = std::numeric_limits<double>::max();
		olc::v_2d<T2> vClosest;
		for (const auto& vContact : vAllIntersections)
		{
			double dDistance = (vContact - q.origin).mag2();
			if (dDistance < dClosest)
			{
				dClosest = dDistance;
				vClosest = vContact;
			}
		}

		return vClosest;
	}

	// project(c,t)
	// project a circle, onto a triangle, via a ray
	template<typename T1, typename T2, typename T3>
	inline std::optional<olc::v_2d<T2>> project(const circle<T1>& c, const triangle<T2>& t, const ray<T3>& q)
	{
		const auto s1 = project(c, t.side(0), q);
		const auto s2 = project(c, t.side(1), q);
		const auto s3 = project(c, t.side(2), q);

		std::vector<olc::v_2d<T2>> vAllIntersections;
		if (s1.has_value()) vAllIntersections.push_back(s1.value());
		if (s2.has_value()) vAllIntersections.push_back(s2.value());
		if (s3.has_value()) vAllIntersections.push_back(s3.value());

		if (vAllIntersections.size() == 0)
		{
			// No intersections at all, so
			return std::nullopt;
		}

		// Find closest
		double dClosest = std::numeric_limits<double>::max();
		olc::v_2d<T2> vClosest;
		for (const auto& vContact : vAllIntersections)
		{
			double dDistance = (vContact - q.origin).mag2();
			if (dDistance < dClosest)
			{
				dClosest = dDistance;
				vClosest = vContact;
			}
		}

		return vClosest;
	}


	// RAYS =================================================================================================================

	// intersects(q,q)
	// return intersection point (if it exists) of a ray and a ray
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q1, const ray<T2>& q2)
	{
		const auto origin_diff = q2.origin - q1.origin;
		const auto cp1 = q1.direction.cross(q2.direction);
		const auto cp2 = origin_diff.cross(q2.direction);

		if (cp1 == 0) // Early rejection
		{
			if (cp2 == 0)
				return { q1.origin }; // co-linear
			else
				return {}; // parallel
		}

		const auto cp3 = origin_diff.cross(q1.direction);
		const auto t1 = cp2 / cp1; // distance along q1 to intersection
		const auto t2 = cp3 / cp1; // distance along q2 to intersection

		if (t1 >= 0 && t2 >= 0)
			return { q1.origin + q1.direction * t1 }; // Intersection, both rays positive
		else
			return {}; // Intersection, but behind a rays origin, so not really an intersection in context
	}

	// intersects(q,p)
	// return intersection point (if it exists) of a ray and a point
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const v_2d<T2>& p)
	{
		const line<T1> l = { q.origin, q.origin + q.direction };

		if (std::abs(l.side(p)) < epsilon)
			return { p }; // Intersection
		else
			return {};
	}

	// intersects(q,l)
	// return intersection point (if it exists) of a ray and a line segment
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const line<T2>& l)
	{
		const auto line_direction = l.vector();
		const auto origin_diff = l.start - q.origin;
		const auto cp1 = q.direction.cross(line_direction);
		const auto cp2 = origin_diff.cross(line_direction);

		if (cp1 == 0) // Early rejection
		{
			if (cp2 == 0)
				return { q.origin }; // co-linear
			else
				return {}; // parallel
		}

		const auto cp3 = origin_diff.cross(q.direction);
		const auto t1 = cp2 / cp1; // distance along ray to intersection
		const auto t2 = cp3 / cp1; // distance along line segment to intersection

		if (t1 >= 0 && t2 >= 0 && t2 <= 1)
			return { q.origin + q.direction * t1 }; // Intersection, both rays positive
		else
			return {};	// Intersection, but behind a rays origin, or outside line segment bounds.
		// so not really an intersection in context
	}

	// collision(q,l)
	// optionally returns collision point and collision normal of ray and a line segment, if it collides
	template<typename T1, typename T2>
	inline std::optional<std::pair<olc::v_2d<T2>, olc::v_2d<T2>>> collision(const ray<T1>& q, const line<T2>& l)
	{
		const auto vIntersection = intersects(q, l);
		if (vIntersection.size() > 0)
		{
			return { {vIntersection[0], l.vector().perp().norm() * l.side(q.origin)} };
		}

		return std::nullopt;
	}

	// reflect(q,l)
	// optionally returns a ray reflected off a line segement if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const line<T2>& l)
	{
		const auto vCollision = collision(q, l);
		if (vCollision.has_value())
		{
			return { ray<T1>{vCollision.value().first, q.direction.reflect(vCollision.value().second)} };
		}

		return std::nullopt;
	}

	// reflect(q,p)
	// optionally returns a ray reflected off a point if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const olc::v_2d<T2>& p)
	{
		// TODO:
		return std::nullopt;
	}

	// collision(q,r)
	// optionally returns collision point and collision normal of ray and a line segment, if it collides
	template<typename T1, typename T2>
	inline std::optional<std::pair<olc::v_2d<T1>, olc::v_2d<T1>>> collision(const ray<T1>& q, const rect<T2>& r)
	{
		olc::v_2d<T1> vClosestIntersection;
		olc::v_2d<T1> vIntersectionNormal;
		double dClosestDistance2 = std::numeric_limits<double>::max();
		bool bCollide = false;

		for (size_t i = 0; i < r.side_count(); i++)
		{
			auto v = intersects(q, r.side(i));
			if (v.size() > 0)
			{
				bCollide = true;
				double d = (v[0] - q.origin).mag2();
				if (d < dClosestDistance2)
				{
					dClosestDistance2 = d;
					vClosestIntersection = v[0];
					vIntersectionNormal = r.side(i).vector().perp().norm();
				}
			}
		}

		if (bCollide)
		{
			return { {vClosestIntersection, vIntersectionNormal} };
		}

		return std::nullopt;
	}

	// reflect(q,r)
	// optionally returns a ray reflected off a rectangle if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const rect<T2>& r)
	{
		const auto vCollision = collision(q, r);
		if (vCollision.has_value())
		{
			return { ray<T1>{vCollision.value().first, q.direction.reflect(vCollision.value().second)} };
		}

		return std::nullopt;
	}

	// collision(q,c)
	// optionally returns collision point and collision normal of ray and a circle, if it collides
	template<typename T1, typename T2>
	inline std::optional<std::pair<olc::v_2d<T2>, olc::v_2d<T2>>> collision(const ray<T1>& q, const circle<T2>& c)
	{
		const auto vIntersection = intersects(q, c);
		if (vIntersection.size() > 0)
		{
			return { {vIntersection[0], (vIntersection[0] - c.pos).norm()} };
		}

		return std::nullopt;
	}

	// reflect(q,c)
	// optionally returns a ray reflected off a circle if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const circle<T2>& c)
	{
		const auto vCollision = collision(q, c);
		if (vCollision.has_value())
		{
			return { ray<T1>{vCollision.value().first, q.direction.reflect(vCollision.value().second)} };
		}

		return std::nullopt;
	}

	// collision(q,r)
	// optionally returns collision point and collision normal of ray and a triangle, if it collides
	template<typename T1, typename T2>
	inline std::optional<std::pair<olc::v_2d<T1>, olc::v_2d<T1>>> collision(const ray<T1>& q, const triangle<T2>& t)
	{
		olc::v_2d<T1> vClosestIntersection;
		olc::v_2d<T1> vIntersectionNormal;
		double dClosestDistance2 = std::numeric_limits<double>::max();
		bool bCollide = false;

		for (size_t i = 0; i < t.side_count(); i++)
		{
			auto v = intersects(q, t.side(i));
			if (v.size() > 0)
			{
				bCollide = true;
				double d = (v[0] - q.origin).mag2();
				if (d < dClosestDistance2)
				{
					dClosestDistance2 = d;
					vClosestIntersection = v[0];
					vIntersectionNormal = t.side(i).vector().perp().norm();
				}
			}
		}

		if (bCollide)
		{
			return { {vClosestIntersection, vIntersectionNormal} };
		}

		return std::nullopt;
	}

	// reflect(q,t)
	// optionally returns a ray reflected off a triangle if collision occurs
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q, const triangle<T2>& t)
	{
		const auto vCollision = collision(q, t);
		if (vCollision.has_value())
		{
			return { ray<T1>{vCollision.value().first, q.direction.reflect(vCollision.value().second)} };
		}

		return std::nullopt;
	}

	// reflect(q,r)
	// can't reflect a ray of a ray
	template<typename T1, typename T2>
	inline std::optional<ray<T1>> reflect(const ray<T1>& q1, const ray<T2>& q2)
	{
		// Can't reflect!
		return std::nullopt;
	}


	// intersects(q,c)
	// Get intersection points where a ray intersects a circle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const circle<T2>& c)
	{
		// Look familiar?
		double A = q.direction.mag2();
		double B = 2.0 * (q.origin.dot(q.direction) - c.pos.dot(q.direction));
		double C = c.pos.mag2() + q.origin.mag2() - (2.0 * c.pos.x * q.origin.x) - (2.0 * c.pos.y * q.origin.y) - (c.radius * c.radius);
		double D = B * B - 4.0 * A * C;

		if (D < 0.0)
			return {};
		else
		{
			const auto sD = std::sqrt(D);
			const auto s1 = (-B + sD) / (2.0 * A);
			const auto s2 = (-B - sD) / (2.0 * A);

			if (s1 < 0 && s2 < 0)
				return {};
			if (s1 < 0)
				return { q.origin + q.direction * s2 };
			if (s2 < 0)
				return { q.origin + q.direction * s1 };

			const auto [min_s, max_s] = std::minmax(s1, s2);
			return { q.origin + q.direction * min_s, q.origin + q.direction * max_s };
		}
	}

	// intersects(q,r)
	// Get intersection points where a ray intersects a rectangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const rect<T2>& r)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < r.side_count(); i++)
		{
			auto v = intersects(q, r.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}

	// intersects(q,t)
	// Get intersection points where a ray intersects a triangle
	template<typename T1, typename T2>
	inline std::vector<olc::v_2d<T2>> intersects(const ray<T1>& q, const triangle<T2>& t)
	{
		std::vector<olc::v_2d<T2>> intersections;

		for (size_t i = 0; i < t.side_count(); i++)
		{
			auto v = intersects(q, t.side(i));
			intersections.insert(intersections.end(), v.begin(), v.end());
		}

		return internal::filter_duplicate_points(intersections);
	}
}

#endif // PGE_VER
