#pragma once

#include <map>

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"

#include "cAutomata.h"


class cCityMap;

enum CellType
{
	CELL_BLANK,
	CELL_GRASS,
	CELL_CONCRETE,
	CELL_WATER,
	CELL_BUILDING,
	CELL_ROAD,
	CELL_PAVEMENT,
};

class cCell
{
public:
	cCell();
	cCell(cCityMap* map, int x, int y);
	~cCell();

protected:
	cCityMap* pMap = nullptr;

public:
	int nWorldX = 0;
	int nWorldY = 0;
	bool bSolid = false;
	CellType nCellType = CELL_BLANK;

	// This cell may actuall be occupied by a multi-cell body
	// so this pointer points to the host cell that contains
	// that body
	cCell* pHostCell = nullptr;

	// Each cell links to 20 automata transport nodes, 5 on each side
	cAuto_Node* pNaviNodes[49];

	// Each cell can have a number of automata transport tracks, it owns them
	// These connect nodes together as determined by the cell
	std::list<cAuto_Track> listTracks;

public:
	virtual void CalculateAdjacency();
	virtual bool LinkAssets(std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms);
	virtual bool Update(float fElapsedTime);
	virtual bool DrawBase(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
	virtual bool DrawAlpha(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
	virtual bool DrawDebug(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
};

