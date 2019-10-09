#pragma once

#include <iostream>
#include <string>
#include <vector>

extern "C"
{
#include "lua533/include/lua.h"
#include "lua533/include/lauxlib.h"
#include "lua533/include/lualib.h"
}

#ifdef _WIN32
	#pragma comment(lib, "lua533/liblua53.a")
#endif

/*
	This is a singleton that stores all the games configuration settings.
	These settings are loaded on game start up and are to be considered
	read-only.
*/

struct sAssetModel
{
	std::string sCreator;
	std::string sDescription;
	std::string sModelOBJ;
	std::string sModelPNG;
	float fRotate[3];
	float fScale[3];
	float fTranslate[3];
};

struct sAssetTexture
{
	std::string sName;
	std::string sFile;	
};

class cGameSettings
{
public:
	cGameSettings();
	~cGameSettings();

public:
	bool LoadConfigFile(std::string sFile);

public:
	static int nScreenWidth;
	static int nScreenHeight;
	static int nPixelWidth;
	static int nPixelHeight;
	static bool bFullScreen;

	static int nDefaultMapWidth;
	static int nDefaultMapHeight;
	static std::string sDefaultCityFile;

	static std::vector<sAssetTexture> vecAssetTextures;
	static std::vector<sAssetModel> vecAssetBuildings;
	static std::vector<sAssetModel> vecAssetVehicles;
};

