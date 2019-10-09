#include "cCell_Road.h"
#include "cCityMap.h"


cCell_Road::cCell_Road(cCityMap* map, int x, int y) : cCell(map, x, y)
{
	bSolid = false;
	nCellType = CELL_ROAD;
}

cCell_Road::~cCell_Road()
{
}

void cCell_Road::CalculateAdjacency()
{
	
	// Calculate suitable road junction type
	auto r = [&](int i, int j)
	{
		return (pMap->Cell(nWorldX + i, nWorldY + j) != nullptr && pMap->Cell(nWorldX + i, nWorldY + j)->nCellType == CELL_ROAD);
	};

	if (r(0, -1) && r(0, +1) && !r(-1, 0) && !r(+1, 0)) nRoadType = ROAD_V;
	if (!r(0, -1) && !r(0, +1) && r(-1, 0) && r(+1, 0)) nRoadType =ROAD_H;
	if (!r(0, -1) && r(0, +1) && !r(-1, 0) && r(+1, 0)) nRoadType = ROAD_C1;
	if (!r(0, -1) && r(0, +1) && r(-1, 0) && r(+1, 0)) nRoadType =ROAD_T1;
	if (!r(0, -1) && r(0, +1) && r(-1, 0) && !r(+1, 0)) nRoadType = ROAD_C2;
	if (r(0, -1) && r(0, +1) && !r(-1, 0) && r(+1, 0)) nRoadType = ROAD_T2;
	if (r(0, -1) && r(0, +1) && r(-1, 0) && r(+1, 0)) nRoadType = ROAD_X;
	if (r(0, -1) && r(0, +1) && r(-1, 0) && !r(+1, 0)) nRoadType = ROAD_T3;
	if (r(0, -1) && !r(0, +1) && !r(-1, 0) && r(+1, 0)) nRoadType = ROAD_C3;
	if (r(0, -1) && !r(0, +1) && r(-1, 0) && r(+1, 0)) nRoadType = ROAD_T4;
	if (r(0, -1) && !r(0, +1) && r(-1, 0) && !r(+1, 0)) nRoadType = ROAD_C4;

	// Add navigation tracks based on type

	auto AddTrack = [&](int n1, int n2) -> cAuto_Track*
	{
		if (pNaviNodes[n1] == nullptr || pNaviNodes[n2] == nullptr)
		{
			// Can't add track
			return nullptr;
		}
		else
		{
			// Nodes exist so add track
			cAuto_Track t;
			t.node[0] = pNaviNodes[n1];
			t.node[1] = pNaviNodes[n2];
			t.cell = this;
			t.fTrackLength = (pNaviNodes[n1]->pos - pNaviNodes[n2]->pos).mag();
			listTracks.push_back(t);

			// Add pointers to track to start and end nodes
			pNaviNodes[n1]->listTracks.push_back(&listTracks.back());
			pNaviNodes[n2]->listTracks.push_back(&listTracks.back());

			return &listTracks.back();
		}
	};

	// Ensure list of tracks for this cell is clear
	listTracks.clear();

	// Add tracks depending on junction type
	pSafePedestrianTrack = nullptr;
	pSafeCarTrack = nullptr;
	pSafeChaseTrack = nullptr;

	// Add Pedestrian Tracks
	switch (nRoadType)
	{
	case ROAD_H:	pSafePedestrianTrack = AddTrack(7, 13); AddTrack(41, 35); break;
	case ROAD_V:	pSafePedestrianTrack = AddTrack(1, 43); AddTrack(5, 47); break;

	case ROAD_C1:	pSafePedestrianTrack = AddTrack(43, 8); AddTrack(8, 13); AddTrack(47, 40); AddTrack(40, 41); break;
	case ROAD_C2:	AddTrack(7, 12); AddTrack(12, 47); pSafePedestrianTrack = AddTrack(35, 36); AddTrack(36, 43);	break;
	case ROAD_C3:	AddTrack(1, 36); pSafePedestrianTrack = AddTrack(36, 41); AddTrack(5, 12); AddTrack(12, 13);	break;
	case ROAD_C4:	AddTrack(35, 40); AddTrack(40, 5); pSafePedestrianTrack = AddTrack(7, 8); AddTrack(8, 1); break;

	case ROAD_T1:	pSafePedestrianTrack = AddTrack(7, 8); AddTrack(8, 12); AddTrack(12, 13); AddTrack(35, 36); AddTrack(36, 38); AddTrack(38, 40); AddTrack(40, 41); AddTrack(8, 22); AddTrack(22, 36);  AddTrack(36, 43); AddTrack(12, 26); AddTrack(26, 40); AddTrack(40, 47); 	break;
	case ROAD_T2:	pSafePedestrianTrack = AddTrack(1, 8); AddTrack(8, 36); AddTrack(36, 43); AddTrack(5, 12); AddTrack(12, 26); AddTrack(26, 40); AddTrack(40, 47); AddTrack(8, 10); AddTrack(10, 12); AddTrack(12, 13); AddTrack(36, 38), AddTrack(38, 40); AddTrack(40, 41); 	break;
	case ROAD_T3:	pSafePedestrianTrack = AddTrack(5, 12); AddTrack(12, 40); AddTrack(40, 47); AddTrack(1, 8); AddTrack(8, 22); AddTrack(22, 36); AddTrack(36, 43); AddTrack(12, 10); AddTrack(10, 8); AddTrack(8, 7); AddTrack(40, 38); AddTrack(38, 36); AddTrack(36, 35); 	break;
	case ROAD_T4:	pSafePedestrianTrack = AddTrack(35, 36); AddTrack(36, 40); AddTrack(40, 41); AddTrack(7, 8); AddTrack(8, 10); AddTrack(10, 12); AddTrack(12, 13); AddTrack(36, 22); AddTrack(22, 8); AddTrack(8, 1); AddTrack(40, 26); AddTrack(26, 12); AddTrack(12, 5); 	break;

	case ROAD_X:	AddTrack(35, 36); AddTrack(36, 38); AddTrack(38, 40); AddTrack(40, 41); AddTrack(7, 8); AddTrack(8, 10); AddTrack(10, 12); AddTrack(12, 13); AddTrack(36, 22); AddTrack(22, 8); AddTrack(8, 1); AddTrack(40, 26); AddTrack(26, 12); AddTrack(12, 5); pSafePedestrianTrack = AddTrack(36, 43); AddTrack(40, 47);	break;
	}
	

	// Add Chase Tracks	
	switch (nRoadType)
	{
	case ROAD_H:	AddTrack(21, 27); break;
	case ROAD_V:	AddTrack(3, 45); break;

	case ROAD_C1:	AddTrack(45, 24); AddTrack(24, 27); break;
	case ROAD_C2:	AddTrack(21, 24); AddTrack(24, 45); break;
	case ROAD_C3:	AddTrack(3, 24); AddTrack(24, 27); break;
	case ROAD_C4:	AddTrack(21, 24); AddTrack(24, 3); break;

	case ROAD_T1:	AddTrack(21, 24); AddTrack(24, 27); AddTrack(24, 45); break;
	case ROAD_T2:	AddTrack(3, 24); AddTrack(24, 45); AddTrack(24, 27); break;
	case ROAD_T3:	AddTrack(3, 24); AddTrack(24, 45); AddTrack(24, 21); break;
	case ROAD_T4:	AddTrack(21, 24); AddTrack(24, 27); AddTrack(24, 3); break;

	case ROAD_X:	AddTrack(3, 24); AddTrack(27, 24); AddTrack(45, 24); AddTrack(21, 24); break;
	}


	//// Road traffic tracks
	switch (nRoadType)
	{
	case ROAD_H:	pSafeCarTrack = AddTrack(14, 20);	AddTrack(28, 34);	break;
	case ROAD_V:	AddTrack(2, 44);	pSafeCarTrack = AddTrack(4, 46);	break;

	case ROAD_C1:	pSafeCarTrack = AddTrack(44, 16); AddTrack(16, 20); AddTrack(46, 32); AddTrack(32, 34); break;
	case ROAD_C2:	pSafeCarTrack = AddTrack(14, 18); AddTrack(18, 46); AddTrack(28, 30); AddTrack(30, 44); break;
	case ROAD_C3:	AddTrack(2, 30); AddTrack(30, 34); pSafeCarTrack = AddTrack(4, 18); AddTrack(18, 20); break;
	case ROAD_C4:	AddTrack(2, 16); AddTrack(16, 14); pSafeCarTrack = AddTrack(4, 32); AddTrack(32, 28); break;

		
	case ROAD_T1:	 AddTrack(14, 16); AddTrack(16, 18); AddTrack(18, 20); AddTrack(28, 30); AddTrack(30, 32); AddTrack(32, 34);
		AddTrack(16, 30); AddTrack(30, 44); AddTrack(18, 32); AddTrack(32, 46); break;

	case ROAD_T4:	 AddTrack(14, 16); AddTrack(16, 18); AddTrack(18, 20); AddTrack(28, 30); AddTrack(30, 32); AddTrack(32, 34);
		AddTrack(16, 30); AddTrack(16, 2); AddTrack(18, 32); AddTrack(18, 4); break;

	case ROAD_T2:	AddTrack(2, 16); AddTrack(16, 30); AddTrack(30, 44);  AddTrack(4, 18); AddTrack(18, 32); AddTrack(32, 46);
		AddTrack(16, 18); AddTrack(18, 20); AddTrack(30, 32); AddTrack(32, 34);	break;

	case ROAD_T3:	AddTrack(2, 16); AddTrack(16, 30); AddTrack(30, 44);  AddTrack(4, 18); AddTrack(18, 32); AddTrack(32, 46);
		AddTrack(14, 16); AddTrack(16, 18); AddTrack(28, 30); AddTrack(30, 32);	break;

	case ROAD_X:	
		AddTrack(2, 16); AddTrack(16, 30); AddTrack(30, 44); AddTrack(4, 18); AddTrack(18, 32); AddTrack(32, 46);
		 AddTrack(14, 16); AddTrack(16, 18); AddTrack(18, 20); AddTrack(28, 30); AddTrack(30, 32); AddTrack(32, 34); break;
	}


	// Stop Patterns, here we go, loads of data... :(

	// .PO.OP.
	// PP.P.PP
	// O.O.O.O
	// .P...P.
	// O.O.O.O
	// PP.P.PP
	// .PO.OP.

	// .PO.OP.
	// PP.P.PP
	// O.X.X.O
	// .P...P.
	// O.X.X.O
	// PP.P.PP
	// .PO.OP.

	// .PO.OP.
	// PP.X.PP
	// O.X.X.O
	// .X...X.
	// O.X.X.O
	// PP.X.PP
	// .PO.OP.

	auto stopmap = [&](const std::string &s)
	{
		StopPattern p;
		for (size_t i = 0; i < s.size(); i++)
			p.bStop[i] = (s[i] == 'X');			
		return p;
	};

	switch (nRoadType)
	{
	case ROAD_H:
	case ROAD_V:
	case ROAD_C1:
	case ROAD_C2:
	case ROAD_C3:
	case ROAD_C4:
		// Allow all
		/*vStopPattern.push_back(
			stopmap(
				".PO.OP."
				"PP.P.PP"
				"O.O.O.O"
				".P...P."
				"O.O.O.O"
				"PP.P.PP"
				".PO.OP."));*/
		break;

	case ROAD_X:
		// Allow Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".P...P."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.X.X"
				".X...X."
				"X.X.X.X"
				"PP.X.PP"
				".PX.XP."));
		// Allow West Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"O.O.O.O"
				".X...X."
				"X.X.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Drain West Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.O.O"
				".X...X."
				"X.X.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Allow North Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.OP."
				"PP.X.PP"
				"X.X.O.O"
				".X...X."
				"O.O.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Drain North Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.O.O"
				".X...X."
				"O.O.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Allow Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".P...P."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.X.X"
				".X...X."
				"X.X.X.X"
				"PP.X.PP"
				".PX.XP."));
		// Allow EAST Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.X.X"
				".X...X."
				"O.O.O.O"
				"PP.X.PP"
				".PX.OP."));
		// Drain East Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.X.X"
				".X...X."
				"O.O.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Allow SOUTH Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.O.O"
				".X...X."
				"O.O.X.X"
				"PP.X.PP"
				".PO.XP."));
		// Drain SOUTH Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.O.O"
				".X...X."
				"O.O.X.X"
				"PP.X.PP"
				".PX.XP."));

		break;

	case ROAD_T1:
		// Allow Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".P...P."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".X...X."
				"X.X.X.X"
				"PP.X.PP"
				".PX.XP."));
		// Allow West Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"O.O.O.O"
				".X...X."
				"X.X.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Drain West Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.O.O.O"
				".X...X."
				"X.X.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Allow Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".P...P."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.X.X"
				".X...X."
				"X.X.X.X"
				"PP.X.PP"
				".PX.XP."));
		// Allow EAST Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".X...X."
				"O.O.O.O"
				"PP.X.PP"
				".PX.OP."));
		// Drain East Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".X...X."
				"O.O.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Allow SOUTH Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.O.O.O"
				".X...X."
				"O.O.X.X"
				"PP.X.PP"
				".PO.XP."));
		// Drain SOUTH Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.O.O.O"
				".X...X."
				"O.O.X.X"
				"PP.X.PP"
				".PX.XP."));
		break;

	case ROAD_T2:
		// Allow Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".P...P."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.X.X"
				".P...X."
				"X.X.X.X"
				"PP.X.PP"
				".PX.XP."));
		// Allow North Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.OP."
				"PP.X.PP"
				"X.X.O.O"
				".P...X."
				"X.X.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Drain North Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.O.O"
				".P...X."
				"X.X.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Allow Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".P...P."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.X.X"
				".X...X."
				"X.X.X.X"
				"PP.X.PP"
				".PX.XP."));
		// Allow EAST Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.X.X"
				".P...X."
				"X.O.O.O"
				"PP.X.PP"
				".PX.OP."));
		// Drain East Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.X.X"
				".P...X."
				"X.O.O.X"
				"PP.X.PP"
				".PX.OP."));
		// Allow SOUTH Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.O.O"
				".P...X."
				"X.O.X.X"
				"PP.X.PP"
				".PO.XP."));
		// Drain SOUTH Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.O.O"
				".P...X."
				"X.O.X.X"
				"PP.X.PP"
				".PX.XP."));
		break;
	case ROAD_T3:
				// Allow Pedestrians
			vStopPattern.push_back(
				stopmap(
					".PX.XP." 
					"PP.P.PP" 
					"X.X.X.X" 
					".P...P." 
					"X.X.X.X" 
					"PP.P.PP" 
					".PX.XP."));
			// Drain Pedestrians
			vStopPattern.push_back(
				stopmap(
					".PX.XP."
					"PP.X.PP"
					"X.X.X.X"
					".X...P."
					"X.X.X.X"
					"PP.X.PP"
					".PX.XP."));
			// Allow West Traffic
			vStopPattern.push_back(
				stopmap(
					".PO.XP."
					"PP.X.PP"
					"O.O.O.X"
					".X...P."
					"X.X.O.X"
					"PP.X.PP"
					".PX.OP."));
			// Drain West Traffic
			vStopPattern.push_back(
				stopmap(
					".PO.XP."
					"PP.X.PP"
					"X.O.O.X"
					".X...P."
					"X.X.O.X"
					"PP.X.PP"
					".PX.OP."));
			// Allow Pedestrians
			vStopPattern.push_back(
				stopmap(
					".PX.XP."
					"PP.P.PP"
					"X.X.X.X"
					".P...P."
					"X.X.X.X"
					"PP.P.PP"
					".PX.XP."));
			// Drain Pedestrians
			vStopPattern.push_back(
				stopmap(
					".PX.XP."
					"PP.X.PP"
					"X.X.X.X"
					".X...X."
					"X.X.X.X"
					"PP.X.PP"
					".PX.XP."));
			// Allow North Traffic
			vStopPattern.push_back(
				stopmap(
					".PX.OP."
					"PP.X.PP"
					"X.X.O.X"
					".X...P."
					"O.O.O.X"
					"PP.X.PP"
					".PX.OP."));
			// Drain North Traffic
			vStopPattern.push_back(
				stopmap(
					".PX.XP."
					"PP.X.PP"
					"X.X.O.X"
					".X...P."
					"O.O.O.X"
					"PP.X.PP"
					".PX.OP."));
			
			// Allow SOUTH Traffic
			vStopPattern.push_back(
				stopmap(
					".PO.XP."
					"PP.X.PP"
					"X.O.X.X"
					".X...P."
					"O.O.X.X"
					"PP.X.PP"
					".PO.XP."));
			// Drain SOUTH Traffic
			vStopPattern.push_back(
				stopmap(
					".PO.XP."
					"PP.X.PP"
					"X.O.X.X"
					".X...P."
					"O.O.X.X"
					"PP.X.PP"
					".PX.XP."));
			break;

	case ROAD_T4:
		// Allow Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".P...P."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.X.X"
				".X...X."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Allow West Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"O.O.O.O"
				".X...X."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain West Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.O.O"
				".X...X."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Allow North Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.OP."
				"PP.X.PP"
				"X.X.O.O"
				".X...X."
				"O.O.O.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain North Traffic
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.O.O"
				".X...X."
				"O.O.O.X"
				"PP.P.PP"
				".PX.XP."));
		// Allow Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.P.PP"
				"X.X.X.X"
				".P...P."
				"X.X.X.X"
				"PP.P.PP"
				".PX.XP."));
		// Drain Pedestrians
		vStopPattern.push_back(
			stopmap(
				".PX.XP."
				"PP.X.PP"
				"X.X.X.X"
				".X...X."
				"X.X.X.X"
				"PP.X.PP"
				".PX.XP."));
		// Allow EAST Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.X.X"
				".X...X."
				"O.O.O.O"
				"PP.P.PP"
				".PX.XP."));
		// Drain East Traffic
		vStopPattern.push_back(
			stopmap(
				".PO.XP."
				"PP.X.PP"
				"X.O.X.X"
				".X...X."
				"O.O.O.X"
				"PP.P.PP"
				".PX.XP."));		
		break;


	}

}

bool cCell_Road::LinkAssets(std::map<std::string, olc::Sprite*>& mapTextures, std::map<std::string, olc::GFX3D::mesh*>& mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms)
{
	meshUnitQuad = mapMesh["UnitQuad"];
	sprRoadTex[ROAD_V] = mapTextures["Road_V"];
	sprRoadTex[ROAD_H] = mapTextures["Road_H"];
	sprRoadTex[ROAD_C1] = mapTextures["Road_C1"];
	sprRoadTex[ROAD_T1] = mapTextures["Road_T1"];
	sprRoadTex[ROAD_C2] = mapTextures["Road_C2"];
	sprRoadTex[ROAD_T2] = mapTextures["Road_T2"];
	sprRoadTex[ROAD_X] = mapTextures["Road_X"];
	sprRoadTex[ROAD_T3] = mapTextures["Road_T3"];
	sprRoadTex[ROAD_C3] = mapTextures["Road_C3"];
	sprRoadTex[ROAD_T4] = mapTextures["Road_T4"];
	sprRoadTex[ROAD_C4] = mapTextures["Road_C4"];
	return false;
}

bool cCell_Road::Update(float fElapsedTime)
{
	if (vStopPattern.empty())
		return false;

	fStopPatternTimer += fElapsedTime;
	if (fStopPatternTimer >= 5.0f)
	{
		fStopPatternTimer -= 5.0f;
		nCurrentStopPattern++;
		nCurrentStopPattern %= vStopPattern.size();
		for (int i = 0; i < 49; i++)
			if(pNaviNodes[i] != nullptr)
				pNaviNodes[i]->bBlock = vStopPattern[nCurrentStopPattern].bStop[i];
	}
	
	
	return false;
}

bool cCell_Road::DrawBase(olc::PixelGameEngine* pge, olc::GFX3D::PipeLine & pipe)
{
	olc::GFX3D::mat4x4 matWorld;
	matWorld = olc::GFX3D::Math::Mat_MakeTranslation((float)nWorldX, (float)nWorldY, 0.0f);
	pipe.SetTransform(matWorld);
	pipe.SetTexture(sprRoadTex[nRoadType]);
	pipe.Render(meshUnitQuad->tris, olc::GFX3D::RENDER_CULL_CW | olc::GFX3D::RENDER_DEPTH | olc::GFX3D::RENDER_TEXTURED);
	return false;
}

bool cCell_Road::DrawAlpha(olc::PixelGameEngine* pge, olc::GFX3D::PipeLine & pipe)
{
	return false;
}

bool cCell_Road::DrawDebug(olc::PixelGameEngine* pge, olc::GFX3D::PipeLine & pipe)
{



	// Draw Automata navigation tracks
	for (auto &track : listTracks)
	{
		olc::GFX3D::vec3d p1 = { track.node[0]->pos.x, track.node[0]->pos.y, 0.0f };
		olc::GFX3D::vec3d p2 = { track.node[1]->pos.x, track.node[1]->pos.y, 0.0f };
		pipe.RenderLine(p1, p2, olc::CYAN);
	}


	for (int i = 0; i < 49; i++)
	{
		if (pNaviNodes[i] != nullptr)
		{
			olc::GFX3D::vec3d p1 = { pNaviNodes[i]->pos.x, pNaviNodes[i]->pos.y,  0.01f };
			pipe.RenderCircleXZ(p1, 0.03f, pNaviNodes[i]->bBlock ? olc::RED : olc::GREEN);
		}
	}
	
	return false;
}
