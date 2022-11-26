/*
	OneLoneCoder - Palette v1.00
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Convenient tool for creating interpolated palettes, that can be
	sampled discretely or continuously


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

/*	
	Example
	~~~~~~~

	// Create palette object
	olc::Palette pal;

	// Use a stock palette
	pal = Palette(Stock::Spectrum);

	// Clearing a palete
	pal.Clear();

	// Custom palette - normalised index [0.0 ... 1.0], Colour
	pal.SetColour(00.0 / 24.0, olc::Pixel(000, 000, 000));
	pal.SetColour(03.0 / 24.0, olc::Pixel(000, 000, 030));
	pal.SetColour(05.0 / 24.0, olc::Pixel(200, 240, 255));
	pal.SetColour(12.0 / 24.0, olc::Pixel(200, 240, 255));
	pal.SetColour(15.0 / 24.0, olc::Pixel(050, 100, 255));
	pal.SetColour(20.0 / 24.0, olc::Pixel(245, 000, 146));
	pal.SetColour(22.0 / 24.0, olc::Pixel(000, 000, 128));
	pal.SetColour(24.0 / 24.0, olc::Pixel(000, 000, 000));

	// Access palette via interpolation [0.0 ... 1.0]
	// Continuous, Smooth, potentially slower
	olc::Pixel p1 = pal.Sample(0.2);
	olc::Pixel p2 = pal.Sample(0.6);
	olc::Pixel p3 = pal.Sample(1.0);

	// Access palette via index [0 ... 255]
	// Discrete, Not Smooth, very fast
	olc::Pixel i1 = pal.Index(10);			
*/



#pragma once

#include "olcPixelGameEngine.h"

namespace olc::utils
{
	class Palette
	{
	public:
		enum class Stock
		{
			Empty,
			Greyscale,
			ColdHot,
			Spectrum,
		};

	public:
		// Construct empty palette (default) or populate with pre-defined
		inline Palette(const Stock stock = Stock::Empty)
		{
			switch (stock)
			{
			case Stock::Empty:
				Clear();
				break;
			case Stock::Greyscale:
				vColours =
				{
					{0.0, olc::BLACK}, {1.0, olc::WHITE}
				};
				break;
			case Stock::ColdHot:
				vColours =
				{
					{0.0, olc::CYAN}, {0.5, olc::BLACK}, {1.0, olc::YELLOW}
				};
				break;
			case Stock::Spectrum:
				vColours =
				{
					{0.0 / 6.0, olc::RED},
					{1.0 / 6.0, olc::YELLOW},
					{2.0 / 6.0, olc::GREEN},
					{3.0 / 6.0, olc::CYAN},
					{4.0 / 6.0, olc::BLUE},
					{5.0 / 6.0, olc::MAGENTA},
					{6.0 / 6.0, olc::RED}
				};
				break;
			}

			ReconstructIndex();
		}

	public:
		// Sample continously from palette, where t is [0.0 ... 1.0]
		inline olc::Pixel Sample(const double t) const
		{
			// Return obvious sample values
			if (vColours.empty())
				return olc::BLACK;

			if (vColours.size() == 1)
				return vColours.front().second;

			// Iterate through colour entries until we find the first entry
			// with a location greater than our sample point
			double i = std::clamp(t, 0.0, 1.0);
			auto it = vColours.begin();
			while (i > it->first)
				++it;

			// If that is the first entry, just return it
			if (it == std::begin(vColours))
				return it->second;
			else
			{
				// else get the preceeding entry, and lerp between the two
				// proportionally
				auto it_p = std::prev(it);
				return olc::PixelLerp(it_p->second, it->second,
					float((i - it_p->first) / (it->first - it_p->first)));
			}
		}

		// Sample discretely from palette where idx is [0 ... 255]
		inline olc::Pixel Index(const uint8_t idx) const
		{
			return aIndexedPalette[idx];
		}

		// Clear palette, defaulting to black
		inline void Clear()
		{
			vColours.clear();
			aIndexedPalette.fill(olc::BLACK);
		}

		// Add a colour at a particular continuous location in palette
		inline void SetColour(const double d, const olc::Pixel col)
		{
			double i = std::clamp(d, 0.0, 1.0);

			// If d already exists, replace it
			auto it = std::find_if(vColours.begin(), vColours.end(),
				[&i](const std::pair<double, olc::Pixel>& p)
				{
					return p.first == i;
				});

			if (it != std::end(vColours))
			{
				// Palette entry was found, so replace colour entry
				it->second = col;
			}
			else
			{
				// Palette entry not found, sp add it, and sort palette vector
				vColours.push_back({ i, col });
				std::sort(vColours.begin(), vColours.end(),
					[](const std::pair<double, olc::Pixel>& p1, std::pair<double, olc::Pixel>& p2)
					{
						return p2.first > p1.first;
					});
			}

			ReconstructIndex();
		}

	private:
		// Vector stores all palette milestones
		std::vector<std::pair<double, olc::Pixel>> vColours;
		// Array stores fast LUT
		std::array<olc::Pixel, 256> aIndexedPalette;

		inline void ReconstructIndex()
		{
			// Reconstruct Indexed palette
			for (int i = 0; i < 256; i++)
			{
				double t = double(i) / 255.0;
				aIndexedPalette[i] = Sample(t);
			}
		}
	};

}