#include "cCell_Water.h"
#include "cCityMap.h"


cCell_Water::cCell_Water(cCityMap* map, int x, int y) : cCell(map, x, y)
{
	nCellType = CELL_WATER;
	bNeighboursAreWater[0] = false;
	bNeighboursAreWater[1] = false;
	bNeighboursAreWater[2] = false;
	bNeighboursAreWater[3] = false;
}


cCell_Water::~cCell_Water()
{
}

bool cCell_Water::LinkAssets(std::map<std::string, olc::Sprite*>& mapTextures, std::map<std::string, olc::GFX3D::mesh*>& mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms)
{
	meshUnitQuad = mapMesh["UnitQuad"];
	meshWalls = mapMesh["WallsOut"];
	sprWater = mapTextures["Water"];
	sprSides = mapTextures["WaterSide"];
	sprClouds = mapTextures["Clouds"];	
	return false;
}

bool cCell_Water::Update(float fElapsedTime)
{
	return false;
}

bool cCell_Water::DrawBase(olc::PixelGameEngine * pge, olc::GFX3D::PipeLine & pipe)
{
	olc::GFX3D::mat4x4 matWorld;
	matWorld = olc::GFX3D::Math::Mat_MakeTranslation((float)nWorldX, (float)nWorldY, 0.0f);
	pipe.SetTransform(matWorld);
	pipe.SetTexture(sprSides);
	if (!bNeighboursAreWater[1]) pipe.Render(meshWalls->tris, olc::GFX3D::RENDER_LIGHTS | olc::GFX3D::RENDER_CULL_CCW | olc::GFX3D::RENDER_TEXTURED | olc::GFX3D::RENDER_DEPTH, 0, 2);
	if (!bNeighboursAreWater[3]) pipe.Render(meshWalls->tris, olc::GFX3D::RENDER_LIGHTS | olc::GFX3D::RENDER_CULL_CCW | olc::GFX3D::RENDER_TEXTURED | olc::GFX3D::RENDER_DEPTH, 2, 2);
	if (!bNeighboursAreWater[2]) pipe.Render(meshWalls->tris, olc::GFX3D::RENDER_LIGHTS | olc::GFX3D::RENDER_CULL_CCW | olc::GFX3D::RENDER_TEXTURED | olc::GFX3D::RENDER_DEPTH, 4, 2);
	if (!bNeighboursAreWater[0]) pipe.Render(meshWalls->tris, olc::GFX3D::RENDER_LIGHTS | olc::GFX3D::RENDER_CULL_CCW | olc::GFX3D::RENDER_TEXTURED | olc::GFX3D::RENDER_DEPTH, 6, 2);
	return false;
}

bool cCell_Water::DrawAlpha(olc::PixelGameEngine * pge, olc::GFX3D::PipeLine & pipe)
{
	auto renderWater = [&](const int x, const int y, const olc::Pixel& pSource, const olc::Pixel& pDest)
	{
		float a = (float)(pSource.a / 255.0f) * 0.6f;
		float c = 1.0f - a;
		float r = a * (float)pSource.r + c * (float)pDest.r;
		float g = a * (float)pSource.g + c * (float)pDest.g;
		float b = a * (float)pSource.b + c * (float)pDest.b;

		a = 0.4f;
		c = 1.0f - a;
		olc::Pixel sky = sprClouds->GetPixel(x, y);
		float sr = a * (float)sky.r + c * r;
		float sg = a * (float)sky.g + c * g;
		float sb = a * (float)sky.b + c * b;

		return olc::Pixel((uint8_t)sr, (uint8_t)sg, (uint8_t)sb);
	};

	pge->SetPixelMode(renderWater);
	olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeTranslation((float)nWorldX, (float)nWorldY, 0.07f);
	pipe.SetTransform(matWorld);
	pipe.SetTexture(sprWater);
	pipe.Render(meshUnitQuad->tris, olc::GFX3D::RENDER_CULL_CW | olc::GFX3D::RENDER_DEPTH | olc::GFX3D::RENDER_TEXTURED);
	pge->SetPixelMode(olc::Pixel::NORMAL);
	return false;
}


void cCell_Water::CalculateAdjacency()
{
	auto r = [&](int i, int j)
	{
		if (pMap->Cell(nWorldX + i, nWorldY + j) != nullptr)
			return pMap->Cell(nWorldX + i, nWorldY + j)->nCellType == CELL_WATER;
		else
			return false;
	};

	bNeighboursAreWater[0] = r(0, -1);
	bNeighboursAreWater[1] = r(+1, 0);
	bNeighboursAreWater[2] = r(0, +1);
	bNeighboursAreWater[3] = r(-1, 0);	
}