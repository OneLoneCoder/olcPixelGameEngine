#include "cCell_Plane.h"



cCell_Plane::cCell_Plane(cCityMap* map, int x, int y, CELL_PLANE type) : cCell(map, x, y)
{
	bSolid = false;
	nType = type;
	if (nType == PLANE_GRASS) nCellType = CELL_GRASS;
	if (nType == PLANE_ASPHALT) nCellType = CELL_PAVEMENT;
}


cCell_Plane::~cCell_Plane()
{
}

bool cCell_Plane::LinkAssets(std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms)
{
	sprGrass = mapTextures["Grass"];
	sprPavement = mapTextures["Pavement"];
	meshUnitQuad = mapMesh["UnitQuad"];
	return true;
}

bool cCell_Plane::Update(float fElapsedTime)
{
	return false;
}

bool cCell_Plane::DrawBase(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe)
{
	olc::GFX3D::mat4x4 matWorld;
	matWorld = olc::GFX3D::Math::Mat_MakeTranslation((float)nWorldX, (float)nWorldY, 0.0f);
	pipe.SetTransform(matWorld);
	
	if(nType == PLANE_GRASS)
		pipe.SetTexture(sprGrass);
	else
		pipe.SetTexture(sprPavement);

	pipe.Render(meshUnitQuad->tris, olc::GFX3D::RENDER_CULL_CW | olc::GFX3D::RENDER_DEPTH | olc::GFX3D::RENDER_TEXTURED);
	return false;
}

bool cCell_Plane::DrawAlpha(olc::PixelGameEngine *pge, olc::GFX3D::PipeLine &pipe)
{
	return false;
}
