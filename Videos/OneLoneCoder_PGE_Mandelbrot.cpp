/*
	Brute Force Processing a Mandelbrot Renderer
	"Dammit Moros & Saladin, you guys keep making tools, I'll have nothing left to video..." - javidx9

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

	Relevant Video: https://youtu.be/PBvLs88hvJ8

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

	Community Blog: https://community.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018, 2019, 2020
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <condition_variable>
#include <atomic>
#include <complex>
#include <cstdlib>
#include <immintrin.h>

constexpr int nMaxThreads = 32;

class olcFractalExplorer : public olc::PixelGameEngine
{
public:
	olcFractalExplorer()
	{
		sAppName = "Brute Force Processing";
	}

	int* pFractal = nullptr;
	int nMode = 4;
	int nIterations = 128;

public:
	bool OnUserCreate() override
	{
		//pFractal = new int[ScreenWidth() * ScreenHeight()]{ 0 };

		// Using Vector extensions, align memory (not as necessary as it used to be)
		// MS Specific - see std::aligned_alloc for others
		pFractal = (int*)_aligned_malloc(size_t(ScreenWidth()) * size_t(ScreenHeight()) * sizeof(int), 64);

		InitialiseThreadPool();
		return true;
	}

	bool OnUserDestroy() override
	{
		// Stop Worker threads
		for (int i = 0; i < nMaxThreads; i++)
		{
			workers[i].alive = false;		 // Allow thread exit
			workers[i].cvStart.notify_one(); // Fake starting gun
		}

		// Clean up worker threads
		for (int i = 0; i < nMaxThreads; i++)
			workers[i].thread.join();

		// Clean up memory
		_aligned_free(pFractal);
		return true;
	}

	// Method 1) - Super simple, no effort at optimising
	void CreateFractalBasic(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
	{
		double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
		double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));
		
		for (int y = pix_tl.y; y < pix_br.y; y++)
		{
			for (int x = pix_tl.x; x < pix_br.x; x++)
			{
				std::complex<double> c(x * x_scale + frac_tl.x, y * y_scale + frac_tl.y);
				std::complex<double> z(0, 0);

				int n = 0;
				while (abs(z) < 2.0 && n < iterations)
				{
					z = (z * z) + c;
					n++;
				}

				pFractal[y * ScreenWidth() + x] = n;
			}
		}
	}

	// Method 2) - Attempt to pre-calculate as much as possible, and reduce
	// repeated multiplications
	void CreateFractalPreCalculate(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
	{
		double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
		double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

		double x_pos = frac_tl.x;
		double y_pos = frac_tl.y;

		int y_offset = 0;
		int row_size = pix_br.x - pix_tl.x;

		int x, y, n;
		std::complex<double> c, z;

		for (y = pix_tl.y; y < pix_br.y; y++)
		{
			x_pos = frac_tl.x;
			for (x = pix_tl.x; x < pix_br.x; x++)
			{				
				c = { x_pos, y_pos };
				z = { 0,0 };

				n = 0;
				while (abs(z) < 2.0 && n < iterations)
				{
					z = (z * z) + c;
					n++;
				}

				pFractal[y_offset + x] = n;
				x_pos += x_scale;
			}

			y_pos += y_scale;
			y_offset += row_size;
		}
	}


	// Method 3) - Replace std::complex with just hard coded mathematics
	void CreateFractalNoComplex(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
	{
		double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
		double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

		double x_pos = frac_tl.x;
		double y_pos = frac_tl.y;

		int y_offset = 0;
		int row_size = ScreenWidth();

		int x, y, n;

		double cr = 0;
		double ci = 0;
		double zr = 0;
		double zi = 0;
		double re = 0;
		double im = 0;

		for (y = pix_tl.y; y < pix_br.y; y++)
		{
			x_pos = frac_tl.x;
			ci = y_pos;
			for (x = pix_tl.x; x < pix_br.x; x++)
			{
				cr = x_pos;
				zr = 0;
				zi = 0;

				n = 0;
				while ((zr * zr + zi * zi) < 4.0 && n < iterations)
				{
					re = zr * zr - zi * zi + cr;
					im = zr * zi * 2.0 + ci;
					zr = re;
					zi = im;
					n++;
				}

				pFractal[y_offset + x] = n;
				x_pos += x_scale;
			}

			y_pos += y_scale;
			y_offset += row_size;
		}
	}

	// Method 4) - Use AVX2 Vector co-processor to handle 4 fractal locations at once
	void CreateFractalIntrinsics(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
	{
		double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
		double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

		double y_pos = frac_tl.y;

		int y_offset = 0;
		int row_size = ScreenWidth();

		int x, y;

		__m256d _a, _b, _two, _four, _mask1;
		__m256d _zr, _zi, _zr2, _zi2, _cr, _ci;
		__m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump;
		__m256i _one, _c, _n, _iterations, _mask2;

		_one = _mm256_set1_epi64x(1);
		_two = _mm256_set1_pd(2.0);
		_four = _mm256_set1_pd(4.0);
		_iterations = _mm256_set1_epi64x(iterations);

		_x_scale = _mm256_set1_pd(x_scale);
		_x_jump = _mm256_set1_pd(x_scale * 4);
		_x_pos_offsets = _mm256_set_pd(0, 1, 2, 3);
		_x_pos_offsets = _mm256_mul_pd(_x_pos_offsets, _x_scale);


		for (y = pix_tl.y; y < pix_br.y; y++)
		{
			// Reset x_position
			_a = _mm256_set1_pd(frac_tl.x);
			_x_pos = _mm256_add_pd(_a, _x_pos_offsets);

			_ci = _mm256_set1_pd(y_pos);

			for (x = pix_tl.x; x < pix_br.x; x += 4)
			{
				_cr = _x_pos;
				_zr = _mm256_setzero_pd();
				_zi = _mm256_setzero_pd();
				_n = _mm256_setzero_si256();


			repeat:
				_zr2 = _mm256_mul_pd(_zr, _zr);
				_zi2 = _mm256_mul_pd(_zi, _zi);
				_a = _mm256_sub_pd(_zr2, _zi2);
				_a = _mm256_add_pd(_a, _cr);
				_b = _mm256_mul_pd(_zr, _zi);
				_b = _mm256_fmadd_pd(_b, _two, _ci);
				_zr = _a;
				_zi = _b;
				_a = _mm256_add_pd(_zr2, _zi2);
				_mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ);
				_mask2 = _mm256_cmpgt_epi64(_iterations, _n);
				_mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));
				_c = _mm256_and_si256(_one, _mask2); // Zero out ones where n < iterations													
				_n = _mm256_add_epi64(_n, _c); // n++ Increase all n
				if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0)
					goto repeat;

				pFractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
				pFractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
				pFractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
				pFractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
				_x_pos = _mm256_add_pd(_x_pos, _x_jump);
			}

			y_pos += y_scale;
			y_offset += row_size;
		}
	}

	// Method 5) - Spawn threads that use AVX method above
	void CreateFractalThreads(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
	{
		int nSectionWidth = (pix_br.x - pix_tl.x) / nMaxThreads;
		double dFractalWidth = (frac_br.x - frac_tl.x) / double(nMaxThreads);

		std::thread t[nMaxThreads];

		for (size_t i = 0; i < nMaxThreads; i++)
			t[i] = std::thread(&olcFractalExplorer::CreateFractalIntrinsics, this,
				olc::vi2d(pix_tl.x + nSectionWidth * (i), pix_tl.y),
				olc::vi2d(pix_tl.x + nSectionWidth * (i + 1), pix_br.y),
				olc::vd2d(frac_tl.x + dFractalWidth * double(i), frac_tl.y),
				olc::vd2d(frac_tl.x + dFractalWidth * double(i + 1), frac_br.y),
				iterations);

		for (size_t i = 0; i < nMaxThreads; i++)
			t[i].join();

	}


	// Method 6) - Threadpool, keep threads alive and reuse them, reducing setup overhead
	struct WorkerThread
	{
		olc::vi2d pix_tl = { 0,0 };
		olc::vi2d pix_br = { 0,0 };
		olc::vd2d frac_tl = { 0,0 };
		olc::vd2d frac_br = { 0,0 };
		int iterations = 0;
		std::condition_variable cvStart;
		bool alive = true;
		std::mutex mux;
		int screen_width = 0;
		int* fractal = nullptr;

		std::thread thread;

		void Start(const olc::vi2d& ptl, const olc::vi2d& pbr, const olc::vd2d& ftl, const olc::vd2d& fbr, const int it)
		{
			pix_tl = ptl;
			pix_br = pbr;
			frac_tl = ftl;
			frac_br = fbr;
			iterations = it;
			std::unique_lock<std::mutex> lm(mux);
			cvStart.notify_one();
		}

		void CreateFractal()
		{
			while (alive)
			{
				std::unique_lock<std::mutex> lm(mux);
				cvStart.wait(lm);

				double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
				double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

				double y_pos = frac_tl.y;

				int y_offset = 0;
				int row_size = screen_width;

				int x, y;

				__m256d _a, _b, _two, _four, _mask1;
				__m256d _zr, _zi, _zr2, _zi2, _cr, _ci;
				__m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump;
				__m256i _one, _c, _n, _iterations, _mask2;

				_one = _mm256_set1_epi64x(1);
				_two = _mm256_set1_pd(2.0);
				_four = _mm256_set1_pd(4.0);
				_iterations = _mm256_set1_epi64x(iterations);

				_x_scale = _mm256_set1_pd(x_scale);
				_x_jump = _mm256_set1_pd(x_scale * 4);
				_x_pos_offsets = _mm256_set_pd(0, 1, 2, 3);
				_x_pos_offsets = _mm256_mul_pd(_x_pos_offsets, _x_scale);


				for (y = pix_tl.y; y < pix_br.y; y++)
				{
					// Reset x_position
					_a = _mm256_set1_pd(frac_tl.x);
					_x_pos = _mm256_add_pd(_a, _x_pos_offsets);

					_ci = _mm256_set1_pd(y_pos);

					for (x = pix_tl.x; x < pix_br.x; x += 4)
					{
						_cr = _x_pos;
						_zr = _mm256_setzero_pd();
						_zi = _mm256_setzero_pd();
						_n = _mm256_setzero_si256();

					repeat:
						_zr2 = _mm256_mul_pd(_zr, _zr);
						_zi2 = _mm256_mul_pd(_zi, _zi);
						_a = _mm256_sub_pd(_zr2, _zi2);
						_a = _mm256_add_pd(_a, _cr);
						_b = _mm256_mul_pd(_zr, _zi);
						_b = _mm256_fmadd_pd(_b, _two, _ci);
						_zr = _a;
						_zi = _b;
						_a = _mm256_add_pd(_zr2, _zi2);
						_mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ);
						_mask2 = _mm256_cmpgt_epi64(_iterations, _n);
						_mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));
						_c = _mm256_and_si256(_one, _mask2); // Zero out ones where n < iterations													
						_n = _mm256_add_epi64(_n, _c); // n++ Increase all n
						if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0)
							goto repeat;

						fractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
						fractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
						fractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
						fractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
						_x_pos = _mm256_add_pd(_x_pos, _x_jump);
					}

					y_pos += y_scale;
					y_offset += row_size;
				}
				nWorkerComplete++;
			}
		}
	};

	WorkerThread workers[nMaxThreads];
	static std::atomic<int> nWorkerComplete;

	void InitialiseThreadPool()
	{
		for (int i = 0; i < nMaxThreads; i++)
		{
			workers[i].alive = true;
			workers[i].fractal = pFractal;
			workers[i].screen_width = ScreenWidth();
			workers[i].thread = std::thread(&WorkerThread::CreateFractal, &workers[i]);
		}
	}

	void CreateFractalThreadPool(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
	{
		int nSectionWidth = (pix_br.x - pix_tl.x) / nMaxThreads;
		double dFractalWidth = (frac_br.x - frac_tl.x) / double(nMaxThreads);
		nWorkerComplete = 0;

		for (size_t i = 0; i < nMaxThreads; i++)
			workers[i].Start(
				olc::vi2d(pix_tl.x + nSectionWidth * i, pix_tl.y), 
				olc::vi2d(pix_tl.x + nSectionWidth * (i + 1), pix_br.y), 
				olc::vd2d(frac_tl.x + dFractalWidth * double(i), frac_tl.y), 
				olc::vd2d(frac_tl.x + dFractalWidth * double(i + 1), frac_br.y), 
				iterations);


		while (nWorkerComplete < nMaxThreads) // Wait for all workers to complete
		{ }			
	}


	bool OnUserUpdate(float fElapsedTime) override
	{

		// Get mouse location this frame
		olc::vd2d vMouse = { (double)GetMouseX(), (double)GetMouseY() };

		// Handle Pan & Zoom
		if (GetMouse(2).bPressed)
		{
			vStartPan = vMouse;
		}

		if (GetMouse(2).bHeld)
		{
			vOffset -= (vMouse - vStartPan) / vScale;
			vStartPan = vMouse;
		}

		olc::vd2d vMouseBeforeZoom;
		ScreenToWorld(vMouse, vMouseBeforeZoom);

		if (GetKey(olc::Key::Q).bHeld || GetMouseWheel() > 0) vScale *= 1.1;
		if (GetKey(olc::Key::A).bHeld || GetMouseWheel() < 0) vScale *= 0.9;
		
		olc::vd2d vMouseAfterZoom;
		ScreenToWorld(vMouse, vMouseAfterZoom);
		vOffset += (vMouseBeforeZoom - vMouseAfterZoom);
		
		olc::vi2d pix_tl = { 0,0 };
		olc::vi2d pix_br = { ScreenWidth(), ScreenHeight() };
		olc::vd2d frac_tl = { -2.0, -1.0 };
		olc::vd2d frac_br = { 1.0, 1.0 };

		ScreenToWorld(pix_tl, frac_tl);
		ScreenToWorld(pix_br, frac_br);

		// Handle User Input
		if (GetKey(olc::K1).bPressed) nMode = 0;
		if (GetKey(olc::K2).bPressed) nMode = 1;
		if (GetKey(olc::K3).bPressed) nMode = 2;
		if (GetKey(olc::K4).bPressed) nMode = 3;
		if (GetKey(olc::K5).bPressed) nMode = 4;
		if (GetKey(olc::K6).bPressed) nMode = 5;
		if (GetKey(olc::UP).bPressed) nIterations += 64;
		if (GetKey(olc::DOWN).bPressed) nIterations -= 64;
		if (nIterations < 64) nIterations = 64;


		// START TIMING
		auto tp1 = std::chrono::high_resolution_clock::now();

		// Do the computation
		switch (nMode)
		{
		case 0: CreateFractalBasic(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
		case 1: CreateFractalPreCalculate(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
		case 2: CreateFractalNoComplex(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
		case 3: CreateFractalIntrinsics(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
		case 4: CreateFractalThreads(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
		case 5: CreateFractalThreadPool(pix_tl, pix_br, frac_tl, frac_br, nIterations); break;
		}

		// STOP TIMING
		auto tp2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = tp2 - tp1;


		// Render result to screen
		for (int y = 0; y < ScreenHeight(); y++)
		{
			for (int x = 0; x < ScreenWidth(); x++)
			{
				int i = pFractal[y * ScreenWidth() + x];
				float n = (float)i;
				float a = 0.1f;
				// Thank you @Eriksonn - Wonderful Magic Fractal Oddball Man
				Draw(x, y, olc::PixelF(0.5f * sin(a * n) + 0.5f, 0.5f * sin(a * n + 2.094f) + 0.5f,  0.5f * sin(a * n + 4.188f) + 0.5f));
			}
		}

		// Render UI
		switch (nMode)
		{
		case 0: DrawString(0, 0, "1) Naive Method", olc::WHITE, 3); break;
		case 1: DrawString(0, 0, "2) Precalculate Method", olc::WHITE, 3); break;
		case 2: DrawString(0, 0, "3) Hand-code Maths Method", olc::WHITE, 3); break;
		case 3: DrawString(0, 0, "4) Vector Extensions (AVX2) Method", olc::WHITE, 3); break;
		case 4: DrawString(0, 0, "5) Threads Method", olc::WHITE, 3); break;
		case 5: DrawString(0, 0, "6) ThreadPool Method", olc::WHITE, 3); break;
		}

		DrawString(0, 30, "Time Taken: " + std::to_string(elapsedTime.count()) + "s", olc::WHITE, 3);
		DrawString(0, 60, "Iterations: " + std::to_string(nIterations), olc::WHITE, 3);
		return !(GetKey(olc::Key::ESCAPE).bPressed);
	}

	// Pan & Zoom variables
	olc::vd2d vOffset = { 0.0, 0.0 };
	olc::vd2d vStartPan = { 0.0, 0.0 };
	olc::vd2d vScale = { 1280.0 / 2.0, 720.0 };
	

	// Convert coordinates from World Space --> Screen Space
	void WorldToScreen(const olc::vd2d& v, olc::vi2d &n)
	{
		n.x = (int)((v.x - vOffset.x) * vScale.x);
		n.y = (int)((v.y - vOffset.y) * vScale.y);
	}

	// Convert coordinates from Screen Space --> World Space
	void ScreenToWorld(const olc::vi2d& n, olc::vd2d& v)
	{
		v.x = (double)(n.x) / vScale.x + vOffset.x;
		v.y = (double)(n.y) / vScale.y + vOffset.y;
	}
};

std::atomic<int> olcFractalExplorer::nWorkerComplete = 0;

int main()
{
	olcFractalExplorer demo;
	if (demo.Construct(1280, 720, 1, 1, false, false))
		demo.Start();
	return 0;
}