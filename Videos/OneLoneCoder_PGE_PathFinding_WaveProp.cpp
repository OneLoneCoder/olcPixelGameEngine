/*
	OneLoneCoder.com - Path Finding #2 - Wave Propagation & Potential Fields
	"...never get lost again, so long as you know where you are" - @Javidx9


	Background
	~~~~~~~~~~
	A nice follow up alternative to the A* Algorithm. Wave propagation is less
	applicable to multiple objects with multiple destinations, but fantatsic
	for multiple objects all reaching the same destination.

	WARNING! This code is NOT OPTIMAL!! It is however very robust. There
	are many ways to optimise this further.

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
	Patreon:	https://www.patreon/javidx9
	Homepage:	https://www.onelonecoder.com

	Relevant Videos
	~~~~~~~~~~~~~~~
	Part #1 https://youtu.be/icZj67PTFhc
	Part #2 https://youtu.be/0ihciMKlcP8

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018
*/
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <vector>
#include <list>
#include <algorithm>
#include <utility>


// Override base class with your custom functionality
class PathFinding_FlowFields : public olc::PixelGameEngine
{
public:
	PathFinding_FlowFields()
	{
		sAppName = "PathFinding - Flow Fields";
	}

private:
	int nMapWidth;
	int nMapHeight;
	int nCellSize;
	int nBorderWidth;

	bool *bObstacleMap;

	int *nFlowFieldZ;
	float *fFlowFieldY;
	float *fFlowFieldX;

	int nStartX;
	int nStartY;
	int nEndX;
	int nEndY;

	int nWave = 1;

public:
	bool OnUserCreate() override
	{
		nBorderWidth = 4;
		nCellSize = 32;
		nMapWidth = ScreenWidth() / nCellSize;
		nMapHeight = ScreenHeight() / nCellSize;
		bObstacleMap = new bool[nMapWidth * nMapHeight]{ false };
		nFlowFieldZ = new int[nMapWidth * nMapHeight]{ 0 };
		fFlowFieldX = new float[nMapWidth * nMapHeight]{ 0 };
		fFlowFieldY = new float[nMapWidth * nMapHeight]{ 0 };

		nStartX = 3;
		nStartY = 7;
		nEndX = 12;
		nEndY = 7;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Little convenience lambda 2D -> 1D
		auto p = [&](int x, int y) { return y * nMapWidth + x;  };

		// User Input
		int nSelectedCellX = GetMouseX() / nCellSize;
		int nSelectedCellY = GetMouseY() / nCellSize;

		if (GetMouse(0).bReleased)
		{
			// Toggle Obstacle if mouse left clicked
			bObstacleMap[p(nSelectedCellX, nSelectedCellY)] =
				!bObstacleMap[p(nSelectedCellX, nSelectedCellY)];
		} 

		if (GetMouse(1).bReleased)
		{
			nStartX = nSelectedCellX;
			nStartY = nSelectedCellY;
		}

		if (GetKey(olc::Key::Q).bReleased)
		{
			nWave++;
		}

		if (GetKey(olc::Key::A).bReleased)
		{
			nWave--;
			if (nWave == 0)
				nWave = 1;
		}



		// 1) Prepare flow field, add a boundary, and add obstacles
		//    by setting the flow Field Height (Z) to -1
		for (int x = 0; x < nMapWidth; x++)
		{
			for (int y = 0; y < nMapHeight; y++)
			{
				// Set border or obstacles
				if (x == 0 || y == 0 || x == (nMapWidth - 1) || y == (nMapHeight - 1) || bObstacleMap[p(x, y)])
				{
					nFlowFieldZ[p(x, y)] = -1;
				}
				else
				{
					nFlowFieldZ[p(x, y)] = 0;
				}
			}
		}

		// 2) Propagate a wave (i.e. flood-fill) from target location. Here I use
		//    a tuple, of {x, y, distance} - though you could use a struct or 
		//    similar.
		std::list<std::tuple<int, int, int>> nodes;

		// Add the first discovered node - the target location, with a distance of 1
		nodes.push_back({ nEndX, nEndY, 1 });

		while (!nodes.empty())
		{
			// Each iteration through the discovered nodes may create newly discovered
			// nodes, so I maintain a second list. It's important not to contaminate
			// the list being iterated through.
			std::list<std::tuple<int, int, int>> new_nodes;

			// Now iterate through each discovered node. If it has neighbouring nodes
			// that are empty space and undiscovered, add those locations to the
			// new nodes list
			for (auto &n : nodes)
			{
				int x = std::get<0>(n); // Map X-Coordinate
				int y = std::get<1>(n); // Map Y-Coordinate
				int d = std::get<2>(n); // Distance From Target Location

				// Set distance count for this node. NOte that when we add nodes we add 1
				// to this distance. This emulates propagating a wave across the map, where
				// the front of that wave increments each iteration. In this way, we can 
				// propagate distance information 'away from target location'
				nFlowFieldZ[p(x, y)] = d;

				// Add neigbour nodes if unmarked, i.e their "height" is 0. Any discovered
				// node or obstacle will be non-zero

				// Check East
				if ((x + 1) < nMapWidth && nFlowFieldZ[p(x + 1, y)] == 0)
					new_nodes.push_back({ x + 1, y, d + 1 });

				// Check West
				if ((x - 1) >= 0 && nFlowFieldZ[p(x - 1, y)] == 0)
					new_nodes.push_back({ x - 1, y, d + 1 });

				// Check South
				if ((y + 1) < nMapHeight && nFlowFieldZ[p(x, y + 1)] == 0)
					new_nodes.push_back({ x, y + 1, d + 1 });

				// Check North
				if ((y - 1) >= 0 && nFlowFieldZ[p(x, y - 1)] == 0)
					new_nodes.push_back({ x, y - 1, d + 1 });
			}

			// We will now have potentially multiple nodes for a single location. This means our
			// algorithm will never complete! So we must remove duplicates form our new node list.
			// Im doing this with some clever code - but it is not performant(!) - it is merely
			// convenient. I'd suggest doing away with overhead structures like linked lists and sorts
			// if you are aiming for fastest path finding.

			// Sort the nodes - This will stack up nodes that are similar: A, B, B, B, B, C, D, D, E, F, F
			new_nodes.sort([&](const std::tuple<int, int, int> &n1, const std::tuple<int, int, int> &n2)
			{
				// In this instance I dont care how the values are sorted, so long as nodes that
				// represent the same location are adjacent in the list. I can use the p() lambda
				// to generate a unique 1D value for a 2D coordinate, so I'll sort by that.
				return p(std::get<0>(n1), std::get<1>(n1)) < p(std::get<0>(n2), std::get<1>(n2));
			});

			// Use "unique" function to remove adjacent duplicates       : A, B, -, -, -, C, D, -, E, F -
			// and also erase them                                       : A, B, C, D, E, F
			new_nodes.unique([&](const std::tuple<int, int, int> &n1, const std::tuple<int, int, int> &n2)
			{
				return  p(std::get<0>(n1), std::get<1>(n1)) == p(std::get<0>(n2), std::get<1>(n2));
			});

			// We've now processed all the discoverd nodes, so clear the list, and add the newly
			// discovered nodes for processing on the next iteration
			nodes.clear();
			nodes.insert(nodes.begin(), new_nodes.begin(), new_nodes.end());

			// When there are no more newly discovered nodes, we have "flood filled" the entire
			// map. The propagation phase of the algorithm is complete
		}


		// 3) Create Path. Starting a start location, create a path of nodes until you reach target
		//    location. At each node find the neighbour with the lowest "distance" score.
		std::list<std::pair<int, int>> path;
		path.push_back({ nStartX, nStartY });
		int nLocX = nStartX;
		int nLocY = nStartY;
		bool bNoPath = false;

		while (!(nLocX == nEndX && nLocY == nEndY) && !bNoPath)
		{
			std::list<std::tuple<int, int, int>> listNeighbours;

			// 4-Way Connectivity
			if ((nLocY - 1) >= 0 && nFlowFieldZ[p(nLocX, nLocY - 1)] > 0)
				listNeighbours.push_back({ nLocX, nLocY - 1, nFlowFieldZ[p(nLocX, nLocY - 1)] });

			if ((nLocX + 1) < nMapWidth && nFlowFieldZ[p(nLocX + 1, nLocY)] > 0)
				listNeighbours.push_back({ nLocX + 1, nLocY, nFlowFieldZ[p(nLocX + 1, nLocY)] });

			if ((nLocY + 1) < nMapHeight && nFlowFieldZ[p(nLocX, nLocY + 1)] > 0)
				listNeighbours.push_back({ nLocX, nLocY + 1, nFlowFieldZ[p(nLocX, nLocY + 1)] });

			if ((nLocX - 1) >= 0 && nFlowFieldZ[p(nLocX - 1, nLocY)] > 0)
				listNeighbours.push_back({ nLocX - 1, nLocY, nFlowFieldZ[p(nLocX - 1, nLocY)] });

			// 8-Way Connectivity
			if ((nLocY - 1) >= 0 && (nLocX - 1) >= 0 && nFlowFieldZ[p(nLocX - 1, nLocY - 1)] > 0)
				listNeighbours.push_back({ nLocX - 1, nLocY - 1, nFlowFieldZ[p(nLocX - 1, nLocY - 1)] });

			if ((nLocY - 1) >= 0 && (nLocX + 1) < nMapWidth && nFlowFieldZ[p(nLocX + 1, nLocY - 1)] > 0)
				listNeighbours.push_back({ nLocX + 1, nLocY - 1, nFlowFieldZ[p(nLocX + 1, nLocY - 1)] });

			if ((nLocY + 1) < nMapHeight && (nLocX - 1) >= 0 && nFlowFieldZ[p(nLocX - 1, nLocY + 1)] > 0)
				listNeighbours.push_back({ nLocX - 1, nLocY + 1, nFlowFieldZ[p(nLocX - 1, nLocY + 1)] });

			if ((nLocY + 1) < nMapHeight && (nLocX + 1) < nMapWidth && nFlowFieldZ[p(nLocX + 1, nLocY + 1)] > 0)
				listNeighbours.push_back({ nLocX + 1, nLocY + 1, nFlowFieldZ[p(nLocX + 1, nLocY + 1)] });

			// Sprt neigbours based on height, so lowest neighbour is at front
			// of list
			listNeighbours.sort([&](const std::tuple<int, int, int> &n1, const std::tuple<int, int, int> &n2)
			{
				return std::get<2>(n1) < std::get<2>(n2); // Compare distances
			});

			if (listNeighbours.empty()) // Neighbour is invalid or no possible path
				bNoPath = true;
			else
			{
				nLocX = std::get<0>(listNeighbours.front());
				nLocY = std::get<1>(listNeighbours.front());
				path.push_back({ nLocX, nLocY });
			}
		}


		// 4) Create Flow "Field"
		for (int x = 1; x < nMapWidth - 1; x++)
		{
			for (int y = 1; y < nMapHeight - 1; y++)
			{
				float vx = 0.0f;
				float vy = 0.0f;

				vy -= (float)((nFlowFieldZ[p(x, y + 1)] <= 0 ? nFlowFieldZ[p(x, y)] : nFlowFieldZ[p(x, y + 1)]) - nFlowFieldZ[p(x, y)]);
				vx -= (float)((nFlowFieldZ[p(x + 1, y)] <= 0 ? nFlowFieldZ[p(x, y)] : nFlowFieldZ[p(x + 1, y)]) - nFlowFieldZ[p(x, y)]);
				vy += (float)((nFlowFieldZ[p(x, y - 1)] <= 0 ? nFlowFieldZ[p(x, y)] : nFlowFieldZ[p(x, y - 1)]) - nFlowFieldZ[p(x, y)]);
				vx += (float)((nFlowFieldZ[p(x - 1, y)] <= 0 ? nFlowFieldZ[p(x, y)] : nFlowFieldZ[p(x - 1, y)]) - nFlowFieldZ[p(x, y)]);

				float r = 1.0f / sqrtf(vx*vx + vy * vy);
				fFlowFieldX[p(x, y)] = vx * r;
				fFlowFieldY[p(x, y)] = vy * r;
			}
		}




		// Draw Map
		Clear(olc::BLACK);

		for (int x = 0; x < nMapWidth; x++)
		{
			for (int y = 0; y < nMapHeight; y++)
			{
				olc::Pixel colour = olc::BLUE;

				if (bObstacleMap[p(x, y)])
					colour = olc::GREY;

				if (nWave == nFlowFieldZ[p(x, y)])
					colour = olc::DARK_CYAN;

				if (x == nStartX && y == nStartY)
					colour = olc::GREEN;

				if (x == nEndX && y == nEndY)
					colour = olc::RED;

				// Draw Base
				FillRect(x * nCellSize, y * nCellSize, nCellSize - nBorderWidth, nCellSize - nBorderWidth, colour);

				// Draw "potential" or "distance" or "height" :D
				//DrawString(x * nCellSize, y * nCellSize, std::to_string(nFlowFieldZ[p(x, y)]), olc::WHITE);

				if (nFlowFieldZ[p(x, y)] > 0)
				{
					float ax[4], ay[4];
					float fAngle = atan2f(fFlowFieldY[p(x, y)], fFlowFieldX[p(x, y)]);
					float fRadius = (float)(nCellSize - nBorderWidth) / 2.0f;
					int fOffsetX = x * nCellSize + ((nCellSize - nBorderWidth) / 2);
					int fOffsetY = y * nCellSize + ((nCellSize - nBorderWidth) / 2);
					ax[0] = cosf(fAngle) * fRadius + fOffsetX;
					ay[0] = sinf(fAngle) * fRadius + fOffsetY;
					ax[1] = cosf(fAngle) * -fRadius + fOffsetX;
					ay[1] = sinf(fAngle) * -fRadius + fOffsetY;
					ax[2] = cosf(fAngle + 0.1f) * fRadius * 0.7f + fOffsetX;
					ay[2] = sinf(fAngle + 0.1f) * fRadius * 0.7f + fOffsetY;
					ax[3] = cosf(fAngle - 0.1f) * fRadius * 0.7f + fOffsetX;
					ay[3] = sinf(fAngle - 0.1f) * fRadius * 0.7f + fOffsetY;

					DrawLine(ax[0], ay[0], ax[1], ay[1], olc::CYAN);
					DrawLine(ax[0], ay[0], ax[2], ay[2], olc::CYAN);
					DrawLine(ax[0], ay[0], ax[3], ay[3], olc::CYAN);

				}
			}
		}


		bool bFirstPoint = true;
		int ox, oy;
		for (auto &a : path)
		{
			if (bFirstPoint)
			{
				ox = a.first;
				oy = a.second;
				bFirstPoint = false;
			}
			else
			{
				DrawLine(
					ox * nCellSize + ((nCellSize - nBorderWidth) / 2),
					oy  * nCellSize + ((nCellSize - nBorderWidth) / 2),
					a.first * nCellSize + ((nCellSize - nBorderWidth) / 2),
					a.second * nCellSize + ((nCellSize - nBorderWidth) / 2), olc::YELLOW);

				ox = a.first;
				oy = a.second;

				FillCircle(ox * nCellSize + ((nCellSize - nBorderWidth) / 2), oy * nCellSize + ((nCellSize - nBorderWidth) / 2), 10, olc::YELLOW);
			}
		}

		
		return true;
	}
};


int main()
{
	PathFinding_FlowFields demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}