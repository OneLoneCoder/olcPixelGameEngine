/*
	Example file for Patch Experiments

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

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021, 2022, 2023, 2024, 2025

*/


#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>



class TEST_Patches : public olc::PixelGameEngine
{
public:
	TEST_Patches()
	{
		sAppName = "Testing Patches";
	}

	std::unique_ptr<olc::Sprite> pSprite;
	std::unique_ptr<olc::Decal> pDecal;

public:
	bool OnUserCreate() override
	{
		pSprite = std::make_unique<olc::Sprite>("./assets/DirectionalSolids.png");
		pDecal = std::make_unique<olc::Decal>(pSprite.get());
		return true;
	}


	bool OnUserUpdate(float fElapsedTime) override
	{

		Clear(olc::TANGERINE);
		DrawSprite({ 10,10 }, pSprite.get());
		
		/*DrawSprite(
			olc::vf2d(GetMousePos()), 
			pSprite->Patch({ 16, 0 }, { 16, 16 }), 
			{ -30.0f, -30.0f });
		
		DrawSprite(
			olc::vf2d(GetMousePos()), 
			pSprite->Patch({ 0.25f, 0.5f }, { 0.5f, 0.0f }, { 0.75f, 0.5f }, { 0.5f, 1.0f }),
			{ 100.0f, 100.0f });

		DrawSprite(
			olc::vf2d(GetMousePos()),
			*pSprite,
			{ 50.0f, -50.0f });*/



		DrawDecal(
			olc::vf2d(GetMousePos()),
			pDecal->Patch({ 16, 0 }, { 16, 16 }),
			{ -30.0f, -30.0f });

		DrawDecal(
			olc::vf2d(GetMousePos()),
			pDecal->Patch({ 0.25f, 0.5f }, { 0.5f, 0.0f }, { 0.75f, 0.5f }, { 0.5f, 1.0f }),
			{ 100.0f, 100.0f });

		DrawDecal(
			olc::vf2d(GetMousePos()),
			*pDecal,
			{ 320.0f, -180.0f });

		DrawLine({ 0,0 }, GetScreenSize(), olc::TANGERINE);

		return true;
	}
};

int main()
{
	TEST_Patches demo;
	if (demo.Construct(1280, 720, 1, 1, false, false))
		demo.Start();
	return 0;
}