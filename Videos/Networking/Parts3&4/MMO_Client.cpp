
#include "../MMO_Server/MMO_Common.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"

#include <unordered_map>

class MMOGame : public olc::PixelGameEngine, olc::net::client_interface<GameMsg>
{
public:
	MMOGame()
	{
		sAppName = "MMO Client";
	}

private:
	olc::TileTransformedView tv;

	std::string sWorldMap =
		"################################"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..........####...####.........#"
		"#..........#.........#.........#"
		"#..........#.........#.........#"
		"#..........#.........#.........#"
		"#..........##############......#"
		"#..............................#"
		"#..................#.#.#.#.....#"
		"#..............................#"
		"#..................#.#.#.#.....#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"################################";

	olc::vi2d vWorldSize = { 32, 32 };

private:
	std::unordered_map<uint32_t, sPlayerDescription> mapObjects;
	uint32_t nPlayerID = 0;
	sPlayerDescription descPlayer;

	bool bWaitingForConnection = true;

public:
	bool OnUserCreate() override
	{
		tv = olc::TileTransformedView({ ScreenWidth(), ScreenHeight() }, { 8, 8 });

		//mapObjects[0].nUniqueID = 0;
		//mapObjects[0].vPos = { 3.0f, 3.0f };

		if (Connect("127.0.0.1", 60000))
		{
			return true;
		}

		return false;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Check for incoming network messages
		if (IsConnected())
		{
			while (!Incoming().empty())
			{
				auto msg = Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case(GameMsg::Client_Accepted):
				{
					std::cout << "Server accepted client - you're in!\n";
					olc::net::message<GameMsg> msg;
					msg.header.id = GameMsg::Client_RegisterWithServer;
					descPlayer.vPos = { 3.0f, 3.0f };
					msg << descPlayer;
					Send(msg);
					break;
				}

				case(GameMsg::Client_AssignID):
				{
					// Server is assigning us OUR id
					msg >> nPlayerID;
					std::cout << "Assigned Client ID = " << nPlayerID << "\n";
					break;
				}

				case(GameMsg::Game_AddPlayer):
				{
					sPlayerDescription desc;
					msg >> desc;
					mapObjects.insert_or_assign(desc.nUniqueID, desc);

					if (desc.nUniqueID == nPlayerID)
					{
						// Now we exist in game world
						bWaitingForConnection = false;
					}
					break;
				}

				case(GameMsg::Game_RemovePlayer):
				{
					uint32_t nRemovalID = 0;
					msg >> nRemovalID;
					mapObjects.erase(nRemovalID);
					break;
				}

				case(GameMsg::Game_UpdatePlayer):
				{
					sPlayerDescription desc;
					msg >> desc;
					mapObjects.insert_or_assign(desc.nUniqueID, desc);
					break;
				}


				}
			}
		}

		if (bWaitingForConnection)
		{
			Clear(olc::DARK_BLUE);
			DrawString({ 10,10 }, "Waiting To Connect...", olc::WHITE);
			return true;
		}






		// Control of Player Object
		mapObjects[nPlayerID].vVel = { 0.0f, 0.0f };
		if (GetKey(olc::Key::W).bHeld) mapObjects[nPlayerID].vVel += { 0.0f, -1.0f };
		if (GetKey(olc::Key::S).bHeld) mapObjects[nPlayerID].vVel += { 0.0f, +1.0f };
		if (GetKey(olc::Key::A).bHeld) mapObjects[nPlayerID].vVel += { -1.0f, 0.0f };
		if (GetKey(olc::Key::D).bHeld) mapObjects[nPlayerID].vVel += { +1.0f, 0.0f };

		if (mapObjects[nPlayerID].vVel.mag2() > 0)
			mapObjects[nPlayerID].vVel = mapObjects[nPlayerID].vVel.norm() * 4.0f;

		// Update objects locally
		for (auto& object : mapObjects)
		{
			// Where will object be worst case?
			olc::vf2d vPotentialPosition = object.second.vPos + object.second.vVel * fElapsedTime;

			// Extract region of world cells that could have collision this frame
			olc::vi2d vCurrentCell = object.second.vPos.floor();
			olc::vi2d vTargetCell = vPotentialPosition;
			olc::vi2d vAreaTL = (vCurrentCell.min(vTargetCell) - olc::vi2d(1, 1)).max({ 0,0 });
			olc::vi2d vAreaBR = (vCurrentCell.max(vTargetCell) + olc::vi2d(1, 1)).min(vWorldSize);

			// Iterate through each cell in test area
			olc::vi2d vCell;
			for (vCell.y = vAreaTL.y; vCell.y <= vAreaBR.y; vCell.y++)
			{
				for (vCell.x = vAreaTL.x; vCell.x <= vAreaBR.x; vCell.x++)
				{
					// Check if the cell is actually solid...
				//	olc::vf2d vCellMiddle = vCell.floor();
					if (sWorldMap[vCell.y * vWorldSize.x + vCell.x] == '#')
					{
						// ...it is! So work out nearest point to future player position, around perimeter
						// of cell rectangle. We can test the distance to this point to see if we have
						// collided.

						olc::vf2d vNearestPoint;
						// Inspired by this (very clever btw) 
						// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response
						vNearestPoint.x = std::max(float(vCell.x), std::min(vPotentialPosition.x, float(vCell.x + 1)));
						vNearestPoint.y = std::max(float(vCell.y), std::min(vPotentialPosition.y, float(vCell.y + 1)));

						// But modified to work :P
						olc::vf2d vRayToNearest = vNearestPoint - vPotentialPosition;
						float fOverlap = object.second.fRadius - vRayToNearest.mag();
						if (std::isnan(fOverlap)) fOverlap = 0;// Thanks Dandistine!

						// If overlap is positive, then a collision has occurred, so we displace backwards by the 
						// overlap amount. The potential position is then tested against other tiles in the area
						// therefore "statically" resolving the collision
						if (fOverlap > 0)
						{
							// Statically resolve the collision
							vPotentialPosition = vPotentialPosition - vRayToNearest.norm() * fOverlap;
						}
					}
				}
			}

			// Set the objects new position to the allowed potential position
			object.second.vPos = vPotentialPosition;
		}






		// Handle Pan & Zoom
		if (GetMouse(2).bPressed) tv.StartPan(GetMousePos());
		if (GetMouse(2).bHeld) tv.UpdatePan(GetMousePos());
		if (GetMouse(2).bReleased) tv.EndPan(GetMousePos());
		if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(1.5f, GetMousePos());
		if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.75f, GetMousePos());

		// Clear World
		Clear(olc::BLACK);

		// Draw World
		olc::vi2d vTL = tv.GetTopLeftTile().max({ 0,0 });
		olc::vi2d vBR = tv.GetBottomRightTile().min(vWorldSize);
		olc::vi2d vTile;
		for (vTile.y = vTL.y; vTile.y < vBR.y; vTile.y++)
			for (vTile.x = vTL.x; vTile.x < vBR.x; vTile.x++)
			{
				if (sWorldMap[vTile.y * vWorldSize.x + vTile.x] == '#')
				{
					tv.DrawRect(vTile, { 1.0f, 1.0f });
					tv.DrawRect(olc::vf2d(vTile) + olc::vf2d(0.1f, 0.1f), { 0.8f, 0.8f });
				}
			}

		// Draw World Objects
		for (auto& object : mapObjects)
		{
			// Draw Boundary
			tv.DrawCircle(object.second.vPos, object.second.fRadius);

			// Draw Velocity
			if (object.second.vVel.mag2() > 0)
				tv.DrawLine(object.second.vPos, object.second.vPos + object.second.vVel.norm() * object.second.fRadius, olc::MAGENTA);

			// Draw Name
			olc::vi2d vNameSize = GetTextSizeProp("ID: " + std::to_string(object.first));
			tv.DrawStringPropDecal(object.second.vPos - olc::vf2d{ vNameSize.x * 0.5f * 0.25f * 0.125f, -object.second.fRadius * 1.25f }, "ID: " + std::to_string(object.first), olc::BLUE, { 0.25f, 0.25f });
		}

		// Send player description
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_UpdatePlayer;
		msg << mapObjects[nPlayerID];
		Send(msg);
		return true;
	}
};

int main()
{
	MMOGame demo;
	if (demo.Construct(480, 480, 1, 1))
		demo.Start();
	return 0;
}