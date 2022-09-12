/*
	olcPGEX_SplashScreen.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                     Splash Screen v1.0                      |
	+-------------------------------------------------------------+

	NOTE: UNDER ACTIVE DEVELOPMENT - THERE ARE BUGS/GLITCHES

	What is this?
	~~~~~~~~~~~~~
	This extension produces an animated splashscreen and copyright notice
	at the beginning of a PGE application, for the purposes of remaining
	OLC-3 compliant should it be ambiguous in your deployment.

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

#include "olcPixelGameEngine.h"

namespace olc
{
	class SplashScreen : public olc::PGEX
	{
	public:
		SplashScreen();

	protected:
		virtual void OnAfterUserCreate() override;
		virtual bool OnBeforeUserUpdate(float& fElapsedTime) override;

	private:
		olc::Renderable spr;
		std::vector<std::pair<olc::vf2d, olc::vf2d>> vBoom;
		olc::vf2d vScale;
		olc::vf2d vPosition;
		float fParticleTime = 0.0f;
		float fAspect = 0.0f;
		bool bComplete = false;
	};


}

#ifdef OLC_PGEX_SPLASHSCREEN
#undef OLC_PGEX_SPLASHSCREEN

namespace olc
{
	SplashScreen::SplashScreen() : olc::PGEX(true)
	{		
	}

	void SplashScreen::OnAfterUserCreate()
	{
		const char logo[] =
			"000000000000000000000000000000000000000000000000000000000000000000005"
			"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED1EE"
			"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED5EEE"
			"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE@E@000"
			"0000000000000000000000000000000000000000000000000000000000001E1D:ZZZZ"
			"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ1D5BZZZZZZ"
			"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ5@E:P0002Z0"
			"02ZZX000000000000ZP0000000000000000000000000000ZX000Z002XE1DX?o`o:Poo"
			"800SooaE5@E1ED5BX?ol5E@E0E1ED?oo5@E1ED5DE1D5E@ZQEEBPEE2QD5BSooclZ?olQ"
			"AB?oo5DEEDEEDE:SooaEEAE5DEEDoolEADEEDEAE5AEEBZ5EE:5EE:5@E:?oo?bXoob55"
			"8o3lEAEEAD5ADZ?oo5@E5EEAD5Cl01E5AD5AE5DE5@E:X01DXEEDXE1DXo3lo:Sl0800S"
			"ooaE1ED5EE5BXo00EEDEEE5EE?oo5EE5EE5DEEDEEDZQEEBQD5BQD5BSl?cl0?`0ZZZ?o"
			"o5D5E@EEDE03loaEEAEEDEEDoolEED5EDEAEEAEEBZ5EE:5@E:5@E:?oo?oloob008o00"
			"EAEEAD01EE?co5EE5EEAD03l01DE@05AE5AE5@0:XE000EEDXE1DXooloocoo8DDSlZQE"
			"5EE5EE5EDoolE1DE4E5EE?oo5AE5EE5DE5DEEDZQEEAAEEBQD5BPoo3oo3olQAB?bZ5DE"
			"1D5EDEE@ooaD5AD1D5EDoolE1DEE@EAD5@EEBZ5EE51ED:5@E:P000000020080:X0000"
			"00000000000000000000000000000000000:X0000002XE1DZZZZZZZZZZZZZZZZZZZZZ"
			"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZQD5@ZZZZZZZZZZZZZZZZZZZZZZ"
			"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZX5@E@00000000000000000000000"
			"00000000000000000000000000000000000000001E1EEEEEEEEEEEEEEEEEEEEEEEEEE"
			"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED5EEEEEEEEEEEEEEEEEEEEEEEEEEE"
			"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE@5EEEEEEEEEEEEEEEEEEEEEEEEEEEE"
			"EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED0000000000000000000000000000000"
			"0000000000000000000000000000000000000";

		spr.Create(203, 24);
		int px = 0, py = 0;
		for (size_t b = 0; b < 1624; b += 4)
		{
			uint32_t sym1 = (uint32_t)logo[b + 0] - 48;
			uint32_t sym2 = (uint32_t)logo[b + 1] - 48;
			uint32_t sym3 = (uint32_t)logo[b + 2] - 48;
			uint32_t sym4 = (uint32_t)logo[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 12; i++)
			{
				olc::Pixel p = olc::RED;
				switch ((r & 0xC00000) >> 22)
				{
				case 0: p = olc::Pixel(0, 0, 0, 255);  break;
				case 1: p = olc::Pixel(255, 255, 255, 255);  break;
				case 2: p = olc::Pixel(255, 120, 26, 255);  break;
				case 3: p = olc::Pixel(79, 193, 255, 255);  break;
				}
				spr.Sprite()->SetPixel(px, py, p);
				if (++px == 203) { py++; px = 0; }
				r <<= 2;
			}
		}

		spr.Decal()->Update();
		vBoom.resize(spr.Sprite()->width * spr.Sprite()->height);
		vScale = { float(pge->ScreenWidth()) / 500.0f, float(pge->ScreenWidth()) / 500.0f };
		fAspect = float(pge->ScreenWidth()) / float(pge->ScreenHeight());
		vPosition = olc::vf2d(
			(250 - spr.Sprite()->width) / 2.0f,
			(250 - spr.Sprite()->height) / 2.0f / fAspect);
		for (int y = 0; y < spr.Sprite()->height; y++)
			for (int x = 0; x < spr.Sprite()->width; x++)
				vBoom[y * spr.Sprite()->width + x] = std::make_pair(
					vPosition + olc::vf2d(x, y),
					olc::vf2d(
						(float(rand()) / float(RAND_MAX)) * 10.0f - 5.0f,
						(float(rand()) / float(RAND_MAX)) * 10.0f - 5.0f)
				);
	}

	bool SplashScreen::OnBeforeUserUpdate(float& fElapsedTime)
	{
		if (bComplete) return false;

		fParticleTime += fElapsedTime;

		for (int y = 0; y < spr.Sprite()->height; y++)
			for (int x = 0; x < spr.Sprite()->width; x++)
			{


				if (fParticleTime < 1.0f)
				{

				}
				else if (fParticleTime < 2.0f)
				{
					vBoom[y * spr.Sprite()->width + x].first =
						olc::vf2d(
							(250 - spr.Sprite()->width) / 2.0f + float(x),
							(250 - spr.Sprite()->height) / 2.0f / fAspect + float(y)
						) +
						olc::vf2d(
							(float(rand()) / float(RAND_MAX)) * 0.5f - 0.25f,
							(float(rand()) / float(RAND_MAX)) * 0.5f - 0.25f);
				}
				else if(fParticleTime < 5.0f)
				{
					vBoom[y * spr.Sprite()->width + x].first += vBoom[y * spr.Sprite()->width + x].second * fElapsedTime * 20.0f;
				}
				else
				{
					bComplete = true;
				}

				pge->DrawPartialDecal(vScale * vBoom[y * spr.Sprite()->width + x].first * 2.0f, spr.Decal(), olc::vf2d(x, y), { 1, 1 }, vScale * 2.0f, olc::PixelF(1.0f, 1.0f, 1.0f, std::min(1.0f, std::max(4.0f - fParticleTime, 0.0f))));
			}

		olc::vi2d vSize = pge->GetTextSizeProp("Copyright OneLoneCoder.com 2022");
		pge->DrawStringPropDecal(olc::vf2d(float(pge->ScreenWidth()/2) - vSize.x/2, float(pge->ScreenHeight()) - vSize.y * 3.0f), "Copyright OneLoneCoder.com 2022", olc::PixelF(1.0f, 1.0f, 1.0f, 0.5f), olc::vf2d(1.0, 2.0f));
		return true;
	}

}

#endif