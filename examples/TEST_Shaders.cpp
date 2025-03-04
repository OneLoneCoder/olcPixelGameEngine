/*
	Example file for olcPGEX_Shaders.h

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
	David Barr, aka javidx9 
	©OneLoneCoder 2019, 2020, 2021, 2022, 2022, 2023, 2024, 2025

*/

#define OLC_PGE_APPLICATION
#define OLC_GFX_OPENGL33
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SHADERS
#include "extensions/olcPGEX_Shaders.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "extensions/olcPGEX_TransformedView.h"

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

	/*
	
	PLEASE NOTE! This is all an ill-thought through idea. I do not recommend you actually use this.
	
	*/

	// You need an instance of a shader object to apply pixel shading
	olc::Shade shader;

	// Effects are the pixel shaders. They can be custom, or selected from a built in library
	olc::Effect fxScanlines;
	olc::Effect fxNormal;
	olc::Effect fxBoxblur;
	olc::Effect fxGreyscale;
	olc::Effect fxThreshold;
	olc::Effect fxSobel;

	// Pixel shading is all done in Decal land, so for this demo we render to several decals with
	// different shading, and composite the result
	olc::Renderable gfxNormalBouncers;
	olc::Renderable gfxShadedBouncers;
	olc::Renderable gfxTexture1;
	olc::Renderable gfxTexture2;

	olc::Renderable gfxEffect1;
	olc::Renderable gfxEffect2;
	olc::Renderable gfxEffect3;
	olc::Renderable gfxEffect4;
	olc::Renderable gfxEffect5;


	size_t nBouncers = 50;
	std::vector<std::pair<olc::vf2d, olc::vf2d>> vBouncers;

	olc::TransformedView tv;


public:
	bool OnUserCreate() override
	{
		// Transformed view is for panning and zooming
		tv.Initialise({ ScreenWidth(), ScreenHeight() }, { 1.0f, 1.0f });

		// These are decals, but created manually rather than from a file
		gfxNormalBouncers.Create(ScreenWidth(), ScreenHeight());
		gfxShadedBouncers.Create(ScreenWidth(), ScreenHeight());
		gfxEffect1.Create(200, 200);
		gfxEffect2.Create(200, 200);
		gfxEffect3.Create(200, 200);
		gfxEffect4.Create(200, 200);
		gfxEffect5.Create(200, 200);

		// This decal uses a graphic - you'll need your own
		gfxTexture2.Load("E:\\work\\repos\\OneLoneCoder\\olcProjects2\\DEMO_Quad3D\\assets\\Baby.png");
		
		// All of these effects "pixel shaders" are built-in. You can easily define your own EffectConfig
		fxScanlines = shader.MakeEffect(olc::fx::FX_SCANLINE);
		fxNormal = shader.MakeEffect(olc::fx::FX_NORMAL);
		fxBoxblur = shader.MakeEffect(olc::fx::FX_BOXBLUR);
		fxGreyscale = shader.MakeEffect(olc::fx::FX_GREYSCALE);
		fxThreshold = shader.MakeEffect(olc::fx::FX_THRESHOLD);
		fxSobel = shader.MakeEffect(olc::fx::FX_SOBEL);

		



		vBouncers.resize(nBouncers);
		for (size_t i = 0; i < nBouncers; i++)
		{
			float a = (float(rand()) / float(RAND_MAX)) * 2.0f * 3.14159f;

			vBouncers[i] =
				std::make_pair<olc::vf2d, olc::vf2d>(
					{ float(rand() % (ScreenWidth() - gfxTexture2.Sprite()->width)) + 100, float(rand() % (ScreenHeight() - gfxTexture2.Sprite()->height)) + 100 },
					{ cos(a), sin(a) });
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Handle Pan & Zoom
		if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
		if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
		if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
		if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(2.0f, GetMousePos());
		if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.5f, GetMousePos());


		// Stage 1) Just draw the bouncers normally into the NormalBouncers decal
		// This uses the "pass-thru" effect, i.e. nothing

		// Set the target decal
		shader.SetTargetDecal(gfxNormalBouncers.Decal(), 0);
		// Tell the shading system to use a shader
		shader.Start(&fxNormal);
		// Several PGE-like drawing commands are available. Here we clear the decal
		shader.Clear();

		// Update Locations & Render "Stock" Bouncers
		for (auto& bouncer : vBouncers)
		{
			bouncer.first += bouncer.second * fElapsedTime * 100.0f;

			if (bouncer.first.x < 0.0f)
			{
				bouncer.first.x = 0.0f; bouncer.second.x *= -1.0f;
			}
			if (bouncer.first.x + gfxTexture2.Sprite()->width > ScreenWidth())
			{
				bouncer.first.x = float(ScreenWidth() - gfxTexture2.Sprite()->width); bouncer.second.x *= -1.0f;
			}

			if (bouncer.first.y < 0.0f)
			{
				bouncer.first.y = 0.0f; bouncer.second.y *= -1.0f;
			}
			if (bouncer.first.y + gfxTexture2.Sprite()->height > ScreenHeight())
			{
				bouncer.first.y = float(ScreenHeight() - gfxTexture2.Sprite()->height); bouncer.second.y *= -1.0f;
			}

			// Using the transformed view, draw the bouncer using the pass-thru shader
			tv.DrawDecal(shader, bouncer.first, gfxTexture2.Decal(), { 1.0f, 1.0f }, olc::WHITE);
		}

		// Stop shading
		shader.End();

		// Stage 2) A selection of different effects are demonstrated. For each one, we "cut" out of normal bouncers
		// a small region, then draw it into its own decal just for that effect. The cut is done with DrawPartialDecal()

		shader.SetTargetDecal(gfxEffect1.Decal(), 0);
		shader.Start(&fxBoxblur);
		shader.Clear(olc::WHITE);
		shader.DrawPartialDecal({ 0.0f, 0.0f }, gfxNormalBouncers.Decal(), { 50.0f, 50.0f }, { 200.0f, 200.0f });
		shader.End();

		shader.SetTargetDecal(gfxEffect2.Decal(), 0);
		shader.Start(&fxScanlines);
		shader.Clear(olc::WHITE);
		shader.DrawPartialDecal({ 0.0f, 0.0f }, gfxNormalBouncers.Decal(), { 300.0f, 50.0f }, { 200.0f, 200.0f });
		shader.End();

		shader.SetTargetDecal(gfxEffect3.Decal(), 0);
		shader.Start(&fxGreyscale);
		shader.Clear(olc::WHITE);
		shader.DrawPartialDecal({ 0.0f, 0.0f }, gfxNormalBouncers.Decal(), { 550.0f, 50.0f }, { 200.0f, 200.0f });
		shader.End();

		shader.SetTargetDecal(gfxEffect4.Decal(), 0);
		shader.Start(&fxThreshold);
		shader.Clear(olc::WHITE);
		shader.DrawPartialDecal({ 0.0f, 0.0f }, gfxNormalBouncers.Decal(), { 800.0f, 50.0f }, { 200.0f, 200.0f });
		shader.End();

		shader.SetTargetDecal(gfxEffect5.Decal(), 0);
		shader.Start(&fxSobel);
		shader.Clear(olc::WHITE);
		shader.DrawPartialDecal({ 0.0f, 0.0f }, gfxNormalBouncers.Decal(), { 1050.0f, 50.0f }, { 200.0f, 200.0f });
		shader.End();

		// Stage 3) Composite final display - NOT USING SHADERS NOW

		Clear(olc::WHITE);
		DrawDecal({ 0,0 }, gfxNormalBouncers.Decal());

		FillRectDecal({ 48.0f, 48.0f }, { 204.0f, 204.0f }, olc::BLUE);
		DrawDecal({ 50.0f, 50.0f }, gfxEffect1.Decal());

		FillRectDecal({ 298.0f, 48.0f }, { 204.0f, 204.0f }, olc::BLUE);
		DrawDecal({ 300.0f, 50.0f }, gfxEffect2.Decal());

		FillRectDecal({ 548.0f, 48.0f }, { 204.0f, 204.0f }, olc::BLUE);
		DrawDecal({ 550.0f, 50.0f }, gfxEffect3.Decal());

		FillRectDecal({ 798.0f, 48.0f }, { 204.0f, 204.0f }, olc::BLUE);
		DrawDecal({ 800.0f, 50.0f }, gfxEffect4.Decal());

		FillRectDecal({ 1048.0f, 48.0f }, { 204.0f, 204.0f }, olc::BLUE);
		DrawDecal({ 1050.0f, 50.0f }, gfxEffect5.Decal());


		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(1280, 720, 1, 1, false, false))
		demo.Start();
	return 0;
}