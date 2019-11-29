#include "cCell.h"

#include "cCityMap.h"
#include "olcPixelGameEngine.h"
#include <map>

cCell::cCell()
{
}


cCell::~cCell()
{
	// Cells own a list of automata navigation tracks
	// but this will be destroyed when the cell is deleted
}

cCell::cCell(cCityMap* map, int x, int y)
{
	pMap = map;
	nWorldX = x;
	nWorldY = y;
	nCellType = CELL_BLANK;

	// Connect internal nodes
	for (int i = 0; i < 49; i++)
		pNaviNodes[i] = pMap->GetAutoNodeBase(x, y) + i;

	// Link cell into maps node pool
	if (y > 0)
	{
		for (int i = 0; i < 7; i++)
			pNaviNodes[i] = pMap->GetAutoNodeBase(x, y - 1) + 42 + i;
	}
	else
	{
		for (int i = 0; i < 7; i++)
			pNaviNodes[i] = nullptr;
	}

	if (x > 0)
	{
		// Link West side
		for (int i = 0; i < 7; i++)
			pNaviNodes[i * 7] = pMap->GetAutoNodeBase(x - 1, y) + 6 + i * 7;
	}
	else
	{
		for (int i = 0; i < 7; i++)
			pNaviNodes[i * 7] = nullptr;
	}

	// South Side
	if (y < pMap->GetHeight() - 1)
	{
	
	}
	else
	{
		for (int i = 0; i < 7; i++)
			pNaviNodes[42 + i] = nullptr;
	}

	// East Side
	if (x < pMap->GetWidth() - 1)
	{
	}
	else
	{
		for (int i = 0; i < 7; i++)
			pNaviNodes[6 + i * 7] = nullptr;
	}

	// Unused Nodes
	pNaviNodes[9] = nullptr;
	pNaviNodes[11] = nullptr;
	pNaviNodes[15] = nullptr;
	pNaviNodes[19] = nullptr;
	pNaviNodes[29] = nullptr;
	pNaviNodes[33] = nullptr;
	pNaviNodes[37] = nullptr;
	pNaviNodes[39] = nullptr;
	pNaviNodes[0] = nullptr;
	pNaviNodes[6] = nullptr;
	pNaviNodes[42] = nullptr;
	pNaviNodes[48] = nullptr;
	
}


bool cCell::LinkAssets(std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms)
{
	return false;
}

bool cCell::Update(float fElapsedTime)
{
	return false;
}

bool cCell::DrawBase(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe)
{
	return false;
}

bool cCell::DrawAlpha(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe)
{
	return false;
}

bool cCell::DrawDebug(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe)
{
	
	
	return false;
}

void cCell::CalculateAdjacency()
{

}
