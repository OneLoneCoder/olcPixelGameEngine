/*
	Top Down City Based Car Crime Game - Part #2
	"Colin, I hope you're shooting 600+ wherever you are buddy. RIP." - javidx9

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
	Scroll with middle mouse wheel, TAB toggle edit mode, R to place road
	P to place pavement, Q to place building, Arrow keys to drive car

	Relevant Video: https://youtu.be/fIV6P1W-wuo

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


#include "cGameSettings.h"
#include "cCarCrimeCity.h"


int main()
{
	// Load the settings singleton
	cGameSettings config;
	if (!config.LoadConfigFile("assets/config.lua"))
	{
		std::cout << "Failed to load '/assets/config.lua'" << std::endl;
		std::cout << "  -> Using default configuration" << std::endl;
	}

	// Start the PixelGameEngine
	cCarCrimeCity game;
	if (game.Construct(config.nScreenWidth, config.nScreenHeight, config.nPixelWidth, config.nPixelHeight, config.bFullScreen))
		game.Start();

	// Exit!
	return 0;
}

//#define OLC_PGE_APPLICATION
//#include "olcPixelGameEngine.h"
//
//#define OLC_PGEX_GRAPHICS3D
//#include "olcPGEX_Graphics3D.h"
//
//
//
//enum CELLTYPE
//{
//	CELL_BLANK = 0,
//	CELL_GRASS = 1,
//	CELL_CONCRETE = 2,
//	CELL_WATER = 3,
//	CELL_BUILDING = 4,
//	CELL_ROAD_H = 5,
//	CELL_ROAD_V = 6,
//	CELL_ROAD_C1 = 7,
//	CELL_ROAD_C2 = 8,
//	CELL_ROAD_C3 = 9,
//	CELL_ROAD_C4 = 10,
//	CELL_ROAD_T1 = 11,
//	CELL_ROAD_T2 = 12,
//	CELL_ROAD_T3 = 13,
//	CELL_ROAD_T4 = 14,
//	CELL_ROAD_X = 15,
//};
//
//struct cCityCell
//{
//	int nType = 5;// CELL_GRASS;
//};
//
//class cCityMap
//{
//public:
//	// Construct a "blank" city w units wide by h units high
//	cCityMap(int w, int h);
//
//	// Cleans up city, like Batman
//	~cCityMap();
//
//
//public:
//	// Return width of city in cells
//	int GetWidth();
//	// Return height of city in cells
//	int GetHeight();
//	// Return a specific cell reference if inside city limits, or nullptr
//	cCityCell* Cell(int x, int y);
//
//private:
//	int m_nWidth = 0;
//	int m_nHeight = 0;
//	cCityCell *m_pCells = nullptr;
//
//private:
//	// Creates a "default" city of specified size
//	void CreateCity(int w, int h);
//	// Destroy city
//	void ReleaseCity();
//};
//
//cCityMap::cCityMap(int w, int h)
//{
//	CreateCity(w, h);
//}
//
//cCityMap::~cCityMap()
//{
//	//ReleaseCity();
//}
//
//int cCityMap::GetWidth()
//{
//	return m_nWidth;
//}
//
//int cCityMap::GetHeight()
//{
//	return m_nHeight;
//}
//
//cCityCell* cCityMap::Cell(int x, int y)
//{
//	if (x >= 0 && x < m_nWidth && y >= 0 && y < m_nHeight)
//		return &m_pCells[y*m_nWidth + x];
//	else
//		return nullptr;
//}
//
//void cCityMap::CreateCity(int w, int h)
//{
//	//ReleaseCity();
//	m_nWidth = w;
//	m_nHeight = h;
//	m_pCells = new cCityCell[m_nHeight * m_nWidth];
//
//	for (int x = 0; x < m_nWidth; x++)
//	{
//		for (int y = 0; y < m_nHeight; y++)
//		{
//			//m_pCells[y*m_nWidth + x] = new cCityCell();
//			//Cell(x, y)->bRoad = false;
//			//Cell(x, y)->nHeight = 0;
//			//Cell(x, y)->nWorldX = x;
//			//Cell(x, y)->nWorldY = y;
//			Cell(x, y)->nType = CELL_GRASS;
//			//Cell(x, y)->bBuilding = false;
//		}
//	}
//}
//
//void cCityMap::ReleaseCity()
//{
//	if (m_pCells != nullptr) delete m_pCells;
//	m_nWidth = 0;
//	m_nHeight = 0;
//}
//
//
//class cCarCrimeCity : public olc::PixelGameEngine
//{
//public:
//	cCarCrimeCity()
//	{
//		sAppName = "Car Crime City";
//	}
//
//	~cCarCrimeCity()
//	{
//	}
//
//	bool OnUserCreate()
//	{
//		// Initialise PGEX 3D
//		olc::GFX3D::ConfigureDisplay();
//
//		// Create Default city
//		pCity = new cCityMap(64, 32);// cGameSettings::nDefaultMapWidth, cGameSettings::nDefaultMapHeight);
//
//
//		// A simple flat unit quad
//		meshQuad.tris =
//		{
//				{ 0.0f, 0.0f, 0.0f, 1.0f,	    0.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 		1.0f, 1.0f, 0.0f, olc::RED },
//				{ 0.0f, 0.0f, 0.0f, 1.0f,  		1.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f, 		1.0f, 0.0f, 0.0f, olc::RED},
//		};
//		
//
//		sprOld = new olc::Sprite("assets/system/grass1.png");
//
//		
//
//		SetDrawTarget(nullptr);
//		return true;
//	}
//
//
//	bool OnUserUpdate(float fElapsedTime)
//	{
//		// User Input
//		if (GetKey(olc::Key::W).bHeld) vCamera.y -= 2.0f * fElapsedTime;
//		if (GetKey(olc::Key::S).bHeld) vCamera.y += 2.0f * fElapsedTime;
//		if (GetKey(olc::Key::A).bHeld) vCamera.x -= 2.0f * fElapsedTime;
//		if (GetKey(olc::Key::D).bHeld) vCamera.x += 2.0f * fElapsedTime;
//		if (GetKey(olc::Key::Z).bHeld) vCamera.z += 10.0f * fElapsedTime;
//		if (GetKey(olc::Key::X).bHeld) vCamera.z -= 10.0f * fElapsedTime;
//
//
//		vEye = vCamera;
//
//		// Perform Ray casting to calculate visible world extents and mouse position
//		olc::GFX3D::vec3d vLookTarget = olc::GFX3D::Math::Vec_Add(vEye, vLookDir);
//		olc::GFX3D::mat4x4 matProj = olc::GFX3D::Math::Mat_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.5f, 1000.0f);
//		olc::GFX3D::mat4x4 matView = olc::GFX3D::Math::Mat_PointAt(vEye, vLookTarget, vUp);
//		
//
//
//		// Render Scene
//		Clear(olc::BLUE);
//		olc::GFX3D::ClearDepth();
//
//		// Create rendering pipeline
//		olc::GFX3D::PipeLine pipe;
//		pipe.SetProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.5f, 1000.0f, 0.0f, 0.0f, (float)ScreenWidth(), (float)ScreenHeight());
//		pipe.SetCamera(vEye, vLookTarget, vUp);
//
//		
//
//		int nStartX = 0;
//		int nEndX = pCity->GetWidth();
//		int nStartY = 0;
//		int nEndY = pCity->GetHeight();
//
//		// Render Ground, Roads, Walls & Buildings
//		for (int x = nStartX; x < nEndX; x++)
//		{
//			if (x == 15)
//				int k = 7;
//
//			for (int y = nStartY; y < nEndY; y++)
//			{
//
//
//				switch (pCity->Cell(x, y)->nType)
//				{
//				case CELL_GRASS:
//				{
//					olc::GFX3D::mat4x4 matWorld;
//					matWorld = olc::GFX3D::Math::Mat_MakeTranslation((float)x, (float)y, 0.0f);
//					pipe.SetTransform(matWorld);
//					pipe.SetTexture(sprOld);
//					//pipe.SetTexture(vecSpriteSystem[0]);
//					//pipe.Render(vecMeshSystem[0].tris);
//					pipe.Render(meshQuad.tris);
//					//pipe.Render(vecMeshSystem[0].tris, olc::GFX3D::RENDER_FLAT);
//					break;
//				}
//				
//
//				default:
//				{
//					olc::GFX3D::mat4x4 matWorld;
//					matWorld = olc::GFX3D::Math::Mat_MakeTranslation((float)x, (float)y, 0.0f);
//					pipe.SetTransform(matWorld);
//					pipe.Render(meshQuad.tris, olc::GFX3D::RENDER_WIRE);
//					break;
//				}
//				}
//
//
//				
//
//			}
//		}
//
//		
//
//		return true;
//	}
//
//	bool OnUserDestroy()
//	{
//		return true;
//	}
//
//
//private:
//	olc::GFX3D::vec3d vCamera = { 0.0f, 0.0f, -10.0f };
//	olc::GFX3D::vec3d vUp = { 0.0f, 1.0f, 0.0f };
//	olc::GFX3D::vec3d vEye = { 0.0f, 0.0f, -10.0f };
//	olc::GFX3D::vec3d vLookDir = { 0.0f, 0.0f, 1.0f };
//
//
//
//	olc::Sprite *sprOld = nullptr;
//	olc::GFX3D::mesh meshQuad;
//
//	cCityMap *pCity = nullptr;
//
//
//
//};
//
//int main()
//{
//	// Load the settings singleton
//	/*cGameSettings config;
//	if (!config.LoadConfigFile("assets/config.lua"))
//	{
//		std::cout << "Failed to load '/assets/config.lua'" << std::endl;
//		std::cout << "  -> Using default configuration" << std::endl;
//	}*/
//
//	// Start the PixelGameEngine
//	cCarCrimeCity game;
//	if (game.Construct(256, 240, 4, 4))// config.nScreenWidth, config.nScreenHeight, config.nPixelWidth, config.nPixelHeight))
//		game.Start();
//
//	// Exit!
//	return 0;
//}