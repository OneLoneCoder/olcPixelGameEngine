/*
	8-Bits Of Image Processing You Should Know
	"Colin, at least you'll always get 700s now..." - javidx9

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

	Instructions:
	~~~~~~~~~~~~~
	Choose algorithm 1-8, instructions on screen


	Relevant Video: https://youtu.be/mRM5Js3VLCk

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
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "escapi.h"

int nFrameWidth = 320;
int nFrameHeight = 240;

struct frame
{
	float *pixels = nullptr;

	frame()
	{
		pixels = new float[nFrameWidth * nFrameHeight];
	}

	~frame()
	{
		delete[] pixels;
	}


	float get(int x, int y)
	{
		if (x >= 0 && x < nFrameWidth && y >= 0 && y < nFrameHeight)
		{
			return pixels[y*nFrameWidth + x];
		}
		else
			return 0.0f;
	}

	void set(int x, int y, float p)
	{
		if (x >= 0 && x < nFrameWidth && y >= 0 && y < nFrameHeight)
		{
			pixels[y*nFrameWidth + x] = p;
		}
	}


	void operator=(const frame &f)
	{
		memcpy(this->pixels, f.pixels, nFrameWidth * nFrameHeight * sizeof(float));
	}
};



class WIP_ImageProcessing : public olc::PixelGameEngine
{
public:
	WIP_ImageProcessing()
	{
		sAppName = "WIP_ImageProcessing";
	}

	union RGBint
	{
		int rgb;
		unsigned char c[4];
	};

	int nCameras = 0;
	SimpleCapParams capture;

public:
	bool OnUserCreate() override
	{
		// Initialise webcam to screen dimensions
		nCameras = setupESCAPI();
		if (nCameras == 0)	return false;
		capture.mWidth = nFrameWidth;
		capture.mHeight = nFrameHeight;
		capture.mTargetBuf = new int[nFrameWidth * nFrameHeight];
		if (initCapture(0, &capture) == 0)	return false;		
		return true;
	}

	void DrawFrame(frame &f, int x, int y)
	{
		for (int i = 0; i < nFrameWidth; i++)
			for (int j = 0; j < nFrameHeight; j++)
			{
				int c = (int)std::min(std::max(0.0f, f.pixels[j*nFrameWidth + i] * 255.0f), 255.0f);				
				Draw(x + i, y + j, olc::Pixel(c, c, c));
			}
	}

	enum ALGORITHM
	{
		THRESHOLD, MOTION, LOWPASS,	CONVOLUTION,
		SOBEL, MORPHO, MEDIAN, ADAPTIVE,
	};

	enum MORPHOP
	{
		DILATION,
		EROSION,
		EDGE
	};

	frame input, output, prev_input, activity, threshold;

	// Algorithm Currently Running
	ALGORITHM algo = THRESHOLD;
	MORPHOP morph = DILATION;
	int nMorphCount = 1;

	float fThresholdValue = 0.5f;
	float fLowPassRC = 0.1f;
	float fAdaptiveBias = 1.1f;

	float *pConvoKernel = kernel_blur;

	float kernel_blur[9] =
	{
		0.0f,   0.125,  0.0f,
		0.125f, 0.5f,   0.125f,
		0.0f,   0.125f, 0.0f,
	};

	float kernel_sharpen[9] =
	{
		0.0f,  -1.0f,  0.0f,
		-1.0f,  5.0f, -1.0f,
		0.0f,  -1.0f,  0.0f,
	};

	float kernel_sobel_v[9] =
	{
		-1.0f, 0.0f, +1.0f,
		-2.0f, 0.0f, +2.0f,
		-1.0f, 0.0f, +1.0f,
	};

	float kernel_sobel_h[9] =
	{
		-1.0f, -2.0f, -1.0f,
		 0.0f, 0.0f, 0.0f,
		+1.0f, +2.0f, +1.0f,
	};

	bool OnUserUpdate(float fElapsedTime) override
	{
		// CAPTURING WEBCAM IMAGE
		prev_input = input;
		doCapture(0); while (isCaptureDone(0) == 0) {}
		for (int y = 0; y < capture.mHeight; y++)
			for (int x = 0; x < capture.mWidth; x++)
			{
				RGBint col;
				int id = y * capture.mWidth + x;
				col.rgb = capture.mTargetBuf[id];
				input.pixels[y*nFrameWidth + x] = (float)col.c[1] / 255.0f;
			}

		if (GetKey(olc::Key::K1).bReleased) algo = THRESHOLD;
		if (GetKey(olc::Key::K2).bReleased) algo = MOTION;
		if (GetKey(olc::Key::K3).bReleased) algo = LOWPASS;
		if (GetKey(olc::Key::K4).bReleased) algo = CONVOLUTION;
		if (GetKey(olc::Key::K5).bReleased) algo = SOBEL;
		if (GetKey(olc::Key::K6).bReleased) algo = MORPHO;
		if (GetKey(olc::Key::K7).bReleased) algo = MEDIAN;
		if (GetKey(olc::Key::K8).bReleased) algo = ADAPTIVE;


		switch (algo)
		{
		case THRESHOLD:

			// Respond to user input
			if (GetKey(olc::Key::Z).bHeld) fThresholdValue -= 0.1f * fElapsedTime;
			if (GetKey(olc::Key::X).bHeld) fThresholdValue += 0.1f * fElapsedTime;
			if (fThresholdValue > 1.0f) fThresholdValue = 1.0f;
			if (fThresholdValue < 0.0f) fThresholdValue = 0.0f;

			// Perform threshold per pixel
			for (int i = 0; i < nFrameWidth; i++)
				for (int j = 0; j < nFrameHeight; j++)
					output.set(i, j, input.get(i, j) >= fThresholdValue ? 1.0f : 0.0f);
			break;

		case MOTION:

			// Returns the absolute difference between successive frames per pixel
			for (int i = 0; i < nFrameWidth; i++)
				for (int j = 0; j < nFrameHeight; j++)
					output.set(i, j, fabs(input.get(i, j) - prev_input.get(i, j)));
			break;


		case LOWPASS:

			// Respond to user input
			if (GetKey(olc::Key::Z).bHeld) fLowPassRC -= 0.1f * fElapsedTime;
			if (GetKey(olc::Key::X).bHeld) fLowPassRC += 0.1f * fElapsedTime;
			if (fLowPassRC > 1.0f) fLowPassRC = 1.0f;
			if (fLowPassRC < 0.0f) fLowPassRC = 0.0f;

			// Pass each pixel through a temporal RC filter
			for (int i = 0; i < nFrameWidth; i++)
				for (int j = 0; j < nFrameHeight; j++)
				{
					float dPixel = input.get(i, j) - output.get(i, j);
					dPixel *= fLowPassRC;
					output.set(i, j, dPixel + output.get(i, j));
				}
			break;

		case CONVOLUTION:
			// Respond to user input
			if (GetKey(olc::Key::Z).bHeld) pConvoKernel = kernel_blur;
			if (GetKey(olc::Key::X).bHeld) pConvoKernel = kernel_sharpen;

			for (int i = 0; i < nFrameWidth; i++)
				for (int j = 0; j < nFrameHeight; j++)
				{
					float fSum = 0.0f;
					for (int n = -1; n < +2; n++)
						for (int m = -1; m < +2; m++)
							fSum += input.get(i + n, j + m) * pConvoKernel[(m + 1) * 3 + (n + 1)];

					output.set(i, j, fSum);
				}
			break;

		case SOBEL:
			for (int i = 0; i < nFrameWidth; i++)
				for (int j = 0; j < nFrameHeight; j++)
				{
					float fKernelSumH = 0.0f;
					float fKernelSumV = 0.0f;

					for (int n = -1; n < +2; n++)
						for (int m = -1; m < +2; m++)
						{
							fKernelSumH += input.get(i + n, j + m) * kernel_sobel_h[(m + 1) * 3 + (n + 1)];
							fKernelSumV += input.get(i + n, j + m) * kernel_sobel_v[(m + 1) * 3 + (n + 1)];
						}

					output.set(i, j, fabs((fKernelSumH + fKernelSumV) / 2.0f));
				}
			break;

		case MORPHO:

			// Respond to user input
			if (GetKey(olc::Key::Z).bHeld) morph = DILATION;
			if (GetKey(olc::Key::X).bHeld) morph = EROSION;
			if (GetKey(olc::Key::C).bHeld) morph = EDGE;

			if (GetKey(olc::Key::A).bReleased) nMorphCount--;
			if (GetKey(olc::Key::S).bReleased) nMorphCount++;
			if (nMorphCount > 10.0f) nMorphCount = 10.0f;
			if (nMorphCount < 1.0f) nMorphCount = 1.0f;

			// Threshold First to binarise image
			for (int i = 0; i < nFrameWidth; i++)
				for (int j = 0; j < nFrameHeight; j++)
				{
					activity.set(i, j, input.get(i, j) > fThresholdValue ? 1.0f : 0.0f);
				}

			threshold = activity;

			switch (morph)
			{
			case DILATION:
				for (int n = 0; n < nMorphCount; n++)
				{
					output = activity;

					for (int i = 0; i < nFrameWidth; i++)
						for (int j = 0; j < nFrameHeight; j++)
						{
							if (activity.get(i, j) == 1.0f)
							{
								output.set(i, j, 1.0f);
								output.set(i - 1, j, 1.0f);
								output.set(i + 1, j, 1.0f);
								output.set(i, j - 1, 1.0f);
								output.set(i, j + 1, 1.0f);
								output.set(i - 1, j - 1, 1.0f);
								output.set(i + 1, j + 1, 1.0f);
								output.set(i + 1, j - 1, 1.0f);
								output.set(i - 1, j + 1, 1.0f);
							}
						}

					activity = output;
				}
				break;

			case EROSION:
				for (int n = 0; n < nMorphCount; n++)
				{
					output = activity;
					for (int i = 0; i < nFrameWidth; i++)
						for (int j = 0; j < nFrameHeight; j++)
						{

							float sum = activity.get(i - 1, j) + activity.get(i + 1, j) + activity.get(i, j - 1) + activity.get(i, j + 1) +
								activity.get(i - 1, j - 1) + activity.get(i + 1, j + 1) + activity.get(i + 1, j - 1) + activity.get(i - 1, j + 1);

							if (activity.get(i, j) == 1.0f && sum <  8.0f)
							{
								output.set(i, j, 0.0f);
							}
						}
					activity = output;
				}
				break;

			case EDGE:
				output = activity;
				for (int i = 0; i < nFrameWidth; i++)
					for (int j = 0; j < nFrameHeight; j++)
					{

						float sum = activity.get(i - 1, j) + activity.get(i + 1, j) + activity.get(i, j - 1) + activity.get(i, j + 1) +
							activity.get(i - 1, j - 1) + activity.get(i + 1, j + 1) + activity.get(i + 1, j - 1) + activity.get(i - 1, j + 1);

						if (activity.get(i, j) == 1.0f && sum == 8.0f)
						{
							output.set(i, j, 0.0f);
						}
					}
				break;

			}
			break;

		case MEDIAN:
			for (int i = 0; i < nFrameWidth; i++)
				for (int j = 0; j < nFrameHeight; j++)
				{
					std::vector<float> v;

					for (int n = -2; n < +3; n++)
						for (int m = -2; m < +3; m++)
							v.push_back(input.get(i + n, j + m));

					std::sort(v.begin(), v.end(), std::greater<float>());
					output.set(i, j, v[12]);
				}
			break;

		case ADAPTIVE:
			// Respond to user input
			if (GetKey(olc::Key::Z).bHeld) fAdaptiveBias -= 0.1f * fElapsedTime;
			if (GetKey(olc::Key::X).bHeld) fAdaptiveBias += 0.1f * fElapsedTime;
			if (fAdaptiveBias > 1.5f) fAdaptiveBias = 1.5f;
			if (fAdaptiveBias < 0.5f) fAdaptiveBias = 0.5f;


			for (int i = 0; i < nFrameWidth; i++)
				for (int j = 0; j < nFrameHeight; j++)
				{
					float fRegionSum = 0.0f;

					for (int n = -2; n < +3; n++)
						for (int m = -2; m < +3; m++)
							fRegionSum += input.get(i + n, j + m);

					fRegionSum /= 25.0f;				
					output.set(i, j, input.get(i, j) > (fRegionSum * fAdaptiveBias) ? 1.0f : 0.0f);
				}
			break;
		}
		
		// DRAW STUFF ONLY HERE
		Clear(olc::DARK_BLUE);
		DrawFrame(algo == MORPHO ? threshold : input, 10, 10);
		DrawFrame(output, 340, 10);

		DrawString(150, 255, "INPUT");
		DrawString(480, 255, "OUTPUT");

		DrawString(10, 275, "1) Threshold");
		DrawString(10, 285, "2) Absolute Motion");
		DrawString(10, 295, "3) Low-Pass Temporal Filtering");
		DrawString(10, 305, "4) Convolution (Blurring/Sharpening)");
		DrawString(10, 315, "5) Sobel Edge Detection");
		DrawString(10, 325, "6) Binary Morphological Operations (Erosion/Dilation)");
		DrawString(10, 335, "7) Median Filter");
		DrawString(10, 345, "8) Adaptive Threshold");


		switch (algo)
		{
		case THRESHOLD:
			DrawString(10, 375, "Change threshold value with Z and X keys");
			DrawString(10, 385, "Current value = " + std::to_string(fThresholdValue));
			break;

		case LOWPASS:
			DrawString(10, 375, "Change RC constant value with Z and X keys");
			DrawString(10, 385, "Current value = " + std::to_string(fLowPassRC));
			break;

		case CONVOLUTION:
			DrawString(10, 375, "Change convolution kernel with Z and X keys");
			DrawString(10, 385, "Current kernel = " + std::string((pConvoKernel == kernel_blur) ? "Blur" : "Sharpen"));
			break;

		case MORPHO:
			DrawString(10, 375, "Change operation with Z and X and C keys");
			if (morph == DILATION) DrawString(10, 385, "Current operation = DILATION");
			if (morph == EROSION) DrawString(10, 385, "Current operation = EROSION");
			if (morph == EDGE) DrawString(10, 385, "Current operation = EDGE");
			DrawString(10, 395, "Change Iterations with A and S keys");
			DrawString(10, 405, "Current iteration count = " + std::to_string(nMorphCount));


			break;

		case ADAPTIVE:
			DrawString(10, 375, "Change adaptive threshold bias with Z and X keys");
			DrawString(10, 385, "Current value = " + std::to_string(fAdaptiveBias));
			break;

		default:
			break;
		}
	
		if (GetKey(olc::Key::ESCAPE).bPressed) return false;
		return true;
	}
};


int main()
{
	WIP_ImageProcessing demo;
	if (demo.Construct(670, 460, 2, 2))
		demo.Start();

	return 0;
}


