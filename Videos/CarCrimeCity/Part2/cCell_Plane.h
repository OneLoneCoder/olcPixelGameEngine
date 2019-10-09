#pragma once
#include "cCell.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"
#include <map>


enum CELL_PLANE
{
	PLANE_GRASS,
	PLANE_ASPHALT
};

class cCell_Plane : public cCell
{
public:
	cCell_Plane(cCityMap* map, int x, int y, CELL_PLANE type);
	~cCell_Plane();

protected:
	CELL_PLANE nType = PLANE_GRASS;

private:
	olc::GFX3D::mesh* meshUnitQuad = nullptr;
	olc::Sprite* sprGrass = nullptr;
	olc::Sprite* sprPavement = nullptr;

public:
	virtual bool LinkAssets(std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms);
	virtual bool Update(float fElapsedTime);
	virtual bool DrawBase(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
	virtual bool DrawAlpha(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
};

