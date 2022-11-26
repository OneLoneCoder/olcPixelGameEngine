/*
	OneLoneCoder - Camera2D v1.00
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	A 2D world camera with various modes


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

*/

#pragma once

#include "olcPixelGameEngine.h"

namespace olc::utils
{
	class Camera2D
	{
	public:
		enum class Mode : uint8_t
		{			
			Simple,			// No motion, just directly settable			
			EdgeMove,		// Moves as target crosses boundary			
			LazyFollow,		// Lazily follows the target			
			FixedScreens,	// Moves statically between "screens"
		};

	public:
		inline Camera2D() : m_pTarget(&m_vLocalTarget) {}
		
		// Construct a camera with a viewable area size, and an optional starting position
		inline Camera2D(const olc::vf2d& vViewSize, const olc::vf2d& vViewPos = { 0.0f, 0.0f }) : m_pTarget(&m_vLocalTarget)
		{
			m_vViewSize = vViewSize;
			m_vViewPos = vViewPos;
		}

		// Set the operational mode of this camera
		inline void SetMode(const Mode t)
		{
			m_nMode = t;
		}

		// Get the operational mode of this camera
		inline Mode GetMode() const
		{
			return m_nMode;
		}

		// Get the position of the target being tracked by this camera
		inline const olc::vf2d& GetTarget() const
		{
			return *m_pTarget;
		}

		// Get the position of the cameras focus point
		inline const olc::vf2d& GetPosition() const
		{
			return m_vPosition;
		}

		// Get the top left of teh cameras visible area in world space
		inline const olc::vf2d& GetViewPosition() const
		{
			return m_vViewPos;
		}

		// Get the camera's visible area
		inline const olc::vf2d& GetViewSize() const
		{
			return m_vViewSize;
		}

		// Set tracked point via pointer
		inline void SetTarget(olc::vf2d& vTarget)
		{
			m_pTarget = &vTarget;
		}

		// Set tracked point via const ref - {10, 35} for example
		inline void SetTarget(const olc::vf2d&& vTarget)
		{
			m_vLocalTarget = vTarget;
			m_pTarget = &m_vLocalTarget;
		}

		// Set world boundary rectangle
		inline void SetWorldBoundary(const olc::vf2d& vPos, const olc::vf2d& vSize)
		{
			m_vWorldBoundaryPos = vPos;
			m_vWorldBoundarySize = vSize;
		}

		// Instruct camera to respect world boundaries
		inline void EnableWorldBoundary(const bool bEnable)
		{
			m_bWorldBoundary = bEnable;
		}

		// Are we using a world boundary?
		inline bool IsWorldBoundaryEnabled() const
		{
			return m_bWorldBoundary;
		}

		// Get the world boundary rectangle position
		inline const olc::vf2d& GetWorldBoundaryPosition() const
		{
			return m_vWorldBoundaryPos;
		}

		// Get the world boundary rectangle size
		inline const olc::vf2d& GetWorldBoundarySize() const
		{
			return m_vWorldBoundarySize;
		}

		// Set the velocity at which the lazy follower reaches tracked point
		inline void SetLazyFollowRate(const float fRate)
		{
			m_fLazyFollowRate = fRate;
		}

		// Get the velocity at which the lazy follower reaches tracked point
		inline float GetLazyFollowRate() const
		{
			return m_fLazyFollowRate;
		}

		// Set distance from tracked point to start nudging screen
		inline void SetEdgeTriggerDistance(const olc::vf2d& vEdge)
		{
			m_vEdgeTriggerDistance = vEdge;
		}

		// Return disance from tracked point that screen will nudge
		inline const olc::vf2d& GetEdgeTriggerDistance() const
		{
			return m_vEdgeTriggerDistance;
		}

		// Update camera, animating if necessary, obeying world boundary rules
		// returns true if target is visible
		inline virtual bool Update(const float fElapsedTime)
		{
			switch (m_nMode)
			{
				case Mode::Simple:
				{
					m_vPosition = GetTarget();
				}
				break;

				case Mode::EdgeMove:
				{
					olc::vf2d vOverlap = GetTarget() - m_vPosition;
					if (vOverlap.x > m_vEdgeTriggerDistance.x) m_vPosition.x += vOverlap.x - m_vEdgeTriggerDistance.x;
					if (vOverlap.x < -m_vEdgeTriggerDistance.x) m_vPosition.x += vOverlap.x + m_vEdgeTriggerDistance.x;
					if (vOverlap.y > m_vEdgeTriggerDistance.y) m_vPosition.y += vOverlap.y - m_vEdgeTriggerDistance.y;
					if (vOverlap.y < -m_vEdgeTriggerDistance.y) m_vPosition.y += vOverlap.y + m_vEdgeTriggerDistance.y;
				}
				break;

				case Mode::LazyFollow:
				{
					m_vPosition += (GetTarget() - m_vPosition) * m_fLazyFollowRate * fElapsedTime;
				}
				break;

				case Mode::FixedScreens:
				{
					m_vPosition = olc::vf2d(olc::vi2d(GetTarget() / m_vScreenSize) * olc::vi2d(m_vScreenSize)) + (m_vViewSize * 0.5f);
				}
				break;
			}

			// Make camera target the middle of the view
			m_vViewPos = m_vPosition - (m_vViewSize * 0.5f);

			// Clamp to World Boundary (if in place)
			if (m_bWorldBoundary)
			{
				m_vViewPos = m_vViewPos.max(m_vWorldBoundaryPos).min(m_vWorldBoundaryPos + m_vWorldBoundarySize - m_vViewSize);
			}

			return GetTarget().x >= m_vViewPos.x && GetTarget().x < (m_vViewPos.x + m_vViewSize.x) &&
				GetTarget().y >= m_vViewPos.y && GetTarget().y < (m_vViewPos.y + m_vViewSize.y);
		}

	protected:
		// Position of camera focus point in the world
		olc::vf2d m_vPosition;
		// Rectangular size of camera viewing area
		olc::vf2d m_vViewSize;
		// Top left coordinate of camera viewing area
		olc::vf2d m_vViewPos;
		// Camera movement mode
		Mode m_nMode = Mode::Simple;

		// Target Vector2D object camera should follow (either ref or ptr)
		olc::vf2d* m_pTarget = nullptr;
		olc::vf2d m_vLocalTarget;

		// World Boundary
		bool m_bWorldBoundary = false;
		olc::vf2d m_vWorldBoundaryPos = { 0.0f, 0.0f };
		olc::vf2d m_vWorldBoundarySize = { 256.0f, 240.0f };

		// Mode specific
		olc::vf2d m_vEdgeTriggerDistance = { 1.0f, 1.0f };
		float m_fLazyFollowRate = 4.0f;
		olc::vi2d m_vScreenSize = { 16,15 };
	};
}