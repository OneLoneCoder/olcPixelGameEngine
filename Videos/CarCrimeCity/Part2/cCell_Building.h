#pragma once
#include "cCell.h"
#include "olcPGEX_Graphics3D.h"


class cCell_Building : public cCell
{
public:
	cCell_Building(const std::string &name, cCityMap* map, int x, int y);
	~cCell_Building();

private:
	std::string sName;
	olc::Sprite*      texture = nullptr;
	olc::GFX3D::mesh* mesh = nullptr;
	olc::GFX3D::mat4x4 transform;

public:
	virtual void CalculateAdjacency();
	virtual bool LinkAssets(std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms);
	virtual bool Update(float fElapsedTime);
	virtual bool DrawBase(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
	virtual bool DrawAlpha(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
};

