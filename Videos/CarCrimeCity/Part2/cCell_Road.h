#pragma once
#include "cCell.h"

enum RoadType
{
	ROAD_H,
	ROAD_V,
	ROAD_C1,
	ROAD_C2,
	ROAD_C3,
	ROAD_C4,
	ROAD_T1,
	ROAD_T2,
	ROAD_T3,
	ROAD_T4,
	ROAD_X,
};


class cCell_Road : public cCell
{
public:
	cCell_Road(cCityMap* map, int x, int y);
	~cCell_Road();

private:
	struct StopPattern
	{
		bool bStop[49];
	};

private:
	bool bNeighboursAreRoads[4];
	
	olc::GFX3D::mesh *meshUnitQuad = nullptr;
	olc::Sprite* sprRoadTex[11];

	std::vector<StopPattern> vStopPattern;
	int nCurrentStopPattern = 0;
	float fStopPatternTimer = 0.0f;
public:
	RoadType nRoadType = ROAD_X;
	cAuto_Track* pSafeCarTrack = nullptr;
	cAuto_Track* pSafePedestrianTrack = nullptr;
	cAuto_Track* pSafeChaseTrack = nullptr;

	virtual void CalculateAdjacency();
	virtual bool LinkAssets(std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms);
	virtual bool Update(float fElapsedTime);
	virtual bool DrawBase(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
	virtual bool DrawAlpha(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
	virtual bool DrawDebug(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe);
};

