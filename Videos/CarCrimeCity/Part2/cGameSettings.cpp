#include "cGameSettings.h"



cGameSettings::cGameSettings()
{
}

cGameSettings::~cGameSettings()
{
}

bool cGameSettings::LoadConfigFile(std::string sFile)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	// Load game settings file
	int r = luaL_loadfile(L, sFile.c_str());
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}

	// Execute it
	int i = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (i != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}	

	lua_getglobal(L, "PixelWidth");
	if (lua_isinteger(L, -1)) cGameSettings::nPixelWidth = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "PixelHeight");
	if (lua_isinteger(L, -1)) cGameSettings::nPixelHeight = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "ScreenWidth");
	if (lua_isinteger(L, -1)) cGameSettings::nScreenWidth = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "ScreenHeight");
	if (lua_isinteger(L, -1)) cGameSettings::nScreenHeight = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "DefaultMapWidth");
	if (lua_isinteger(L, -1)) cGameSettings::nDefaultMapWidth = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "DefaultMapHeight");
	if (lua_isinteger(L, -1)) cGameSettings::nDefaultMapHeight = (int)lua_tointeger(L, -1);

	lua_getglobal(L, "DefaultCityFile");
	if (lua_isstring(L, -1)) cGameSettings::sDefaultCityFile = lua_tostring(L, -1);

	lua_getglobal(L, "FullScreen");
	if (lua_isboolean(L, -1)) cGameSettings::bFullScreen = lua_toboolean(L, -1);


	//// Load System Texture files
	
	// Load Texture Assets
	lua_getglobal(L, "Textures"); // -1 Table "Teams"
	if (lua_istable(L, -1))
	{
		lua_pushnil(L); // -2 Key Nil : -1 Table "Teams"

		while (lua_next(L, -2) != 0) // -1 Table : -2 Key "TeamName" : -3 Table "Teams"
		{
			sAssetTexture texture;
			int stage = 0;
			if (lua_istable(L, -1))
			{
				lua_gettable(L, -1); // -1 Table : -2 Table Value : -3 Key "TeamName" : -4 Table "Teams" 
				lua_pushnil(L);      // -1 Key Nil : -2 Table : -3 Table Value : -4 Key "TeamName" : -5 Table "Teams" 
				while (lua_next(L, -2) != 0) // -1 Value "BotFile" : -2 Key Nil : -3 Table : -4 Table Value : -5 Key "TeamName" : -6 Table "Teams" 
				{
					if (stage == 0) texture.sName = lua_tostring(L, -1);
					if (stage == 1) texture.sFile = lua_tostring(L, -1);					
					lua_pop(L, 1); // -1 Key Nil : -2 Table : -3 Table Value : -4 Key "TeamName" : -5 Table "Teams" 
					stage++;
				}
			}
			lua_pop(L, 1); // -1 Table : -2 Table Value : -3 Key "TeamName" : -4 Table "Teams" 
			vecAssetTextures.push_back(texture);
		}
	}

	auto GroupLoadAssets = [L](const std::string &group, std::vector<sAssetModel> &vec)
	{
		lua_getglobal(L, group.c_str()); 
		if (lua_istable(L, -1))
		{
			lua_pushnil(L); 
			while (lua_next(L, -2) != 0) 
			{
				sAssetModel model;
				int stage = 0;
				if (lua_istable(L, -1))
				{
					lua_gettable(L, -1); 
					lua_pushnil(L);     
					while (lua_next(L, -2) != 0)
					{
						if (stage == 0) model.sCreator = lua_tostring(L, -1);
						if (stage == 1) model.sDescription = lua_tostring(L, -1);
						if (stage == 2) model.sModelOBJ = lua_tostring(L, -1);
						if (stage == 3) model.sModelPNG = lua_tostring(L, -1);

						if (stage == 4) model.fRotate[0] = (float)lua_tonumber(L, -1);
						if (stage == 5) model.fRotate[1] = (float)lua_tonumber(L, -1);
						if (stage == 6) model.fRotate[2] = (float)lua_tonumber(L, -1);

						if (stage == 7) model.fScale[0] = (float)lua_tonumber(L, -1);
						if (stage == 8) model.fScale[1] = (float)lua_tonumber(L, -1);
						if (stage == 9) model.fScale[2] = (float)lua_tonumber(L, -1);

						if (stage == 10) model.fTranslate[0] = (float)lua_tonumber(L, -1);
						if (stage == 11) model.fTranslate[1] = (float)lua_tonumber(L, -1);
						if (stage == 12) model.fTranslate[2] = (float)lua_tonumber(L, -1);
						lua_pop(L, 1); 
						stage++;
					}
				}
				lua_pop(L, 1);
				vec.push_back(model);
			}
		}
	};

	// Load Building Assets
	GroupLoadAssets("Buildings", vecAssetBuildings);

	// Load Vehicle Assets
	GroupLoadAssets("Vehicles", vecAssetVehicles);


	lua_close(L);

	return true;
}

int cGameSettings::nScreenWidth = 768;
int cGameSettings::nScreenHeight = 480;
int cGameSettings::nPixelWidth = 2;
int cGameSettings::nPixelHeight = 2;
bool cGameSettings::bFullScreen = false;

int cGameSettings::nDefaultMapWidth = 64;
int cGameSettings::nDefaultMapHeight = 32;
std::string cGameSettings::sDefaultCityFile = ""; 

std::vector<sAssetTexture> cGameSettings::vecAssetTextures;
std::vector<sAssetModel> cGameSettings::vecAssetBuildings;
std::vector<sAssetModel> cGameSettings::vecAssetVehicles;