#pragma once
#include <cstdint>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_NETWORK
#include "olcPGEX_Network.h"

enum class GameMsg : uint32_t
{
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
};

struct sPlayerDescription
{
	uint32_t nUniqueID = 0;
	uint32_t nAvatarID = 0;

	uint32_t nHealth = 100;
	uint32_t nAmmo = 20;
	uint32_t nKills = 0;
	uint32_t nDeaths = 0;

	float fRadius = 0.5f;

	olc::vf2d vPos;
	olc::vf2d vVel;
};