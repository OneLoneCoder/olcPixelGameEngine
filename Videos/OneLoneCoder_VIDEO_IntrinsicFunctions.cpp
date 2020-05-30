/*
	Vector Extensions - Mandelbrot
	"Succesfully programmed the dishwasher and debugged the pump this week..." - javidx9

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

	Relevant Video: https://youtu.be/x9Scb5Mku1g

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

// NOTE: THIS PROGRAM CANNOT BE EXCUTED - IT IS INTENDED AS A GUIDE
// TO THIS VIDEO: https://youtu.be/x9Scb5Mku1g


// Method 4) - Use AVX2 Vector co-processor to handle 4 fractal locations at once
void CreateFractalIntrinsics(const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
{
	double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
	double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

	double y_pos = frac_tl.y;

	int y_offset = 0;
	int row_size = ScreenWidth();

	int x, y;

	// 64-bit "double" registers
	__m256d _a, _b, _two, _four, _mask1;
	__m256d _zr, _zi, _zr2, _zi2, _cr, _ci;
	__m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump;
	
	// 64-bit "integer" registers
	__m256i _one, _c, _n, _iterations, _mask2;

	// Expand constants into vectors of constants
	// one = |(int)1|(int)1|(int)1|(int)1|		
	_one = _mm256_set1_epi64x(1);
	
	// two = |2.0|2.0|2.0|2.0|
	_two = _mm256_set1_pd(2.0);
	
	// four = |4.0|4.0|4.0|4.0|
	_four = _mm256_set1_pd(4.0);
	
	// iterations = |iterations|iterations|iterations|iterations|
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
			
			// Zreal = 0
			_zr = _mm256_setzero_pd();
			
			// Zimag = 0
			_zi = _mm256_setzero_pd();
			
			// nIterations = 0
			_n = _mm256_setzero_si256();


		repeat:
			// Normal: z = (z * z) + c;
			// Manual: a = zr * zr - zi * zi + cr;
			//         b = zr * zi * 2.0 + ci;
			//         zr = a;
			//         zi = b;
		
		
			// zr^2 = zr * zr
			_zr2 = _mm256_mul_pd(_zr, _zr);     // zr * zr
			
			// zi^2 = zi * zi
			_zi2 = _mm256_mul_pd(_zi, _zi);     // zi * zi
			
			// a = zr^2 - zi^2
			_a = _mm256_sub_pd(_zr2, _zi2);     // a = (zr * zr) - (zi * zi)
			
			// a = a + cr
			_a = _mm256_add_pd(_a, _cr);        // a = ((zr * zr) - (zi * zi)) + cr
			
			
			
			// b = zr * zi
			_b = _mm256_mul_pd(_zr, _zi);        // b = zr * zi
			
			// b = b * 2.0 + ci
			// b = b * |2.0|2.0|2.0|2.0| + ci
			_b = _mm256_fmadd_pd(_b, _two, _ci); // b = (zr * zi) * 2.0 + ci
			
			// zr = a
			_zr = _a;                            // zr = a
			
			// zi = b
			_zi = _b;                            // zr = b
			
			
			
			// Normal: while (abs(z) < 2.0 && n < iterations)
			// Manual: while ((zr * zr + zi * zi) < 4.0 && n < iterations)
			
			
			// a = zr^2 + zi^2
			_a = _mm256_add_pd(_zr2, _zi2);     // a = (zr * zr) + (zi * zi)
			
			// m1 = if (a < 4.0)
			// m1 = |if(a[3] < 4.0)|if(a[2] < 4.0)|if(a[1] < 4.0)|if(a[0] < 4.0)|
			// m1 = |111111...11111|000000...00000|111111...11111|000000...00000|
			// m1 = |11...11|00...00|11...11|00...00| <- Shortened to reduce typing :P
			_mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ); 
			
			// m2 = if (iterations > n)
			// m2 = |00...00|11...11|11...11|00...00|
			_mask2 = _mm256_cmpgt_epi64(_iterations, _n);  
			
			// m2 = m2 AND m1 = if(a < 4.0 && iterations > n)
			//
			// m2 =    |00...00|11...11|11...11|00...00|
			// m1 = AND|11...11|00...00|11...11|00...00|
			// m2 =    |00...00|00...00|11...11|00...00|
			_mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));
			
			//  c = |(int)1|(int)1|(int)1|(int)1| AND m2
			//
			//  c =    |00...01|00...01|00...01|00...01| 
			// m2 = AND|00...00|00...00|11...11|00...00|
			//  c =    |00...00|00...00|00...01|00...00|
			//
			//  c = |(int)0|(int)0|(int)1|(int)0|
			_c = _mm256_and_si256(_one, _mask2);				
			
			// n = n + c
			// n =  |00...24|00...13|00...08|00...21| 
			// c = +|00...00|00...00|00...01|00...00|
			// n =  |00...24|00...13|00...09|00...21| (Increment only applied to 'enabled' element)
			_n = _mm256_add_epi64(_n, _c);
			
			// if ((zr * zr + zi * zi) < 4.0 && n < iterations) goto repeat
			// i.e. if our mask has any elements that are 1
			// |00...00|00...00|11...11|00...00|
			// |   0   |   0   |   1   |   0   | = 0b0010 = 2
			// so... if (2 > 0) goto repeat
			if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0)
				goto repeat;
				
			// Tight loop has finished, all 4 pixels have been evaluated. Increment
			// fractal space x positions for next 4 pixels
			// x_pos = x_pos + x_jump
			_x_pos = _mm256_add_pd(_x_pos, _x_jump);
				
			// Unpack our 4x64-bit Integer Vector into normal 32-bit Integers
			// and write into memory at correct location. Note, depending on
			// how you structure the memory, and the types you use, this step
			// may not be required. If I was working with 64-bit integers I
			// could choose to just write the vector entirely, saving this
			// truncation at the expense of 2x the memory required
			
			#if defined(__linux__)				
				// Intrinsics are not cross platform!
				pFractal[y_offset + x + 0] = int(_n[3]);
				pFractal[y_offset + x + 1] = int(_n[2]);
				pFractal[y_offset + x + 2] = int(_n[1]);
				pFractal[y_offset + x + 3] = int(_n[0]);
			#endif
			
			#if defined(_WIN32)				
				pFractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
				pFractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
				pFractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
				pFractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
			#endif
			
			
		}

		y_pos += y_scale;
		y_offset += row_size;
	}
}