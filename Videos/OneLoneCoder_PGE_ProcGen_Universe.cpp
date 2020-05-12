/*
	Procedural Generation: Programming The Universe
	"Here we go again! Year 4 begins now..." - javidx9

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

	Relevant Video: https://youtu.be/ZZY9YE7rZJw

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

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018, 2019, 2020
*/



#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <random>

constexpr uint32_t g_starColours[8] = 
{
	0xFFFFFFFF, 0xFFD9FFFF, 0xFFA3FFFF, 0xFFFFC8C8,
	0xFFFFCB9D, 0xFF9F9FFF, 0xFF415EFF, 0xFF28199D
};


struct sPlanet
{
	double distance = 0.0;
	double diameter = 0.0;
	double foliage = 0.0;
	double minerals = 0.0;
	double water = 0.0;
	double gases = 0.0;
	double temperature = 0.0;
	double population = 0.0;
	bool ring = false;
	std::vector<double> vMoons;
};

class cStarSystem
{
public:
	cStarSystem(uint32_t x, uint32_t y, bool bGenerateFullSystem = false)
	{
		// Set seed based on location of star system
		nProcGen = (x & 0xFFFF) << 16 | (y & 0xFFFF);

		// Not all locations contain a system
		starExists = (rndInt(0, 20) == 1);
		if (!starExists) return;

		// Generate Star
		starDiameter = rndDouble(10.0, 40.0);
		starColour.n = g_starColours[rndInt(0, 8)];

		// When viewing the galaxy map, we only care about the star
		// so abort early
		if (!bGenerateFullSystem) return;

		// If we are viewing the system map, we need to generate the
		// full system
		
		// Generate Planets
		double dDistanceFromStar = rndDouble(60.0, 200.0);
		int nPlanets = rndInt(0, 10);
		for (int i = 0; i < nPlanets; i++)
		{
			sPlanet p;
			p.distance = dDistanceFromStar;
			dDistanceFromStar += rndDouble(20.0, 200.0);
			p.diameter = rndDouble(4.0, 20.0);

			// Could make temeprature a function of distance from star
			p.temperature = rndDouble(-200.0, 300.0);

			// Composition of planet
			p.foliage = rndDouble(0.0, 1.0);
			p.minerals = rndDouble(0.0, 1.0);
			p.gases = rndDouble(0.0, 1.0);
			p.water = rndDouble(0.0, 1.0);

			// Normalise to 100%
			double dSum = 1.0 / (p.foliage + p.minerals + p.gases + p.water);
			p.foliage *= dSum;
			p.minerals *= dSum;
			p.gases *= dSum;
			p.water *= dSum;

			// Population could be a function of other habitat encouraging
			// properties, such as temperature and water
			p.population = std::max(rndInt(-5000000, 20000000), 0);

			// 10% of planets have a ring
			p.ring = rndInt(0, 10) == 1;

			// Satellites (Moons)
			int nMoons = std::max(rndInt(-5, 5), 0);
			for (int n = 0; n < nMoons; n++)
			{
				// A moon is just a diameter for now, but it could be
				// whatever you want!
				p.vMoons.push_back(rndDouble(1.0, 5.0));
			}

			// Add planet to vector
			vPlanets.push_back(p);
		}		
	}

	~cStarSystem()
	{

	}

public:
	std::vector<sPlanet> vPlanets;

public:
	bool		starExists = false;
	double		starDiameter = 0.0f;
	olc::Pixel	starColour = olc::WHITE;

private:
	uint32_t nProcGen = 0;

	double rndDouble(double min, double max)
	{
		return ((double)rnd() / (double)(0x7FFFFFFF)) * (max - min) + min;
	}

	int rndInt(int min, int max)
	{
		return (rnd() % (max - min)) + min;
	}
	
	// Modified from this for 64-bit systems:
	// https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/
	// Now I found the link again - Also, check out his blog, it's a fantastic resource!
	uint32_t rnd()
	{
		nProcGen += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)nProcGen * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;
	}
};

class olcGalaxy : public olc::PixelGameEngine
{
public:
	olcGalaxy()
	{
		sAppName = "olcGalaxy";
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	olc::vf2d vGalaxyOffset = { 0,0 };
	bool bStarSelected = false;
	uint32_t nSelectedStarSeed1 = 0;
	uint32_t nSelectedStarSeed2 = 0;


	/*uint32_t nLehmer = 0;
	uint32_t Lehmer32()
	{
		nLehmer += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)nLehmer * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;
	}*/

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (fElapsedTime <= 0.0001f) return true;
		Clear(olc::BLACK);

		//if (GetKey(olc::SPACE).bReleased)
		//{

		//	//srand(1000);

		//	std::random_device rd;
		//	std::mt19937 mt(1000);
		//	std::uniform_int_distribution<int> dist(0, 256);

		//	auto tp1 = std::chrono::system_clock::now();
		//	// Ranomness Tests
		//	for (int x = 0; x < ScreenWidth(); x++)
		//	{
		//		for (int y = 0; y < ScreenHeight(); y++)
		//		{
		//			bool bIsStar = false;
		//			int nSeed = y << 16 | x;
		//			
		//			// Standard C++ rand()
		//			//srand(nSeed);
		//			//bIsStar = rand() % 256 < 32;

		//			// std::random
		//			//mt.seed(nSeed);
		//			//bIsStar = dist(mt) < 32;

		//			// Lehmer32
		//			nLehmer = nSeed;
		//			bIsStar = Lehmer32() % 256 < 32;

		//			Draw(x, y, bIsStar ? olc::WHITE : olc::BLACK);
		//		}
		//	}
		//	auto tp2 = std::chrono::system_clock::now();
		//	std::chrono::duration<float> elapsedTime = tp2 - tp1;			
		//	DrawString(3, 3, "Time: " + std::to_string(elapsedTime.count()), olc::RED, 2);
		//}


		//return true;


		if (GetKey(olc::W).bHeld) vGalaxyOffset.y -= 50.0f * fElapsedTime;
		if (GetKey(olc::S).bHeld) vGalaxyOffset.y += 50.0f * fElapsedTime;
		if (GetKey(olc::A).bHeld) vGalaxyOffset.x -= 50.0f * fElapsedTime;
		if (GetKey(olc::D).bHeld) vGalaxyOffset.x += 50.0f * fElapsedTime;

		int nSectorsX = ScreenWidth() / 16;
		int nSectorsY = ScreenHeight() / 16;

		olc::vi2d mouse = { GetMouseX() / 16, GetMouseY() / 16 };
		olc::vi2d galaxy_mouse = mouse + vGalaxyOffset;
		olc::vi2d screen_sector = { 0,0 };

		for (screen_sector.x = 0; screen_sector.x < nSectorsX; screen_sector.x++)
			for (screen_sector.y = 0; screen_sector.y < nSectorsY; screen_sector.y++)
			{
				uint32_t seed1 = (uint32_t)vGalaxyOffset.x + (uint32_t)screen_sector.x;
				uint32_t seed2 = (uint32_t)vGalaxyOffset.y + (uint32_t)screen_sector.y;

				cStarSystem star(seed1, seed2);
				if (star.starExists)
				{
					FillCircle(screen_sector.x * 16 + 8, screen_sector.y * 16 + 8, 
						(int)star.starDiameter / 8, star.starColour);

					// For convenience highlight hovered star
					if (mouse.x == screen_sector.x && mouse.y == screen_sector.y)
					{
						DrawCircle(screen_sector.x * 16 + 8, screen_sector.y * 16 + 8, 12, olc::YELLOW);
					}
				}
			}

		// Handle Mouse Click
		if (GetMouse(0).bPressed)
		{
			uint32_t seed1 = (uint32_t)vGalaxyOffset.x + (uint32_t)mouse.x;
			uint32_t seed2 = (uint32_t)vGalaxyOffset.y + (uint32_t)mouse.y;

			cStarSystem star(seed1, seed2);
			if (star.starExists)
			{
				bStarSelected = true;
				nSelectedStarSeed1 = seed1;
				nSelectedStarSeed2 = seed2;
			}
			else
				bStarSelected = false;
		}

		// Draw Details of selected star system
		if (bStarSelected)
		{
			// Generate full star system
			cStarSystem star(nSelectedStarSeed1, nSelectedStarSeed2, true);

			// Draw Window
			FillRect(8, 240, 496, 232, olc::DARK_BLUE);
			DrawRect(8, 240, 496, 232, olc::WHITE);

			// Draw Star
			olc::vi2d vBody = { 14, 356 };

			vBody.x += star.starDiameter * 1.375;
			FillCircle(vBody, (int)(star.starDiameter * 1.375), star.starColour);			 
			vBody.x += (star.starDiameter * 1.375) + 8;

			

			// Draw Planets
			for (auto& planet : star.vPlanets)
			{
				if (vBody.x + planet.diameter >= 496) break;

				vBody.x += planet.diameter;
				FillCircle(vBody, (int)(planet.diameter * 1.0), olc::RED);

				olc::vi2d vMoon = vBody;
				vMoon.y += planet.diameter + 10;

				// Draw Moons
				for (auto& moon : planet.vMoons)
				{
					vMoon.y += moon;
					FillCircle(vMoon, (int)(moon * 1.0), olc::GREY);
					vMoon.y += moon + 10;
				}

				vBody.x += planet.diameter + 8;
			}
		}

		return true;
	}
};

int main()
{
	olcGalaxy demo;
	if (demo.Construct(512, 480, 2, 2, false, false))
		demo.Start();
	return 0;
}
