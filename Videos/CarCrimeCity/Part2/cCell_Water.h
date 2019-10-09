#pragma once
#include "cCell.h"
class cCell_Water : public cCell
{
public:
	cCell_Water(cCityMap* map, int x, int y);
	~cCell_Water();

private:
	olc::GFX3D::mesh* meshUnitQuad = nullptr;
	olc::GFX3D::mesh* meshWalls = nullptr;
	olc::Sprite* sprWater = nullptr;
	olc::Sprite* sprSides = nullptr;
	olc::Sprite* sprClouds = nullptr;

	bool bNeighboursAreWater[4];

public:
	virtual void CalculateAdjacency();
	virtual bool LinkAssets(std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms);
	virtual bool Update(float fElapsedTime);
	virtual bool DrawBase(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
	virtual bool DrawAlpha(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
};

