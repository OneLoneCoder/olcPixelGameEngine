/*
	olcPGEX_Wireframe.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                      Wireframe v1.0                         |
	+-------------------------------------------------------------+

	NOTE: UNDER ACTIVE DEVELOPMENT - THERE ARE BUGS/GLITCHES

	What is this?
	~~~~~~~~~~~~~
	This extension provides drawing routines giving simple wireframe
	shapes and models constructed in a transform hierachy

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

	Revisions:
	1.00:	Initial Release
	
*/

#pragma once
#ifndef OLC_PGEX_WIREFRAME_H
#define OLC_PGEX_WIREFRAME_H

#include "olcPixelGameEngine.h"

namespace olc
{
#ifndef OLC_MAT3_DESC
#define OLC_MAT3_DESC
	template<typename T>
	struct mat3_generic
	{
		std::array<T, 9> m{ 0 };
		constexpr size_t idx(size_t r, size_t c) const { return r * 3 + c; }
		T& operator()(size_t row, size_t col) { return m[idx(row, col)]; }
		const T& operator()(size_t row, size_t col) const { return m[idx(row, col)]; }
		mat3_generic() { identity(); }
		mat3_generic(const mat3_generic& m) = default;
		mat3_generic& operator=(const mat3_generic& m) = default;

		void clear() { std::fill(m.begin(), m.end(), T(0)); }
		void identity() { clear(); (*this)(0, 0) = 1; (*this)(1, 1) = 1; (*this)(2, 2) = 1; }

		void translate(float x, float y) { identity();  auto& m = (*this); m(2, 0) = x; m(2, 1) = y; }
		void translate(const olc::v2d_generic<T>& v) { translate(v.x, v.y); }
		void scale(float x, float y) { identity(); auto& m = (*this); m(0, 0) = x; m(1, 1) = y; }
		void scale(const olc::v2d_generic<T>& v) { return scale(v.x, v.y); }
		void rotate(float a) { identity(); auto& m = (*this); m(0, 0) = cos(a); m(0, 1) = sin(a); m(1, 0) = -m(0, 1); m(1, 1) = m(0, 0); }

		olc::v2d_generic<T> operator * (const olc::v2d_generic<T>& v) const
		{
			auto& m = *this;
			olc::v2d_generic<T> vOut;
			vOut.x = m(0, 0) * v.x + m(1, 0) * v.y + m(2, 0) * T(1);
			vOut.y = m(0, 1) * v.x + m(1, 1) * v.y + m(2, 1) * T(1);
			T    z = m(0, 2) * v.x + m(1, 2) * v.y + m(2, 2) * T(1);
			return (vOut / z);
		}

		mat3_generic operator * (const mat3_generic& rhs) const
		{
			auto& m = *this;
			mat3_generic<T> out;
			for (size_t c = 0; c < 3; c++)
				for (size_t r = 0; r < 3; r++)
					out(r, c) = m(r, 0) * rhs(0, c) + m(r, 1) * rhs(1, c) + m(r, 2) * rhs(2, c);
			return out;
		}
	};

	typedef mat3_generic<float> Matrix2D;
#endif
		
	namespace wire
	{
		typedef std::vector<olc::vf2d> Mesh;
		//Mesh NullMesh;

		class Model
		{
		public:
			static constexpr uint8_t DRAW_ORIGIN = 0x01;
			static constexpr uint8_t DRAW_NODES = 0x02;
			static constexpr uint8_t DRAW_MEASURES = 0x04;
		public:
			Model() = default;

		public:
			void Attach(Model* child, const olc::vf2d& position = { 0.0f, 0.0f }, const float angle = 0.0f);
			void SetRotation(const float angle);
			void SetPosition(const olc::vf2d& position);
			void UpdateInWorld(const Matrix2D& matParent);
			olc::vf2d LocalToWorld(const olc::vf2d& local);
			void SetMesh(const Mesh& mesh);
			const Mesh& GetWorldPoints() const;
			const std::vector<Model*>& GetChildren() const;

		protected:
			Mesh vLocalPoints;;
			Mesh vWorldPoints;
			olc::Matrix2D matLocalTranslation;
			olc::Matrix2D matLocalRotation;
			olc::Matrix2D matLocal;
			olc::Matrix2D matWorld;

		protected:
			std::vector<Model*> vChildren;
		};



		inline const Mesh MeshCircle(const float fRadius, const int nPoints = 100)
		{
			Mesh m;
			for (int i = 0; i < nPoints; i++)
			{
				float fTheta = (float(i) / float(nPoints)) * 2.0f * 3.14159f;
				m.push_back(olc::vf2d{ cos(fTheta), sin(fTheta) } *fRadius);
			}
			return m;
		}

		inline const Mesh MeshRectangle(const olc::vf2d& size, const olc::vf2d& offset = { 0.0f, 0.0f })
		{			
			return { -offset, {-offset.x + size.x, -offset.y}, -offset + size, {-offset.x, -offset.y + size.y} };			
		}

		inline const Mesh MeshGear(const int nTeeth, const float fOuterRadius, const float fInnerRadius)
		{
			Mesh m;
			for (int i = 0; i < nTeeth * 4; i++)
			{
				float fTheta = (float(i) / float(nTeeth * 4)) * 2.0f * 3.14159f;
				m.push_back(olc::vf2d{ cos(fTheta), sin(fTheta) }  * 2.0f * (float((i / 2) % 2) ? fOuterRadius : fInnerRadius));
			}
			return m;
		}

		template<typename T>
		void DrawModel(T& render, Model& m, const olc::Pixel col = olc::BLACK, const uint8_t flags = -1)
		{
			const auto& points = m.GetWorldPoints();
			for(size_t i = 0; i < points.size(); i++)
				render.DrawLine(points[i], points[(i+1)%points.size()], col);	

			// Draw Nodes
			if (flags & Model::DRAW_NODES)			
				for (size_t i = 0; i < points.size(); i++)								
					render.FillCircle(points[i], render.ScaleToWorld({ 3,3 }).x, olc::RED);

			if (flags & Model::DRAW_ORIGIN)
			{
				render.FillCircle(m.LocalToWorld({ 0,0 }), render.ScaleToWorld({ 3,3 }).x, olc::BLUE);
				render.DrawLine(
					m.LocalToWorld({ 0,0 }),
					m.LocalToWorld(render.ScaleToWorld({ 10, 0 })),
					olc::BLUE);
			}

			// Draw Children
			for (auto& child : m.GetChildren())
				DrawModel<T>(render, *child, col, flags);
		}

		
	}
}

#ifdef OLC_PGEX_WIREFRAME
#undef OLC_PGEX_WIREFRAME

namespace olc
{
	namespace wire
	{
		void Model::SetMesh(const Mesh& mesh)
		{
			vLocalPoints = mesh;
			vWorldPoints.clear();
			vWorldPoints.resize(vLocalPoints.size());			
		}

		void Model::SetRotation(const float angle)
		{
			matLocalRotation.rotate(angle);
			matLocal = matLocalRotation * matLocalTranslation;
		}

		void Model::SetPosition(const olc::vf2d& position)
		{
			matLocalTranslation.translate(position);
			matLocal = matLocalRotation * matLocalTranslation;		
		}

		void Model::Attach(Model* child, const olc::vf2d& position, const float angle)
		{
			if (child != nullptr)
			{
				child->SetPosition(position);
				child->SetRotation(angle);
				vChildren.push_back(child);
			}
		}

		olc::vf2d Model::LocalToWorld(const olc::vf2d& local)
		{
			return matWorld * local;
		}

		const Mesh& Model::GetWorldPoints() const
		{
			return vWorldPoints;
		}

		const std::vector<Model*>& Model::GetChildren() const
		{
			return vChildren;
		}

		void Model::UpdateInWorld(const Matrix2D& matParent)
		{
			// Propagate matrix transform 
			matWorld = matLocal * matParent;

			// Transform vertices
			for (size_t i = 0; i < vLocalPoints.size(); i++)
			{
				vWorldPoints[i] = matWorld * vLocalPoints[i];
			}
	
			// Transform Children
			for (auto& child : vChildren)
				child->UpdateInWorld(matWorld);
		}
	}
}

#endif // OLC_PGEX_WIREFRAME
#endif // OLC_PGEX_WIREFRAME_H

