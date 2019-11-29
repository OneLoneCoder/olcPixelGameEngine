
/*
	Saving Sedit: An OLC Code Jam 2018 Submission
	"At least this is checked off my list now..." - javidx9

	Download the playable version here: https://onelonecoder.itch.io/saving-sedit

	Note: This was a JAM entry, it is incomplete, has bugs, has features that
	dont go anywhere. I've not tidied up the code - it is a record of how
	the JAM went.

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

	Relevant Video: https://youtu.be/Qco5BstCxRM

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
#define OLC_PGE_GRAPHICS2D
#include "olcPGEX_Graphics2D.h"

#include "olcPGEX_TileMaps_new.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

#include "Zix_PGE_Controller.h"

#include <list>
#include <algorithm>
#include <string>
#include <stack>
#include <map>
using namespace std;


class cAnimator
{
public:
	std::map<std::string, std::vector<olc::Sprite*>> mapStates;

public:
	std::string sCurrentState;
	int nCurrentFrame = 0;
	float fTimeBetweenFrames = 0.1f;
	float fTimeCounter = 0.0f;
	

	void ChangeState(std::string s)
	{
		if (s != sCurrentState)
		{
			sCurrentState = s;
			fTimeCounter = 0;
			nCurrentFrame = 0;
		}
	}

	void Update(float fElapsedTime)
	{
		fTimeCounter += fElapsedTime;
		if (fTimeCounter >= fTimeBetweenFrames)
		{
			fTimeCounter -= fTimeBetweenFrames;
			nCurrentFrame++;
			if (nCurrentFrame >= mapStates[sCurrentState].size())
				nCurrentFrame = 0;
		}
	}

	void DrawSelf(olc::PixelGameEngine *pge, olc::GFX2D::Transform2D &t)
	{
		olc::GFX2D::DrawSprite(mapStates[sCurrentState][nCurrentFrame], t);
	}

};

// Override base class with your custom functionality
class Discovery : public olc::PixelGameEngine
{
public:
	Discovery()
	{
		sAppName = "Discovery - OLC CodeJam 2018";
	}

private:
	
	int nTileSizeX = 64;
	int nTileSizeY = 64;
	olc::TILE::Layer<olc::TILE::BasicTile>	layerWorld;
	olc::TILE::Layer<olc::TILE::BasicTile>	layerCollectibles;
	olc::TILE::Layer<olc::TILE::BasicTile>	layerJavid;
	olc::TILE::Atlas						atlasWorldMagetzub;
	olc::TILE::Atlas						atlasWorldJavid;
	olc::TILE::Atlas						atlasWorldHopson;
	olc::TILE::Atlas						atlasCollectibles;

	olc::ResourcePack rpPlayer;

	olc::Sprite *sprBackdropMagetzub;
	olc::Sprite *sprBackdropHopson;

	olc::Sprite *sprTitleScreen;
	olc::Sprite *sprStoryScreen;
	olc::Sprite *sprControlScreen;
	olc::Sprite *sprCreditScreen;
	olc::Sprite *sprCompleteScreen;
	
	float fBackdropScaleX = 1.0f;
	float fBackdropScaleY = 1.0f;

	olc::Sprite *sprMiniMap = nullptr;

	olc::Sprite *backBuff;

	
	
	cAnimator animPlayer;
	ControllerManager controller;

	float fPlayerPosX = 2.0f;
	float fPlayerPosY = 2.0f;
	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;
	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;
	bool bPlayerOnGround = false;
	bool bPlayerTouchingWall = false;
	bool bPlayerTouchingWallOld = false;
	bool bCollisionLeft = false;
	float fTimeToJump = 0.0f;
	float fTimeToJumpMax = 0.25f;
	float fFaceDir = 1.0f;
	int nFloppyDisks = 0;
	float fGameTime = 0.0f;
	float fGameTimeMultiplier = 1.0f;
	int nHopsonTokens = 0;
	int nJavidTokens = 0;

	int nKeys = 0;
	float fEffectTime = 0.0f;
	float fModeTime = 0.0f;

	bool bFire = false;

	list<pair<int, int>> listKeys;

	struct sBullet
	{
		float bx;
		float by;
		float vx;
		bool bRemove = false;
	};

	enum
	{
		MODE_MAGETZUB,
		MODE_JAVID,
		MODE_HOPSON,
		MODE_HUHLIG

	} nRenderMode;

	enum
	{
		EFFECT_NONE,
		EFFECT_UGLYSWEDISHFISH,
		EFFECT_SEDIT,
		EFFECT_BRANK,
		EFFECT_GORBIT,

	} nRenderEffect;


	enum
	{
		GS_LOADING,
		GS_TITLE,
		GS_STORY,
		GS_CREDITS,
		GS_GENERATE,
		GS_MAIN,
		GS_COMPLETE,
	} nGameState = GS_LOADING;

	int nChallengeMapSizeX = 8;
	int nChallengeMapSizeY = 8;
	int nChallengePathSizeX = 4; 
	int nChallengePathSizeY = 4;

	void CreateMaze(bool* &pMap, int nCellWidth, int nCellHeight, int nCellsX, int nCellsY, int &nMapWidth, int &nMapHeight)
	{
		int *pLevel = new int[nCellsX * nCellsY]{ 0 };

		enum
		{
			CELL_PATH_N = 0x01,
			CELL_PATH_E = 0x02,
			CELL_PATH_S = 0x04,
			CELL_PATH_W = 0x08,
			CELL_VISITED = 0x10,
		};

		std::stack<std::pair<int, int>> m_stack;


		auto offset = [&](int x, int y)
		{
			return (m_stack.top().second + y) * nCellsX + (m_stack.top().first + x);
		};

		m_stack.push(std::make_pair(0, 0));
		pLevel[0] = CELL_VISITED;
		int nVisitedCells = 1;

		// Do Maze Algorithm
		while (nVisitedCells < nCellsX * nCellsY)
		{
			// Step 1: Create a set of the unvisted neighbours

			std::vector<int> neighbours;

			// North Neighbour
			if (m_stack.top().second > 0 && (pLevel[offset(0, -1)] & CELL_VISITED) == 0)
				neighbours.push_back(0);
			// East neighbour
			if (m_stack.top().first < nCellsX - 1 && (pLevel[offset(1, 0)] & CELL_VISITED) == 0)
				neighbours.push_back(1);
			// South neighbour
			if (m_stack.top().second < nCellsY - 1 && (pLevel[offset(0, 1)] & CELL_VISITED) == 0)
				neighbours.push_back(2);
			// West neighbour
			if (m_stack.top().first > 0 && (pLevel[offset(-1, 0)] & CELL_VISITED) == 0)
				neighbours.push_back(3);


			// Are there any neighbours available?
			if (!neighbours.empty())
			{
				// Choose one available neighbour at random
				int next_cell_dir = neighbours[rand() % neighbours.size()];

				// Create a path between the neighbour and the current cell
				switch (next_cell_dir)
				{
				case 0: // North
					pLevel[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
					pLevel[offset(0, 0)] |= CELL_PATH_N;
					m_stack.push(std::make_pair((m_stack.top().first + 0), (m_stack.top().second - 1)));
					break;

				case 1: // East
					pLevel[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
					pLevel[offset(0, 0)] |= CELL_PATH_E;
					m_stack.push(std::make_pair((m_stack.top().first + 1), (m_stack.top().second + 0)));
					break;

				case 2: // South
					pLevel[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
					pLevel[offset(0, 0)] |= CELL_PATH_S;
					m_stack.push(std::make_pair((m_stack.top().first + 0), (m_stack.top().second + 1)));
					break;

				case 3: // West
					pLevel[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
					pLevel[offset(0, 0)] |= CELL_PATH_W;
					m_stack.push(std::make_pair((m_stack.top().first - 1), (m_stack.top().second + 0)));
					break;

				}

				nVisitedCells++;
			}
			else
			{
				m_stack.pop(); // Backtrack
			}
		}
		
		printf("M1 %d\n", m_stack.size());

		// Convert Maze into tile map
		nMapWidth = (nCellWidth + 1) * nCellsX + 1;
		nMapHeight = (nCellHeight + 1) * nCellsY + 1;
		pMap = new bool[nMapWidth * nMapHeight]{ 0 };
		printf("M1\n");

		// Draw Maze
		for (int x = 0; x < nCellsX; x++)
		{
			for (int y = 0; y < nCellsY; y++)
			{
				for (int py = 0; py < nCellHeight; py++)
					for (int px = 0; px < nCellWidth; px++)
					{
						if (pLevel[y * nCellsX + x] & CELL_VISITED)
							pMap[(y* (nCellHeight + 1) + py + 1) * nMapWidth + (x * (nCellWidth + 1) + px) + 1] = true;
						else
							pMap[(y* (nCellHeight + 1) + py + 1) * nMapWidth + (x * (nCellWidth + 1) + px + 1)] = false;
					}

				for (int p = 0; p < nCellWidth; p++)
					if (pLevel[y * nCellsX + x] & CELL_PATH_S)
						pMap[(y * (nCellHeight + 1) + nCellHeight + 1) * nMapWidth + (x * (nCellWidth + 1) + p + 1)] = true;

				for (int p = 0; p < nCellHeight; p++)
					if (pLevel[y * nCellsX + x] & CELL_PATH_E)
						pMap[(y * (nCellHeight + 1) + p + 1) * nMapWidth + (x * (nCellWidth + 1) + nCellWidth + 1)] = true;
			}
		}

		printf("M1\n");
		delete[] pLevel;
		printf("M1\n");
	}

	void CalculateFlowMap()
	{
		// Update Flow map

		int nFlowWidth = layerJavid.nLayerWidth;
		int nFlowHeight = layerJavid.nLayerHeight;

		// 1) Prepare it centered on player
		for (int x = 0; x < nFlowWidth; x++)
			for (int y = 0; y < nFlowHeight; y++)
			{
				layerJavid.GetTile(x, y)->id = 2;
				layerJavid.GetTile(x, y)->edge_id[0] = 0;

				if (x == 0 || x == (nFlowWidth - 1) || y == 0 || y == (nFlowHeight - 1))
					layerJavid.GetTile(x, y)->edge_id[0] = -1;
				else
					//layerJavid.GetTile(x, y)->exist = layerWorld.GetTile(x, y)->exist;
					layerJavid.GetTile(x, y)->edge_id[0] = layerWorld.GetTile(x, y)->exist ? -1 : 0;

				
			}

		

		list<tuple<int, int, int>> nodes;
		
		nodes.push_back({ listKeys.back().first,listKeys.back().second, 1 });

		while (!nodes.empty())
		{
			list<tuple<int, int, int>> new_nodes;

			// For each node in processing queue, set its count value, and add unmarked
			// neighbour nodes
			for (auto &n : nodes)
			{
				int x = get<0>(n);
				int y = get<1>(n);
				int d = get<2>(n);

				// Set distance for this node
				layerJavid.GetTile(x, y)->edge_id[0] = d;

				// Add neigbour nodes if unmarked
				if ((x + 1) < nFlowWidth && layerJavid.GetTile(x+1, y)->edge_id[0] == 0)
					new_nodes.push_back({ x + 1, y, d + 1 });
				if ((x - 1) >= 0 && layerJavid.GetTile(x - 1, y)->edge_id[0] == 0)
					new_nodes.push_back({ x - 1, y, d + 1});
				if ((y + 1) < nFlowHeight && layerJavid.GetTile(x, y+1)->edge_id[0] == 0)
					new_nodes.push_back({ x, y + 1, d + 1 });
				if ((y - 1) >= 0 && layerJavid.GetTile(x, y-1)->edge_id[0] == 0)
					new_nodes.push_back({ x, y - 1, d + 1 });
			}

			new_nodes.sort([&](const tuple<int, int, int> &n1, const tuple<int, int, int> &n2)
			{
				return (get<1>(n1) * layerJavid.nLayerWidth + get<0>(n1)) < (get<1>(n2) * layerJavid.nLayerWidth + get<0>(n2));
			});

			new_nodes.unique([&](const tuple<int, int, int> &n1, const tuple<int, int, int> &n2)
			{
				return (get<1>(n1) * layerJavid.nLayerWidth + get<0>(n1)) == (get<1>(n2) * layerJavid.nLayerWidth + get<0>(n2));
			});


			nodes.clear();
			nodes.insert(nodes.begin(), new_nodes.begin(), new_nodes.end());
		}

		for (int x = 0; x < nFlowWidth; x++)
		{
			for (int y = 0; y < nFlowHeight; y++)
			{
				if (layerJavid.GetTile(x, y)->edge_id[0] > 0)
				{
					int flow_xa, flow_xb, flow_ya, flow_yb;
					flow_xa = flow_xb = flow_ya = flow_yb = layerJavid.GetTile(x, y)->edge_id[0];

					if ((x + 1) < nFlowWidth && layerJavid.GetTile(x+1, y)->edge_id[0] > 0)
						flow_xb = layerJavid.GetTile(x + 1, y)->edge_id[0];

					if ((x - 1) >= 0 && layerJavid.GetTile(x-1, y)->edge_id[0] > 0)
						flow_xa = layerJavid.GetTile(x - 1, y)->edge_id[0];

					if ((y + 1) < nFlowHeight && layerJavid.GetTile(x, y+1)->edge_id[0] > 0)
						flow_yb = layerJavid.GetTile(x, y + 1)->edge_id[0];

					if ((y - 1) >= 0 && layerJavid.GetTile(x, y-1)->edge_id[0] > 0)
						flow_ya = layerJavid.GetTile(x, y - 1)->edge_id[0];

					float fdx = (float)(flow_xa - flow_xb);
					float fdy = (float)(flow_ya - flow_yb);
					float r = 1.0f / sqrtf(fdx * fdx + fdy * fdy);

					if (fabs(fdx) > fabs(fdy))
					{
						if (fdx > 0)
						{
							layerJavid.GetTile(x, y)->exist = true;
							layerJavid.GetTile(x, y)->id = 8;
						}

						if (fdx < 0)
						{
							layerJavid.GetTile(x, y)->exist = true;
							layerJavid.GetTile(x, y)->id = 10;
						}
					}
					else
					{
						if (fdy > 0)
						{
							layerJavid.GetTile(x, y)->exist = true;
							layerJavid.GetTile(x, y)->id = 9;
						}

						if (fdy < 0)
						{
							layerJavid.GetTile(x, y)->exist = true;
							layerJavid.GetTile(x, y)->id = 7;
						}
					}
				}
			}
		}
	}

	void CreateBorderMap()
	{
		// Use OLC Standard Bordered tile atlas
		for (int x = 0; x < layerWorld.nLayerWidth; x++)
		{
			for (int y = 0; y < layerWorld.nLayerHeight; y++)
			{
				int s = 0;
				auto a = [&](int i, int j)
				{
					return (layerWorld.GetTile(i, j) && layerWorld.GetTile(i, j)->exist);
				};

				if (a(x, y))
				{
					s |= a(x - 1, y + 0) ? 1 : 0; s <<= 1;
					s |= a(x - 1, y + 1) ? 1 : 0; s <<= 1;
					s |= a(x + 0, y + 1) ? 1 : 0; s <<= 1;
					s |= a(x + 1, y + 1) ? 1 : 0; s <<= 1;
					s |= a(x + 1, y + 0) ? 1 : 0; s <<= 1;
					s |= a(x + 1, y - 1) ? 1 : 0; s <<= 1;
					s |= a(x - 0, y - 1) ? 1 : 0; s <<= 1;
					s |= a(x - 1, y - 1) ? 1 : 0;

					int ix = s % 16;
					int iy = s / 16;
					layerWorld.GetTile(x, y)->id = (iy * 256 + 64) + (ix * 4) + 1;
				}
				else
					layerWorld.GetTile(x, y)->id = 0;
			}
		}
	}

public:

	int sndHelperChange;
	int sndJump;
	int sndWall;
	int sndGithubPatch;
	int sndKeyCollect;
	int sndDiskCollect;
	int sndThump;
	int sndTheme;



	bool OnUserCreate() override
	{
		controller.Initialize();
		backBuff = new olc::Sprite(ScreenWidth(), ScreenHeight());
		
		olc::SOUND::InitialiseAudio();
		
		animPlayer.ChangeState("idle");
		return true;
	}

	bool OnUserDestroy()
	{
		olc::SOUND::DestroyAudio();
		return true;
	}

	bool bFirstFrameLoading = true;

	bool GameState_Loading(float fElapsedTime)
	{
		
		
		
		if (bFirstFrameLoading)
		{
			Clear(olc::BLACK);
			DrawString(60, 240, "- Loading, Please Wait - ", olc::WHITE, 2);
			bFirstFrameLoading = false;
			
			return true;
		}

		rpPlayer.LoadPack("./discres/savingsedit.olcdat");

		sndHelperChange = olc::SOUND::LoadAudioSample("E:\\linshare\\olcSimpleGameEngine\\discres\\PP_Negative_Trigger_1_2.wav", &rpPlayer);
		sndJump = olc::SOUND::LoadAudioSample("E:\\linshare\\olcSimpleGameEngine\\discres\\PP_Jump_1_5.wav", &rpPlayer);
		sndWall = olc::SOUND::LoadAudioSample("E:\\linshare\\olcSimpleGameEngine\\discres\\LQ_Back_Button.wav", &rpPlayer);
		sndThump = olc::SOUND::LoadAudioSample("E:\\linshare\\olcSimpleGameEngine\\discres\\PP_Small_Impact_1_1.wav", &rpPlayer);
		sndGithubPatch = olc::SOUND::LoadAudioSample("E:\\linshare\\olcSimpleGameEngine\\discres\\PP_Skill_Unlock.wav", &rpPlayer);
		sndKeyCollect = olc::SOUND::LoadAudioSample("E:\\linshare\\olcSimpleGameEngine\\discres\\FA_Special_Item_2_1.wav", &rpPlayer);
		sndDiskCollect = olc::SOUND::LoadAudioSample("E:\\linshare\\olcSimpleGameEngine\\discres\\FA_Funny_Impact_1_3.wav", &rpPlayer);
		sndTheme = olc::SOUND::LoadAudioSample("E:\\linshare\\olcSimpleGameEngine\\discres\\Funky Chill 2 loop.wav", &rpPlayer);

		olc::SOUND::PlaySample(sndHelperChange);

		// Load Game Resources
		// Define OLC Standard atlas
		//atlasWorldMagetzub.Create(new olc::Sprite());// new olc::Sprite("thruster_landscape_all3_neo.png"));
		//atlasWorldMagetzub.sprTileSheet->SaveToPGESprFile("discres\\discovery_magetzub_64x64.olcspr");
		//atlasWorldMagetzub.sprTileSheet->LoadFromPGESprFile("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_magetzub_64x64.olcspr");
		atlasWorldMagetzub.Create(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_magetzub_64x64.olcspr", &rpPlayer));

		//atlasWorldJavid.Create(new olc::Sprite());//new olc::Sprite("discovery_javid_64x64.png"));
		//atlasWorldJavid.sprTileSheet->SaveToPGESprFile("discres\\discovery_javid_64x64.olcspr");
		//atlasWorldJavid.sprTileSheet->LoadFromPGESprFile("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_javid_64x64.olcspr");
		atlasWorldJavid.Create(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_javid_64x64.olcspr", &rpPlayer));

		//atlasWorldHopson.Create(new olc::Sprite());//new olc::Sprite("discovery_hopson_64x64.png"));
		//atlasWorldHopson.sprTileSheet->SaveToPGESprFile("discres\\discovery_hopson_64x64.olcspr");
		atlasWorldHopson.Create(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_hopson_64x64.olcspr", &rpPlayer));
		//atlasWorldHopson.sprTileSheet->LoadFromPGESprFile("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_hopson_64x64.olcspr");
		for (int i = 0; i < 64; i++)
			for (int j = 0; j < 64; j++)
			{
				//atlasWorld.location.emplace_back(j * 16, i * 16, 16, 16);
				atlasWorldMagetzub.location.emplace_back(j * 64, i * 64, 64, 64);
				atlasWorldJavid.location.emplace_back(j * 64, i * 64, 64, 64);
				atlasWorldHopson.location.emplace_back(j * 64, i * 64, 64, 64);
			}

		//atlasCollectibles.Create(new olc::Sprite());// "discovery_collect1.png"));
		//atlasCollectibles.sprTileSheet->SaveToPGESprFile("discres\\discovery_collectibles.olcspr");
		//atlasCollectibles.sprTileSheet->LoadFromPGESprFile("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_collectibles.olcspr");
		atlasCollectibles.Create(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_collectibles.olcspr", &rpPlayer));
		atlasCollectibles.location.emplace_back(0, 0, 16, 16);  // Blank
		atlasCollectibles.location.emplace_back(16, 0, 16, 16); // Disk
		atlasCollectibles.location.emplace_back(32, 0, 16, 16); // Exit
		atlasCollectibles.location.emplace_back(48, 0, 16, 16); // Key
		atlasCollectibles.location.emplace_back(64, 0, 16, 16); // Javid
		atlasCollectibles.location.emplace_back(80, 0, 16, 16); // Github
		atlasCollectibles.location.emplace_back(96, 0, 16, 16); // Hopson
		atlasCollectibles.location.emplace_back(112, 0, 16, 16); // Up
		atlasCollectibles.location.emplace_back(128, 0, 16, 16); // Right
		atlasCollectibles.location.emplace_back(144, 0, 16, 16); // Down
		atlasCollectibles.location.emplace_back(160, 0, 16, 16); // Left


		

		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_000.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_001.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_002.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_003.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_004.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_005.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_006.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_007.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_008.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_009.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_010.olcspr", &rpPlayer));
		animPlayer.mapStates["idle"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Idle_011.olcspr", &rpPlayer));

		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_000.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_001.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_002.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_003.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_004.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_005.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_006.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_007.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_008.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_009.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_010.olcspr", &rpPlayer));
		animPlayer.mapStates["idle_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_IdleFire_011.olcspr", &rpPlayer));

		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_000.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_001.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_002.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_003.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_004.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_005.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_006.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_007.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_008.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_009.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_010.olcspr", &rpPlayer));
		animPlayer.mapStates["run"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Run_011.olcspr", &rpPlayer));

		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_000.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_001.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_002.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_003.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_004.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_005.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_006.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_007.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_008.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_009.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_010.olcspr", &rpPlayer));
		animPlayer.mapStates["run_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_RunFire_011.olcspr", &rpPlayer));

		animPlayer.mapStates["jump"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Jump_000.olcspr", &rpPlayer));
		animPlayer.mapStates["jump"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Jump_001.olcspr", &rpPlayer));
		animPlayer.mapStates["jump"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Jump_002.olcspr", &rpPlayer));
		animPlayer.mapStates["jump"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Jump_003.olcspr", &rpPlayer));
		animPlayer.mapStates["jump"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Jump_004.olcspr", &rpPlayer));
		animPlayer.mapStates["jump"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Jump_005.olcspr", &rpPlayer));

		animPlayer.mapStates["fall"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Fall_000.olcspr", &rpPlayer));
		animPlayer.mapStates["fall"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Fall_001.olcspr", &rpPlayer));
		animPlayer.mapStates["fall"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Fall_002.olcspr", &rpPlayer));
		animPlayer.mapStates["fall"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Fall_003.olcspr", &rpPlayer));
		animPlayer.mapStates["fall"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Fall_004.olcspr", &rpPlayer));
		animPlayer.mapStates["fall"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_Fall_005.olcspr", &rpPlayer));

		animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_AirFire_000.olcspr", &rpPlayer));
		animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_AirFire_001.olcspr", &rpPlayer));
		animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_AirFire_002.olcspr", &rpPlayer));
		animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_AirFire_003.olcspr", &rpPlayer));
		animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_AirFire_004.olcspr", &rpPlayer));
		animPlayer.mapStates["air_fire"].push_back(new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\Player_AirFire_005.olcspr", &rpPlayer));

		//sprBackdropMagetzub = new olc::Sprite("discovery_magetzub_bg.png");
		//sprBackdropHopson = new olc::Sprite("discovery_hopson_bg.png");
		//sprTitleScreen = new olc::Sprite("discovery_titlescreen.png");
		//sprStoryScreen = new olc::Sprite("discovery_story.png");
		//sprCreditScreen = new olc::Sprite("discovery_credits.png");
		//sprControlScreen = new olc::Sprite("discovery_story2.png");
		//sprCompleteScreen = new olc::Sprite("discovery_title_bg.png");

		//sprBackdropMagetzub->SaveToPGESprFile("discres\\discovery_magetzub_bg.olcspr");
		//sprBackdropHopson->SaveToPGESprFile("discres\\discovery_hopson_bg.olcspr");
		//sprTitleScreen->SaveToPGESprFile("discres\\discovery_titlescreen.olcspr");
		//sprStoryScreen->SaveToPGESprFile("discres\\discovery_story.olcspr");
		//sprCreditScreen->SaveToPGESprFile("discres\\discovery_credits.olcspr");
		//sprControlScreen->SaveToPGESprFile("discres\\discovery_story2.olcspr");
		//sprCompleteScreen->SaveToPGESprFile("discres\\discovery_title_bg.olcspr");

		sprBackdropMagetzub = new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_magetzub_bg.olcspr", &rpPlayer);
		sprBackdropHopson = new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_hopson_bg.olcspr", &rpPlayer);
		sprTitleScreen = new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_titlescreen.olcspr", &rpPlayer);
		sprStoryScreen = new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_story.olcspr", &rpPlayer);
		sprCreditScreen = new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_credits.olcspr", &rpPlayer);
		sprControlScreen = new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_story2.olcspr", &rpPlayer);
		sprCompleteScreen = new olc::Sprite("E:\\linshare\\olcSimpleGameEngine\\discres\\discovery_title_bg.olcspr", &rpPlayer);

		rpPlayer.ClearPack();

		nGameState = GS_TITLE;

		return true;
	}

	int nTitleSelection = 0;
	int nLevelSeed = 1;
	int nLevelSize = 2;

	bool GameState_Title(float fElapsedTime)
	{
		controller.Update(fElapsedTime);

		DrawSprite(0, 0, sprTitleScreen);

		DrawString(300, 280, "Story & Help", nTitleSelection == 0 ? olc::WHITE : olc::YELLOW, 2);
		DrawString(300, 310, "Credits", nTitleSelection == 1 ? olc::WHITE : olc::YELLOW, 2);
		DrawString(300, 340, "Level: " + to_string(nLevelSeed), nTitleSelection == 2 ? olc::WHITE : olc::YELLOW, 2);
		DrawString(300, 370, "Size: " + to_string((int)pow(2, (nLevelSize+2))), nTitleSelection == 3 ? olc::WHITE : olc::YELLOW, 2);
		DrawString(300, 400, "Start", nTitleSelection == 4 ? olc::WHITE : olc::YELLOW, 2);

		if (GetKey(olc::Key::UP).bPressed || controller.GetButton(UP).bPressed)
		{
			nTitleSelection--;
			if (nTitleSelection < 0) nTitleSelection = 4;
			olc::SOUND::PlaySample(sndThump);
		}

		if (GetKey(olc::Key::DOWN).bPressed || controller.GetButton(DOWN).bPressed)
		{
			nTitleSelection++;
			if (nTitleSelection > 4 ) nTitleSelection = 0;
			olc::SOUND::PlaySample(sndThump);
		}

		if (GetKey(olc::Key::LEFT).bPressed || controller.GetButton(LEFT).bPressed)
		{
			if (nTitleSelection == 2)
			{
				nLevelSeed--;
				if (nLevelSeed < 0) nLevelSeed = 999;
			}

			if (nTitleSelection == 3)
			{
				nLevelSize--;
				if (nLevelSize < 1) nLevelSize = 3;
			}
		}

		if (GetKey(olc::Key::RIGHT).bPressed || controller.GetButton(RIGHT).bPressed)
		{
			if (nTitleSelection == 2)
			{
				nLevelSeed++;
				if (nLevelSeed > 999) nLevelSeed = 0;
			}

			if (nTitleSelection == 3)
			{
				nLevelSize++;
				if (nLevelSize > 3) nLevelSize = 1;
			}
		}

		if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		{
			if (nTitleSelection == 0) nGameState = GS_STORY;
			if (nTitleSelection == 1) nGameState = GS_CREDITS;

			if (nTitleSelection == 4) nGameState = GS_GENERATE;
		}

		return true;
	}

	bool GameState_Generating(float fElapsedTime)
	{
		fPlayerPosX = 2.0f;
		fPlayerPosY = 2.0f;
		fPlayerVelX = 0.0f;
		fPlayerVelY = 0.0f;
		fCameraPosX = 0.0f;
		fCameraPosY = 0.0f;
		bPlayerOnGround = false;
		bPlayerTouchingWall = false;
		bPlayerTouchingWallOld = false;
		bCollisionLeft = false;
		fTimeToJump = 0.0f;
		fTimeToJumpMax = 0.25f;
		fFaceDir = 1.0f;
		nFloppyDisks = 0;
		fGameTime = 0.0f;
		fGameTimeMultiplier = 1.0f;
		nHopsonTokens = 0;
		nJavidTokens = 0;
		nKeys = 0;
		fEffectTime = 0.0f;
		fModeTime = 0.0f;
		
		nChallengeMapSizeX = (int)pow(2, nLevelSize + 2);
		nChallengeMapSizeY = (int)pow(2, nLevelSize + 2);
		srand(nLevelSeed);
		
		
		// Generate a boolean maze
		bool *bTileMaze = nullptr;
		int nMapWidth, nMapHeight;
		printf("D\n");
		CreateMaze(bTileMaze, nChallengePathSizeX - 1, nChallengePathSizeY - 1, nChallengeMapSizeX, nChallengeMapSizeY, nMapWidth, nMapHeight);

		if (sprMiniMap != nullptr) delete sprMiniMap;
		sprMiniMap = new olc::Sprite(nChallengeMapSizeX, nChallengeMapSizeY);
		printf("D\n");
		// Create tilemap to match dimensions of generated maze
		layerWorld.Create(nMapWidth, nMapHeight, nTileSizeX, nTileSizeY);
		layerCollectibles.Create(nMapWidth, nMapHeight, nTileSizeX, nTileSizeY);
		layerJavid.Create(nMapWidth, nMapHeight, nTileSizeX, nTileSizeY);

		printf("D\n");
		// Transfer over boolean maze to tilemap
		for (int x = 0; x < layerWorld.nLayerWidth; x++)
			for (int y = 0; y < layerWorld.nLayerHeight; y++)
			{
				layerWorld.GetTile(x, y)->exist = !bTileMaze[y*layerWorld.nLayerWidth + x];
			}

		printf("D\n");
		CreateBorderMap();

		printf("D\n");
		delete[] bTileMaze;
		printf("D\n");
		listKeys.clear();
		printf("D\n");
		// Add 100 Drops
		for (int i = 0; i < 136; i++)
		{
			bool bDone = false;
			do
			{
				int x = rand() % layerWorld.nLayerWidth;
				int y = rand() % layerWorld.nLayerHeight;
				if (!layerWorld.GetTile(x, y)->exist && !layerCollectibles.GetTile(x, y)->exist)
				{

					layerCollectibles.GetTile(x, y)->exist = true;
					if (i < 100)	layerCollectibles.GetTile(x, y)->id = 1; // Place disks
					if (i == 100)  // Place Exit
					{
						layerCollectibles.GetTile(x, y)->id = 2; listKeys.push_back(make_pair(x, y));
					}
					if (i > 100 && i <= 104)// Place Keys
					{
						layerCollectibles.GetTile(x, y)->id = 3; listKeys.push_back(make_pair(x, y));
					}
					if (i > 105 && i <= 125)	layerCollectibles.GetTile(x, y)->id = 5; // Place Githubs
					if (i > 125 && i <= 130)	layerCollectibles.GetTile(x, y)->id = 4; // Place Javids
					if (i > 130 && i <= 135)	layerCollectibles.GetTile(x, y)->id = 6; // Place Hopsons
					bDone = true;
				}

			} while (!bDone);
		}
		
		olc::SOUND::PlaySample(sndTheme, true);
		nGameState = GS_MAIN;
		printf("Entering Main\n");

		return true;
	}

	bool GameState_Main(float fElapsedTime)
	{
		fGameTime += fElapsedTime * fGameTimeMultiplier;

		controller.Update(fElapsedTime);
		animPlayer.Update(fElapsedTime);

		bool bStartWallJump = bPlayerTouchingWall & !bPlayerTouchingWallOld;
		bPlayerTouchingWallOld = bPlayerTouchingWall;

		if (bStartWallJump)
		{
			fTimeToJump = fTimeToJumpMax;
			olc::SOUND::PlaySample(sndWall);
		}

		if (fTimeToJump >= 0.0f)
			fTimeToJump -= fElapsedTime;

		bPlayerTouchingWall = false;

		// Handle Input
		if (IsFocused())
		{
			/*if (GetKey(olc::Key::F1).bReleased) nRenderMode = MODE_MAGETZUB;
			if (GetKey(olc::Key::F2).bReleased) {
				nRenderMode = MODE_JAVID; fModeTime = 500.0f; CalculateFlowMap();
			}
			if (GetKey(olc::Key::F3).bReleased) nRenderMode = MODE_HOPSON;


			if (GetKey(olc::Key::F5).bReleased) nRenderEffect = EFFECT_NONE;
			if (GetKey(olc::Key::F6).bReleased) nRenderEffect = EFFECT_UGLYSWEDISHFISH;
			if (GetKey(olc::Key::F7).bReleased) nRenderEffect = EFFECT_GORBIT;
			if (GetKey(olc::Key::F8).bReleased) nRenderEffect = EFFECT_SEDIT;*/

			/*if (GetKey(olc::Key::UP).bHeld || controller.GetButton(UP).bHeld)
			{
				fPlayerVelY = -2.0f;
			}

			if (GetKey(olc::Key::DOWN).bHeld || controller.GetButton(DOWN).bHeld)
			{
				fPlayerVelY = 2.0f;
			}*/

			if (GetKey(olc::Key::LEFT).bHeld || controller.GetButton(LEFT).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? -25.0f : -15.0f) *fElapsedTime;
				fFaceDir = -1.0f;
			}

			if (GetKey(olc::Key::RIGHT).bHeld || controller.GetButton(RIGHT).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? 25.0f : 15.0f) *fElapsedTime;
				fFaceDir = +1.0f;
			}

			if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
			{
				if (bPlayerOnGround)
				{
					fPlayerVelY = -12.0f;
					olc::SOUND::PlaySample(sndJump);
				}

				if (fTimeToJump >= 0.0f)
				{
					if (fPlayerVelX < 0 && !bCollisionLeft)
					{
						fPlayerVelY = -12.0f;
						olc::SOUND::PlaySample(sndJump);
					}

					if (fPlayerVelX > 0 && bCollisionLeft)
					{
						fPlayerVelY = -12.0f;
						olc::SOUND::PlaySample(sndJump);
					}
				}
			}


			bFire = false;
			if (GetKey(olc::Key::A).bPressed || controller.GetButton(X).bPressed)
			{
				//bFire = true;

				if (nRenderMode == MODE_HOPSON)
				{
					if (fFaceDir < 0)
					{
						int bx = fPlayerPosX;
						int by = fPlayerPosY + 0.5f;
						if (bx > 0 && bx < (layerWorld.nLayerWidth - 1) && by > 0 && by < (layerWorld.nLayerHeight - 1) && layerWorld.GetTile(bx, by)->exist)
						{
							layerWorld.GetTile(bx, by)->exist = false;
							CreateBorderMap();
						}
					}
					else
					{
						int bx = fPlayerPosX + 1.0f;
						int by = fPlayerPosY + 0.5f;
						if (bx > 0 && bx < (layerWorld.nLayerWidth - 1) && by > 0 && by < (layerWorld.nLayerHeight - 1) && layerWorld.GetTile(bx, by)->exist)
						{
							layerWorld.GetTile(bx, by)->exist = false;
							CreateBorderMap();
						}

					}
				}
			}

			if (GetKey(olc::Key::J).bPressed || controller.GetButton(Y).bPressed)
			{
				if (nRenderMode != MODE_JAVID && nJavidTokens > 0)
				{
					nJavidTokens--;
					nRenderMode = MODE_JAVID; 
					fModeTime = 60.0f; 
					CalculateFlowMap();
					olc::SOUND::PlaySample(sndHelperChange);
				}
			}

			if (GetKey(olc::Key::H).bPressed || controller.GetButton(B).bPressed)
			{
				if (nRenderMode != MODE_HOPSON && nHopsonTokens > 0)
				{
					nHopsonTokens--;
					nRenderMode = MODE_HOPSON;
					fModeTime = 30.0f;	
					olc::SOUND::PlaySample(sndHelperChange);
				}
			}
		}


		fPlayerVelY += 20.0f * fElapsedTime;

		if (bPlayerOnGround)
		{

			fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
			if (fabs(fPlayerVelX) < 0.01f)
			{
				fPlayerVelX = 0.0f;
				animPlayer.ChangeState(bFire ? "idle_fire" : "idle");
			}
			else
			{
				animPlayer.ChangeState(bFire ? "run_fire" : "run");
			}

		}
		else
		{
			if (!bFire)
			{
				if (fPlayerVelY < 0)
					animPlayer.ChangeState("jump");
				else
					animPlayer.ChangeState("fall");
			}
			else
				animPlayer.ChangeState("air_fire");
		}


		if (layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id == 1) // Disk
		{
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id = 0;
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->exist = false;
			nFloppyDisks++;
			olc::SOUND::PlaySample(sndDiskCollect);
		}

		if (layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id == 2) // Exit
		{
			if (nKeys == 4)
			{
				// Game Completed
				nGameState = GS_COMPLETE;
			}
		}

		if (layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id == 3) // Keys
		{
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id = 0;
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->exist = false;
			nKeys++;
			

			// Remove key from list
			listKeys.remove_if([&](pair<int, int> &p) {return p.first == ((int)(fPlayerPosX + 0.5f)) && p.second == ((int)(fPlayerPosY + 0.5f)); });
			CalculateFlowMap();
			olc::SOUND::PlaySample(sndKeyCollect);
		}

		if (layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id == 4) // Javid
		{
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id = 0;
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->exist = false;
			nJavidTokens++;
			
			//fModeTime = 30.0f;
			//nRenderMode = MODE_JAVID;

			//if (nRenderMode == MODE_JAVID)
			//{
			//	// Javid is helpful and likes to simplify so help out the user
			//	CalculateFlowMap();
			//}
			olc::SOUND::PlaySample(sndKeyCollect);
		}

		if (layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id == 6) // Hopson
		{
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id = 0;
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->exist = false;
			nHopsonTokens++;

			/*fModeTime = 30.0f;
			nRenderMode = MODE_HOPSON;*/
			olc::SOUND::PlaySample(sndKeyCollect);
		}

		if (layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id == 5) // Github
		{
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->id = 0;
			layerCollectibles.GetTile(fPlayerPosX + 0.5f, fPlayerPosY + 0.5f)->exist = false;
			fEffectTime = 10.0f;
			switch (rand() % 4)
			{
			case 0: nRenderEffect = EFFECT_UGLYSWEDISHFISH; break;
			case 1: nRenderEffect = EFFECT_SEDIT; break;
			case 2: nRenderEffect = EFFECT_GORBIT; break;
			case 3: nRenderEffect = EFFECT_BRANK; break;
			}
			fGameTimeMultiplier = 0.75f;
			olc::SOUND::PlaySample(sndGithubPatch);
		}

		if (fEffectTime > 0.0f)
		{
			fEffectTime -= fElapsedTime;
		}
		else
		{
			nRenderEffect = EFFECT_NONE;
			fGameTimeMultiplier = 1.0f;
		}


		if (fModeTime > 0.0f)
		{
			fModeTime -= fElapsedTime;
		}
		else
		{
			nRenderMode = MODE_MAGETZUB;
			//olc::SOUND::PlaySample(sndHelperChange);
		}


		if (fPlayerVelX > 10.0f)
			fPlayerVelX = 10.0f;

		if (fPlayerVelX < -10.0f)
			fPlayerVelX = -10.0f;

		if (fPlayerVelY > 100.0f)
			fPlayerVelY = 100.0f;

		if (fPlayerVelY < -100.0f)
			fPlayerVelY = -100.0f;

		sprMiniMap->SetPixel(fPlayerPosX / nChallengePathSizeX, fPlayerPosY / nChallengePathSizeY, olc::GREEN);

		float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

		sprMiniMap->SetPixel(fNewPlayerPosX / nChallengePathSizeX, fNewPlayerPosY / nChallengePathSizeY, olc::WHITE);

		// Collision
		float fOffset = 0.2f;
		if (fPlayerVelX <= 0)
		{
			if (layerWorld.GetTile(fNewPlayerPosX + 0.0f + fOffset, fPlayerPosY + 0.0f)->exist || layerWorld.GetTile(fNewPlayerPosX + 0.0f + fOffset, fPlayerPosY + 0.9f)->exist)
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + 1 - fOffset;
				fPlayerVelX = 0;
				bPlayerTouchingWall = true;
				bCollisionLeft = true;
			}
		}
		else
		{
			if (layerWorld.GetTile(fNewPlayerPosX + 1.0f - fOffset, fPlayerPosY + 0.0f)->exist || layerWorld.GetTile(fNewPlayerPosX + 1.0f - fOffset, fPlayerPosY + 0.9f)->exist)
			{
				fNewPlayerPosX = (int)fNewPlayerPosX + fOffset;
				fPlayerVelX = 0;
				bPlayerTouchingWall = true;
				bCollisionLeft = false;
			}
		}

		bPlayerOnGround = false;
		if (fPlayerVelY <= 0)
		{
			if (layerWorld.GetTile(fNewPlayerPosX + 0.0f + fOffset, fNewPlayerPosY)->exist || layerWorld.GetTile(fNewPlayerPosX + 0.9f - fOffset, fNewPlayerPosY)->exist)
			{
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}
		else
		{
			if (layerWorld.GetTile(fNewPlayerPosX + 0.0f + fOffset, fNewPlayerPosY + 1.0f)->exist || layerWorld.GetTile(fNewPlayerPosX + 0.9f - fOffset, fNewPlayerPosY + 1.0f)->exist)
			{
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				bPlayerOnGround = true;

			}
		}

		fPlayerPosX = fNewPlayerPosX;
		fPlayerPosY = fNewPlayerPosY;

		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;

		int nVisibleTilesX = ScreenWidth() / nTileSizeX;
		int nVisibleTilesY = ScreenHeight() / nTileSizeY;

		// Calculate Top-Leftmost visible tile
		float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
		float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;


		// Clamp camera to game boundaries
		if (fOffsetX < 0) fOffsetX = 0;
		if (fOffsetY < 0) fOffsetY = 0;
		if (fOffsetX > layerWorld.nLayerWidth - nVisibleTilesX) fOffsetX = layerWorld.nLayerWidth - nVisibleTilesX;
		if (fOffsetY > layerWorld.nLayerHeight - nVisibleTilesY) fOffsetY = layerWorld.nLayerHeight - nVisibleTilesY;

		



		SetDrawTarget(backBuff);
		//Clear(olc::VERY_DARK_BLUE);


		if (nRenderMode == MODE_MAGETZUB)
		{
			fBackdropScaleX = (float)(sprBackdropMagetzub->width - ScreenWidth()) / (float)((layerWorld.nLayerWidth) + (float)nVisibleTilesX);
			fBackdropScaleY = (float)(sprBackdropMagetzub->height - ScreenHeight()) / (float)((layerWorld.nLayerHeight) + (float)nVisibleTilesY);
			DrawPartialSprite(0, 0, sprBackdropMagetzub, fOffsetX  * fBackdropScaleX, fOffsetY * fBackdropScaleY, ScreenWidth(), ScreenHeight());


			SetPixelMode(olc::Pixel::ALPHA);
			olc::TILE::DrawLayer(layerWorld, atlasWorldMagetzub, fOffsetX, fOffsetY, nVisibleTilesX + 1, nVisibleTilesY + 1, 1);
			olc::TILE::DrawLayer(layerCollectibles, atlasCollectibles, fOffsetX, fOffsetY, nVisibleTilesX + 1, nVisibleTilesY + 1, 3);
			SetPixelMode(olc::Pixel::NORMAL);
		}

		if (nRenderMode == MODE_HOPSON)
		{
			fBackdropScaleX = (float)(sprBackdropHopson->width - ScreenWidth()) / (float)((layerWorld.nLayerWidth) + (float)nVisibleTilesX);
			fBackdropScaleY = (float)(sprBackdropHopson->height - ScreenHeight()) / (float)((layerWorld.nLayerHeight) + (float)nVisibleTilesY);
			DrawPartialSprite(0, 0, sprBackdropHopson, fOffsetX * fBackdropScaleX, fOffsetY * fBackdropScaleY, ScreenWidth(), ScreenHeight());


			SetPixelMode(olc::Pixel::ALPHA);
			olc::TILE::DrawLayer(layerWorld, atlasWorldHopson, fOffsetX, fOffsetY, nVisibleTilesX + 1, nVisibleTilesY + 1, 1);
			olc::TILE::DrawLayer(layerCollectibles, atlasCollectibles, fOffsetX, fOffsetY, nVisibleTilesX + 1, nVisibleTilesY + 1, 3);
			SetPixelMode(olc::Pixel::NORMAL);
		}

		if (nRenderMode == MODE_JAVID)
		{
			Clear(olc::BLACK);
			SetPixelMode(olc::Pixel::ALPHA);
			olc::TILE::DrawLayer(layerWorld, atlasWorldJavid, fOffsetX, fOffsetY, nVisibleTilesX + 1, nVisibleTilesY + 1, 1);

			olc::TILE::DrawLayer(layerJavid, atlasCollectibles, fOffsetX, fOffsetY, nVisibleTilesX + 1, nVisibleTilesY + 1, 3);

			olc::TILE::DrawLayer(layerCollectibles, atlasCollectibles, fOffsetX, fOffsetY, nVisibleTilesX + 1, nVisibleTilesY + 1, 3);
			SetPixelMode(olc::Pixel::NORMAL);
		}

		// Draw Player

		olc::GFX2D::Transform2D t;
		t.Translate(-220.0f, 174.0f);
		t.Scale(fFaceDir *  0.25f, 0.25f);

		t.Translate((fPlayerPosX - fOffsetX) * nTileSizeX + 32, (fPlayerPosY - fOffsetY) * nTileSizeY - 48);

		SetPixelMode(olc::Pixel::ALPHA);
		animPlayer.DrawSelf(this, t);
		SetPixelMode(olc::Pixel::NORMAL);


		//this_thread::sleep_for(20ms);

		string s = "Disks: " + to_string(nFloppyDisks) + "/100\nJx9: " + to_string(nJavidTokens) + " Hop: " + to_string(nHopsonTokens) + "\nKeys: " + to_string(nKeys) + " /4";



		DrawString(11, 11, s, olc::BLACK, 2);
		DrawString(9, 9, s, olc::YELLOW, 2);

		SetPixelBlend(0.5f);
		SetPixelMode(olc::Pixel::ALPHA);
		DrawSprite(10, ScreenHeight() - (sprMiniMap->height + 4) * 3, sprMiniMap, 3);
		SetPixelMode(olc::Pixel::NORMAL);
		SetPixelBlend(1.0f);

		//Draw(10 + (fNewPlayerPosX / nChallengePathSizeX), ScreenHeight() - sprMiniMap->height - 8 + (fNewPlayerPosY / nChallengePathSizeY), olc::YELLOW);

		//DrawString(10, ScreenHeight() - (sprMiniMap->height - 16) * 2, "Map", olc::BLACK, 2);
		//DrawString(9,  ScreenHeight() - (sprMiniMap->height - 17) * 2, "Map", olc::YELLOW, 2);

		// Draw Time Information
		int minutes = (int)fGameTime / 60;
		int seconds = (int)fGameTime - (minutes * 60);
		string sMinutes = (minutes <= 9 ? "0" : "") + to_string(minutes);
		string sSeconds = (seconds <= 9 ? "0" : "") + to_string(seconds);
		DrawString(ScreenWidth() - 158, 12, sMinutes + ":" + sSeconds, olc::BLACK, 3);
		DrawString(ScreenWidth() - 160, 10, sMinutes + ":" + sSeconds, olc::YELLOW, 3);

		if (fGameTimeMultiplier != 1.0f)
		{
			DrawString(ScreenWidth() - 158, 50, "x0.75", olc::BLACK, 4);
			DrawString(ScreenWidth() - 160, 53, "x0.75", olc::YELLOW, 4);
		}

		SetDrawTarget(nullptr);


		// UglySwedishFisk Mode
		if (nRenderEffect == EFFECT_UGLYSWEDISHFISH)
		{
			int sparkles = 100 / fElapsedTime;
			for (int i = 0; i < sparkles; i++)
			{
				int x = rand() % ScreenWidth();
				int y = rand() % ScreenHeight();
				Draw(x, y, backBuff->GetPixel(x, y));
			}
		}

		if (nRenderEffect == EFFECT_GORBIT)
		{
			int sparkles = 10 / fElapsedTime;
			for (int i = 0; i < sparkles; i++)
			{
				int x = rand() % 32;
				int y = rand() % 32;
				DrawPartialSprite(x * 32, y * 32, backBuff, x * 32, y * 32, 32, 32);
			}
		}


		if (nRenderEffect == EFFECT_SEDIT)
		{
			SetPixelMode([](int x, int y, const olc::Pixel &s, const olc::Pixel &d)
			{
				olc::Pixel p = s;
				if (y % 2)
				{
					float c = ((float)s.r * 1.5f + (float)s.g * 1.5f + (float)s.b * 1.5f) / 3.0f;
					p.r = c;
					p.g = c;
					p.b = c;
				}
				return p;
			});

			DrawSprite(0, 0, backBuff);
			
			SetPixelMode(olc::Pixel::NORMAL);
		}

		if (nRenderEffect == EFFECT_BRANK)
		{
			for (int i = 0; i < 4; i++)
			{
				int sx = rand() % 2;
				int sy = rand() % 2;
				int tx = rand() % 2;
				int ty = rand() % 2;
				DrawPartialSprite(0, 0, backBuff,  ScreenWidth()/2,ScreenHeight()/2, ScreenWidth()/2, ScreenHeight()/2);
				DrawPartialSprite(ScreenWidth()/2, 0, backBuff, 0, ScreenHeight() / 2, ScreenWidth() / 2, ScreenHeight() / 2);
				DrawPartialSprite( 0,ScreenHeight()/2, backBuff, ScreenWidth()/2, 0, ScreenWidth() / 2, ScreenHeight() / 2);
				DrawPartialSprite(ScreenWidth() / 2, ScreenHeight()/2, backBuff, 0, 0, ScreenWidth() / 2, ScreenHeight() / 2);
			}
		}



		// No effect mode
		if (nRenderEffect == EFFECT_NONE)
			DrawSprite(0, 0, backBuff);

		string sHelper = "";
		if (nRenderEffect == EFFECT_UGLYSWEDISHFISH)
			sHelper = "UGLYSWEDISHFISH Is Patching Your Code!\nHonestly, It's better like this...";
		if (nRenderEffect == EFFECT_GORBIT)
			sHelper = "GORBIT Is Patching Your Code!\nLook how beautiful my gifs are...";
		if (nRenderEffect == EFFECT_SEDIT)
			sHelper = "SEDIT Is Patching Your Code!\nI'm only going to change one little thing... oh.";
		if (nRenderEffect == EFFECT_BRANK)
			sHelper = "BRANKEC Is Patching Your Code!\nNow it'll be at least 7623 FPS...";

		DrawString(ScreenWidth() - 338, ScreenHeight() - 22, sHelper, olc::BLACK);
		DrawString(ScreenWidth() - 339, ScreenHeight() - 23, sHelper, olc::YELLOW);
		return true;
	}

	bool GameState_Complete(float fElapsedTime)
	{

		controller.Update(fElapsedTime);
		DrawSprite(0, 0, sprCompleteScreen);

		DrawString(4, 10, "YOU SAVED SEDIT!", olc::YELLOW, 4);

		int minutes = (int)fGameTime / 60;
		int seconds = (int)fGameTime - (minutes * 60);
		string sMinutes = (minutes <= 9 ? "0" : "") + to_string(minutes);
		string sSeconds = (seconds <= 9 ? "0" : "") + to_string(seconds);
		string sTime = "Your Time: " + sMinutes + ":" + sSeconds;	
		DrawString(10, 200, sTime, olc::YELLOW, 2);
		DrawString(10, 240, "OS Disks Found: " + to_string(nFloppyDisks) + "/100", olc::YELLOW, 2);
		DrawString(10, 260, "Level: " + to_string(nLevelSeed) + " Size: " + to_string((int)pow(2,2+nLevelSize)), olc::YELLOW, 2);
		DrawString(10, 300, "Jump to Title Screen?", olc::YELLOW, 2);

		if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		{
			nGameState = GS_TITLE;
		}
		
		return true;
	}

	int nStoryStage = 0;

	bool GameState_Story(float fElapsedTime)
	{
		controller.Update(fElapsedTime);
		
		if(nStoryStage == 0)
			DrawSprite(0, 0, sprStoryScreen);
		else
			DrawSprite(0, 0, sprControlScreen);
	
		if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		{
			nStoryStage++;
			if (nStoryStage == 2)
			{
				nGameState = GS_TITLE;
				nStoryStage = 0;
			}
		}

		return true;
	}

	bool GameState_Credits(float fElapsedTime)
	{
		controller.Update(fElapsedTime);
		DrawSprite(0, 0, sprCreditScreen);
		if (GetKey(olc::Key::SPACE).bPressed || controller.GetButton(A).bPressed)
		{
			nGameState = GS_TITLE;
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		switch (nGameState)
		{
		case GS_LOADING: GameState_Loading(fElapsedTime); break;
		case GS_TITLE: GameState_Title(fElapsedTime); break;
		case GS_GENERATE: GameState_Generating(fElapsedTime); break;
		case GS_MAIN: GameState_Main(fElapsedTime); break;
		case GS_STORY: GameState_Story(fElapsedTime); break;
		case GS_CREDITS: GameState_Credits(fElapsedTime); break;
		case GS_COMPLETE: GameState_Complete(fElapsedTime); break;
		}

		return true;
	}
};

int main()
{
	Discovery demo;
	if (demo.Construct(512, 448, 2, 2))
		demo.Start();
	return 0;
}
