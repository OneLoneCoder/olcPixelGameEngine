/*
	olcPGEX_Graphics3D.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                    3D Rendering - v0.4                      |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	support for software rendering 3D graphics.

	NOTE!!! This file is under development and may change!

	Big Thanks to MaGetzUb for finding an OOB error, and joshinils
	for pointing out sampling inaccuracy.

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

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018
*/


#ifndef OLC_PGEX_GFX3D
#define OLC_PGEX_GFX3D

#include <algorithm>
#include <vector>
#include <list>
#include <sstream>
#include <string>
#include <cstdint>
#undef min
#undef max

//#include <omp.h>

namespace olc
{
	// Container class for Advanced 2D Drawing functions
	class GFX3D : public olc::PGEX
	{

	public:

		struct vec2d
		{
			float x = 0;
			float y = 0;
			float z = 0;
		};

		struct vec3d
		{
			float x = 0;
			float y = 0;
			float z = 0;
			float w = 1; // Need a 4th term to perform sensible matrix vector multiplication
		};

		struct triangle
		{
			vec3d p[3];
			vec2d t[3];
			olc::Pixel col[3];
		};

		struct mat4x4
		{
			float m[4][4] = { 0 };
		};

		struct mesh
		{
			std::vector<triangle> tris;
			bool LoadOBJFile(std::string sFilename, bool bHasTexture = false);
		};

		/*class MipMap : public olc::Sprite
		{
		public:
			MipMap();
			MipMap(std::string sImageFile);
			MipMap(std::string sImageFile, olc::ResourcePack *pack);
			MipMap(int32_t w, int32_t h);
			~MipMap();

		public:
			olc::rcode LoadFromFile(std::string sImageFile, olc::ResourcePack *pack = nullptr);
			olc::rcode LoadFromPGESprFile(std::string sImageFile, olc::ResourcePack *pack = nullptr);			
			Pixel Sample(float x, float y, float z);
			Pixel SampleBL(float u, float v, float z);

		private:
			int GenerateMipLevels();
			std::vector<olc::Sprite> vecMipMaps;

		};*/

		class Math
		{
		public:
			Math();
		public:
			static vec3d  Mat_MultiplyVector(mat4x4 &m, vec3d &i);
			static mat4x4 Mat_MultiplyMatrix(mat4x4 &m1, mat4x4 &m2);
			static mat4x4 Mat_MakeIdentity();
			static mat4x4 Mat_MakeRotationX(float fAngleRad);
			static mat4x4 Mat_MakeRotationY(float fAngleRad);
			static mat4x4 Mat_MakeRotationZ(float fAngleRad);
			static mat4x4 Mat_MakeScale(float x, float y, float z);
			static mat4x4 Mat_MakeTranslation(float x, float y, float z);
			static mat4x4 Mat_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
			static mat4x4 Mat_PointAt(vec3d &pos, vec3d &target, vec3d &up);
			static mat4x4 Mat_QuickInverse(mat4x4 &m); // Only for Rotation/Translation Matrices
			static mat4x4 Mat_Inverse(olc::GFX3D::mat4x4 &m);
		
			static vec3d Vec_Add(vec3d &v1, vec3d &v2);
			static vec3d Vec_Sub(vec3d &v1, vec3d &v2);
			static vec3d Vec_Mul(vec3d &v1, float k);
			static vec3d Vec_Div(vec3d &v1, float k);
			static float Vec_DotProduct(vec3d &v1, vec3d &v2);
			static float Vec_Length(vec3d &v);
			static vec3d Vec_Normalise(vec3d &v);
			static vec3d Vec_CrossProduct(vec3d &v1, vec3d &v2);
			static vec3d Vec_IntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd, float &t);

			static int Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2);
		};

		enum RENDERFLAGS
		{
			RENDER_WIRE = 0x01,
			RENDER_FLAT = 0x02,
			RENDER_TEXTURED = 0x04,
			RENDER_CULL_CW = 0x08,
			RENDER_CULL_CCW = 0x10,
			RENDER_DEPTH = 0x20,
			RENDER_LIGHTS = 0x40,
		};

		enum LIGHTS
		{
			LIGHT_DISABLED,
			LIGHT_AMBIENT,
			LIGHT_DIRECTIONAL,
			LIGHT_POINT
		};
		

		class PipeLine
		{
		public:
			PipeLine();

		public:
			void SetProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar, float fLeft, float fTop, float fWidth, float fHeight);
			void SetCamera(olc::GFX3D::vec3d &pos, olc::GFX3D::vec3d &lookat, olc::GFX3D::vec3d &up);
			void SetTransform(olc::GFX3D::mat4x4 &transform);
			void SetTexture(olc::Sprite *texture);
			//void SetMipMapTexture(olc::GFX3D::MipMap *texture);
			void SetLightSource(uint32_t nSlot, uint32_t nType, olc::Pixel col, olc::GFX3D::vec3d pos, olc::GFX3D::vec3d dir = { 0.0f, 0.0f, 1.0f }, float fParam = 0.0f);
			uint32_t Render(std::vector<olc::GFX3D::triangle> &triangles, uint32_t flags = RENDER_CULL_CW | RENDER_TEXTURED | RENDER_DEPTH);
			uint32_t Render(std::vector<olc::GFX3D::triangle> &triangles, uint32_t flags, int nOffset, int nCount);
			uint32_t RenderLine(olc::GFX3D::vec3d &p1, olc::GFX3D::vec3d &p2, olc::Pixel col = olc::WHITE);
			uint32_t RenderCircleXZ(olc::GFX3D::vec3d &p1, float r, olc::Pixel col = olc::WHITE);

		private:
			olc::GFX3D::mat4x4 matProj;
			olc::GFX3D::mat4x4 matView;
			olc::GFX3D::mat4x4 matWorld;
			olc::Sprite *sprTexture;
			//olc::GFX3D::MipMap *sprMipMap;
			//bool bUseMipMap;
			float fViewX;
			float fViewY;
			float fViewW;
			float fViewH;

			struct sLight
			{
				uint32_t type;
				olc::GFX3D::vec3d pos;
				olc::GFX3D::vec3d dir;
				olc::Pixel col;
				float param;
			} lights[4];
		};

		

	public:
		//static const int RF_TEXTURE = 0x00000001;
		//static const int RF_ = 0x00000002;

		 static void ConfigureDisplay();
		 static void ClearDepth();
		 static void AddTriangleToScene(olc::GFX3D::triangle &tri);
		 static void RenderScene();

		 static void DrawTriangleFlat(olc::GFX3D::triangle &tri);
		 static void DrawTriangleWire(olc::GFX3D::triangle &tri, olc::Pixel col = olc::WHITE);
		 static void DrawTriangleTex(olc::GFX3D::triangle &tri, olc::Sprite* spr);
		 static void TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
			int x2, int y2, float u2, float v2, float w2,
			int x3, int y3, float u3, float v3, float w3, olc::Sprite* spr);

		 static void RasterTriangle(int x1, int y1, float u1, float v1, float w1, olc::Pixel c1,
			 int x2, int y2, float u2, float v2, float w2, olc::Pixel c2,
			 int x3, int y3, float u3, float v3, float w3, olc::Pixel c3,
			 olc::Sprite* spr,
			 uint32_t nFlags);

		// Draws a sprite with the transform applied
		//inline static void DrawSprite(olc::Sprite *sprite, olc::GFX2D::Transform2D &transform);

	private:
		static float* m_DepthBuffer;
	};
}

#endif


#ifdef OLC_PGEX_GRAPHICS3D
#undef OLC_PGEX_GRAPHICS3D

namespace olc
{
	olc::GFX3D::Math::Math()
	{

	}

	olc::GFX3D::vec3d olc::GFX3D::Math::Mat_MultiplyVector(olc::GFX3D::mat4x4 &m, olc::GFX3D::vec3d &i)
	{
		vec3d v;
		v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
		v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
		v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
		v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
		return v;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_MakeIdentity()
	{
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_MakeRotationX(float fAngleRad)
	{
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = cosf(fAngleRad);
		matrix.m[1][2] = sinf(fAngleRad);
		matrix.m[2][1] = -sinf(fAngleRad);
		matrix.m[2][2] = cosf(fAngleRad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_MakeRotationY(float fAngleRad)
	{
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = cosf(fAngleRad);
		matrix.m[0][2] = sinf(fAngleRad);
		matrix.m[2][0] = -sinf(fAngleRad);
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = cosf(fAngleRad);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_MakeRotationZ(float fAngleRad)
	{
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = cosf(fAngleRad);
		matrix.m[0][1] = sinf(fAngleRad);
		matrix.m[1][0] = -sinf(fAngleRad);
		matrix.m[1][1] = cosf(fAngleRad);
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_MakeScale(float x, float y, float z)
	{
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = x;
		matrix.m[1][1] = y;
		matrix.m[2][2] = z;
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_MakeTranslation(float x, float y, float z)
	{
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = 1.0f;
		matrix.m[1][1] = 1.0f;
		matrix.m[2][2] = 1.0f;
		matrix.m[3][3] = 1.0f;
		matrix.m[3][0] = x;
		matrix.m[3][1] = y;
		matrix.m[3][2] = z;
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
	{
		float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = fAspectRatio * fFovRad;
		matrix.m[1][1] = fFovRad;
		matrix.m[2][2] = fFar / (fFar - fNear);
		matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matrix.m[2][3] = 1.0f;
		matrix.m[3][3] = 0.0f;
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_MultiplyMatrix(olc::GFX3D::mat4x4 &m1, olc::GFX3D::mat4x4 &m2)
	{
		olc::GFX3D::mat4x4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_PointAt(olc::GFX3D::vec3d &pos, olc::GFX3D::vec3d &target, olc::GFX3D::vec3d &up)
	{
		// Calculate new forward direction
		olc::GFX3D::vec3d newForward = Vec_Sub(target, pos);
		newForward = Vec_Normalise(newForward);

		// Calculate new Up direction
		olc::GFX3D::vec3d a = Vec_Mul(newForward, Vec_DotProduct(up, newForward));
		olc::GFX3D::vec3d newUp = Vec_Sub(up, a);
		newUp = Vec_Normalise(newUp);

		// New Right direction is easy, its just cross product
		olc::GFX3D::vec3d newRight = Vec_CrossProduct(newUp, newForward);

		// Construct Dimensioning and Translation Matrix	
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
		return matrix;

	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_QuickInverse(olc::GFX3D::mat4x4 &m) // Only for Rotation/Translation Matrices
	{
		olc::GFX3D::mat4x4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;
	}

	olc::GFX3D::mat4x4 olc::GFX3D::Math::Mat_Inverse(olc::GFX3D::mat4x4 &m)
	{
		double  det;
		

		mat4x4 matInv;

		matInv.m[0][0] =  m.m[1][1] * m.m[2][2] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[2][1] * m.m[1][2] * m.m[3][3] + m.m[2][1] * m.m[1][3] * m.m[3][2] + m.m[3][1] * m.m[1][2] * m.m[2][3] - m.m[3][1] * m.m[1][3] * m.m[2][2];
		matInv.m[1][0] = -m.m[1][0] * m.m[2][2] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2] + m.m[2][0] * m.m[1][2] * m.m[3][3] - m.m[2][0] * m.m[1][3] * m.m[3][2] - m.m[3][0] * m.m[1][2] * m.m[2][3] + m.m[3][0] * m.m[1][3] * m.m[2][2];
		matInv.m[2][0] =  m.m[1][0] * m.m[2][1] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[2][0] * m.m[1][1] * m.m[3][3] + m.m[2][0] * m.m[1][3] * m.m[3][1] + m.m[3][0] * m.m[1][1] * m.m[2][3] - m.m[3][0] * m.m[1][3] * m.m[2][1];
		matInv.m[3][0] = -m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[2][0] * m.m[1][1] * m.m[3][2] - m.m[2][0] * m.m[1][2] * m.m[3][1] - m.m[3][0] * m.m[1][1] * m.m[2][2] + m.m[3][0] * m.m[1][2] * m.m[2][1];
		matInv.m[0][1] = -m.m[0][1] * m.m[2][2] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2] + m.m[2][1] * m.m[0][2] * m.m[3][3] - m.m[2][1] * m.m[0][3] * m.m[3][2] - m.m[3][1] * m.m[0][2] * m.m[2][3] + m.m[3][1] * m.m[0][3] * m.m[2][2];
		matInv.m[1][1] =  m.m[0][0] * m.m[2][2] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2] - m.m[2][0] * m.m[0][2] * m.m[3][3] + m.m[2][0] * m.m[0][3] * m.m[3][2] + m.m[3][0] * m.m[0][2] * m.m[2][3] - m.m[3][0] * m.m[0][3] * m.m[2][2];
		matInv.m[2][1] = -m.m[0][0] * m.m[2][1] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1] + m.m[2][0] * m.m[0][1] * m.m[3][3] - m.m[2][0] * m.m[0][3] * m.m[3][1] - m.m[3][0] * m.m[0][1] * m.m[2][3] + m.m[3][0] * m.m[0][3] * m.m[2][1];
		matInv.m[3][1] =  m.m[0][0] * m.m[2][1] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1] - m.m[2][0] * m.m[0][1] * m.m[3][2] + m.m[2][0] * m.m[0][2] * m.m[3][1] + m.m[3][0] * m.m[0][1] * m.m[2][2] - m.m[3][0] * m.m[0][2] * m.m[2][1];
		matInv.m[0][2] =  m.m[0][1] * m.m[1][2] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2] - m.m[1][1] * m.m[0][2] * m.m[3][3] + m.m[1][1] * m.m[0][3] * m.m[3][2] + m.m[3][1] * m.m[0][2] * m.m[1][3] - m.m[3][1] * m.m[0][3] * m.m[1][2];
		matInv.m[1][2] = -m.m[0][0] * m.m[1][2] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2] + m.m[1][0] * m.m[0][2] * m.m[3][3] - m.m[1][0] * m.m[0][3] * m.m[3][2] - m.m[3][0] * m.m[0][2] * m.m[1][3] + m.m[3][0] * m.m[0][3] * m.m[1][2];
		matInv.m[2][2] =  m.m[0][0] * m.m[1][1] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1] - m.m[1][0] * m.m[0][1] * m.m[3][3] + m.m[1][0] * m.m[0][3] * m.m[3][1] + m.m[3][0] * m.m[0][1] * m.m[1][3] - m.m[3][0] * m.m[0][3] * m.m[1][1];
		matInv.m[3][2] = -m.m[0][0] * m.m[1][1] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1] + m.m[1][0] * m.m[0][1] * m.m[3][2] - m.m[1][0] * m.m[0][2] * m.m[3][1] - m.m[3][0] * m.m[0][1] * m.m[1][2] + m.m[3][0] * m.m[0][2] * m.m[1][1];
		matInv.m[0][3] = -m.m[0][1] * m.m[1][2] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2] + m.m[1][1] * m.m[0][2] * m.m[2][3] - m.m[1][1] * m.m[0][3] * m.m[2][2] - m.m[2][1] * m.m[0][2] * m.m[1][3] + m.m[2][1] * m.m[0][3] * m.m[1][2];
		matInv.m[1][3] =  m.m[0][0] * m.m[1][2] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2] - m.m[1][0] * m.m[0][2] * m.m[2][3] + m.m[1][0] * m.m[0][3] * m.m[2][2] + m.m[2][0] * m.m[0][2] * m.m[1][3] - m.m[2][0] * m.m[0][3] * m.m[1][2];
		matInv.m[2][3] = -m.m[0][0] * m.m[1][1] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1] + m.m[1][0] * m.m[0][1] * m.m[2][3] - m.m[1][0] * m.m[0][3] * m.m[2][1] - m.m[2][0] * m.m[0][1] * m.m[1][3] + m.m[2][0] * m.m[0][3] * m.m[1][1];
		matInv.m[3][3] =  m.m[0][0] * m.m[1][1] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1] - m.m[1][0] * m.m[0][1] * m.m[2][2] + m.m[1][0] * m.m[0][2] * m.m[2][1] + m.m[2][0] * m.m[0][1] * m.m[1][2] - m.m[2][0] * m.m[0][2] * m.m[1][1];

		det = m.m[0][0] * matInv.m[0][0] + m.m[0][1] * matInv.m[1][0] + m.m[0][2] * matInv.m[2][0] + m.m[0][3] * matInv.m[3][0];
	//	if (det == 0) return false;

		det = 1.0 / det;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				matInv.m[i][j] *= (float)det;

		return matInv;
	}

	olc::GFX3D::vec3d olc::GFX3D::Math::Vec_Add(olc::GFX3D::vec3d &v1, olc::GFX3D::vec3d &v2)
	{
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	olc::GFX3D::vec3d olc::GFX3D::Math::Vec_Sub(olc::GFX3D::vec3d &v1, olc::GFX3D::vec3d &v2)
	{
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	olc::GFX3D::vec3d olc::GFX3D::Math::Vec_Mul(olc::GFX3D::vec3d &v1, float k)
	{
		return { v1.x * k, v1.y * k, v1.z * k };
	}

	olc::GFX3D::vec3d olc::GFX3D::Math::Vec_Div(olc::GFX3D::vec3d &v1, float k)
	{
		return { v1.x / k, v1.y / k, v1.z / k };
	}

	float olc::GFX3D::Math::Vec_DotProduct(olc::GFX3D::vec3d &v1, olc::GFX3D::vec3d &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z * v2.z;
	}

	float olc::GFX3D::Math::Vec_Length(olc::GFX3D::vec3d &v)
	{
		return sqrtf(Vec_DotProduct(v, v));
	}

	olc::GFX3D::vec3d olc::GFX3D::Math::Vec_Normalise(olc::GFX3D::vec3d &v)
	{
		float l = Vec_Length(v);
		return { v.x / l, v.y / l, v.z / l };
	}

	olc::GFX3D::vec3d olc::GFX3D::Math::Vec_CrossProduct(olc::GFX3D::vec3d &v1, olc::GFX3D::vec3d &v2)
	{
		vec3d v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	}

	olc::GFX3D::vec3d olc::GFX3D::Math::Vec_IntersectPlane(olc::GFX3D::vec3d &plane_p, olc::GFX3D::vec3d &plane_n, olc::GFX3D::vec3d &lineStart, olc::GFX3D::vec3d &lineEnd, float &t)
	{
		plane_n = Vec_Normalise(plane_n);
		float plane_d = -Vec_DotProduct(plane_n, plane_p);
		float ad = Vec_DotProduct(lineStart, plane_n);
		float bd = Vec_DotProduct(lineEnd, plane_n);
		t = (-plane_d - ad) / (bd - ad);
		olc::GFX3D::vec3d lineStartToEnd = Vec_Sub(lineEnd, lineStart);
		olc::GFX3D::vec3d lineToIntersect = Vec_Mul(lineStartToEnd, t);
		return Vec_Add(lineStart, lineToIntersect);
	}
	

	int olc::GFX3D::Math::Triangle_ClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2)
	{
		// Make sure plane normal is indeed normal
		plane_n = Math::Vec_Normalise(plane_n);

		out_tri1.t[0] = in_tri.t[0];
		out_tri2.t[0] = in_tri.t[0];
		out_tri1.t[1] = in_tri.t[1];
		out_tri2.t[1] = in_tri.t[1];
		out_tri1.t[2] = in_tri.t[2];
		out_tri2.t[2] = in_tri.t[2];

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto dist = [&](vec3d &p)
		{
			vec3d n = Math::Vec_Normalise(p);
			return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Math::Vec_DotProduct(plane_n, plane_p));
		};

		// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		vec3d* inside_points[3];  int nInsidePointCount = 0;
		vec3d* outside_points[3]; int nOutsidePointCount = 0;
		vec2d* inside_tex[3]; int nInsideTexCount = 0;
		vec2d* outside_tex[3]; int nOutsideTexCount = 0;


		// Get signed distance of each point in triangle to plane
		float d0 = dist(in_tri.p[0]);
		float d1 = dist(in_tri.p[1]);
		float d2 = dist(in_tri.p[2]);

		if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; inside_tex[nInsideTexCount++] = &in_tri.t[0]; }
		else {
			outside_points[nOutsidePointCount++] = &in_tri.p[0]; outside_tex[nOutsideTexCount++] = &in_tri.t[0];
		}
		if (d1 >= 0) {
			inside_points[nInsidePointCount++] = &in_tri.p[1]; inside_tex[nInsideTexCount++] = &in_tri.t[1];
		}
		else {
			outside_points[nOutsidePointCount++] = &in_tri.p[1];  outside_tex[nOutsideTexCount++] = &in_tri.t[1];
		}
		if (d2 >= 0) {
			inside_points[nInsidePointCount++] = &in_tri.p[2]; inside_tex[nInsideTexCount++] = &in_tri.t[2];
		}
		else {
			outside_points[nOutsidePointCount++] = &in_tri.p[2];  outside_tex[nOutsideTexCount++] = &in_tri.t[2];
		}

		// Now classify triangle points, and break the input triangle into 
		// smaller output triangles if required. There are four possible
		// outcomes...

		if (nInsidePointCount == 0)
		{
			// All points lie on the outside of plane, so clip whole triangle
			// It ceases to exist

			return 0; // No returned triangles are valid
		}

		if (nInsidePointCount == 3)
		{
			// All points lie on the inside of plane, so do nothing
			// and allow the triangle to simply pass through
			out_tri1 = in_tri;

			return 1; // Just the one returned original triangle is valid
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2)
		{
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// Copy appearance info to new triangle
			out_tri1.col[0] = in_tri.col[0];
			out_tri1.col[1] = in_tri.col[1];
			out_tri1.col[2] = in_tri.col[2];

			// The inside point is valid, so keep that...
			out_tri1.p[0] = *inside_points[0];
			out_tri1.t[0] = *inside_tex[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			float t;
			out_tri1.p[1] = Math::Vec_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.t[1].x = t * (outside_tex[0]->x - inside_tex[0]->x) + inside_tex[0]->x;
			out_tri1.t[1].y = t * (outside_tex[0]->y - inside_tex[0]->y) + inside_tex[0]->y;
			out_tri1.t[1].z = t * (outside_tex[0]->z - inside_tex[0]->z) + inside_tex[0]->z;

			out_tri1.p[2] = Math::Vec_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1], t);
			out_tri1.t[2].x = t * (outside_tex[1]->x - inside_tex[0]->x) + inside_tex[0]->x;
			out_tri1.t[2].y = t * (outside_tex[1]->y - inside_tex[0]->y) + inside_tex[0]->y;
			out_tri1.t[2].z = t * (outside_tex[1]->z - inside_tex[0]->z) + inside_tex[0]->z;

			return 1; // Return the newly formed single triangle
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1)
		{
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// Copy appearance info to new triangles
			out_tri1.col[0] = in_tri.col[0];
			out_tri2.col[0] = in_tri.col[0];
			out_tri1.col[1] = in_tri.col[1];
			out_tri2.col[1] = in_tri.col[1];
			out_tri1.col[2] = in_tri.col[2];
			out_tri2.col[2] = in_tri.col[2];
			
			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri1.p[0] = *inside_points[0];			
			out_tri1.t[0] = *inside_tex[0];

			out_tri1.p[1] = *inside_points[1];
			out_tri1.t[1] = *inside_tex[1];

			float t;
			out_tri1.p[2] = Math::Vec_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0], t);
			out_tri1.t[2].x = t * (outside_tex[0]->x - inside_tex[0]->x) + inside_tex[0]->x;
			out_tri1.t[2].y = t * (outside_tex[0]->y - inside_tex[0]->y) + inside_tex[0]->y;
			out_tri1.t[2].z = t * (outside_tex[0]->z - inside_tex[0]->z) + inside_tex[0]->z;

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			out_tri2.p[1] = *inside_points[1];
			out_tri2.t[1] = *inside_tex[1];
			out_tri2.p[0] = out_tri1.p[2];
			out_tri2.t[0] = out_tri1.t[2];
			out_tri2.p[2] = Math::Vec_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0], t);
			out_tri2.t[2].x = t * (outside_tex[0]->x - inside_tex[1]->x) + inside_tex[1]->x;
			out_tri2.t[2].y = t * (outside_tex[0]->y - inside_tex[1]->y) + inside_tex[1]->y;
			out_tri2.t[2].z = t * (outside_tex[0]->z - inside_tex[1]->z) + inside_tex[1]->z;
			return 2; // Return two newly formed triangles which form a quad
		}

		return 0;
	}

	void GFX3D::DrawTriangleFlat(olc::GFX3D::triangle &tri)
	{
		pge->FillTriangle((int32_t)tri.p[0].x, (int32_t)tri.p[0].y, (int32_t)tri.p[1].x, (int32_t)tri.p[1].y, (int32_t)tri.p[2].x, (int32_t)tri.p[2].y, tri.col[0]);
	}

	void GFX3D::DrawTriangleWire(olc::GFX3D::triangle &tri, olc::Pixel col)
	{
		pge->DrawTriangle((int32_t)tri.p[0].x, (int32_t)tri.p[0].y, (int32_t)tri.p[1].x, (int32_t)tri.p[1].y, (int32_t)tri.p[2].x, (int32_t)tri.p[2].y, col);
	}

	void GFX3D::TexturedTriangle(int x1, int y1, float u1, float v1, float w1,
		int x2, int y2, float u2, float v2, float w2,
		int x3, int y3, float u3, float v3, float w3, olc::Sprite* spr)
		
	{
		if (y2 < y1)
		{
			std::swap(y1, y2);
			std::swap(x1, x2);
			std::swap(u1, u2);
			std::swap(v1, v2);
			std::swap(w1, w2);
		}

		if (y3 < y1)
		{
			std::swap(y1, y3);
			std::swap(x1, x3);
			std::swap(u1, u3);
			std::swap(v1, v3);
			std::swap(w1, w3);
		}

		if (y3 < y2)
		{
			std::swap(y2, y3);
			std::swap(x2, x3);
			std::swap(u2, u3);
			std::swap(v2, v3);
			std::swap(w2, w3);
		}

		int dy1 = y2 - y1;
		int dx1 = x2 - x1;
		float dv1 = v2 - v1;
		float du1 = u2 - u1;
		float dw1 = w2 - w1;

		int dy2 = y3 - y1;
		int dx2 = x3 - x1;
		float dv2 = v3 - v1;
		float du2 = u3 - u1;
		float dw2 = w3 - w1;

		float tex_u, tex_v, tex_w;

		float dax_step = 0, dbx_step = 0,
			du1_step = 0, dv1_step = 0,
			du2_step = 0, dv2_step = 0,
			dw1_step = 0, dw2_step = 0;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy2) du2_step = du2 / (float)abs(dy2);
		if (dy2) dv2_step = dv2 / (float)abs(dy2);
		if (dy2) dw2_step = dw2 / (float)abs(dy2);

		if (dy1)
		{
			for (int i = y1; i <= y2; i++)
			{
				int ax = int(x1 + (float)(i - y1) * dax_step);
				int bx = int(x1 + (float)(i - y1) * dbx_step);

				float tex_su = u1 + (float)(i - y1) * du1_step;
				float tex_sv = v1 + (float)(i - y1) * dv1_step;
				float tex_sw = w1 + (float)(i - y1) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx)
				{
					std::swap(ax, bx);
					std::swap(tex_su, tex_eu);
					std::swap(tex_sv, tex_ev);
					std::swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;
					if (tex_w > m_DepthBuffer[i*pge->ScreenWidth() + j])
					{
						/*if (bMipMap)
							pge->Draw(j, i, ((olc::GFX3D::MipMap*)spr)->Sample(tex_u / tex_w, tex_v / tex_w, tex_w));
						else*/
						if(pge->Draw(j, i, spr != nullptr ? spr->Sample(tex_u / tex_w, tex_v / tex_w) : olc::GREY))
							m_DepthBuffer[i*pge->ScreenWidth() + j] = tex_w;
					}
					t += tstep;
				}

			}
		}

		dy1 = y3 - y2;
		dx1 = x3 - x2;
		dv1 = v3 - v2;
		du1 = u3 - u2;
		dw1 = w3 - w2;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		du1_step = 0, dv1_step = 0;
		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy1)
		{
			for (int i = y2; i <= y3; i++)
			{
				int ax = int(x2 + (float)(i - y2) * dax_step);
				int bx = int(x1 + (float)(i - y1) * dbx_step);

				float tex_su = u2 + (float)(i - y2) * du1_step;
				float tex_sv = v2 + (float)(i - y2) * dv1_step;
				float tex_sw = w2 + (float)(i - y2) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				if (ax > bx)
				{
					std::swap(ax, bx);
					std::swap(tex_su, tex_eu);
					std::swap(tex_sv, tex_ev);
					std::swap(tex_sw, tex_ew);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;

					if (tex_w > m_DepthBuffer[i*pge->ScreenWidth() + j])
					{						
						/*if(bMipMap)
							pge->Draw(j, i, ((olc::GFX3D::MipMap*)spr)->Sample(tex_u / tex_w, tex_v / tex_w, tex_w));
						else*/
						if(pge->Draw(j, i, spr != nullptr ? spr->Sample(tex_u / tex_w, tex_v / tex_w) : olc::GREY))
							m_DepthBuffer[i*pge->ScreenWidth() + j] = tex_w;
					}
					t += tstep;
				}
			}
		}
	}


	void GFX3D::DrawTriangleTex(olc::GFX3D::triangle &tri, olc::Sprite* spr)
	{
	
	}

	float* GFX3D::m_DepthBuffer = nullptr;

	void GFX3D::ConfigureDisplay()
	{
		m_DepthBuffer = new float[pge->ScreenWidth() * pge->ScreenHeight()]{ 0 };
	}


	void GFX3D::ClearDepth()
	{
		memset(m_DepthBuffer, 0, pge->ScreenWidth() * pge->ScreenHeight() * sizeof(float));
	}

	bool GFX3D::mesh::LoadOBJFile(std::string sFilename, bool bHasTexture)
	{
		std::ifstream f(sFilename);
		if (!f.is_open()) return false;

		// Local cache of verts
		std::vector<vec3d> verts;
		std::vector<vec3d> norms;
		std::vector<vec2d> texs;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::stringstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				if (line[1] == 't')
				{
					vec2d v;
					s >> junk >> junk >> v.x >> v.y;
					//v.x = 1.0f - v.x;
					v.y = 1.0f - v.y;
					texs.push_back(v);
				}
				else if (line[1] == 'n')
				{
					vec3d v;
					s >> junk >> junk >> v.x >> v.y >> v.z;										
					norms.push_back(v);
				}
				else
				{
					vec3d v;
					s >> junk >> v.x >> v.y >> v.z;
					verts.push_back(v);
				}
			}


			/*if (!bHasTexture)
			{
				if (line[0] == 'f')
				{
					int f[3];
					s >> junk >> f[0] >> f[1] >> f[2];
					tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
				}
			}
			else*/
			{
				if (line[0] == 'f')
				{
					s >> junk;

					std::string tokens[9];
					int nTokenCount = -1;
					while (!s.eof())
					{
						char c = s.get();
						if (c == ' ' || c == '/')
						{
							if (tokens[nTokenCount].size() > 0)
							{
								nTokenCount++;
							}
						}
						else
							tokens[nTokenCount].append(1, c);
					}

					tokens[nTokenCount].pop_back();

					int stride = 1;
					if (!texs.empty()) stride++;
					if (!norms.empty()) stride++;

					if (!texs.empty())
					{
						tris.push_back({ 
							verts[stoi(tokens[0 * stride]) - 1], 
							verts[stoi(tokens[1 * stride]) - 1], 
							verts[stoi(tokens[2 * stride]) - 1],
							texs[stoi(tokens[0 * stride + 1]) - 1], 
							texs[stoi(tokens[1 * stride + 1]) - 1], 
							texs[stoi(tokens[2 * stride + 1]) - 1], 
							olc::WHITE, olc::WHITE, olc::WHITE});
					}
					else
					{
						tris.push_back({ 
							verts[stoi(tokens[0 * stride]) - 1], 
							verts[stoi(tokens[1 * stride]) - 1], 
							verts[stoi(tokens[2 * stride]) - 1], 
							olc::GFX3D::vec2d{0,0,0}, 
							olc::GFX3D::vec2d{0,0,0},
							olc::GFX3D::vec2d{0,0,0},
							olc::WHITE, olc::WHITE, olc::WHITE });

					}
				}
			}
		}
		return true;
	}


	GFX3D::PipeLine::PipeLine()
	{
		//bUseMipMap = false;
	}
	
	void GFX3D::PipeLine::SetProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar, float fLeft, float fTop, float fWidth, float fHeight)
	{
		matProj = GFX3D::Math::Mat_MakeProjection(fFovDegrees, fAspectRatio, fNear, fFar);
		fViewX = fLeft;
		fViewY = fTop;
		fViewW = fWidth;
		fViewH = fHeight;
	}

	void GFX3D::PipeLine::SetCamera(olc::GFX3D::vec3d &pos, olc::GFX3D::vec3d &lookat, olc::GFX3D::vec3d &up)
	{
		matView = GFX3D::Math::Mat_PointAt(pos, lookat, up);
		matView = GFX3D::Math::Mat_QuickInverse(matView);
	}

	void GFX3D::PipeLine::SetTransform(olc::GFX3D::mat4x4 &transform)
	{
		matWorld = transform;
	}

	void GFX3D::PipeLine::SetTexture(olc::Sprite *texture)
	{
		sprTexture = texture;
		//bUseMipMap = false;
	}

	/*void GFX3D::PipeLine::SetMipMapTexture(olc::GFX3D::MipMap *texture)
	{
		sprMipMap = texture;
		bUseMipMap = true;
	}*/

	void GFX3D::PipeLine::SetLightSource(uint32_t nSlot, uint32_t nType, olc::Pixel col, olc::GFX3D::vec3d pos, olc::GFX3D::vec3d dir, float fParam)
	{
		if (nSlot < 4)
		{
			lights[nSlot].type = nType;
			lights[nSlot].pos = pos;
			lights[nSlot].dir = dir;
			lights[nSlot].col = col;
			lights[nSlot].param = fParam;
		}
	}

	uint32_t GFX3D::PipeLine::Render(std::vector<olc::GFX3D::triangle> &triangles, uint32_t flags)
	{
		return Render(triangles, flags, 0, triangles.size());
	}

	uint32_t GFX3D::PipeLine::RenderLine(olc::GFX3D::vec3d &p1, olc::GFX3D::vec3d &p2, olc::Pixel col)
	{
		// Coordinates are assumed to be in world space
		olc::GFX3D::vec3d t1, t2;
		
		// Transform into view
		t1 = GFX3D::Math::Mat_MultiplyVector(matView, p1);
		t2 = GFX3D::Math::Mat_MultiplyVector(matView, p2);

		// Project onto screen
		t1 = GFX3D::Math::Mat_MultiplyVector(matProj, t1);
		t2 = GFX3D::Math::Mat_MultiplyVector(matProj, t2);

		// Project
		t1.x = t1.x / t1.w;
		t1.y = t1.y / t1.w;
		t1.z = t1.z / t1.w;

		t2.x = t2.x / t2.w;
		t2.y = t2.y / t2.w;
		t2.z = t2.z / t2.w;

		vec3d vOffsetView = { 1,1,0 };
		t1 = Math::Vec_Add(t1, vOffsetView);
		t2 = Math::Vec_Add(t2, vOffsetView);
		
		t1.x *= 0.5f * fViewW;
		t1.y *= 0.5f * fViewH;
		t2.x *= 0.5f * fViewW;
		t2.y *= 0.5f * fViewH;
		
		vOffsetView = { fViewX,fViewY,0 };
		t1 = Math::Vec_Add(t1, vOffsetView);
		t2 = Math::Vec_Add(t2, vOffsetView);
		
		pge->DrawLine((int32_t)t1.x, (int32_t)t1.y, (int32_t)t2.x, (int32_t)t2.y, col);
		
		return 0;
	}

	uint32_t GFX3D::PipeLine::RenderCircleXZ(olc::GFX3D::vec3d &p1, float r, olc::Pixel col)
	{
		// Coordinates are assumed to be in world space
		olc::GFX3D::vec3d t1;
		olc::GFX3D::vec3d t2 = { p1.x + r, p1.y, p1.z };

		// Transform into view
		t1 = GFX3D::Math::Mat_MultiplyVector(matView, p1);
		t2 = GFX3D::Math::Mat_MultiplyVector(matView, t2);

		// Project onto screen
		t1 = GFX3D::Math::Mat_MultiplyVector(matProj, t1);
		t2 = GFX3D::Math::Mat_MultiplyVector(matProj, t2);

		// Project
		t1.x = t1.x / t1.w;
		t1.y = t1.y / t1.w;
		t1.z = t1.z / t1.w;

		t2.x = t2.x / t2.w;
		t2.y = t2.y / t2.w;
		t2.z = t2.z / t2.w;

		vec3d vOffsetView = { 1,1,0 };
		t1 = Math::Vec_Add(t1, vOffsetView);
		t2 = Math::Vec_Add(t2, vOffsetView);

		t1.x *= 0.5f * fViewW;
		t1.y *= 0.5f * fViewH;
		t2.x *= 0.5f * fViewW;
		t2.y *= 0.5f * fViewH;

		vOffsetView = { fViewX,fViewY,0 };
		t1 = Math::Vec_Add(t1, vOffsetView);
		t2 = Math::Vec_Add(t2, vOffsetView);

		pge->FillCircle((int32_t)t1.x, (int32_t)t1.y, (int32_t)fabs(t2.x - t1.x), col);

		return 0;
	}

	uint32_t GFX3D::PipeLine::Render(std::vector<olc::GFX3D::triangle> &triangles, uint32_t flags, int nOffset, int nCount)
	{
		// Calculate Transformation Matrix
		mat4x4 matWorldView = Math::Mat_MultiplyMatrix(matWorld, matView);
		//matWorldViewProj = Math::Mat_MultiplyMatrix(matWorldView, matProj);

		// Store triangles for rastering later
		std::vector<GFX3D::triangle> vecTrianglesToRaster;

		int nTriangleDrawnCount = 0;

		// Process Triangles
		//for (auto &tri : triangles)
//		omp_set_dynamic(0);
//		omp_set_num_threads(4);
//#pragma omp parallel for schedule(static)
		for(int tx = nOffset; tx < nOffset+nCount; tx++)
		{
			GFX3D::triangle &tri = triangles[tx];
			GFX3D::triangle triTransformed;

			// Just copy through texture coordinates
			triTransformed.t[0] = { tri.t[0].x, tri.t[0].y, tri.t[0].z };
			triTransformed.t[1] = { tri.t[1].x, tri.t[1].y, tri.t[1].z };
			triTransformed.t[2] = { tri.t[2].x, tri.t[2].y, tri.t[2].z }; // Think!

			// Dont forget vertex colours
			triTransformed.col[0] = tri.col[0];
			triTransformed.col[1] = tri.col[1];
			triTransformed.col[2] = tri.col[2];

			// Transform Triangle from object into projected space
			triTransformed.p[0] = GFX3D::Math::Mat_MultiplyVector(matWorldView, tri.p[0]);
			triTransformed.p[1] = GFX3D::Math::Mat_MultiplyVector(matWorldView, tri.p[1]);
			triTransformed.p[2] = GFX3D::Math::Mat_MultiplyVector(matWorldView, tri.p[2]);

			// Calculate Triangle Normal in WorldView Space
			GFX3D::vec3d normal, line1, line2;
			line1 = GFX3D::Math::Vec_Sub(triTransformed.p[1], triTransformed.p[0]);
			line2 = GFX3D::Math::Vec_Sub(triTransformed.p[2], triTransformed.p[0]);
			normal = GFX3D::Math::Vec_CrossProduct(line1, line2);
			normal = GFX3D::Math::Vec_Normalise(normal);

			// Cull triangles that face away from viewer
			if (flags & RENDER_CULL_CW && GFX3D::Math::Vec_DotProduct(normal, triTransformed.p[0]) > 0.0f) continue;
			if (flags & RENDER_CULL_CCW && GFX3D::Math::Vec_DotProduct(normal, triTransformed.p[0]) < 0.0f) continue;
					
			// If Lighting, calculate shading
			if (flags & RENDER_LIGHTS)
			{
				olc::Pixel ambient_clamp = { 0,0,0 };
				olc::Pixel light_combined = { 0,0,0 };
				uint32_t nLightSources = 0;
				float nLightR = 0, nLightG = 0, nLightB = 0;

				for (int i = 0; i < 4; i++)
				{
					switch (lights[i].type)
					{
					case LIGHT_DISABLED:
						break;
					case LIGHT_AMBIENT:
						ambient_clamp = lights[i].col;						
						break;
					case LIGHT_DIRECTIONAL:
						{
							nLightSources++;
							GFX3D::vec3d light_dir = GFX3D::Math::Vec_Normalise(lights[i].dir);
							float light = GFX3D::Math::Vec_DotProduct(light_dir, normal);
							if (light > 0)
							{
								int j = 0;
							}

							light = std::max(light, 0.0f);
							nLightR += light * (lights[i].col.r/255.0f);
							nLightG += light * (lights[i].col.g/255.0f);
							nLightB += light * (lights[i].col.b/255.0f);
						}
						break;
					case LIGHT_POINT:
						break;
					}
				}

				//nLightR /= nLightSources;
				//nLightG /= nLightSources;
				//nLightB /= nLightSources;

				nLightR = std::max(nLightR, ambient_clamp.r / 255.0f);
				nLightG = std::max(nLightG, ambient_clamp.g / 255.0f);
				nLightB = std::max(nLightB, ambient_clamp.b / 255.0f);

				triTransformed.col[0] = olc::Pixel(uint8_t(nLightR * triTransformed.col[0].r), uint8_t(nLightG * triTransformed.col[0].g), uint8_t(nLightB * triTransformed.col[0].b));
				triTransformed.col[1] = olc::Pixel(uint8_t(nLightR * triTransformed.col[1].r), uint8_t(nLightG * triTransformed.col[1].g), uint8_t(nLightB * triTransformed.col[1].b));
				triTransformed.col[2] = olc::Pixel(uint8_t(nLightR * triTransformed.col[2].r), uint8_t(nLightG * triTransformed.col[2].g), uint8_t(nLightB * triTransformed.col[2].b));



				/*GFX3D::vec3d light_dir = { 1,1,1 };
				light_dir = GFX3D::Math::Vec_Normalise(light_dir);
				float light = GFX3D::Math::Vec_DotProduct(light_dir, normal);
				if (light < 0) light = 0;
				triTransformed.col[0] = olc::Pixel(light * 255.0f, light * 255.0f, light * 255.0f);
				triTransformed.col[1] = olc::Pixel(light * 255.0f, light * 255.0f, light * 255.0f);
				triTransformed.col[2] = olc::Pixel(light * 255.0f, light * 255.0f, light * 255.0f);*/
			}
			//else
			//	triTransformed.col = olc::WHITE;

			// Clip triangle against near plane
			int nClippedTriangles = 0;
			GFX3D::triangle clipped[2];
			nClippedTriangles = GFX3D::Math::Triangle_ClipAgainstPlane({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triTransformed, clipped[0], clipped[1]);
			
			// This may yield two new triangles
			for (int n = 0; n < nClippedTriangles; n++)
			{
				triangle triProjected = clipped[n];

				// Project new triangle
				triProjected.p[0] = GFX3D::Math::Mat_MultiplyVector(matProj, clipped[n].p[0]);
				triProjected.p[1] = GFX3D::Math::Mat_MultiplyVector(matProj, clipped[n].p[1]);
				triProjected.p[2] = GFX3D::Math::Mat_MultiplyVector(matProj, clipped[n].p[2]);

				// Apply Projection to Verts
				triProjected.p[0].x = triProjected.p[0].x / triProjected.p[0].w;
				triProjected.p[1].x = triProjected.p[1].x / triProjected.p[1].w;
				triProjected.p[2].x = triProjected.p[2].x / triProjected.p[2].w;

				triProjected.p[0].y = triProjected.p[0].y / triProjected.p[0].w;
				triProjected.p[1].y = triProjected.p[1].y / triProjected.p[1].w;
				triProjected.p[2].y = triProjected.p[2].y / triProjected.p[2].w;

				triProjected.p[0].z = triProjected.p[0].z / triProjected.p[0].w;
				triProjected.p[1].z = triProjected.p[1].z / triProjected.p[1].w;
				triProjected.p[2].z = triProjected.p[2].z / triProjected.p[2].w;

				// Apply Projection to Tex coords
				triProjected.t[0].x = triProjected.t[0].x / triProjected.p[0].w;
				triProjected.t[1].x = triProjected.t[1].x / triProjected.p[1].w;
				triProjected.t[2].x = triProjected.t[2].x / triProjected.p[2].w;

				triProjected.t[0].y = triProjected.t[0].y / triProjected.p[0].w;
				triProjected.t[1].y = triProjected.t[1].y / triProjected.p[1].w;
				triProjected.t[2].y = triProjected.t[2].y / triProjected.p[2].w;

				triProjected.t[0].z = 1.0f / triProjected.p[0].w;
				triProjected.t[1].z = 1.0f / triProjected.p[1].w;
				triProjected.t[2].z = 1.0f / triProjected.p[2].w;

				// Clip against viewport in screen space
				// Clip triangles against all four screen edges, this could yield
				// a bunch of triangles, so create a queue that we traverse to 
				//  ensure we only test new triangles generated against planes
				GFX3D::triangle sclipped[2];
				std::list<GFX3D::triangle> listTriangles;
				

				// Add initial triangle
				listTriangles.push_back(triProjected);
				int nNewTriangles = 1;

				for (int p = 0; p < 4; p++)
				{
					int nTrisToAdd = 0;
					while (nNewTriangles > 0)
					{
						// Take triangle from front of queue
						triangle test = listTriangles.front();
						listTriangles.pop_front();
						nNewTriangles--;

						// Clip it against a plane. We only need to test each 
						// subsequent plane, against subsequent new triangles
						// as all triangles after a plane clip are guaranteed
						// to lie on the inside of the plane. I like how this
						// comment is almost completely and utterly justified
						switch (p)
						{
						case 0:	nTrisToAdd = GFX3D::Math::Triangle_ClipAgainstPlane({ 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, sclipped[0], sclipped[1]); break;
						case 1:	nTrisToAdd = GFX3D::Math::Triangle_ClipAgainstPlane({ 0.0f, +1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, sclipped[0], sclipped[1]); break;
						case 2:	nTrisToAdd = GFX3D::Math::Triangle_ClipAgainstPlane({ -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, sclipped[0], sclipped[1]); break;
						case 3:	nTrisToAdd = GFX3D::Math::Triangle_ClipAgainstPlane({ +1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, sclipped[0], sclipped[1]); break;
						}


						// Clipping may yield a variable number of triangles, so
						// add these new ones to the back of the queue for subsequent
						// clipping against next planes
						for (int w = 0; w < nTrisToAdd; w++)
							listTriangles.push_back(sclipped[w]);
					}
					nNewTriangles = listTriangles.size();
				}

				for (auto &triRaster : listTriangles)
				{
					// Scale to viewport
					/*triRaster.p[0].x *= -1.0f;
					triRaster.p[1].x *= -1.0f;
					triRaster.p[2].x *= -1.0f;
					triRaster.p[0].y *= -1.0f;
					triRaster.p[1].y *= -1.0f;
					triRaster.p[2].y *= -1.0f;*/
					vec3d vOffsetView = { 1,1,0 };
					triRaster.p[0] = Math::Vec_Add(triRaster.p[0], vOffsetView);
					triRaster.p[1] = Math::Vec_Add(triRaster.p[1], vOffsetView);
					triRaster.p[2] = Math::Vec_Add(triRaster.p[2], vOffsetView);
					triRaster.p[0].x *= 0.5f * fViewW;
					triRaster.p[0].y *= 0.5f * fViewH;
					triRaster.p[1].x *= 0.5f * fViewW;
					triRaster.p[1].y *= 0.5f * fViewH;
					triRaster.p[2].x *= 0.5f * fViewW;
					triRaster.p[2].y *= 0.5f * fViewH;
					vOffsetView = { fViewX,fViewY,0 };
					triRaster.p[0] = Math::Vec_Add(triRaster.p[0], vOffsetView);
					triRaster.p[1] = Math::Vec_Add(triRaster.p[1], vOffsetView);
					triRaster.p[2] = Math::Vec_Add(triRaster.p[2], vOffsetView);

					// For now, just draw triangle

					//if (flags & RENDER_TEXTURED)
					//{/*
					//	TexturedTriangle(
					//		triRaster.p[0].x, triRaster.p[0].y, triRaster.t[0].x, triRaster.t[0].y, triRaster.t[0].z,
					//		triRaster.p[1].x, triRaster.p[1].y, triRaster.t[1].x, triRaster.t[1].y, triRaster.t[1].z,
					//		triRaster.p[2].x, triRaster.p[2].y, triRaster.t[2].x, triRaster.t[2].y, triRaster.t[2].z,
					//		sprTexture);*/

					//	RasterTriangle(
					//		triRaster.p[0].x, triRaster.p[0].y, triRaster.t[0].x, triRaster.t[0].y, triRaster.t[0].z, triRaster.col,
					//		triRaster.p[1].x, triRaster.p[1].y, triRaster.t[1].x, triRaster.t[1].y, triRaster.t[1].z, triRaster.col,
					//		triRaster.p[2].x, triRaster.p[2].y, triRaster.t[2].x, triRaster.t[2].y, triRaster.t[2].z, triRaster.col,
					//		sprTexture, nFlags);

					//}

					if (flags & RENDER_WIRE)
					{
						DrawTriangleWire(triRaster, olc::RED);
					}
					else
					{
						RasterTriangle(
							(int)triRaster.p[0].x,(int)triRaster.p[0].y, triRaster.t[0].x, triRaster.t[0].y, triRaster.t[0].z, triRaster.col[0],
							(int)triRaster.p[1].x,(int)triRaster.p[1].y, triRaster.t[1].x, triRaster.t[1].y, triRaster.t[1].z, triRaster.col[1],
							(int)triRaster.p[2].x,(int)triRaster.p[2].y, triRaster.t[2].x, triRaster.t[2].y, triRaster.t[2].z, triRaster.col[2],
							sprTexture, flags);
						
					}




					nTriangleDrawnCount++;
				}
			}
		}

		return nTriangleDrawnCount;
	}

	void GFX3D::RasterTriangle(int x1, int y1, float u1, float v1, float w1, olc::Pixel c1,
							   int x2, int y2, float u2, float v2, float w2, olc::Pixel c2,
							   int x3, int y3, float u3, float v3, float w3, olc::Pixel c3,
							   olc::Sprite* spr,
							   uint32_t nFlags)

	{
		if (y2 < y1)
		{
			std::swap(y1, y2); std::swap(x1, x2); std::swap(u1, u2); std::swap(v1, v2);	std::swap(w1, w2); std::swap(c1, c2);
		}

		if (y3 < y1)
		{
			std::swap(y1, y3); std::swap(x1, x3); std::swap(u1, u3); std::swap(v1, v3); std::swap(w1, w3); std::swap(c1, c3);
		}

		if (y3 < y2)
		{
			std::swap(y2, y3); std::swap(x2, x3); std::swap(u2, u3); std::swap(v2, v3); std::swap(w2, w3); std::swap(c2, c3);
		}

		int dy1 = y2 - y1;
		int dx1 = x2 - x1;
		float dv1 = v2 - v1;
		float du1 = u2 - u1;
		float dw1 = w2 - w1;
		int dcr1 = c2.r - c1.r;
		int dcg1 = c2.g - c1.g;
		int dcb1 = c2.b - c1.b;
		int dca1 = c2.a - c1.a;

		int dy2 = y3 - y1;
		int dx2 = x3 - x1;
		float dv2 = v3 - v1;
		float du2 = u3 - u1;
		float dw2 = w3 - w1;
		int dcr2 = c3.r - c1.r;
		int dcg2 = c3.g - c1.g;
		int dcb2 = c3.b - c1.b;
		int dca2 = c3.a - c1.a;

		float tex_u, tex_v, tex_w;
		float col_r, col_g, col_b, col_a;

		float dax_step = 0, dbx_step = 0,
			du1_step = 0, dv1_step = 0,
			du2_step = 0, dv2_step = 0,
			dw1_step = 0, dw2_step = 0,
			dcr1_step = 0, dcr2_step = 0,
			dcg1_step = 0, dcg2_step = 0,
			dcb1_step = 0, dcb2_step = 0,
			dca1_step = 0, dca2_step = 0;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		if (dy2) du2_step = du2 / (float)abs(dy2);
		if (dy2) dv2_step = dv2 / (float)abs(dy2);
		if (dy2) dw2_step = dw2 / (float)abs(dy2);

		if (dy1) dcr1_step = dcr1 / (float)abs(dy1);
		if (dy1) dcg1_step = dcg1 / (float)abs(dy1);
		if (dy1) dcb1_step = dcb1 / (float)abs(dy1);
		if (dy1) dca1_step = dca1 / (float)abs(dy1);

		if (dy2) dcr2_step = dcr2 / (float)abs(dy2);
		if (dy2) dcg2_step = dcg2 / (float)abs(dy2);
		if (dy2) dcb2_step = dcb2 / (float)abs(dy2);
		if (dy2) dca2_step = dca2 / (float)abs(dy2);

		float pixel_r = 0.0f;
		float pixel_g = 0.0f;
		float pixel_b = 0.0f;
		float pixel_a = 1.0f;

		if (dy1)
		{
			for (int i = y1; i <= y2; i++)
			{
				int ax = int(x1 + (float)(i - y1) * dax_step);
				int bx = int(x1 + (float)(i - y1) * dbx_step);

				float tex_su = u1 + (float)(i - y1) * du1_step;
				float tex_sv = v1 + (float)(i - y1) * dv1_step;
				float tex_sw = w1 + (float)(i - y1) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				float col_sr = c1.r + (float)(i - y1) * dcr1_step;
				float col_sg = c1.g + (float)(i - y1) * dcg1_step;
				float col_sb = c1.b + (float)(i - y1) * dcb1_step;
				float col_sa = c1.a + (float)(i - y1) * dca1_step;

				float col_er = c1.r + (float)(i - y1) * dcr2_step;
				float col_eg = c1.g + (float)(i - y1) * dcg2_step;
				float col_eb = c1.b + (float)(i - y1) * dcb2_step;
				float col_ea = c1.a + (float)(i - y1) * dca2_step;

				if (ax > bx)
				{
					std::swap(ax, bx);
					std::swap(tex_su, tex_eu);
					std::swap(tex_sv, tex_ev);
					std::swap(tex_sw, tex_ew);
					std::swap(col_sr, col_er);
					std::swap(col_sg, col_eg);
					std::swap(col_sb, col_eb);
					std::swap(col_sa, col_ea);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;
				col_r = col_sr;
				col_g = col_sg;
				col_b = col_sb;
				col_a = col_sa;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;
					col_r = (1.0f - t) * col_sr + t * col_er;
					col_g = (1.0f - t) * col_sg + t * col_eg;
					col_b = (1.0f - t) * col_sb + t * col_eb;
					col_a = (1.0f - t) * col_sa + t * col_ea;

					pixel_r = col_r;
					pixel_g = col_g;
					pixel_b = col_b;
					pixel_a = col_a;

					if (nFlags & GFX3D::RENDER_TEXTURED)
					{
						if (spr != nullptr)
						{
							olc::Pixel sample = spr->Sample(tex_u / tex_w, tex_v / tex_w);
							pixel_r *= sample.r / 255.0f;
							pixel_g *= sample.g / 255.0f;
							pixel_b *= sample.b / 255.0f;
							pixel_a *= sample.a / 255.0f;
						}
					}

					if (nFlags & GFX3D::RENDER_DEPTH)
					{
						if (tex_w > m_DepthBuffer[i*pge->ScreenWidth() + j])
							if (pge->Draw(j, i, olc::Pixel(uint8_t(pixel_r * 1.0f), uint8_t(pixel_g * 1.0f), uint8_t(pixel_b * 1.0f), uint8_t(pixel_a * 1.0f))))
								m_DepthBuffer[i*pge->ScreenWidth() + j] = tex_w;
					}
					else
					{
						pge->Draw(j, i, olc::Pixel(uint8_t(pixel_r * 1.0f), uint8_t(pixel_g * 1.0f), uint8_t(pixel_b * 1.0f), uint8_t(pixel_a * 1.0f)));
					}
				
					t += tstep;
				}
			}
		}

		dy1 = y3 - y2;
		dx1 = x3 - x2;
		dv1 = v3 - v2;
		du1 = u3 - u2;
		dw1 = w3 - w2;
		dcr1 = c3.r - c2.r;
		dcg1 = c3.g - c2.g;
		dcb1 = c3.b - c2.b;
		dca1 = c3.a - c2.a;

		if (dy1) dax_step = dx1 / (float)abs(dy1);
		if (dy2) dbx_step = dx2 / (float)abs(dy2);

		du1_step = 0; dv1_step = 0;
		if (dy1) du1_step = du1 / (float)abs(dy1);
		if (dy1) dv1_step = dv1 / (float)abs(dy1);
		if (dy1) dw1_step = dw1 / (float)abs(dy1);

		dcr1_step = 0; dcg1_step = 0; dcb1_step = 0; dca1_step = 0;
		if (dy1) dcr1_step = dcr1 / (float)abs(dy1);
		if (dy1) dcg1_step = dcg1 / (float)abs(dy1);
		if (dy1) dcb1_step = dcb1 / (float)abs(dy1);
		if (dy1) dca1_step = dca1 / (float)abs(dy1);

		if (dy1)
		{
			for (int i = y2; i <= y3; i++)
			{
				int ax = int(x2 + (float)(i - y2) * dax_step);
				int bx = int(x1 + (float)(i - y1) * dbx_step);

				float tex_su = u2 + (float)(i - y2) * du1_step;
				float tex_sv = v2 + (float)(i - y2) * dv1_step;
				float tex_sw = w2 + (float)(i - y2) * dw1_step;

				float tex_eu = u1 + (float)(i - y1) * du2_step;
				float tex_ev = v1 + (float)(i - y1) * dv2_step;
				float tex_ew = w1 + (float)(i - y1) * dw2_step;

				float col_sr = c2.r + (float)(i - y2) * dcr1_step;
				float col_sg = c2.g + (float)(i - y2) * dcg1_step;
				float col_sb = c2.b + (float)(i - y2) * dcb1_step;
				float col_sa = c2.a + (float)(i - y2) * dca1_step;

				float col_er = c1.r + (float)(i - y1) * dcr2_step;
				float col_eg = c1.g + (float)(i - y1) * dcg2_step;
				float col_eb = c1.b + (float)(i - y1) * dcb2_step;
				float col_ea = c1.a + (float)(i - y1) * dca2_step;

				if (ax > bx)
				{
					std::swap(ax, bx);
					std::swap(tex_su, tex_eu);
					std::swap(tex_sv, tex_ev);
					std::swap(tex_sw, tex_ew);
					std::swap(col_sr, col_er);
					std::swap(col_sg, col_eg);
					std::swap(col_sb, col_eb);
					std::swap(col_sa, col_ea);
				}

				tex_u = tex_su;
				tex_v = tex_sv;
				tex_w = tex_sw;
				col_r = col_sr;
				col_g = col_sg;
				col_b = col_sb;
				col_a = col_sa;

				float tstep = 1.0f / ((float)(bx - ax));
				float t = 0.0f;

				for (int j = ax; j < bx; j++)
				{
					tex_u = (1.0f - t) * tex_su + t * tex_eu;
					tex_v = (1.0f - t) * tex_sv + t * tex_ev;
					tex_w = (1.0f - t) * tex_sw + t * tex_ew;
					col_r = (1.0f - t) * col_sr + t * col_er;
					col_g = (1.0f - t) * col_sg + t * col_eg;
					col_b = (1.0f - t) * col_sb + t * col_eb;
					col_a = (1.0f - t) * col_sa + t * col_ea;

					pixel_r = col_r;
					pixel_g = col_g;
					pixel_b = col_b;
					pixel_a = col_a;

					if (nFlags & GFX3D::RENDER_TEXTURED)
					{
						if (spr != nullptr)
						{
							olc::Pixel sample = spr->Sample(tex_u / tex_w, tex_v / tex_w);
							pixel_r *= sample.r / 255.0f;
							pixel_g *= sample.g / 255.0f;
							pixel_b *= sample.b / 255.0f;
							pixel_a *= sample.a / 255.0f;
						}
					}

					if (nFlags & GFX3D::RENDER_DEPTH)
					{
						if (tex_w > m_DepthBuffer[i*pge->ScreenWidth() + j])
							if (pge->Draw(j, i, olc::Pixel(uint8_t(pixel_r * 1.0f), uint8_t(pixel_g * 1.0f), uint8_t(pixel_b * 1.0f), uint8_t(pixel_a * 1.0f))))
								m_DepthBuffer[i*pge->ScreenWidth() + j] = tex_w;
					}
					else
					{
						pge->Draw(j, i, olc::Pixel(uint8_t(pixel_r * 1.0f), uint8_t(pixel_g * 1.0f), uint8_t(pixel_b * 1.0f), uint8_t(pixel_a * 1.0f)));
					}

					t += tstep;
				}
			}
		}
	}



	//GFX3D::MipMap::MipMap() : olc::Sprite(){}
	//GFX3D::MipMap::MipMap(std::string sImageFile) : olc::Sprite(sImageFile)
	//{
	//	GenerateMipLevels();
	//}
	//GFX3D::MipMap::MipMap(std::string sImageFile, olc::ResourcePack *pack) : olc::Sprite(sImageFile, pack){}
	//GFX3D::MipMap::MipMap(int32_t w, int32_t h) : olc::Sprite(w, h) {}

	//int GFX3D::MipMap::GenerateMipLevels()
	//{
	//	int nLevelsW = 0;
	//	int nLevelsH = 0; 
	//	int w = width;
	//	int h = height;
	//	while (w > 1) { w >>= 1; nLevelsW++; }
	//	while (h > 1) { h >>= 1; nLevelsH++; }

	//	int nLevels = std::min(nLevelsW, nLevelsH);

	//	w = width >> 1;
	//	h = height >> 1;

	//	vecMipMaps.emplace_back(w, h); // Level 0
	//	memcpy(vecMipMaps[0].GetData(), GetData(), w*h*sizeof(uint32_t));

	//	for (int i = 1; i < nLevels; i++)
	//	{
	//		vecMipMaps.emplace_back(w, h);
	//		pge->SetDrawTarget(&vecMipMaps[i]);
	//		for (int x = 0; x < w; x++)
	//			for (int y = 0; y < h; y++)
	//				pge->Draw(x, y, vecMipMaps[i-1].SampleBL((float)x / (float)w, (float)y / (float)h));			
	//		w >>= 1; h >>= 1;			
	//	}

	//	pge->SetDrawTarget(nullptr);			
	//	return nLevels;
	//}
	//
	//olc::rcode GFX3D::MipMap::LoadFromFile(std::string sImageFile, olc::ResourcePack *pack)
	//{
	//	olc::rcode r = olc::Sprite::LoadFromFile(sImageFile, pack);
	//	if (r == olc::FAIL) return r;
	//	GenerateMipLevels();
	//	return r;
	//}

	//olc::rcode GFX3D::MipMap::LoadFromPGESprFile(std::string sImageFile, olc::ResourcePack *pack)
	//{
	//	olc::rcode r = olc::Sprite::LoadFromPGESprFile(sImageFile, pack);
	//	if (r == olc::FAIL) return r;
	//	GenerateMipLevels();
	//	return r;
	//}
	//
	//olc::Pixel GFX3D::MipMap::Sample(float x, float y, float z)
	//{
	//	int nLevel = (int)(z * (float)vecMipMaps.size());
	//	return vecMipMaps[nLevel].Sample(x, y);
	//}

	//olc::Pixel GFX3D::MipMap::SampleBL(float u, float v, float z);

}

#endif