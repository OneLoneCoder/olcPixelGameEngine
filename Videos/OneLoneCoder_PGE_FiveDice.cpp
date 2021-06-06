/*
	Programming Practice: Five Dice
	"OK, it's getting a bit serious now..." - javidx9

	Video: https://youtu.be/D2a5fHX-Qrs

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2021 OneLoneCoder.com

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

	Video:
	~~~~~~
	https://youtu.be/DDV_2cWT94U

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <set>
#include <numeric>

class FiveDice : public olc::PixelGameEngine
{
public:
	FiveDice()
	{
		sAppName = "Five Dice";
	}

	std::vector<uint8_t> vRolled;

public:
	void DrawDie(const olc::vi2d& vPos,
				 const uint8_t nFace,
				 const olc::vi2d& vSize = { 64, 64 },
				 const olc::Pixel colFace = olc::DARK_RED,
				 const olc::Pixel colSpot = olc::WHITE)
	{
		// Draw Background
		FillRect(vPos, vSize, colFace);

		int32_t nColL = int32_t(double(vSize.x) * 0.25);
		int32_t nColM = int32_t(double(vSize.x) * 0.5);
		int32_t nColR = int32_t(double(vSize.x) * 0.75);
		int32_t nRowT = int32_t(double(vSize.y) * 0.25);
		int32_t nRowM = int32_t(double(vSize.y) * 0.5);
		int32_t nRowB = int32_t(double(vSize.y) * 0.75);
		int32_t nRad = int32_t(double(nColL) * 0.4);

		/*
		switch (nFace)
		{
		case 1:
			FillCircle(vPos + olc::vi2d(nColM, nRowM), nRad, colSpot);
			break;
		case 2:
			FillCircle(vPos + olc::vi2d(nColL, nRowT), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowB), nRad, colSpot);
			break;
		case 3:
			FillCircle(vPos + olc::vi2d(nColL, nRowT), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColM, nRowM), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowB), nRad, colSpot);
			break;
		case 4:
			FillCircle(vPos + olc::vi2d(nColL, nRowT), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowB), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColL, nRowB), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowT), nRad, colSpot);
			break;
		case 5:
			FillCircle(vPos + olc::vi2d(nColL, nRowT), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowB), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColL, nRowB), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowT), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColM, nRowM), nRad, colSpot);
			break;
		case 6:
			FillCircle(vPos + olc::vi2d(nColL, nRowT), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowB), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColL, nRowB), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowT), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColL, nRowM), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowM), nRad, colSpot);
			break;
		}
		*/

		/*
		if((std::set<uint8_t>{2, 3, 4, 5, 6}).count(nFace) > 0)
			FillCircle(vPos + olc::vi2d(nColL, nRowT), nRad, colSpot);
		if ((std::set<uint8_t>{6}).count(nFace) > 0) 
			FillCircle(vPos + olc::vi2d(nColL, nRowM), nRad, colSpot);
		if ((std::set<uint8_t>{4, 5, 6}).count(nFace) > 0)
			FillCircle(vPos + olc::vi2d(nColL, nRowB), nRad, colSpot);

		if ((std::set<uint8_t>{1, 3, 5}).count(nFace) > 0)
			FillCircle(vPos + olc::vi2d(nColM, nRowM), nRad, colSpot);

		if ((std::set<uint8_t>{4, 5, 6}).count(nFace) > 0)
			FillCircle(vPos + olc::vi2d(nColR, nRowT), nRad, colSpot);
		if ((std::set<uint8_t>{6}).count(nFace) > 0)
			FillCircle(vPos + olc::vi2d(nColR, nRowM), nRad, colSpot);
		if ((std::set<uint8_t>{2, 3, 4, 5, 6}).count(nFace) > 0)
			FillCircle(vPos + olc::vi2d(nColR, nRowB), nRad, colSpot);
		*/


		if (nFace & 1)
		{
			FillCircle(vPos + olc::vi2d(nColM, nRowM), nRad, colSpot);
		}

		if (nFace > 1)
		{
			FillCircle(vPos + olc::vi2d(nColR, nRowB), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColL, nRowT), nRad, colSpot);
		}

		if (nFace > 3)
		{
			FillCircle(vPos + olc::vi2d(nColR, nRowT), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColL, nRowB), nRad, colSpot);
		}

		if (nFace == 6)
		{
			FillCircle(vPos + olc::vi2d(nColL, nRowM), nRad, colSpot);
			FillCircle(vPos + olc::vi2d(nColR, nRowM), nRad, colSpot);
		}

	}




	bool OnUserCreate() override
	{
		vRolled = { 1, 6, 3, 3, 5 };

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		//if (GetKey(olc::Key::SPACE).bReleased)
		//{
		//	/*
		//	vRolled =
		//	{
		//		uint8_t(rand() % 6 + 1),
		//		uint8_t(rand() % 6 + 1),
		//		uint8_t(rand() % 6 + 1),
		//		uint8_t(rand() % 6 + 1),
		//		uint8_t(rand() % 6 + 1)
		//	};
		//	*/

		//	/*
		//	std::transform(
		//		vRolled.begin(), 
		//		vRolled.end(), 
		//		vRolled.begin(), 
		//		[](uint8_t n) { return rand() % 6 + 1; }
		//	);
		//	*/

		//	std::generate(
		//		vRolled.begin(), 
		//		vRolled.end(), 
		//		[]() { return rand() % 6 + 1; }
		//	);

		//	std::sort(vRolled.begin(), vRolled.end());
		//}
		

		/*
		int nScore_AllDice = std::accumulate(vRolled.begin(), vRolled.end(), 0);

		int nScore_CountOnes = std::count(vRolled.begin(), vRolled.end(), 1) * 1;
		int nScore_CountTwos = std::count(vRolled.begin(), vRolled.end(), 2) * 2;
		int nScore_CountThrees = std::count(vRolled.begin(), vRolled.end(), 3) * 3;
		int nScore_CountFours = std::count(vRolled.begin(), vRolled.end(), 4) * 4;
		int nScore_CountFives = std::count(vRolled.begin(), vRolled.end(), 5) * 5;
		int nScore_CountSixes = std::count(vRolled.begin(), vRolled.end(), 6) * 6;

		int nScore_ThreeOfAKind = 0;
		int nScore_FourOfAKind = 0;
		int nScore_FiveOfAKind = 0;
		int nScore_SmallStraight = 0;
		int nScore_LargeStraight = 0;
		int nScore_FullHouse = 0;
		*/

		/*
		auto PatternMatch = [&](const std::vector<uint8_t> vDice, const std::string& sPattern) -> bool
		{
			// nnnnn                        - Yahtzee
			// nnnn?, ?nnnn                 - four of a kind
			// nnn??, ?nnn?, ??nnn          - three of a kind
			// 1234?, ?2345, 2345?, ?3456   - Small Straight
			// 12345, 23456                 - Large Straight
			// nnn?? & ???nn, nn??? & ??nnn - Full House

			bool bMatch = true;
			uint8_t n = 0;

			for (size_t idx = 0; idx < 5; idx++)
			{
				if (sPattern[idx] == 'n')
				{
					if (n == 0)
					{
						n = vDice[idx];
					}
					else
					{
						bMatch &= (vDice[idx] == n);
					}
				}
				else if (sPattern[idx] == '?')
				{
					bMatch &= true;
				}
				else // is Face Value
				{
					bMatch &= ((sPattern[idx] - '0') == vDice[idx]);
				}

			}
			return bMatch;
		};


		// nnnnn                        - Yahtzee
		if (PatternMatch(vRolled, "nnnnn"))
		{
			nScore_FiveOfAKind = 50;
		}

		// nnnn?, ?nnnn                 - four of a kind
		if (PatternMatch(vRolled, "nnnn?") || PatternMatch(vRolled, "?nnnn"))
		{
			nScore_FourOfAKind = 4 * vRolled[2];
		}

		// nnn??, ?nnn?, ??nnn          - three of a kind
		if (PatternMatch(vRolled, "nnn??") || PatternMatch(vRolled, "?nnn?") || PatternMatch(vRolled, "??nnn"))
		{
			nScore_ThreeOfAKind = 3 * vRolled[2];
		}

		// 1234?, ?2345, 2345?, ?3456   - Small Straight
		if (PatternMatch(vRolled, "1234?") || PatternMatch(vRolled, "2345?") || PatternMatch(vRolled, "?3456") || PatternMatch(vRolled, "?2345"))
		{
			nScore_SmallStraight = 30;
		}

		// 12345, 23456                 - Large Straight
		if (PatternMatch(vRolled, "12345") || PatternMatch(vRolled, "23456"))
		{
			nScore_LargeStraight = 40;
		}

		// nnn?? & ???nn, nn??? & ??nnn - Full House
		if ((PatternMatch(vRolled, "nnn??") && PatternMatch(vRolled, "???nn")) || (PatternMatch(vRolled, "??nnn") && PatternMatch(vRolled, "nn???")))
		{
			nScore_FullHouse = 25;
		}

		std::vector<std::pair<std::string, int>> vScores =
		{
			{"Total Ones      : ", std::count(vRolled.begin(), vRolled.end(), 1) * 1},
			{"Total Twos      : ", std::count(vRolled.begin(), vRolled.end(), 2) * 2},
			{"Total Threes    : ", std::count(vRolled.begin(), vRolled.end(), 3) * 3},
			{"Total Fours     : ", std::count(vRolled.begin(), vRolled.end(), 4) * 4},
			{"Total Fives     : ", std::count(vRolled.begin(), vRolled.end(), 5) * 5},
			{"Total Sixes     : ", std::count(vRolled.begin(), vRolled.end(), 6) * 6},
			{"Chance          : ", std::accumulate(vRolled.begin(), vRolled.end(), 0)},

			{"Three Of A Kind : ", (PatternMatch(vRolled, "nnn??") || PatternMatch(vRolled, "?nnn?") || PatternMatch(vRolled, "??nnn")) ? (std::count(vRolled.begin(), vRolled.end(), vRolled[2]) * vRolled[2]) : 0},
			{"Four Of A Kind  : ", (PatternMatch(vRolled, "nnnn?") || PatternMatch(vRolled, "?nnnn")) ? (std::count(vRolled.begin(), vRolled.end(), vRolled[2]) * vRolled[2]) : 0},
			{"Full House      : ", ((PatternMatch(vRolled, "nnn??") && PatternMatch(vRolled, "???nn")) || (PatternMatch(vRolled, "??nnn") && PatternMatch(vRolled, "nn???"))) ? 25 : 0},
			{"Small Straight  : ", (PatternMatch(vRolled, "1234?") || PatternMatch(vRolled, "2345?") || PatternMatch(vRolled, "?3456") || PatternMatch(vRolled, "?2345")) ? 30 : 0},
			{"Large Straight  : ", (PatternMatch(vRolled, "12345") || PatternMatch(vRolled, "23456")) ? 40 : 0},
			{"Five Of A Kind  : ", (PatternMatch(vRolled, "nnnnn")) ? 50 : 0},
		};

		*/

		auto Match = [&vRolled = vRolled](const std::vector<std::string>& vPatterns) -> bool
		{
			// nnnnn                        - Yahtzee
			// nnnn?, ?nnnn                 - four of a kind
			// nnn??, ?nnn?, ??nnn          - three of a kind
			// 1234?, ?2345, 2345?, ?3456   - Small Straight
			// 12345, 23456                 - Large Straight
			// nnn?? & ???nn, nn??? & ??nnn - Full House

			bool bResult = false;

			for (const auto& sPattern : vPatterns)
			{

				bool bMatch = true;
				uint8_t n = 0;

				for (size_t idx = 0; idx < 5; idx++)
				{
					if (sPattern[idx] == 'n')
					{
						if (n == 0)
						{
							n = vRolled[idx];
						}
						else
						{
							bMatch &= (vRolled[idx] == n);
						}
					}
					else if (sPattern[idx] == '?')
					{
						bMatch &= true;
					}
					else // is Face Value
					{
						bMatch &= ((sPattern[idx] - '0') == vRolled[idx]);
					}

				}

				bResult |= bMatch;
			}
			return bResult;
		};

		/*DrawDie({ 10, 10 }, vRolled[0]);
		DrawDie({ 80, 10 }, vRolled[1]);
		DrawDie({ 150, 10 }, vRolled[2]);
		DrawDie({ 220, 10 }, vRolled[3]);
		DrawDie({ 290, 10 }, vRolled[4]);

		int nOffsetY = 100;
		DrawString(10, nOffsetY += 10, "Total Ones      : " + std::to_string(nScore_CountOnes));
		DrawString(10, nOffsetY += 10, "Total Twos      : " + std::to_string(nScore_CountTwos));
		DrawString(10, nOffsetY += 10, "Total Threes    : " + std::to_string(nScore_CountThrees));
		DrawString(10, nOffsetY += 10, "Total Fours     : " + std::to_string(nScore_CountFours));
		DrawString(10, nOffsetY += 10, "Total Fives     : " + std::to_string(nScore_CountFives));
		DrawString(10, nOffsetY += 10, "Total Sixes     : " + std::to_string(nScore_CountSixes));
		DrawString(10, nOffsetY += 10, "Three Of A Kind : " + std::to_string(nScore_ThreeOfAKind));
		DrawString(10, nOffsetY += 10, "Four Of A Kind  : " + std::to_string(nScore_FourOfAKind));
		DrawString(10, nOffsetY += 10, "Full House      : " + std::to_string(nScore_FullHouse));
		DrawString(10, nOffsetY += 10, "Small Straight  : " + std::to_string(nScore_SmallStraight));
		DrawString(10, nOffsetY += 10, "Large Straight  : " + std::to_string(nScore_LargeStraight));
		DrawString(10, nOffsetY += 10, "Five Of A Kind  : " + std::to_string(nScore_FiveOfAKind));
		DrawString(10, nOffsetY += 10, "Chance          : " + std::to_string(nScore_AllDice));*/

		if (GetKey(olc::Key::SPACE).bReleased)
		{
			std::generate(vRolled.begin(), vRolled.end(), []() { return rand() % 6 + 1; });
			std::sort(vRolled.begin(), vRolled.end());
		}

		std::vector<std::pair<std::string, int>> vScores =
		{
			{"Total Ones      : ", std::count(vRolled.begin(), vRolled.end(), 1) * 1},
			{"Total Twos      : ", std::count(vRolled.begin(), vRolled.end(), 2) * 2},
			{"Total Threes    : ", std::count(vRolled.begin(), vRolled.end(), 3) * 3},
			{"Total Fours     : ", std::count(vRolled.begin(), vRolled.end(), 4) * 4},
			{"Total Fives     : ", std::count(vRolled.begin(), vRolled.end(), 5) * 5},
			{"Total Sixes     : ", std::count(vRolled.begin(), vRolled.end(), 6) * 6},
			{"Chance          : ", std::accumulate(vRolled.begin(), vRolled.end(), 0)},

			{"Three Of A Kind : ", (Match({"nnn??", "?nnn?", "??nnn"})) ? (3 * vRolled[2])                                 : 0},
			{"Four Of A Kind  : ", (Match({"nnnn?", "?nnnn"})) ? (4 * vRolled[2])                                          : 0},
			{"Full House      : ", ((Match({"nnn??"}) && Match({"???nn"})) || (Match({"??nnn"}) && Match({"nn???"}))) ? 25 : 0},
			{"Small Straight  : ", (Match({"1234?", "2345?", "?3456", "?2345"}))                                      ? 30 : 0},
			{"Large Straight  : ", (Match({"12345", "23456"}))                                                        ? 40 : 0},
			{"Five Of A Kind  : ", (Match({"nnnnn"}))                                                                 ? 50 : 0},
		};


		Clear(olc::DARK_GREEN);
		olc::vi2d vOffset = { -60, 90 };
		for (int i = 0; i < 5; i++)       DrawDie({ vOffset.x += 70, 10 }, vRolled[i]);
		for (const auto& score : vScores) DrawString(10, vOffset.y += 10, score.first + std::to_string(score.second));
		
		return true;
	}
};

int main()
{
	FiveDice demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();
	return 0;
}