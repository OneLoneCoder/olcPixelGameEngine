#pragma once

#include <string>
#include <map>

#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"
#include "cCell.h"
#include "cCell_Plane.h"
#include "cCell_Water.h"
#include "cCell_Road.h"
#include "cCell_Building.h"

/*
	This class holds the definition of a map. The map data is actually
	stored within this clap, as well as accessors to access the individual
	map cells
*/
class cCityMap
{
public:
	// Construct a "blank" city w units wide by h units high
	cCityMap(int w, int h, std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms);

	// Cleans up city, like Batman
	~cCityMap();

public:
	// Save the current city to a file, this will overwrite an existing
	// city file without warning. Returns true if successful
	bool SaveCity(std::string sFilename);
	
	// Load a city from file and replace current city with it, retuns
	// true if successful
	bool LoadCity(std::string sFilename);

public:
	// Return width of city in cells
	int GetWidth();
	// Return height of city in cells
	int GetHeight();
	// Return a specific cell reference if inside city limits, or nullptr
	cCell* Cell(int x, int y);
	// Replace a specific cell
	cCell* Replace(int x, int y, cCell* cell);

	cAuto_Node* GetAutoNodeBase(int x, int y);

	void RemoveAllTracks();

private:
	int nWidth = 0;
	int nHeight = 0;
	cCell **pCells = nullptr;
	cAuto_Node *pNodes = nullptr;

private:
	// Creates a "default" city of specified size
	void CreateCity(int w, int h, std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms);
	// Destroy city
	void ReleaseCity();
};

