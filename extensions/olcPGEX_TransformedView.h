/*
	olcPGEX_TransformedView.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                 Transformed View v1.09                      |
	+-------------------------------------------------------------+

	NOTE: UNDER ACTIVE DEVELOPMENT - THERE ARE BUGS/GLITCHES

	What is this?
	~~~~~~~~~~~~~
	This extension provides drawing routines that are compatible with
	changeable world and screen spaces. For example you can pan and
	zoom, and all PGE drawing routines will automatically adopt the current
	world scales and offsets.

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

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021, 2022, 2023, 2024

	Revisions:
	1.00:	Initial Release
	1.01:	Fix for rounding error when scaling to screen
	1.02:	Added DrawLineDecal for convenience
	1.03:	Removed std::floor from WorldToScreen()
			Added HandlePanAndZoom(...) convenience function
			Removed unused "range" facility in TileTransformView
	1.04:	Added DrawPolygonDecal() for arbitrary polygons
	1.05:	Clipped DrawSprite() to visible area, massive performance increase
	1.06:	Fixed error in DrawLine() - Thanks CraisyDaisyRecords (& Fern)!
	1.07:   +DrawRectDecal()
			+GetPGE()
	1.08:  +DrawPolygonDecal() with tint overload, akin to PGE
	1.09:  +SetScaleExtents() - Sets range that world scale can exist within
		   +EnableScaleClamp() - Applies a range that scaling is clamped to
				These are both useful for having zoom clamped between a min and max
				without weird panning artefacts occuring
*/

#pragma once
#ifndef OLC_PGEX_TRANSFORMEDVIEW_H
#define OLC_PGEX_TRANSFORMEDVIEW_H

#include "olcPixelGameEngine.h"



namespace olc
{
	class TransformedView : public olc::PGEX
	{
	public:
		TransformedView() = default;
		virtual void Initialise(const olc::vi2d& vViewArea, const olc::vf2d& vPixelScale = { 1.0f, 1.0f });

		olc::PixelGameEngine* GetPGE();

	public:
		void SetWorldOffset(const olc::vf2d& vOffset);
		void MoveWorldOffset(const olc::vf2d& vDeltaOffset);
		void SetWorldScale(const olc::vf2d& vScale);
		void SetViewArea(const olc::vi2d& vViewArea);
		olc::vf2d GetWorldTL() const;
		olc::vf2d GetWorldBR() const;
		olc::vf2d GetWorldVisibleArea() const;
		void ZoomAtScreenPos(const float fDeltaZoom, const olc::vi2d& vPos);
		void SetZoom(const float fZoom, const olc::vf2d& vPos);
		void StartPan(const olc::vi2d& vPos);
		void UpdatePan(const olc::vi2d& vPos);
		void EndPan(const olc::vi2d& vPos);
		const olc::vf2d& GetWorldOffset() const;
		const olc::vf2d& GetWorldScale() const;
		virtual olc::vf2d WorldToScreen(const olc::vf2d& vWorldPos) const;
		virtual olc::vf2d ScreenToWorld(const olc::vf2d& vScreenPos) const;
		virtual olc::vf2d ScaleToWorld(const olc::vf2d& vScreenSize) const;
		virtual olc::vf2d ScaleToScreen(const olc::vf2d& vWorldSize) const;
		virtual bool IsPointVisible(const olc::vf2d& vPos) const;
		virtual bool IsRectVisible(const olc::vf2d& vPos, const olc::vf2d& vSize) const;
		virtual void HandlePanAndZoom(const int nMouseButton = 2, const float fZoomRate = 0.1f, const bool bPan = true, const bool bZoom = true);
		void SetScaleExtents(const olc::vf2d& vScaleMin, const olc::vf2d& vScaleMax);
		void EnableScaleClamp(const bool bEnable);

	protected:
		olc::vf2d m_vWorldOffset = { 0.0f, 0.0f };
		olc::vf2d m_vWorldScale = { 1.0f, 1.0f };
		olc::vf2d m_vRecipPixel = { 1.0f, 1.0f };
		olc::vf2d m_vPixelScale = { 1.0f, 1.0f };
		bool m_bPanning = false;
		olc::vf2d m_vStartPan = { 0.0f, 0.0f };
		olc::vi2d m_vViewArea;
		bool m_bZoomClamp = false;
		olc::vf2d m_vMaxScale = { 0.0f, 0.0f };
		olc::vf2d m_vMinScale = { 0.0f, 0.0f };

	public: // Hopefully, these should look familiar!
		// Plots a single point
		virtual bool Draw(float x, float y, olc::Pixel p = olc::WHITE);
		bool Draw(const olc::vf2d& pos, olc::Pixel p = olc::WHITE);		
		// Draws a line from (x1,y1) to (x2,y2)
		void DrawLine(float x1, float y1, float x2, float y2, olc::Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF);
		void DrawLine(const olc::vf2d& pos1, const olc::vf2d& pos2, olc::Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF);
		// Draws a circle located at (x,y) with radius
		void DrawCircle(float x, float y, float radius, olc::Pixel p = olc::WHITE, uint8_t mask = 0xFF);
		void DrawCircle(const olc::vf2d& pos, float radius, olc::Pixel p = olc::WHITE, uint8_t mask = 0xFF);
		// Fills a circle located at (x,y) with radius
		void FillCircle(float x, float y, float radius, olc::Pixel p = olc::WHITE);
		void FillCircle(const olc::vf2d& pos, float radius, olc::Pixel p = olc::WHITE);
		// Draws a rectangle at (x,y) to (x+w,y+h)
		void DrawRect(float x, float y, float w, float h, olc::Pixel p = olc::WHITE);
		void DrawRect(const olc::vf2d& pos, const olc::vf2d& size, olc::Pixel p = olc::WHITE);
		// Fills a rectangle at (x,y) to (x+w,y+h)
		void FillRect(float x, float y, float w, float h, olc::Pixel p = olc::WHITE);
		void FillRect(const olc::vf2d& pos, const olc::vf2d& size, olc::Pixel p = olc::WHITE);
		// Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
		void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, olc::Pixel p = olc::WHITE);
		void DrawTriangle(const olc::vf2d& pos1, const olc::vf2d& pos2, const olc::vf2d& pos3, olc::Pixel p = olc::WHITE);
		// Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
		void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, olc::Pixel p = olc::WHITE);
		void FillTriangle(const olc::vf2d& pos1, const olc::vf2d& pos2, const olc::vf2d& pos3, olc::Pixel p = olc::WHITE);
		// Draws an entire sprite at location (x,y)
		void DrawSprite(float x, float y, olc::Sprite* sprite, float scalex = 1, float scaley = 1, uint8_t flip = olc::Sprite::NONE);
		void DrawSprite(const olc::vf2d& pos, olc::Sprite* sprite, const olc::vf2d& scale = { 1.0f, 1.0f }, uint8_t flip = olc::Sprite::NONE);
		// Draws an area of a sprite at location (x,y), where the
		// selected area is (ox,oy) to (ox+w,oy+h)
		void DrawPartialSprite(float x, float y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, float scalex = 1, float scaley = 1, uint8_t flip = olc::Sprite::NONE);
		void DrawPartialSprite(const olc::vf2d& pos, Sprite* sprite, const olc::vi2d& sourcepos, const olc::vi2d& size, const olc::vf2d& scale = { 1.0f, 1.0f }, uint8_t flip = olc::Sprite::NONE);
		void DrawString(float x, float y, const std::string& sText, Pixel col, const olc::vf2d& scale);
		void DrawString(const olc::vf2d& pos, const std::string& sText, const Pixel col, const olc::vf2d& scale);

		
		// Draws a whole decal, with optional scale and tinting
		void DrawDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE);
		// Draws a region of a decal, with optional scale and tinting
		void DrawPartialDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialDecal(const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		// Draws fully user controlled 4 vertices, pos(pixels), uv(pixels), colours
		void DrawExplicitDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d* uv, const olc::Pixel* col, uint32_t elements = 4);
		//// Draws a decal with 4 arbitrary points, warping the texture to look "correct"
		void DrawWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::Pixel& tint = olc::WHITE);
		void DrawWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::Pixel& tint = olc::WHITE);
		void DrawWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::Pixel& tint = olc::WHITE);
		//// As above, but you can specify a region of a decal source sprite
		void DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		//// Draws a decal rotated to specified angle, wit point of rotation offset
		void DrawRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center = { 0.0f, 0.0f }, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE);
		// Draws a multiline string as a decal, with tiniting and scaling
		void DrawStringDecal(const olc::vf2d& pos, const std::string& sText, const olc::Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
		void DrawStringPropDecal(const olc::vf2d& pos, const std::string& sText, const olc::Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
		// Draws a single shaded filled rectangle as a decal
		void FillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col = olc::WHITE);
		void DrawRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col = olc::WHITE);

		// Draws a corner shaded rectangle as a decal
		void GradientFillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel colTL, const olc::Pixel colBL, const olc::Pixel colBR, const olc::Pixel colTR);
		// Draws an arbitrary convex textured polygon using GPU
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const olc::Pixel tint = olc::WHITE);
		void DrawLineDecal(const olc::vf2d& pos1, const olc::vf2d& pos2, Pixel p = olc::WHITE);
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>&pos, const std::vector<olc::vf2d>&uv, const std::vector<olc::Pixel> &tint);
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& colours, const olc::Pixel tint);


#if defined(OLC_PGEX_SHADER)
		// Shader Specific
		void DrawDecal(olc::Shade& shader, const olc::vf2d & pos, olc::Decal * decal, const olc::vf2d & scale = { 1.0f,1.0f }, const olc::Pixel & tint = olc::WHITE);
		void DrawPartialDecal(olc::Shade& shader, const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialDecal(olc::Shade& shader, const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
#endif



	};

	class TileTransformedView : public TransformedView
	{
	public:
		TileTransformedView() = default;	
		TileTransformedView(const olc::vi2d& vViewArea, const olc::vi2d& vTileSize);
	
	public:
		olc::vi2d GetTopLeftTile() const;
		olc::vi2d GetBottomRightTile() const;
		olc::vi2d GetVisibleTiles() const;
		olc::vi2d GetTileUnderScreenPos(const olc::vi2d& vPos) const;
		const olc::vi2d GetTileOffset() const;

	};
}

#ifdef OLC_PGEX_TRANSFORMEDVIEW
#undef OLC_PGEX_TRANSFORMEDVIEW

namespace olc
{
	olc::PixelGameEngine* TransformedView::GetPGE()
	{
		return pge;
	}

	void TransformedView::Initialise(const olc::vi2d& vViewArea, const olc::vf2d& vPixelScale)
	{
		SetViewArea(vViewArea);
		SetWorldScale(vPixelScale);
		m_vPixelScale = vPixelScale;
		m_vRecipPixel = 1.0f / m_vPixelScale;
	}

	void TransformedView::SetWorldOffset(const olc::vf2d& vOffset)
	{
		m_vWorldOffset = vOffset;
	}

	void TransformedView::MoveWorldOffset(const olc::vf2d& vDeltaOffset)
	{
		m_vWorldOffset += vDeltaOffset;
	}

	void TransformedView::SetWorldScale(const olc::vf2d& vScale)
	{
		m_vWorldScale = vScale;
		if (m_bZoomClamp) m_vWorldScale = m_vWorldScale.clamp(m_vMinScale, m_vMaxScale);
	}

	void TransformedView::SetViewArea(const olc::vi2d& vViewArea)
	{
		m_vViewArea = vViewArea;
	}

	olc::vf2d TransformedView::GetWorldTL() const
	{
		return TransformedView::ScreenToWorld({ 0,0 });
	}

	olc::vf2d TransformedView::GetWorldBR() const
	{
		return TransformedView::ScreenToWorld(m_vViewArea);
	}

	olc::vf2d TransformedView::GetWorldVisibleArea() const
	{
		return GetWorldBR() - GetWorldTL();
	}

	void TransformedView::SetScaleExtents(const olc::vf2d& vScaleMin, const olc::vf2d& vScaleMax)
	{
		m_vMaxScale = vScaleMax;
		m_vMinScale = vScaleMin;
	}

	void TransformedView::EnableScaleClamp(const bool bEnable)
	{
		m_bZoomClamp = bEnable;
	}

	void TransformedView::ZoomAtScreenPos(const float fDeltaZoom, const olc::vi2d& vPos)
	{
		olc::vf2d vOffsetBeforeZoom = ScreenToWorld(vPos);
		m_vWorldScale *= fDeltaZoom;
		if (m_bZoomClamp) m_vWorldScale = m_vWorldScale.clamp(m_vMinScale, m_vMaxScale);
		olc::vf2d vOffsetAfterZoom = ScreenToWorld(vPos);
		m_vWorldOffset += vOffsetBeforeZoom - vOffsetAfterZoom;
	}

	void TransformedView::SetZoom(const float fZoom, const olc::vf2d& vPos)
	{
		olc::vf2d vOffsetBeforeZoom = ScreenToWorld(vPos);
		m_vWorldScale = { fZoom, fZoom };
		if (m_bZoomClamp) m_vWorldScale = m_vWorldScale.clamp(m_vMinScale, m_vMaxScale);
		olc::vf2d vOffsetAfterZoom = ScreenToWorld(vPos);
		m_vWorldOffset += vOffsetBeforeZoom - vOffsetAfterZoom;
	}

	void TransformedView::StartPan(const olc::vi2d& vPos)
	{
		m_bPanning = true;
		m_vStartPan = olc::vf2d(vPos);
	}

	void TransformedView::UpdatePan(const olc::vi2d& vPos)
	{
		if (m_bPanning)
		{
			m_vWorldOffset -= (olc::vf2d(vPos) - m_vStartPan) / m_vWorldScale;
			m_vStartPan = olc::vf2d(vPos);
		}
	}

	void TransformedView::EndPan(const olc::vi2d& vPos)
	{
		UpdatePan(vPos);
		m_bPanning = false;
	}

	const olc::vf2d& TransformedView::GetWorldOffset() const
	{
		return m_vWorldOffset;
	}

	const olc::vf2d& TransformedView::GetWorldScale() const
	{
		return m_vWorldScale;
	}

	olc::vf2d TransformedView::WorldToScreen(const olc::vf2d& vWorldPos) const
	{
		olc::vf2d vFloat = ((vWorldPos - m_vWorldOffset) * m_vWorldScale);
		//vFloat = { std::floor(vFloat.x + 0.5f), std::floor(vFloat.y + 0.5f) };
		return vFloat;
	}

	olc::vf2d TransformedView::ScreenToWorld(const olc::vf2d& vScreenPos) const
	{
		return (olc::vf2d(vScreenPos) / m_vWorldScale) + m_vWorldOffset;
	}

	olc::vf2d TransformedView::ScaleToWorld(const olc::vf2d& vScreenSize) const
	{
		return (olc::vf2d(vScreenSize) / m_vWorldScale);
	}

	olc::vf2d TransformedView::ScaleToScreen(const olc::vf2d& vWorldSize) const
	{
		//olc::vf2d vFloat = (vWorldSize * m_vWorldScale) + olc::vf2d(0.5f, 0.5f);		
		//return vFloat.floor();
		return (vWorldSize * m_vWorldScale);
	}

	bool TransformedView::IsPointVisible(const olc::vf2d & vPos) const
	{
		olc::vi2d vScreen = WorldToScreen(vPos);
		return vScreen.x >= 0 && vScreen.x < m_vViewArea.x&& vScreen.y >= 0 && vScreen.y < m_vViewArea.y;
	}

	bool TransformedView::IsRectVisible(const olc::vf2d& vPos, const olc::vf2d& vSize) const
	{
		olc::vi2d vScreenPos = WorldToScreen(vPos);
		olc::vi2d vScreenSize = vSize * m_vWorldScale;
		return (vScreenPos.x < 0 + m_vViewArea.x && vScreenPos.x + vScreenSize.x > 0 && vScreenPos.y < m_vViewArea.y&& vScreenPos.y + vScreenSize.y > 0);
	}

	void TransformedView::HandlePanAndZoom(const int nMouseButton, const float fZoomRate, const bool bPan, const bool bZoom)
	{
		const auto& vMousePos = pge->GetMousePos();
		if (bPan)
		{
			if (pge->GetMouse(nMouseButton).bPressed) StartPan(vMousePos);
			if (pge->GetMouse(nMouseButton).bHeld) UpdatePan(vMousePos);
			if (pge->GetMouse(nMouseButton).bReleased) EndPan(vMousePos);
		}

		if (bZoom)
		{
			if (pge->GetMouseWheel() > 0) ZoomAtScreenPos(1.0f + fZoomRate, vMousePos);
			if (pge->GetMouseWheel() < 0) ZoomAtScreenPos(1.0f - fZoomRate, vMousePos);
		}
	}

	bool TransformedView::Draw(float x, float y, olc::Pixel p)
	{
		return Draw({ x, y }, p);
	}

	bool TransformedView::Draw(const olc::vf2d & pos, olc::Pixel p)
	{
		return pge->Draw(WorldToScreen(pos), p);
	}
	
	void TransformedView::DrawLine(float x1, float y1, float x2, float y2, olc::Pixel p, uint32_t pattern)
	{
		DrawLine({ x1, y1 }, { x2, y2 }, p, pattern);
	}

	void TransformedView::DrawLine(const olc::vf2d & pos1, const olc::vf2d & pos2, olc::Pixel p, uint32_t pattern)
	{
		pge->DrawLine(WorldToScreen(pos1), WorldToScreen(pos2), p, pattern);
	}
	
	void TransformedView::DrawCircle(float x, float y, float radius, olc::Pixel p, uint8_t mask)
	{
		DrawCircle({ x,y }, radius, p, mask);
	}

	void TransformedView::DrawCircle(const olc::vf2d & pos, float radius, olc::Pixel p, uint8_t mask)
	{
		pge->DrawCircle(WorldToScreen(pos), int32_t(radius * m_vWorldScale.x), p, mask);
	}

	void TransformedView::FillCircle(float x, float y, float radius, olc::Pixel p)
	{
		FillCircle({ x,y }, radius, p);
	}

	void TransformedView::FillCircle(const olc::vf2d & pos, float radius, olc::Pixel p)
	{
		pge->FillCircle(WorldToScreen(pos), int32_t(radius * m_vWorldScale.x), p);
	}

	void TransformedView::DrawRect(float x, float y, float w, float h, olc::Pixel p)
	{
		DrawRect({ x, y }, { w, h }, p);
	}

	void TransformedView::DrawRect(const olc::vf2d & pos, const olc::vf2d & size, olc::Pixel p)
	{
		pge->DrawRect(WorldToScreen(pos), ((size * m_vWorldScale) + olc::vf2d(0.5f, 0.5f)).floor(), p);
	}
	
	void TransformedView::FillRect(float x, float y, float w, float h, olc::Pixel p)
	{
		FillRect({ x, y }, { w, h }, p);
	}

	void TransformedView::FillRect(const olc::vf2d & pos, const olc::vf2d & size, olc::Pixel p)
	{
		pge->FillRect(WorldToScreen(pos), size * m_vWorldScale, p);
	}
	
	void TransformedView::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, olc::Pixel p)
	{
		DrawTriangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, p);
	}

	void TransformedView::DrawTriangle(const olc::vf2d & pos1, const olc::vf2d & pos2, const olc::vf2d & pos3, olc::Pixel p)
	{
		pge->DrawTriangle(WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3), p);
	}
	
	void TransformedView::FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, olc::Pixel p)
	{
		FillTriangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, p);
	}

	void TransformedView::FillTriangle(const olc::vf2d & pos1, const olc::vf2d & pos2, const olc::vf2d & pos3, olc::Pixel p)
	{
		pge->FillTriangle(WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3), p);
	}

	void TransformedView::DrawSprite(float x, float y, olc::Sprite* sprite, float scalex, float scaley, uint8_t flip)
	{
		DrawSprite({ x, y }, sprite, { scalex, scaley }, flip);
	}

	void TransformedView::DrawSprite(const olc::vf2d & pos, olc::Sprite * sprite, const olc::vf2d & scale, uint8_t flip)
	{
		olc::vf2d vSpriteSize = olc::vf2d(float(sprite->width), float(sprite->height));
		if (IsRectVisible(pos, vSpriteSize * scale))
		{
			olc::vf2d vSpriteScaledSize = vSpriteSize * m_vRecipPixel * m_vWorldScale * scale;
			olc::vi2d vPixel;
			olc::vi2d vSpritePixelStart = WorldToScreen(pos);
			olc::vi2d vSpritePixelEnd = WorldToScreen((vSpriteSize * scale) + pos);

			olc::vi2d vScreenPixelStart = (vSpritePixelStart).max({0,0});
			olc::vi2d vScreenPixelEnd = (vSpritePixelEnd).min({ pge->ScreenWidth(),pge->ScreenHeight() });
			
			olc::vf2d vPixelStep = 1.0f / vSpriteScaledSize;
			
			for (vPixel.y = vScreenPixelStart.y; vPixel.y < vScreenPixelEnd.y; vPixel.y++)
			{
				for (vPixel.x = vScreenPixelStart.x; vPixel.x < vScreenPixelEnd.x; vPixel.x++)
				{
					olc::vf2d vSample = olc::vf2d(vPixel - vSpritePixelStart) * vPixelStep;
					pge->Draw(vPixel, sprite->Sample(vSample.x, vSample.y));
				}
			}
		}
	}


	void TransformedView::DrawPartialSprite(float x, float y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, float scalex, float scaley, uint8_t flip)
	{
		DrawPartialSprite({ x,y }, sprite, { ox,oy }, { w, h }, { scalex, scaley }, flip);
	}

	void TransformedView::DrawPartialSprite(const olc::vf2d& pos, Sprite* sprite, const olc::vi2d& sourcepos, const olc::vi2d& size, const olc::vf2d& scale, uint8_t flip)
	{
		olc::vf2d vSpriteSize = size;
		if (IsRectVisible(pos, size * scale))
		{
			olc::vf2d vSpriteScaledSize = olc::vf2d(size) * m_vRecipPixel * m_vWorldScale * scale;
			olc::vf2d vSpritePixelStep = 1.0f / olc::vf2d(float(sprite->width), float(sprite->height));
			olc::vi2d vPixel, vStart = WorldToScreen(pos), vEnd = vSpriteScaledSize + vStart;
			olc::vf2d vScreenPixelStep = 1.0f / vSpriteScaledSize;

			for (vPixel.y = vStart.y; vPixel.y < vEnd.y; vPixel.y++)
			{
				for (vPixel.x = vStart.x; vPixel.x < vEnd.x; vPixel.x++)
				{
					olc::vf2d vSample = ((olc::vf2d(vPixel - vStart) * vScreenPixelStep) * size * vSpritePixelStep) + olc::vf2d(sourcepos) * vSpritePixelStep;
					pge->Draw(vPixel, sprite->Sample(vSample.x, vSample.y));
				}
			}
		}
	}

	void TransformedView::DrawString(float x, float y, const std::string& sText, Pixel col, const olc::vf2d& scale)
	{
		DrawString({ x, y }, sText, col, scale);
	}

	void TransformedView::DrawString(const olc::vf2d& pos, const std::string& sText, const Pixel col, const olc::vf2d& scale)
	{
		olc::vf2d vOffset = { 0.0f, 0.0f };
		Pixel::Mode m = pge->GetPixelMode();

		auto StringPlot = [&col](const int x, const int y, const olc::Pixel& pSource, const olc::Pixel& pDest)
		{		
			return pSource.r > 1 ? col : pDest;
		};

		pge->SetPixelMode(StringPlot);

		for (auto c : sText)
		{
			if (c == '\n')
			{
				vOffset.x = 0.0f; vOffset.y += 8.0f * m_vRecipPixel.y * scale.y;
			}
			else
			{
				int32_t ox = ((c - 32) % 16) * 8;
				int32_t oy = ((c - 32) / 16) * 8;
				DrawPartialSprite(pos + vOffset, pge->GetFontSprite(), { ox, oy }, { 8, 8 }, scale);
				vOffset.x += 8.0f * m_vRecipPixel.x * scale.x;
			}
		}
		pge->SetPixelMode(m);
	}


	void TransformedView::DrawDecal(const olc::vf2d & pos, olc::Decal * decal, const olc::vf2d & scale, const olc::Pixel & tint)
	{
		pge->DrawDecal(WorldToScreen(pos), decal, scale * m_vWorldScale * m_vRecipPixel, tint);
	}

	void TransformedView::DrawPartialDecal(const olc::vf2d & pos, olc::Decal * decal, const olc::vf2d & source_pos, const olc::vf2d & source_size, const olc::vf2d & scale, const olc::Pixel & tint)
	{
		pge->DrawPartialDecal(WorldToScreen(pos), decal, source_pos, source_size, scale * m_vWorldScale * m_vRecipPixel, tint);
	}

	void TransformedView::DrawPartialDecal(const olc::vf2d & pos, const olc::vf2d & size, olc::Decal * decal, const olc::vf2d & source_pos, const olc::vf2d & source_size, const olc::Pixel & tint)
	{
		pge->DrawPartialDecal(WorldToScreen(pos), size * m_vWorldScale * m_vRecipPixel, decal, source_pos, source_size, tint);
	}
	
	void TransformedView::DrawExplicitDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d* uv, const olc::Pixel* col, uint32_t elements)
	{
		std::vector<olc::vf2d> vTransformed(elements);
		for (uint32_t n = 0; n < elements; n++)
			vTransformed[n] = WorldToScreen(pos[n]);		
		pge->DrawExplicitDecal(decal, vTransformed.data(), uv, col, elements);
	}

	void TransformedView::DrawWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::Pixel& tint)
	{
		std::array<olc::vf2d, 4> vTransformed = 
		{ {
			WorldToScreen(pos[0]), WorldToScreen(pos[1]),
			WorldToScreen(pos[2]), WorldToScreen(pos[3]),
		} };

		pge->DrawWarpedDecal(decal, vTransformed, tint);
	}

	void TransformedView::DrawWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::Pixel& tint)
	{
		DrawWarpedDecal(decal, &pos[0], tint);
	}

	void TransformedView::DrawWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::Pixel& tint)
	{
		DrawWarpedDecal(decal, pos.data(), tint);
	}

	void TransformedView::DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{
		DrawPartialWarpedDecal(decal, &pos[0], source_pos, source_size, tint);
	}

	void TransformedView::DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{
		std::array<olc::vf2d, 4> vTransformed =
		{ {
			WorldToScreen(pos[0]), WorldToScreen(pos[1]),
			WorldToScreen(pos[2]), WorldToScreen(pos[3]),
		} };

		pge->DrawPartialWarpedDecal(decal, vTransformed, source_pos, source_size, tint);
	}

	void TransformedView::DrawPartialWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{
		DrawPartialWarpedDecal(decal, pos.data(), source_pos, source_size, tint);
	}

	void TransformedView::DrawRotatedDecal(const olc::vf2d & pos, olc::Decal * decal, const float fAngle, const olc::vf2d & center, const olc::vf2d & scale, const olc::Pixel & tint)
	{
		pge->DrawRotatedDecal(WorldToScreen(pos), decal, fAngle, center, scale * m_vWorldScale * m_vRecipPixel, tint);
	}

	void TransformedView::DrawPartialRotatedDecal(const olc::vf2d & pos, olc::Decal * decal, const float fAngle, const olc::vf2d & center, const olc::vf2d & source_pos, const olc::vf2d & source_size, const olc::vf2d & scale, const olc::Pixel & tint)
	{
		pge->DrawPartialRotatedDecal(WorldToScreen(pos), decal, fAngle, center, source_pos, source_size, scale * m_vWorldScale * m_vRecipPixel, tint);
	}
	
	void TransformedView::DrawStringDecal(const olc::vf2d & pos, const std::string & sText, const olc::Pixel col, const olc::vf2d & scale)
	{
		pge->DrawStringDecal(WorldToScreen(pos), sText, col, scale * m_vWorldScale * m_vRecipPixel);
	}

	void TransformedView::DrawStringPropDecal(const olc::vf2d & pos, const std::string & sText, const olc::Pixel col, const olc::vf2d & scale )
	{
		pge->DrawStringPropDecal(WorldToScreen(pos), sText, col, scale * m_vWorldScale * m_vRecipPixel);
	}

	void TransformedView::FillRectDecal(const olc::vf2d & pos, const olc::vf2d & size, const olc::Pixel col)
	{
		pge->FillRectDecal(WorldToScreen(pos), (size * m_vWorldScale).ceil(), col);
	}

	void TransformedView::DrawRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col)
	{
		pge->DrawRectDecal(WorldToScreen(pos), (size * m_vWorldScale).ceil(), col);
	}

	void TransformedView::DrawLineDecal(const olc::vf2d& pos1, const olc::vf2d& pos2, Pixel p)
	{
		pge->DrawLineDecal(WorldToScreen(pos1), WorldToScreen(pos2), p);
	}
	
	void TransformedView::GradientFillRectDecal(const olc::vf2d & pos, const olc::vf2d & size, const olc::Pixel colTL, const olc::Pixel colBL, const olc::Pixel colBR, const olc::Pixel colTR)
	{
		pge->GradientFillRectDecal(WorldToScreen(pos), size * m_vWorldScale, colTL, colBL, colBR, colTR);
	}

	void TransformedView::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const olc::Pixel tint)
	{
		std::vector<olc::vf2d> vTransformed(pos.size());
		for (uint32_t n = 0; n < pos.size(); n++)
			vTransformed[n] = WorldToScreen(pos[n]);
		pge->DrawPolygonDecal(decal, vTransformed, uv, tint);
	}

	void TransformedView::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel> &tint)
	{
		std::vector<olc::vf2d> vTransformed(pos.size());
		for (uint32_t n = 0; n < pos.size(); n++)
			vTransformed[n] = WorldToScreen(pos[n]);
		pge->DrawPolygonDecal(decal, vTransformed, uv, tint);
	}

	void TransformedView::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& colours, const olc::Pixel tint)
	{
		std::vector<olc::vf2d> vTransformed(pos.size());
		for (uint32_t n = 0; n < pos.size(); n++)
			vTransformed[n] = WorldToScreen(pos[n]);
		pge->DrawPolygonDecal(decal, vTransformed, uv, colours, tint);
	}



#if defined (OLC_PGEX_SHADER)

	void TransformedView::DrawDecal(olc::Shade &shade, const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& scale, const olc::Pixel& tint)
	{
		shade.DrawDecal(WorldToScreen(pos), decal, scale * m_vWorldScale * m_vRecipPixel, tint);
	}

	void TransformedView::DrawPartialDecal(olc::Shade& shade, const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale, const olc::Pixel& tint)
	{
		shade.DrawPartialDecal(WorldToScreen(pos), decal, source_pos, source_size, scale * m_vWorldScale * m_vRecipPixel, tint);
	}

	void TransformedView::DrawPartialDecal(olc::Shade& shade, const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{
		shade.DrawPartialDecal(WorldToScreen(pos), size * m_vWorldScale * m_vRecipPixel, decal, source_pos, source_size, tint);
	}

#endif







	/////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////


	TileTransformedView::TileTransformedView(const olc::vi2d& vViewArea, const olc::vi2d& vTileSize)		
	{ 
		Initialise(vViewArea, vTileSize);
	}


	olc::vi2d TileTransformedView::GetTopLeftTile() const
	{
		return ScreenToWorld({ 0,0 }).floor();		
	}

	olc::vi2d TileTransformedView::GetBottomRightTile() const
	{
		return ScreenToWorld(m_vViewArea).ceil();
	}

	olc::vi2d TileTransformedView::GetVisibleTiles() const
	{
		return GetBottomRightTile() - GetTopLeftTile();
	}

	olc::vi2d TileTransformedView::GetTileUnderScreenPos(const olc::vi2d& vPos) const
	{
		return ScreenToWorld(vPos).floor();		
	}

	const olc::vi2d TileTransformedView::GetTileOffset() const
	{
		return { int32_t((m_vWorldOffset.x - std::floor(m_vWorldOffset.x)) * m_vWorldScale.x),
			int32_t((m_vWorldOffset.y - std::floor(m_vWorldOffset.y)) * m_vWorldScale.y) };
	}
}

#endif
#endif
