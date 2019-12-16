/*
	Live 100K Code Party! Code-It-Yourself: SHMUP
	"It's done... 2019 is done..." - javidx9
	
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
		
	Relevant Video: https://youtu.be/CqDfZEX0Yhc
	
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
	
	Community Blog: http://community.onelonecoder.com
	
	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// IMPORTANT!! Requires sprites not provided in repo!
// Sprites are 48x48 pixels.


#include <array>
#include <cstdint>
#include <algorithm>

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "100K Live Special - SHMUP";
	}

	olc::Sprite* sprPlayer = nullptr;
	olc::Sprite* sprEnemy[3];	

	olc::vf2d vPlayerPos;
	float fPlayerSpeed = 100.0f;
	float fScrollSpeed = 60.0f;
	float fPlayerShipRad = 24 * 24;
	float fPlayerHealth = 1000.0f;
	float fPlayerGunTemp = 0.0f;
	float fPlayerGunReload = 0.2f;
	float fPlayerGunReloadTime = 0.0f;

	double dWorldPos = 0.0f;

	std::array<olc::vf2d, 1000> aryStars;

	struct sBullet
	{
		olc::vf2d pos;
		olc::vf2d vel;
		bool remove = false;
	};

	struct sEnemy;

	struct sEnemyDefinition
	{
		double dTriggerTime;
		uint32_t nSpriteID = 0;
		float fHealth = 0.0f;
		std::function<void(sEnemy&, float, float)> funcMove;
		std::function<void(sEnemy&, float, float, std::list<sBullet>& bullets)> funcFire;
		float offset = 0.0f;
	};

	struct sEnemy
	{
		olc::vf2d pos;
		sEnemyDefinition def;

		std::array<float, 4> dataMove{ 0 };
		std::array<float, 4> dataFire{ 0 };
			
		void Update(float fElapsedTime, float fScrollSpeed, std::list<sBullet>& bullets)
		{
			def.funcMove(*this, fElapsedTime, fScrollSpeed);
			def.funcFire(*this, fElapsedTime, fScrollSpeed, bullets);
		}			
	};

	
	olc::vf2d GetMiddle(const olc::Sprite *s)
	{
		return { (float)s->width / 2.0f, (float)s->height / 2.0f };
	}
	

	std::list<sEnemyDefinition> listSpawns;
	std::list<sEnemy> listEnemies;
	std::list<sBullet> listEnemyBullets;
	std::list<sBullet> listPlayerBullets;
	std::list<olc::vf2d> listStars;
	std::list<sBullet> listFragments;

public:
	bool OnUserCreate() override
	{
		// Load resources
		sprPlayer = new olc::Sprite("gfx//100k_player.png");
		sprEnemy[0] = new olc::Sprite("gfx//100k_enemy1.png");
		sprEnemy[1] = new olc::Sprite("gfx//100k_enemy2.png");
		sprEnemy[2] = new olc::Sprite("gfx//100k_enemy3.png");

		// Generate Star Map
		for (auto& s : aryStars) s = { (float)(rand() % ScreenWidth()), (float)(rand() % ScreenHeight()) };
		
		// MOVEMENT PATTERN FUNCTIONS
		auto Move_None = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed)
		{
			e.pos.y += fScrollSpeed * fElapsedTime;
		};

		auto Move_StraightFast = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed)
		{
			e.pos.y += 3.0f * fScrollSpeed * fElapsedTime;
		};

		auto Move_StraightSlow = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed)
		{
			e.pos.y += 0.5f * fScrollSpeed * fElapsedTime;
		};

		auto Move_SinusoidNarrow = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed)
		{
			e.dataMove[0] += fElapsedTime;
			e.pos.y += 0.5f * fScrollSpeed * fElapsedTime;
			e.pos.x += 50.0f * cosf(e.dataMove[0]) * fElapsedTime;
		};

		auto Move_SinusoidWide = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed)
		{
			e.dataMove[0] += fElapsedTime;
			e.pos.y += 0.5f * fScrollSpeed * fElapsedTime;
			e.pos.x += 150.0f * cosf(e.dataMove[0]) * fElapsedTime;
		};

		// FIRING PATTERN FUNCTIONS
		auto Fire_None = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed, std::list<sBullet>& bullets)
		{

		};

		auto Fire_StraightDelay2 = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed, std::list<sBullet>& bullets)
		{		
			constexpr float fDelay = 0.2f;
			e.dataFire[0] += fElapsedTime;
			if (e.dataFire[0] >= fDelay)
			{
				e.dataFire[0] -= fDelay;
				sBullet b;
				b.pos = e.pos + olc::vf2d((float)sprEnemy[e.def.nSpriteID]->width / 2.0f, (float)sprEnemy[e.def.nSpriteID]->height);
				b.vel = { 0.0f, 180.0f };
				bullets.push_back(b);
			}
		};

		auto Fire_CirclePulse2 = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed, std::list<sBullet>& bullets)
		{
			constexpr float fDelay = 0.2f;
			constexpr int nBullets = 10;
			constexpr float fTheta = 2.0f * 3.14159f / (float)nBullets;
			e.dataFire[0] += fElapsedTime;
			if (e.dataFire[0] >= fDelay)
			{
				e.dataFire[0] -= fDelay;				
				for (int i = 0; i < nBullets; i++)
				{
					sBullet b;
					b.pos = e.pos + GetMiddle(sprEnemy[e.def.nSpriteID]);
					b.vel = { 180.0f * cosf(fTheta * i), 180.0f * sinf(fTheta * i)};
					bullets.push_back(b);
				}
			}
		};

		auto Fire_DeathSpiral = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed, std::list<sBullet>& bullets)
		{
			constexpr float fDelay = 0.01f;			
			e.dataFire[0] += fElapsedTime;			
			if (e.dataFire[0] >= fDelay)
			{
				e.dataFire[1] += 0.1f;
				e.dataFire[0] -= fDelay;
				sBullet b;
				b.pos = e.pos + GetMiddle(sprEnemy[e.def.nSpriteID]);
				b.vel = { 180.0f * cosf(e.dataFire[1]), 180.0f * sinf(e.dataFire[1]) };
				bullets.push_back(b);
				
			}
		};

		auto Fire_DeathSpiralCircle = [&](sEnemy& e, float fElapsedTime, float fScrollSpeed, std::list<sBullet>& bullets)
		{
			constexpr float fDelay = 0.2f;
			constexpr int nBullets = 100;
			constexpr float fTheta = 2.0f * 3.14159f / (float)nBullets;
			e.dataFire[0] += fElapsedTime;
			if (e.dataFire[0] >= fDelay)
			{
				e.dataFire[0] -= fDelay;
				e.dataFire[1] += 0.1f;
				for (int i = 0; i < nBullets; i++)
				{
					sBullet b;
					b.pos = e.pos + GetMiddle(sprEnemy[e.def.nSpriteID]);
					b.vel = { 180.0f * cosf(fTheta * i + e.dataFire[1]), 180.0f * sinf(fTheta * i + e.dataFire[1]) };
					bullets.push_back(b);
				}
			}
		};

		// Construct level
		listSpawns = 
		{
			{100.0, 0, 3.0f, Move_None,           Fire_CirclePulse2, 0.25f},
			{100.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.75f},
			{120.0, 1, 3.0f, Move_SinusoidNarrow, Fire_CirclePulse2, 0.50f},

			{200.0, 2, 3.0f, Move_SinusoidWide, Fire_CirclePulse2, 0.30f},
			{200.0, 2, 3.0f, Move_SinusoidWide, Fire_CirclePulse2, 0.70f},

			{500.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.2f},
			{500.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.4f},
			{500.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.6f},
			{500.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.8f},

			{550.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.1f},
			{550.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.3f},
			{550.0, 0, 3.0f, Move_StraightSlow,   Fire_DeathSpiralCircle,  0.5f},
			{550.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.7f},
			{550.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.9f},

			{600.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.2f},
			{600.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.4f},
			{600.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.6f},
			{600.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.8f},

			{1100.0, 0, 3.0f, Move_None,           Fire_CirclePulse2, 0.25f},
			{1100.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.75f},
			{1120.0, 1, 3.0f, Move_SinusoidNarrow, Fire_CirclePulse2, 0.50f},
			 
			{1200.0, 2, 3.0f, Move_SinusoidWide, Fire_CirclePulse2, 0.30f},
			{1200.0, 2, 3.0f, Move_SinusoidWide, Fire_CirclePulse2, 0.70f},
			 
			{1500.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.2f},
			{1500.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.4f},
			{1500.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.6f},
			{1500.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.8f},
			 
			{1550.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.1f},
			{1550.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.3f},
			{1550.0, 0, 3.0f, Move_StraightSlow,   Fire_DeathSpiralCircle,  0.5f},
			{1550.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.7f},
			{1550.0, 0, 3.0f, Move_StraightFast,   Fire_DeathSpiral,  0.9f},
			 
			{1600.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.2f},
			{1600.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.4f},
			{1600.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.6f},
			{1600.0, 0, 3.0f, Move_StraightFast,   Fire_StraightDelay2,  0.8f},
		};


		vPlayerPos = { (float)ScreenWidth() / 2, (float)ScreenHeight() / 2 };

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// AutoScroll World
		dWorldPos += fScrollSpeed * fElapsedTime;

		// Scroll Player Object
		vPlayerPos.y += fScrollSpeed * fElapsedTime;
		
		// Handle Player Input
		if (GetKey(olc::W).bHeld) vPlayerPos.y -= (fPlayerSpeed + fScrollSpeed) * fElapsedTime;
		if (GetKey(olc::S).bHeld) vPlayerPos.y += (fPlayerSpeed - fScrollSpeed) * fElapsedTime;
		if (GetKey(olc::A).bHeld) vPlayerPos.x -= fPlayerSpeed * fElapsedTime * 2.0f;
		if (GetKey(olc::D).bHeld) vPlayerPos.x += fPlayerSpeed * fElapsedTime * 2.0f;

		// Clamp Player to screen
		if (vPlayerPos.x <= 0) vPlayerPos.x = 0;
		if (vPlayerPos.x + (float)sprPlayer->width >= ScreenWidth()) vPlayerPos.x = (float)ScreenWidth() - sprPlayer->width;
		if (vPlayerPos.y <= 0) vPlayerPos.y = 0;
		if (vPlayerPos.y + (float)sprPlayer->height >= ScreenHeight()) vPlayerPos.y = (float)ScreenHeight() - sprPlayer->height;

		// Player Weapon Fire
		bool bCanFire = false;
		fPlayerGunReloadTime -= fElapsedTime;
		if (fPlayerGunReloadTime <= 0.0f)
		{			
			bCanFire = true;
		}

		fPlayerGunTemp -= fElapsedTime * 10.0f;
		if (fPlayerGunTemp < 0) fPlayerGunTemp = 0;		
		if (GetMouse(0).bHeld)
		{
			if (bCanFire && fPlayerGunTemp < 80.0f)
			{
				fPlayerGunReloadTime = fPlayerGunReload;
				fPlayerGunTemp += 5.0f;	
				if (fPlayerGunTemp > 100.0f) fPlayerGunTemp = 100.0f;
				listPlayerBullets.push_back({vPlayerPos + olc::vf2d((float)sprPlayer->width / 2.0f, 0.0f), {0.0f, -200.0f} });
			}
		}

		// Update Player Bullets
		for (auto& b : listPlayerBullets)
		{
			// Position Bullet
			b.pos += (b.vel + olc::vf2d(0.0f, fScrollSpeed)) * fElapsedTime;

			// Check Enemies Vs Player Bullets
			for (auto& e : listEnemies)
			{
				if ((b.pos - (e.pos + olc::vf2d(24.0f, 24.0f))).mag2() < fPlayerShipRad)
				{
					// Enemy has been hit
					b.remove = true;
					e.def.fHealth -= 1.0f;	

					// Trigger explosion
					if (e.def.fHealth <= 0)
					{
						for (int i = 0; i < 500; i++)
						{
							float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
							float speed = ((float)rand() / (float)RAND_MAX) * 200.0f + 50.0f;
							listFragments.push_back({								
								e.pos + GetMiddle(sprEnemy[e.def.nSpriteID]),
								{ cosf(angle)*speed, sinf(angle)*speed }});
						}
					}
				}
			}
		}


		// Perform spawn check
		while(!listSpawns.empty() && dWorldPos >= listSpawns.front().dTriggerTime)
		{
			sEnemy e;
			e.def = listSpawns.front();			
			e.pos = 
			{ 
				listSpawns.front().offset * (float)(ScreenWidth() - sprEnemy[e.def.nSpriteID]->width), 
				0.0f - sprEnemy[e.def.nSpriteID]->height 
			};
			listSpawns.pop_front();
			listEnemies.push_back(e);
		}

		// Update Enemies
		for (auto& e : listEnemies)	e.Update(fElapsedTime, fScrollSpeed, listEnemyBullets);

		// Update Enemy Bullets
		for (auto& b : listEnemyBullets)
		{
			// Position Bullet
			b.pos += (b.vel + olc::vf2d(0.0f, fScrollSpeed)) * fElapsedTime;

			// Check Player Vs Enemy Bullets
			if ((b.pos - (vPlayerPos + olc::vf2d(24.0f, 24.0f))).mag2() < fPlayerShipRad)
			{
				b.remove = true;
				fPlayerHealth -= 10.0f;
			}
		}

		// Update Fragments
		for(auto& f : listFragments) f.pos += (f.vel + olc::vf2d(0.0f, fScrollSpeed)) * fElapsedTime;
		 
		// Remove Offscreen Enemies
		listEnemies.remove_if([&](const sEnemy& e) {return (e.pos.y >= (float)ScreenHeight()) || e.def.fHealth <= 0.0f; });
		
		// Remove finished enemy bullets
		listEnemyBullets.remove_if([&](const sBullet& b) {return b.pos.x<0 || b.pos.x>ScreenWidth() || b.pos.y <0 || b.pos.y>ScreenHeight() || b.remove; });

		// Remove finished player bullets
		listPlayerBullets.remove_if([&](const sBullet& b) {return b.pos.x<0 || b.pos.x>ScreenWidth() || b.pos.y <0 || b.pos.y>ScreenHeight() || b.remove; });

		// Remove finished fragments
		listFragments.remove_if([&](const sBullet& b) {return b.pos.x<0 || b.pos.x>ScreenWidth() || b.pos.y <0 || b.pos.y>ScreenHeight() || b.remove; });

		// GRAPHICS
		Clear(olc::BLACK);

		// Update & Draw Stars		
		for (size_t i=0; i<aryStars.size(); i++)
		{
			auto& s = aryStars[i];
			s.y += fScrollSpeed * fElapsedTime * ((i<aryStars.size() >> 2) ? 0.8f : 1.0f);
			if (s.y >= (float)ScreenHeight())
				s = { (float)(rand() % ScreenWidth()), 0.0f };

			Draw(s, (i < aryStars.size() >> 2) ? olc::DARK_GREY : olc::WHITE);
		}

		SetPixelMode(olc::Pixel::MASK);

		// Draw Enemies
		for (auto& e : listEnemies)	DrawSprite(e.pos, sprEnemy[e.def.nSpriteID]);

		// Draw Player
		DrawSprite(vPlayerPos, sprPlayer);

		SetPixelMode(olc::Pixel::NORMAL);

		// Draw Enemy Bullets
		for (auto& b : listEnemyBullets) FillCircle(b.pos, 3, olc::RED);

		// Draw Player Bullets
		for (auto& b : listPlayerBullets) FillCircle(b.pos, 3, olc::CYAN);

		// Draw Fragments
		for (auto& b : listFragments) Draw(b.pos, olc::YELLOW);

		// Draw Player Health Bar
		DrawString(4, 4, "HEALTH:");
		FillRect(60, 4, (fPlayerHealth / 1000.0f * 576.0f), 8, olc::GREEN);

		DrawString(4, 14, "WEAPON:");
		FillRect(60, 14, (fPlayerGunTemp / 100.0f * 576.0f), 8, olc::YELLOW);

		return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();
	return 0;
}