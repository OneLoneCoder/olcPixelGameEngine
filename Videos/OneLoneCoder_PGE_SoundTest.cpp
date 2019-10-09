/*
	Simple example code for olcPGEX_Sound.h - Mind your speakers!

	You will need SampleA.wav, SampleB.wav and SampleC.wav for this demo.

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 OneLoneCoder.com

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
	Patreon:	https://www.patreon.com/javidx9

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

#include <list>

class SoundTest : public olc::PixelGameEngine
{
public:
	SoundTest()
	{
		sAppName = "Sound Test";
	}

private:
	int sndSampleA;
	int sndSampleB;
	int sndSampleC;
	bool bToggle = false;
	static bool bSynthPlaying;
	static float fSynthFrequency;
	static float fFilterVolume;

	const olc::Key keys[12] = { olc::Key::Z, olc::Key::S, olc::Key::X, olc::Key::D, olc::Key::C,
				olc::Key::V, olc::Key::G, olc::Key::B, olc::Key::H, olc::Key::N, olc::Key::J, olc::Key::M};

	static float fPreviousSamples[128];
	static int nSamplePos;
	

private:

	// This is an optional function that allows the user to generate or synthesize sounds
	// in a custom way, it is fed into the output mixer bu the extension
	static float MyCustomSynthFunction(int nChannel, float fGlobalTime, float fTimeStep)
	{
		// Just generate a sine wave of the appropriate frequency
		if (bSynthPlaying)
			return sin(fSynthFrequency * 2.0f * 3.14159f * fGlobalTime);
		else
			return 0.0f;
	}

	// This is an optional function that allows the user to filter the output from 
	// the internal mixer of the extension. Here you could add effects or just
	// control volume. I also like to use it to extract information about
	// the currently playing output waveform
	static float MyCustomFilterFunction(int nChannel, float fGlobalTime, float fSample)
	{
		// Fundamentally just control volume
		float fOutput = fSample * fFilterVolume;

		// But also add sample to list of previous samples for visualisation
		fPreviousSamples[nSamplePos] = fOutput;
		nSamplePos++;
		nSamplePos %= 128;

		return fOutput;
	}


	bool OnUserCreate()
	{
		olc::SOUND::InitialiseAudio(44100, 1, 8, 512);
		
		sndSampleA = olc::SOUND::LoadAudioSample("SampleA.wav");
		sndSampleB = olc::SOUND::LoadAudioSample("SampleB.wav");
		sndSampleC = olc::SOUND::LoadAudioSample("SampleC.wav");

		// Give the sound engine a hook to a custom generation function
		olc::SOUND::SetUserSynthFunction(MyCustomSynthFunction);

		// Give the sound engine a hook to a custom filtering function
		olc::SOUND::SetUserFilterFunction(MyCustomFilterFunction);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		//olc::SOUND::PlaySample(sndTest);

		auto PointInRect = [&](int x, int y, int rx, int ry, int rw, int rh)
		{
			return x >= rx && x < (rx + rw) && y >= ry && y < (ry + rh);
		};

		int nMouseX = GetMouseX();
		int nMouseY = GetMouseY();

		if(GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 16, 16, 128, 24))
			olc::SOUND::PlaySample(sndSampleA); // Plays the sample once

		if (GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 16, 48, 128, 24))
			olc::SOUND::PlaySample(sndSampleB);

		if (GetMouse(0).bPressed && PointInRect(nMouseX, nMouseY, 16, 80, 128, 24))
		{
			bToggle = !bToggle;
			if (bToggle)
			{
				olc::SOUND::PlaySample(sndSampleC, true); // Plays the sample in looping mode
			}
			else
			{
				olc::SOUND::StopSample(sndSampleC);
			}
		}

		if (GetMouse(0).bHeld && PointInRect(nMouseX, nMouseY, 160, 16, 90, 24))
			fFilterVolume += 2.0f * fElapsedTime;

		if (GetMouse(0).bHeld && PointInRect(nMouseX, nMouseY, 160, 48, 90, 24))
			fFilterVolume -= 2.0f * fElapsedTime;

		if (fFilterVolume < 0.0f) fFilterVolume = 0.0f;
		if (fFilterVolume > 1.0f) fFilterVolume = 1.0f;

		// Detect keyboard - very simple synthesizer
		if (IsFocused())
		{
			bool bKeyIsPressed = false;
			float fFrequency = 0.0f;
			for (int i = 0; i < 12; i++)
			{
				if (GetKey(keys[i]).bHeld)
				{
					bKeyIsPressed = true;
					float fOctaveBaseFrequency = 220.0f; 
					float f12thRootOf2 = pow(2.0f, 1.0f / 12.0f);
					fFrequency = fOctaveBaseFrequency * powf(f12thRootOf2, (float)i);
				}
			}

			fSynthFrequency = fFrequency;
			bSynthPlaying = bKeyIsPressed;
		}
		

		// Draw Buttons
		Clear(olc::BLUE);

		DrawRect(16, 16, 128, 24);
		DrawString(20, 20, "Play Sample A");

		DrawRect(16, 48, 128, 24);
		DrawString(20, 52, "Play Sample B");

		DrawRect(16, 80, 128, 24);
		DrawString(20, 84, (bToggle ? "Stop Sample C" : "Loop Sample C"));


		DrawRect(160, 16, 90, 24);
		DrawString(164, 20, "Volume +");

		DrawRect(160, 48, 90, 24);
		DrawString(164, 52, "Volume -");


		DrawString(164, 80, "Volume: " + std::to_string((int)(fFilterVolume * 10.0f)));

		// Draw Keyboard

		// White Keys
		for (int i = 0; i < 7; i++)
		{
			FillRect(i * 16 + 8, 160, 16, 64);
			DrawRect(i * 16 + 8, 160, 16, 64, olc::BLACK);
			DrawString(i * 16 + 12, 212, std::string(1, "ZXCVBNM"[i]), olc::BLACK);
		}

		// Black Keys
		for (int i = 0; i < 6; i++)
		{
			if (i != 2)
			{
				FillRect(i * 16 + 18, 160, 12, 32, olc::BLACK);
				DrawString(i * 16 + 20, 180, std::string(1, "SDFGHJ"[i]), olc::WHITE);
			}
		}

		// Draw visualisation
		int nStartPos = (nSamplePos + 127) % 128;

		for (int i = 127; i >= 0; i--)
		{
			float fSample = fPreviousSamples[(nSamplePos + i) % 128];
			DrawLine(124 + i, 210, 124 + i, 210 + (int)(fSample * 20.0f), olc::RED);
		}


		return true;
	}


	// Note we must shut down the sound system too!!
	bool OnUserDestroy()
	{
		olc::SOUND::DestroyAudio();
		return true;
	}
};

bool SoundTest::bSynthPlaying = false;
float SoundTest::fSynthFrequency = 0.0f;
float SoundTest::fFilterVolume = 1.0f;
int SoundTest::nSamplePos = 0;
float SoundTest::fPreviousSamples[128];

int main()
{
	SoundTest demo;
	if(demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}