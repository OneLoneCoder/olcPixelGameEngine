/*
	OneLoneCoder - Hardware3D v1.0
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Some assitive utilities for working with the hybrid Hardware3D rendering 
	functions.


	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2025 OneLoneCoder.com

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
	David Barr, aka javidx9, (c) OneLoneCoder 2019, 2020, 2021, 2022, 2023, 2024, 2025

	With significant contributions from:
	dandistine

	Changes:
	v1.0:		Here we go, 3D stuff but fast! (ish)	
				+CreateCube() - Creates a 6-face fully defined cuboid of a set size (with optional offset)
*/


#pragma once

#include <olcPixelGameEngine.h>

#include <optional>
#include <sstream>

#if !defined(OLC_VECTOR3D_DEFINED)
namespace olc
{
	/*
		A complete 3D geometric vector structure, with a variety
		of useful utility functions and operator overloads
	*/
	template<class T>
	struct v_3d
	{
		static_assert(std::is_arithmetic<T>::value, "olc::v_3d<type> must be numeric");

		// x-axis component
		T x = 0;
		// y-axis component
		T y = 0;
		// z-axis component
		T z = 0;
		// (optional) w component
		T w = 1;

		// Default constructor
		inline constexpr v_3d() = default;

		// Specific constructor
		inline constexpr v_3d(T _x, T _y, T _z = T(0), T _w = T(1)) : x(_x), y(_y), z(_z), w(_w)
		{}

		template<typename Q>
		inline constexpr v_3d(const olc::v_2d<Q>& v) : x(v.x), y(v.y), z(T(0)), w(T(0))
		{}

		// Copy constructor
		inline constexpr v_3d(const v_3d& v) = default;

		// Assignment operator
		inline constexpr v_3d& operator=(const v_3d& v) = default;

		// Returns xy component as 2D vector
		inline constexpr auto xy() const
		{
			return olc::v_2d<T>(x, y);
		}

		// Returns xz component as 2D vector
		inline constexpr auto xz() const
		{
			return olc::v_2d<T>(x, z);
		}

		// Returns zw component as 2D vector
		inline constexpr auto zw() const
		{
			return olc::v_2d<T>(z, w);
		}

		// Returns cuboid volume of vector
		inline constexpr auto volume() const
		{
			return x * y * z;
		}

		// Returns magnitude of vector
		inline auto mag() const
		{
			return std::sqrt(x * x + y * y + z * z);
		}

		// Returns magnitude squared of vector (useful for fast comparisons)
		inline constexpr T mag2() const
		{
			return x * x + y * y + z * z;
		}

		// Returns normalised version of vector
		inline v_3d norm() const
		{
			auto r = T(1) / mag();
			return v_3d(x * r, y * r, z * r);
		}

		// Rounds all components down
		inline constexpr v_3d floor() const
		{
			return v_3d(std::floor(x), std::floor(y), std::floor(z), w);
		}

		// Rounds all components up
		inline constexpr v_3d ceil() const
		{
			return v_3d(std::ceil(x), std::ceil(y), std::ceil(z), w);
		}

		// Returns 'element-wise' max of this and another vector
		inline constexpr v_3d max(const v_3d& v) const
		{
			return v_3d(std::max(x, v.x), std::max(y, v.y), std::max(z, v.z));
		}

		// Returns 'element-wise' min of this and another vector
		inline constexpr v_3d min(const v_3d& v) const
		{
			return v_3d(std::min(x, v.x), std::min(y, v.y), std::min(z, v.z));
		}

		// Calculates scalar dot product between this and another vector
		inline constexpr auto dot(const v_3d& rhs) const
		{
			return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
		}

		// Calculates cross product between this and another vector
		inline constexpr auto cross(const v_3d& rhs) const
		{
			return v_3d(
				this->y * rhs.z - this->z * rhs.y,
				this->z * rhs.x - this->x * rhs.z,
				this->x * rhs.y - this->y * rhs.x);
		}

		// Clamp the components of this vector in between the 'element-wise' minimum and maximum of 2 other vectors
		inline constexpr v_3d clamp(const v_3d& v1, const v_3d& v2) const
		{
			return this->max(v1).min(v2);
		}

		// Linearly interpolate between this vector, and another vector, given normalised parameter 't'
		inline constexpr v_3d lerp(const v_3d& v1, const double t) const
		{
			return this->operator*(T(1.0 - t)) + (v1 * T(t));
		}

		// Compare if this vector is numerically equal to another
		inline constexpr bool operator == (const v_3d& rhs) const
		{
			return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
		}

		// Compare if this vector is not numerically equal to another
		inline constexpr bool operator != (const v_3d& rhs) const
		{
			return (this->x != rhs.x || this->y != rhs.y || this != rhs.z);
		}

		// Return this vector as a std::string, of the form "(x,y)"
		inline constexpr std::string str() const
		{
			return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + "," + std::to_string(this->z) + ")";
		}

		inline constexpr std::array<T, 4> a() const
		{
			return std::array<T, 4>{x, y, z, w};
		}

		// Allow 'casting' from other v_2d types
		template<class F>
		inline constexpr operator v_3d<F>()
		{
			return { static_cast<F>(this->x), static_cast<F>(this->y), static_cast<F>(this->z) };
		}
	};

	// Multiplication operator overloads between vectors and scalars, and vectors and vectors
	template<class TL, class TR>
	inline constexpr auto operator * (const TL& lhs, const v_3d<TR>& rhs)
	{
		return v_3d(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}

	template<class TL, class TR>
	inline constexpr auto operator * (const v_3d<TL>& lhs, const TR& rhs)
	{
		return v_3d(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}

	template<class TL, class TR>
	inline constexpr auto operator * (const v_3d<TL>& lhs, const v_3d<TR>& rhs)
	{
		return v_3d(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	template<class TL, class TR>
	inline constexpr auto operator *= (v_3d<TL>& lhs, const TR& rhs)
	{
		lhs = lhs * rhs;
		return lhs;
	}

	// Division operator overloads between vectors and scalars, and vectors and vectors
	template<class TL, class TR>
	inline constexpr auto operator / (const TL& lhs, const v_3d<TR>& rhs)
	{
		return v_3d(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
	}

	template<class TL, class TR>
	inline constexpr auto operator / (const v_3d<TL>& lhs, const TR& rhs)
	{
		return v_3d(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}

	template<class TL, class TR>
	inline constexpr auto operator / (const v_3d<TL>& lhs, const v_3d<TR>& rhs)
	{
		return v_3d(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
	}

	template<class TL, class TR>
	inline constexpr auto operator /= (v_3d<TL>& lhs, const TR& rhs)
	{
		lhs = lhs / rhs;
		return lhs;
	}

	// Unary Addition operator (pointless but i like the platinum trophies)
	template<class T>
	inline constexpr auto operator + (const v_3d<T>& lhs)
	{
		return v_3d(+lhs.x, +lhs.y, +lhs.z);
	}

	// Addition operator overloads between vectors and scalars, and vectors and vectors
	template<class TL, class TR>
	inline constexpr auto operator + (const TL& lhs, const v_3d<TR>& rhs)
	{
		return v_3d(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
	}

	template<class TL, class TR>
	inline constexpr auto operator + (const v_3d<TL>& lhs, const TR& rhs)
	{
		return v_3d(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
	}

	template<class TL, class TR>
	inline constexpr auto operator + (const v_3d<TL>& lhs, const v_3d<TR>& rhs)
	{
		return v_3d(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	template<class TL, class TR>
	inline constexpr auto operator += (v_3d<TL>& lhs, const TR& rhs)
	{
		lhs = lhs + rhs;
		return lhs;
	}

	// Unary negation operator overoad for inverting a vector
	template<class T>
	inline constexpr auto operator - (const v_3d<T>& lhs)
	{
		return v_3d(-lhs.x, -lhs.y, -lhs.z);
	}

	// Subtraction operator overloads between vectors and scalars, and vectors and vectors
	template<class TL, class TR>
	inline constexpr auto operator - (const TL& lhs, const v_3d<TR>& rhs)
	{
		return v_3d(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
	}

	template<class TL, class TR>
	inline constexpr auto operator - (const v_3d<TL>& lhs, const TR& rhs)
	{
		return v_3d(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
	}

	template<class TL, class TR>
	inline constexpr auto operator - (const v_3d<TL>& lhs, const v_3d<TR>& rhs)
	{
		return v_3d(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	template<class TL, class TR>
	inline constexpr auto operator -= (v_3d<TL>& lhs, const TR& rhs)
	{
		lhs = lhs - rhs;
		return lhs;
	}

	// Greater/Less-Than Operator overloads - mathematically useless, but handy for "sorted" container storage
	template<class TL, class TR>
	inline constexpr bool operator < (const v_3d<TL>& lhs, const v_3d<TR>& rhs)
	{
		return (lhs.z < rhs.z) || (lhs.z == rhs.z && lhs.y < rhs.y) || (lhs.z == rhs.z && lhs.y == rhs.y && lhs.x < rhs.x);
	}

	template<class TL, class TR>
	inline constexpr bool operator > (const v_3d<TL>& lhs, const v_3d<TR>& rhs)
	{
		return (lhs.z > rhs.z) || (lhs.z == rhs.z && lhs.y > rhs.y) || (lhs.z == rhs.z && lhs.y == rhs.y && lhs.x > rhs.x);
	}

	// Allow olc::v_2d to play nicely with std::cout
	template<class T>
	inline constexpr std::ostream& operator << (std::ostream& os, const v_3d<T>& rhs)
	{
		os << rhs.str();
		return os;
	}

	// Convenient types ready-to-go
	typedef v_3d<int32_t> vi3d;
	typedef v_3d<uint32_t> vu3d;
	typedef v_3d<float> vf3d;
	typedef v_3d<double> vd3d;
}

#define OLC_VECTOR3D_DEFINED 1
#endif

#if !defined(OLC_MATRIX4D_DEFINED)
namespace olc
{
	/*
		A complete 4x4 Matrix structure, with a variety
		of useful utility functions and operator overloads
		specifically targeting 3D graphical transformations
	*/
	template<class T>
	struct m_4d
	{
		static_assert(std::is_arithmetic<T>::value, "olc::m_4d<type> must be numeric");

		// The 4x4 elements!
		std::array<T, 16> m{ 0 };

		// Constructor created identity matrix
		inline constexpr m_4d()
		{
			identity();
		}

		// Copy constructor
		inline constexpr m_4d(const m_4d& mat) = default;

		// Assignment operator
		inline constexpr m_4d& operator=(const m_4d& mat) = default;

		// Retrieve a specific element's 1D index
		inline constexpr size_t idx(size_t c, size_t r) const
		{
			// Arranged to suite/mirror OpenGL defaults
			return c * 4 + r;
		}

		// Retrieve non-const access to specific element
		inline constexpr T& operator()(size_t col, size_t row)
		{
			return m[idx(col, row)];
		}

		// Retrieve const access to specific element
		inline constexpr const T& operator()(size_t col, size_t row) const
		{
			return m[idx(col, row)];
		}

		// Set all elements to 0
		inline constexpr void clear()
		{
			std::fill(m.begin(), m.end(), T(0));
		}

		// Create identity matrix
		inline constexpr void identity()
		{
			clear();
			auto& me = (*this);
			me(0, 0) = 1;
			me(1, 1) = 1;
			me(2, 2) = 1;
			me(3, 3) = 1;
		}

		// Create translation matrix via components
		template<typename Q>
		inline constexpr void translate(Q x, Q y, Q z)
		{
			identity();
			auto& me = (*this);
			me(3, 0) = T(x);
			me(3, 1) = T(y);
			me(3, 2) = T(z);
		}

		// Create translation matrix via vector
		template<typename Q>
		inline constexpr void translate(const olc::v_3d<Q>& v)
		{
			translate(v.x, v.y, v.z);
		}

		// Create scaling matrix via components
		template<typename Q>
		inline constexpr void scale(Q x, Q y, Q z)
		{
			identity();
			auto& me = (*this);
			me(0, 0) = T(x);
			me(1, 1) = T(y);
			me(2, 2) = T(z);
		}

		// Create scaling matrix via vector
		template<typename Q>
		inline constexpr void scale(const olc::v_3d<Q>& v)
		{
			scale(v.x, v.y, v.z);
		}

		inline constexpr auto transpose() const
		{
			olc::m_4d<T> out;
			auto& me = (*this);
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					out(i, j) = me(j, i);
			return out;
		}

		// Create "projection" matrix
		template<typename Q>
		inline constexpr void projection(const Q fov, const Q ratio, const Q nearplane, const Q farplane)
		{
			identity();
			auto& me = (*this);
			T invFOV = T(1) / tan(fov * T(0.5));
			me(0, 0) = ratio * invFOV;
			me(1, 1) = invFOV;
			me(2, 2) = farplane / (farplane - nearplane);
			me(3, 2) = (-farplane * nearplane) / (farplane - nearplane);
			me(2, 3) = T(1);
			me(3, 3) = T(0);
		}

		// Create "projection" matrix
		template<typename Q>
		inline constexpr void frustum(const olc::v_3d<Q>& vLeftTopNear, const olc::v_3d<Q>& vRightBottomFar)
		{
			// https://learn.microsoft.com/en-us/windows/win32/opengl/glfrustum
			identity();
			auto& me = (*this);
			me(0, 0) = (T(2) * vLeftTopNear.z) / (vRightBottomFar.x - vLeftTopNear.x);
			me(1, 1) = (T(2) * vLeftTopNear.z) / (vLeftTopNear.y - vRightBottomFar.y);
			me(2, 2) = -((vRightBottomFar.z + vLeftTopNear.z) / (vRightBottomFar.z - vLeftTopNear.z));
			me(2, 0) = (vRightBottomFar.x + vLeftTopNear.x) / (vRightBottomFar.x - vLeftTopNear.x);
			me(2, 1) = (vLeftTopNear.y + vRightBottomFar.y) / (vLeftTopNear.y - vRightBottomFar.y);
			me(3, 2) = -((T(2) * vLeftTopNear.z * vRightBottomFar.z) / (vRightBottomFar.z - vLeftTopNear.z));
			me(2, 3) = T(-1);
			me(3, 3) = T(0);

		}

		// Create rotation around x-axis matrix with radians
		template<typename Q>
		inline constexpr void rotateX(Q rads)
		{
			identity();
			auto& me = (*this);
			me(0, 0) = T(1);
			me(1, 1) = cos(rads);
			me(1, 2) = sin(rads);
			me(2, 1) = -me(1, 2);
			me(2, 2) = me(1, 1);
			me(3, 3) = T(1);
		}

		// Create rotation around y-axis matrix with radians
		template<typename Q>
		inline constexpr void rotateY(Q rads)
		{
			identity();
			auto& me = (*this);
			me(0, 0) = cos(rads);
			me(0, 2) = sin(rads);
			me(2, 0) = -me(0, 2);
			me(1, 1) = T(1);
			me(2, 2) = me(0, 0);
			me(3, 3) = T(1);
		}

		// Create rotation around z-axis matrix with radians
		template<typename Q>
		inline constexpr void rotateZ(Q rads)
		{
			identity();
			auto& me = (*this);
			me(0, 0) = cos(rads);
			me(0, 1) = sin(rads);
			me(1, 0) = -me(0, 1);
			me(1, 1) = me(0, 0);
			me(2, 2) = T(1);
			me(3, 3) = T(1);
		}

		// Create "point at" matrix that looks at a given target, from a given location
		template<typename Q>
		inline void pointAt(const olc::v_3d<Q>& origin, const olc::v_3d<Q>& target, const olc::v_3d<Q>& up)
		{
			clear();
			auto vF = (target - origin).norm();
			auto vU = (up - (vF * up.dot(vF))).norm();
			auto vR = vU.cross(vF);

			auto& me = (*this);
			me(0, 0) = vR.x; me(0, 1) = vR.y; me(0, 2) = vR.z;
			me(1, 0) = vU.x; me(1, 1) = vU.y; me(1, 2) = vU.z;
			me(2, 0) = vF.x; me(2, 1) = vF.y; me(2, 2) = vF.z;
			me(3, 0) = origin.x;
			me(3, 1) = origin.y;
			me(3, 2) = origin.z;
			me(3, 3) = T(1);
		}

		// Return inverted matrix for 4x4 matrix with no scale component (eg !pointAt == lookAt)
		inline constexpr auto quickinvert() const
		{
			olc::m_4d<T> out;
			auto& me = (*this);
			out(0, 0) = me(0, 0); out(0, 1) = me(1, 0); out(0, 2) = me(2, 0); out(0, 3) = T(0);
			out(1, 0) = me(0, 1); out(1, 1) = me(1, 1); out(1, 2) = me(2, 1); out(1, 3) = T(0);
			out(2, 0) = me(0, 2); out(2, 1) = me(1, 2); out(2, 2) = me(2, 2); out(2, 3) = T(0);
			out(3, 0) = -(me(3, 0) * out(0, 0) + me(3, 1) * out(1, 0) + me(3, 2) * out(2, 0));
			out(3, 1) = -(me(3, 0) * out(0, 1) + me(3, 1) * out(1, 1) + me(3, 2) * out(2, 1));
			out(3, 2) = -(me(3, 0) * out(0, 2) + me(3, 1) * out(1, 2) + me(3, 2) * out(2, 2));
			out(3, 3) = T(1);
			return out;
		}

		// Return inverted matrix for any 4x4 matrix (for inverse view matrix use alternative)
		inline constexpr auto invert() const
		{
			// gluInvertMatrix
			// https://gamedev.net/forums/topic/648190-algorithm-for-4x4-matrix-inverse/5096412/
			// ...yeah... dont do this very often
			olc::m_4d<T> out;

			out[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			out[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			out[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			out[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
			out[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
			out[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
			out[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
			out[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
			out[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
			out[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
			out[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
			out[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
			out[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
			out[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
			out[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
			out[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

			T det = m[0] * out[0] + m[1] * out[4] + m[2] * out[8] + m[3] * out[12];
			T invdet = T(1) / det;
			for (int i = 0; i < 16; i++) out[i] *= invdet;
			return out;
		}

		// Transform a vector by this matrix
		template<typename Q>
		inline constexpr auto operator * (const olc::v_3d<Q>& v) const
		{
			auto& me = *this;
			olc::v_3d<Q> vOut;
			vOut.x = Q(me(0, 0) * v.x + me(1, 0) * v.y + me(2, 0) * v.z + me(3, 0) * v.w);
			vOut.y = Q(me(0, 1) * v.x + me(1, 1) * v.y + me(2, 1) * v.z + me(3, 1) * v.w);
			vOut.z = Q(me(0, 2) * v.x + me(1, 2) * v.y + me(2, 2) * v.z + me(3, 2) * v.w);
			vOut.w = Q(me(0, 3) * v.x + me(1, 3) * v.y + me(2, 3) * v.z + me(3, 3) * v.w);
			return vOut;
		}

		// Multiply this matrix with another
		template<typename Q>
		inline constexpr auto operator * (const olc::m_4d<Q>& rhs) const
		{
			auto& me = *this;
			olc::m_4d<T> out;
			for (size_t c = 0; c < 4; c++)
				for (size_t r = 0; r < 4; r++)
					out(c, r) = me(0, r) * rhs(c, 0) + me(1, r) * rhs(c, 1) + me(2, r) * rhs(c, 2) + me(3, r) * rhs(c, 3);
			return out;
		}
	};

	// Convenient types ready-to-go
	typedef m_4d<float> mf4d;
	typedef m_4d<double> md4d;
}

#define OLC_MATRIX4D_DEFINED 1
#endif


namespace olc::utils::hw3d
{
	struct mesh
	{
		std::vector<std::array<float, 4>> pos;
		std::vector<std::array<float, 4>> norm;
		std::vector<std::array<float, 2>> uv;
		std::vector<olc::Pixel> col;
		olc::DecalStructure layout = olc::DecalStructure::LIST;
	};

	olc::utils::hw3d::mesh CreateCube(const olc::vf3d& vSize, const olc::vf3d& vOffset = { 0,0,0 })
	{
		olc::utils::hw3d::mesh m;


		/*			 5		   6
				1        2
		
					 4		   7
				0		 3
		
		*/

		std::vector<olc::vf3d> verts;
		verts.push_back(olc::vf3d(0, 0, 0) + vOffset);
		verts.push_back(olc::vf3d(0, vSize.y, 0) + vOffset);
		verts.push_back(olc::vf3d(vSize.x, vSize.y, 0) + vOffset);
		verts.push_back(olc::vf3d(vSize.x, 0, 0) + vOffset);
		verts.push_back(olc::vf3d(0, 0, vSize.z) + vOffset);
		verts.push_back(olc::vf3d(0, vSize.y, vSize.z) + vOffset);
		verts.push_back(olc::vf3d(vSize.x, vSize.y, vSize.z) + vOffset);
		verts.push_back(olc::vf3d(vSize.x, 0, vSize.z) + vOffset);

		// South
		m.pos.push_back(verts[0].a()); m.norm.push_back({ 0, 0, -1, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[1].a()); m.norm.push_back({ 0, 0, -1, 0 }); m.uv.push_back({ 0, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[2].a()); m.norm.push_back({ 0, 0, -1, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[0].a()); m.norm.push_back({ 0, 0, -1, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[2].a()); m.norm.push_back({ 0, 0, -1, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[3].a()); m.norm.push_back({ 0, 0, -1, 0 }); m.uv.push_back({ 1, 1 }); m.col.push_back(olc::WHITE);

		// East
		m.pos.push_back(verts[3].a()); m.norm.push_back({ 1, 0, 0, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[2].a()); m.norm.push_back({ 1, 0, 0, 0 }); m.uv.push_back({ 0, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[6].a()); m.norm.push_back({ 1, 0, 0, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[3].a()); m.norm.push_back({ 1, 0, 0, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[6].a()); m.norm.push_back({ 1, 0, 0, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[7].a()); m.norm.push_back({ 1, 0, 0, 0 }); m.uv.push_back({ 1, 1 }); m.col.push_back(olc::WHITE);
		
		// North
		m.pos.push_back(verts[7].a()); m.norm.push_back({ 0, 0, 1, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[6].a()); m.norm.push_back({ 0, 0, 1, 0 }); m.uv.push_back({ 0, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[5].a()); m.norm.push_back({ 0, 0, 1, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[7].a()); m.norm.push_back({ 0, 0, 1, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[5].a()); m.norm.push_back({ 0, 0, 1, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[4].a()); m.norm.push_back({ 0, 0, 1, 0 }); m.uv.push_back({ 1, 1 }); m.col.push_back(olc::WHITE);

		// West
		m.pos.push_back(verts[4].a()); m.norm.push_back({-1, 0, 0, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[5].a()); m.norm.push_back({-1, 0, 0, 0 }); m.uv.push_back({ 0, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[1].a()); m.norm.push_back({-1, 0, 0, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[4].a()); m.norm.push_back({-1, 0, 0, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[1].a()); m.norm.push_back({-1, 0, 0, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[0].a()); m.norm.push_back({-1, 0, 0, 0 }); m.uv.push_back({ 1, 1 }); m.col.push_back(olc::WHITE);

		// Top
		m.pos.push_back(verts[1].a()); m.norm.push_back({ 0, 1, 0, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[5].a()); m.norm.push_back({ 0, 1, 0, 0 }); m.uv.push_back({ 0, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[6].a()); m.norm.push_back({ 0, 1, 0, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[1].a()); m.norm.push_back({ 0, 1, 0, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[6].a()); m.norm.push_back({ 0, 1, 0, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[2].a()); m.norm.push_back({ 0, 1, 0, 0 }); m.uv.push_back({ 1, 1 }); m.col.push_back(olc::WHITE);

		// Bottom
		m.pos.push_back(verts[7].a()); m.norm.push_back({ 0, -1, 0, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[4].a()); m.norm.push_back({ 0, -1, 0, 0 }); m.uv.push_back({ 0, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[0].a()); m.norm.push_back({ 0, -1, 0, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[7].a()); m.norm.push_back({ 0, -1, 0, 0 }); m.uv.push_back({ 0, 1 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[0].a()); m.norm.push_back({ 0, -1, 0, 0 }); m.uv.push_back({ 1, 0 }); m.col.push_back(olc::WHITE);
		m.pos.push_back(verts[3].a()); m.norm.push_back({ 0, -1, 0, 0 }); m.uv.push_back({ 1, 1 }); m.col.push_back(olc::WHITE);

		return m;
	}

	std::optional<olc::utils::hw3d::mesh> LoadObj(const std::string& path)
	{
		olc::utils::hw3d::mesh m;

		std::ifstream ifs(path);
		if (!ifs.is_open())
			return std::nullopt;

		std::vector<olc::vf3d> verts;
		std::vector<olc::vf3d> norms;
		std::vector<olc::vf2d> texs;
		std::vector<std::vector<std::vector<int>>> faces;

		std::string sLine;
		while (std::getline(ifs, sLine))
		{
			std::stringstream s(sLine);

			char junk;
			float x, y, z, u, v;

			if (sLine[0] == 'v')
			{
				if (sLine[1] == 't')
				{
					// Line is 2D texture coordinate
					s >> junk >> junk >> u >> v;
					texs.push_back({ u, v });
				}
				else if (sLine[1] == 'n')
				{
					// Line is a 3D normal					
					s >> junk >> junk >> x >> y >> z;
					norms.push_back({ x, y, z });
				}
				else
				{
					// Line is a 3D vertex
					s >> junk >> x >> y >> z;
					verts.push_back({ x, y, z });
				}
			}

			if (sLine[0] == 'f')
			{
				// Line is a Face description. A face may consist of multiple vertices.
				std::string sToken;
				std::vector<std::string> vTuples;
				s >> junk; while (s >> sToken) vTuples.push_back(sToken);

				std::vector<std::vector<int>> face;
				for (const auto& d : vTuples)
				{
					std::vector<int> indices;
					std::string temp;
					for (const auto c : d)
					{
						if (c == ' ' || c == '/')
						{
							if (!temp.empty())
							{
								indices.push_back(std::stoi(temp));
								temp.clear();
							}
						}
						else
							temp.append(1, c);
					}

					if (!temp.empty())
						indices.push_back(std::stoi(temp));

					face.push_back(indices);
				}

				faces.push_back(face);
			}
		}

		// Process into mesh
		for (const auto& face : faces)
		{
			if (face.size() == 3)
			{
				// Triangle
				for (const auto& index : face)
				{
					if (!verts.empty())
					{
						// 0 is vertex id
						m.pos.push_back(verts[index[0] - 1].a());
					}
					else
						m.pos.push_back({ 0,0,0,0 });

					if (!texs.empty())
					{
						// 1 is texture id
						m.uv.push_back(texs[index[1] - 1].a());
					}
					else
						m.uv.push_back({ 0,0 });

					if (!norms.empty())
					{
						// 0 is vertex id
						m.norm.push_back(norms[index[2] - 1].a());
					}
					else
						m.norm.push_back({ 0,0,0,0 });

					m.col.push_back(olc::WHITE);
				}
			}
		}

		return m;
	}

}