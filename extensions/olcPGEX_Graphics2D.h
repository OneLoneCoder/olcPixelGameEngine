/*
	olcPGEX_Graphics2D.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                Advanced 2D Rendering - v0.5                 |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	advanced olc::Sprite manipulation and drawing routines. To use
	it, simply include this header file.

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2019 OneLoneCoder.com

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
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/

/*
	Matrices stored as [Column][Row] (i.e. x, y)

	|C0R0 C1R0 C2R0|   | x |   | x'|
	|C0R1 C1R1 C2R1| * | y | = | y'|
	|C0R2 C1R2 C2R2|   |1.0|   | - |
*/



#ifndef OLC_PGEX_GFX2D
#define OLC_PGEX_GFX2D

#include <algorithm>
#undef min
#undef max

namespace olc
{
	// Container class for Advanced 2D Drawing functions
	class GFX2D : public olc::PGEX
	{
		// A representation of an affine transform, used to rotate, scale, offset & shear space
	public:
		class Transform2D
		{
		public:
			 Transform2D();

		public:
			// Set this transformation to unity
			 void Reset();
			// Append a rotation of fTheta radians to this transform
			 void Rotate(float fTheta);
			// Append a translation (ox, oy) to this transform
			 void Translate(float ox, float oy);
			// Append a scaling operation (sx, sy) to this transform
			 void Scale(float sx, float sy);
			// Append a shear operation (sx, sy) to this transform
			 void Shear(float sx, float sy);

			 void Perspective(float ox, float oy);
			// Calculate the Forward Transformation of the coordinate (in_x, in_y) -> (out_x, out_y)
			 void Forward(float in_x, float in_y, float &out_x, float &out_y);
			// Calculate the Inverse Transformation of the coordinate (in_x, in_y) -> (out_x, out_y)
			 void Backward(float in_x, float in_y, float &out_x, float &out_y);
			// Regenerate the Inverse Transformation
			 void Invert();

		private:
			 void Multiply();
			float matrix[4][3][3];
			int nTargetMatrix;
			int nSourceMatrix;
			bool bDirty;
		};

	public:
		// Draws a sprite with the transform applied
		 static void DrawSprite(olc::Sprite *sprite, olc::GFX2D::Transform2D &transform);
	};
}


#ifdef OLC_PGEX_GRAPHICS2D
#undef OLC_PGEX_GRAPHICS2D

namespace olc
{
	void GFX2D::DrawSprite(olc::Sprite *sprite, olc::GFX2D::Transform2D &transform)
	{
		if (sprite == nullptr)
			return;

		// Work out bounding rectangle of sprite
		float ex, ey;
		float sx, sy;		
		float px, py;

		transform.Forward(0.0f, 0.0f, sx, sy);
		px = sx; py = sy;
		sx = std::min(sx, px); sy = std::min(sy, py);
		ex = std::max(ex, px); ey = std::max(ey, py);
		
		transform.Forward((float)sprite->width, (float)sprite->height, px, py);
		sx = std::min(sx, px); sy = std::min(sy, py);
		ex = std::max(ex, px); ey = std::max(ey, py);

		transform.Forward(0.0f, (float)sprite->height, px, py);
		sx = std::min(sx, px); sy = std::min(sy, py);
		ex = std::max(ex, px); ey = std::max(ey, py);

		transform.Forward((float)sprite->width, 0.0f, px, py);
		sx = std::min(sx, px); sy = std::min(sy, py);
		ex = std::max(ex, px); ey = std::max(ey, py);

		// Perform inversion of transform if required
		transform.Invert();

		if (ex < sx) 
			std::swap(ex, sx);
		if (ey < sy) 
			std::swap(ey, sy);

		// Iterate through render space, and sample Sprite from suitable texel location
		for (float i = sx; i < ex; i++)
		{
			for (float j = sy; j < ey; j++)
			{
				float ox, oy;
				transform.Backward(i, j, ox, oy);
				pge->Draw((int32_t)i, (int32_t)j, sprite->GetPixel((int32_t)(ox+0.5f), (int32_t)(oy+0.5f)));
			}
		}
	}

	olc::GFX2D::Transform2D::Transform2D()
	{
		Reset();
	}

	void olc::GFX2D::Transform2D::Reset()
	{
		nTargetMatrix = 0;
		nSourceMatrix = 1;
		bDirty = true;

		// Columns Then Rows

		// Matrices 0 & 1 are used as swaps in Transform accumulation
		matrix[0][0][0] = 1.0f; matrix[0][1][0] = 0.0f; matrix[0][2][0] = 0.0f;
		matrix[0][0][1] = 0.0f; matrix[0][1][1] = 1.0f; matrix[0][2][1] = 0.0f;
		matrix[0][0][2] = 0.0f; matrix[0][1][2] = 0.0f; matrix[0][2][2] = 1.0f;

		matrix[1][0][0] = 1.0f; matrix[1][1][0] = 0.0f; matrix[1][2][0] = 0.0f;
		matrix[1][0][1] = 0.0f; matrix[1][1][1] = 1.0f; matrix[1][2][1] = 0.0f;
		matrix[1][0][2] = 0.0f; matrix[1][1][2] = 0.0f; matrix[1][2][2] = 1.0f;

		// Matrix 2 is a cache matrix to hold the immediate transform operation
		// Matrix 3 is a cache matrix to hold the inverted transform
	}

	void olc::GFX2D::Transform2D::Multiply()
	{
		for (int c = 0; c < 3; c++)
		{
			for (int r = 0; r < 3; r++)
			{
				matrix[nTargetMatrix][c][r] = matrix[2][0][r] * matrix[nSourceMatrix][c][0] +
											  matrix[2][1][r] * matrix[nSourceMatrix][c][1] +
											  matrix[2][2][r] * matrix[nSourceMatrix][c][2];
			}
		}

		std::swap(nTargetMatrix, nSourceMatrix);
		bDirty = true; // Any transform multiply dirties the inversion
	}

	void olc::GFX2D::Transform2D::Rotate(float fTheta)
	{		
		// Construct Rotation Matrix
		matrix[2][0][0] = cosf(fTheta);  matrix[2][1][0] = sinf(fTheta); matrix[2][2][0] = 0.0f;
		matrix[2][0][1] = -sinf(fTheta); matrix[2][1][1] = cosf(fTheta); matrix[2][2][1] = 0.0f;
		matrix[2][0][2] = 0.0f;			 matrix[2][1][2] = 0.0f;		 matrix[2][2][2] = 1.0f;
		Multiply();		
	}

	void olc::GFX2D::Transform2D::Scale(float sx, float sy)
	{
		// Construct Scale Matrix
		matrix[2][0][0] = sx;   matrix[2][1][0] = 0.0f; matrix[2][2][0] = 0.0f;
		matrix[2][0][1] = 0.0f; matrix[2][1][1] = sy;   matrix[2][2][1] = 0.0f;
		matrix[2][0][2] = 0.0f;	matrix[2][1][2] = 0.0f;	matrix[2][2][2] = 1.0f;
		Multiply();
	}

	void olc::GFX2D::Transform2D::Shear(float sx, float sy)
	{
		// Construct Shear Matrix		
		matrix[2][0][0] = 1.0f; matrix[2][1][0] = sx;   matrix[2][2][0] = 0.0f;
		matrix[2][0][1] = sy;   matrix[2][1][1] = 1.0f; matrix[2][2][1] = 0.0f;
		matrix[2][0][2] = 0.0f;	matrix[2][1][2] = 0.0f;	matrix[2][2][2] = 1.0f;
		Multiply();
	}

	void olc::GFX2D::Transform2D::Translate(float ox, float oy)
	{
		// Construct Translate Matrix
		matrix[2][0][0] = 1.0f; matrix[2][1][0] = 0.0f; matrix[2][2][0] = ox;
		matrix[2][0][1] = 0.0f; matrix[2][1][1] = 1.0f; matrix[2][2][1] = oy;
		matrix[2][0][2] = 0.0f;	matrix[2][1][2] = 0.0f;	matrix[2][2][2] = 1.0f;
		Multiply();
	}

	void olc::GFX2D::Transform2D::Perspective(float ox, float oy)
	{
		// Construct Translate Matrix
		matrix[2][0][0] = 1.0f; matrix[2][1][0] = 0.0f; matrix[2][2][0] = 0.0f;
		matrix[2][0][1] = 0.0f; matrix[2][1][1] = 1.0f; matrix[2][2][1] = 0.0f;
		matrix[2][0][2] = ox;	matrix[2][1][2] = oy;	matrix[2][2][2] = 1.0f;
		Multiply();
	}

	void olc::GFX2D::Transform2D::Forward(float in_x, float in_y, float &out_x, float &out_y)
	{
		out_x = in_x * matrix[nSourceMatrix][0][0] + in_y * matrix[nSourceMatrix][1][0] + matrix[nSourceMatrix][2][0];
		out_y = in_x * matrix[nSourceMatrix][0][1] + in_y * matrix[nSourceMatrix][1][1] + matrix[nSourceMatrix][2][1];
		float out_z = in_x * matrix[nSourceMatrix][0][2] + in_y * matrix[nSourceMatrix][1][2] + matrix[nSourceMatrix][2][2];
		if (out_z != 0)
		{
			out_x /= out_z;
			out_y /= out_z;
		}
	} 

	void olc::GFX2D::Transform2D::Backward(float in_x, float in_y, float &out_x, float &out_y)
	{
		out_x = in_x * matrix[3][0][0] + in_y * matrix[3][1][0] + matrix[3][2][0];
		out_y = in_x * matrix[3][0][1] + in_y * matrix[3][1][1] + matrix[3][2][1];
		float out_z = in_x * matrix[3][0][2] + in_y * matrix[3][1][2] + matrix[3][2][2];
		if (out_z != 0)
		{
			out_x /= out_z;
			out_y /= out_z;
		}
	}

	void olc::GFX2D::Transform2D::Invert()
	{
		if (bDirty) // Obviously costly so only do if needed
		{			
			float det = matrix[nSourceMatrix][0][0] * (matrix[nSourceMatrix][1][1] * matrix[nSourceMatrix][2][2] - matrix[nSourceMatrix][1][2] * matrix[nSourceMatrix][2][1]) -
				        matrix[nSourceMatrix][1][0] * (matrix[nSourceMatrix][0][1] * matrix[nSourceMatrix][2][2] - matrix[nSourceMatrix][2][1] * matrix[nSourceMatrix][0][2]) +
				        matrix[nSourceMatrix][2][0] * (matrix[nSourceMatrix][0][1] * matrix[nSourceMatrix][1][2] - matrix[nSourceMatrix][1][1] * matrix[nSourceMatrix][0][2]);

			float idet = 1.0f / det;
			matrix[3][0][0] = (matrix[nSourceMatrix][1][1] * matrix[nSourceMatrix][2][2] - matrix[nSourceMatrix][1][2] * matrix[nSourceMatrix][2][1]) * idet;
			matrix[3][1][0] = (matrix[nSourceMatrix][2][0] * matrix[nSourceMatrix][1][2] - matrix[nSourceMatrix][1][0] * matrix[nSourceMatrix][2][2]) * idet;
			matrix[3][2][0] = (matrix[nSourceMatrix][1][0] * matrix[nSourceMatrix][2][1] - matrix[nSourceMatrix][2][0] * matrix[nSourceMatrix][1][1]) * idet;
			matrix[3][0][1] = (matrix[nSourceMatrix][2][1] * matrix[nSourceMatrix][0][2] - matrix[nSourceMatrix][0][1] * matrix[nSourceMatrix][2][2]) * idet;
			matrix[3][1][1] = (matrix[nSourceMatrix][0][0] * matrix[nSourceMatrix][2][2] - matrix[nSourceMatrix][2][0] * matrix[nSourceMatrix][0][2]) * idet;
			matrix[3][2][1] = (matrix[nSourceMatrix][0][1] * matrix[nSourceMatrix][2][0] - matrix[nSourceMatrix][0][0] * matrix[nSourceMatrix][2][1]) * idet;
			matrix[3][0][2] = (matrix[nSourceMatrix][0][1] * matrix[nSourceMatrix][1][2] - matrix[nSourceMatrix][0][2] * matrix[nSourceMatrix][1][1]) * idet;
			matrix[3][1][2] = (matrix[nSourceMatrix][0][2] * matrix[nSourceMatrix][1][0] - matrix[nSourceMatrix][0][0] * matrix[nSourceMatrix][1][2]) * idet;
			matrix[3][2][2] = (matrix[nSourceMatrix][0][0] * matrix[nSourceMatrix][1][1] - matrix[nSourceMatrix][0][1] * matrix[nSourceMatrix][1][0]) * idet;
			bDirty = false;
		}				
	}
}

#endif
#endif