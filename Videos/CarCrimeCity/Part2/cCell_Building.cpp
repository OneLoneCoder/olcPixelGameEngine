#include "cCell_Building.h"



cCell_Building::cCell_Building(const std::string &name, cCityMap* map, int x, int y) : cCell(map, x, y)
{
	sName = name;
}


cCell_Building::~cCell_Building()
{
}

void cCell_Building::CalculateAdjacency()
{
}

bool cCell_Building::LinkAssets(std::map<std::string, olc::Sprite*>& mapTextures, std::map<std::string, olc::GFX3D::mesh*>& mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms)
{
	texture = mapTextures[sName];
	mesh = mapMesh[sName];
	transform = mapTransforms[sName];
	return false;
}

bool cCell_Building::Update(float fElapsedTime)
{
	return false;
}

bool cCell_Building::DrawBase(olc::PixelGameEngine * pge, olc::GFX3D::PipeLine & pipe)
{
	olc::GFX3D::mat4x4 matTranslate = olc::GFX3D::Math::Mat_MakeTranslation((float)nWorldX, (float)nWorldY, 0.0f);
	olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MultiplyMatrix(transform, matTranslate);
	pipe.SetTransform(matWorld);
	if (texture != nullptr)
	{
		pipe.SetTexture(texture);
		pipe.Render(mesh->tris,olc::GFX3D::RENDER_CULL_CW | olc::GFX3D::RENDER_DEPTH | olc::GFX3D::RENDER_TEXTURED | olc::GFX3D::RENDER_LIGHTS);
	}
	else
	{
		pipe.Render(mesh->tris, olc::GFX3D::RENDER_CULL_CW | olc::GFX3D::RENDER_DEPTH | olc::GFX3D::RENDER_FLAT | olc::GFX3D::RENDER_LIGHTS);
	}
	return false;
}

bool cCell_Building::DrawAlpha(olc::PixelGameEngine * pge, olc::GFX3D::PipeLine & pipe)
{
	
	return false;
}
