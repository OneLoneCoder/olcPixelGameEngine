#include "cCarCrimeCity.h"

cCarCrimeCity::cCarCrimeCity()
{
	sAppName = "Car Crime City";
}

cCarCrimeCity::~cCarCrimeCity()
{
}

bool cCarCrimeCity::OnUserCreate()
{
	// Initialise PGEX 3D
	olc::GFX3D::ConfigureDisplay();

	// Load fixed system assets, i.e. those need to simply do anything
	if (!LoadAssets()) return false;

	// Create Default city
	pCity = new cCityMap(cGameSettings::nDefaultMapWidth, cGameSettings::nDefaultMapHeight, mapAssetTextures, mapAssetMeshes, mapAssetTransform);

	// If a city map file has been specified, then load it
	if (!cGameSettings::sDefaultCityFile.empty())
	{
		if (!pCity->LoadCity(cGameSettings::sDefaultCityFile))
		{
			std::cout << "Failed to load '" << cGameSettings::sDefaultCityFile << "'" << std::endl;
			return false;
		}
	}

	return true;
}

bool cCarCrimeCity::LoadAssets()
{
	// Game Settings should have loaded all the relevant file information
	// to start loading asset information. Game assets will be stored in
	// a map structure. Maps can have slightly longer access times, so each
	// in game object will have facility to extract required resources once
	// when it is created, meaning no map search during normal use

	// System Meshes
	// A simple flat unit quad
	olc::GFX3D::mesh* meshQuad = new olc::GFX3D::mesh(); 
	meshQuad->tris =
	{
			{ 0.0f, 0.0f, 0.0f, 1.0f,	    0.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 1.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 		1.0f, 1.0f, 0.0f, olc::WHITE, olc::WHITE, olc::WHITE },
			{ 0.0f, 0.0f, 0.0f, 1.0f,  		1.0f, 1.0f, 0.0f, 1.0f,		 1.0f, 0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f, 		1.0f, 0.0f, 0.0f, olc::WHITE, olc::WHITE, olc::WHITE },
	};
	mapAssetMeshes["UnitQuad"] = meshQuad;

	//// The four outer walls of a cell
	olc::GFX3D::mesh* meshWallsOut = new olc::GFX3D::mesh();
	meshWallsOut->tris =
	{
		// EAST
		{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.2f, 1.0f,		1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		0.0f, 0.0f, 0.0f, olc::WHITE, olc::WHITE, olc::WHITE },
		{ 1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.2f, 1.0f,		1.0f, 0.0f, 0.2f, 1.0f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		0.0f, 1.0f, 0.0f, olc::WHITE, olc::WHITE, olc::WHITE },

		// WEST
		{ 0.0f, 0.0f, 0.2f, 1.0f,		0.0f, 1.0f, 0.2f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, olc::WHITE, olc::WHITE, olc::WHITE },
		{ 0.0f, 0.0f, 0.2f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, olc::WHITE, olc::WHITE, olc::WHITE },

		// TOP             																 				    
		{ 0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.2f, 1.0f,		1.0f, 1.0f, 0.2f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		0.0f, 1.0f, 0.0f,   olc::WHITE, olc::WHITE, olc::WHITE },
		{ 0.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.2f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 		1.0f, 1.0f, 0.0f,   olc::WHITE, olc::WHITE, olc::WHITE },

		// BOTTOM         																 				   
		{ 1.0f, 0.0f, 0.2f, 1.0f,		0.0f, 0.0f, 0.2f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f, 		1.0f, 0.0f, 0.0f, olc::WHITE, olc::WHITE, olc::WHITE },
		{ 1.0f, 0.0f, 0.2f, 1.0f,		0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f, 		1.0f, 1.0f, 0.0f, olc::WHITE, olc::WHITE, olc::WHITE },
	};
	mapAssetMeshes["WallsOut"] = meshWallsOut;


	// System Textures
	for (auto &asset : cGameSettings::vecAssetTextures)
	{
		olc::Sprite *sprAsset = new olc::Sprite();
		if (sprAsset->LoadFromFile(asset.sFile))
		{
			mapAssetTextures[asset.sName] = sprAsset;
		}
		else
		{
			std::cout << "Failed to load " << asset.sName << std::endl;
			return false;
		}
	}

	// Break up roads sprite into individual sprites. Why? Its easier to maintain
	// the roads sprite as a single image, but easier to use if they are all individual.
	// Breaking it up manually in the image editing software is time consuming so just
	// do it here
	int nRoadTexSize = 256; // In pixels in base texture
	int nRoadTexOffset = 64; // There exists a 64 pixel offset from top left of source image
	for (int r = 0; r < 12; r++)
	{
		olc::Sprite* road = new olc::Sprite(nRoadTexSize, nRoadTexSize);
		SetDrawTarget(road);
		DrawPartialSprite(0, 0, mapAssetTextures["AllRoads"], ((r % 3) * nRoadTexSize) + nRoadTexOffset, ((r / 3) * nRoadTexSize) + nRoadTexOffset, nRoadTexSize, nRoadTexSize);
		switch (r)
		{
		case 0: mapAssetTextures["Road_V"] = road; break;
		case 1: mapAssetTextures["Road_H"] = road; break;
		case 2: mapAssetTextures["Pavement"] = road; break;
		case 3: mapAssetTextures["Road_C1"] = road; break;
		case 4: mapAssetTextures["Road_T1"] = road; break;
		case 5: mapAssetTextures["Road_C2"] = road; break;
		case 6: mapAssetTextures["Road_T2"] = road; break;
		case 7: mapAssetTextures["Road_X"] = road; break;
		case 8: mapAssetTextures["Road_T3"] = road; break;
		case 9: mapAssetTextures["Road_C3"] = road; break;
		case 10: mapAssetTextures["Road_T4"] = road; break;
		case 11: mapAssetTextures["Road_C4"] = road; break;
		}
	}
	SetDrawTarget(nullptr);


	// Load Buildings
	for (auto &asset : cGameSettings::vecAssetBuildings)
	{
		mapAssetMeshes[asset.sDescription] = new olc::GFX3D::mesh();
		mapAssetMeshes[asset.sDescription]->LoadOBJFile(asset.sModelOBJ);
		mapAssetTextures[asset.sDescription] =  new olc::Sprite(asset.sModelPNG);

		olc::GFX3D::mat4x4 matScale = olc::GFX3D::Math::Mat_MakeScale(asset.fScale[0], asset.fScale[1], asset.fScale[2]);
		olc::GFX3D::mat4x4 matTranslate = olc::GFX3D::Math::Mat_MakeTranslation(asset.fTranslate[0], asset.fTranslate[1], asset.fTranslate[2]);
		olc::GFX3D::mat4x4 matRotateX = olc::GFX3D::Math::Mat_MakeRotationX(asset.fRotate[0]);
		olc::GFX3D::mat4x4 matRotateY = olc::GFX3D::Math::Mat_MakeRotationY(asset.fRotate[1]);
		olc::GFX3D::mat4x4 matRotateZ = olc::GFX3D::Math::Mat_MakeRotationZ(asset.fRotate[2]);
		olc::GFX3D::mat4x4 matTransform = olc::GFX3D::Math::Mat_MultiplyMatrix(matTranslate, matScale);
		matTransform = olc::GFX3D::Math::Mat_MultiplyMatrix(matTransform, matRotateX);
		matTransform = olc::GFX3D::Math::Mat_MultiplyMatrix(matTransform, matRotateY);
		matTransform = olc::GFX3D::Math::Mat_MultiplyMatrix(matTransform, matRotateZ);
		mapAssetTransform[asset.sDescription] = matTransform;
	}

	// Load Vehicles
	for (auto &asset : cGameSettings::vecAssetVehicles)
	{
		mapAssetMeshes[asset.sDescription] = new olc::GFX3D::mesh();
		mapAssetMeshes[asset.sDescription]->LoadOBJFile(asset.sModelOBJ);
		mapAssetTextures[asset.sDescription] = new olc::Sprite(asset.sModelPNG);

		olc::GFX3D::mat4x4 matScale = olc::GFX3D::Math::Mat_MakeScale(asset.fScale[0], asset.fScale[1], asset.fScale[2]);
		olc::GFX3D::mat4x4 matTranslate = olc::GFX3D::Math::Mat_MakeTranslation(asset.fTranslate[0], asset.fTranslate[1], asset.fTranslate[2]);
		olc::GFX3D::mat4x4 matRotateX = olc::GFX3D::Math::Mat_MakeRotationX(asset.fRotate[0]);
		olc::GFX3D::mat4x4 matRotateY = olc::GFX3D::Math::Mat_MakeRotationY(asset.fRotate[1]);
		olc::GFX3D::mat4x4 matRotateZ = olc::GFX3D::Math::Mat_MakeRotationZ(asset.fRotate[2]);
		olc::GFX3D::mat4x4 matTransform = olc::GFX3D::Math::Mat_MultiplyMatrix(matTranslate, matScale);
		matTransform = olc::GFX3D::Math::Mat_MultiplyMatrix(matTransform, matRotateX);
		matTransform = olc::GFX3D::Math::Mat_MultiplyMatrix(matTransform, matRotateY);
		matTransform = olc::GFX3D::Math::Mat_MultiplyMatrix(matTransform, matRotateZ);
		mapAssetTransform[asset.sDescription] = matTransform;
	}

	return true;
}

void cCarCrimeCity::SpawnPedestrian(int x, int y)
{
	cCell* cell = pCity->Cell(x, y);

	cAuto_Track *t = ((cCell_Road*)cell)->pSafePedestrianTrack;
	if (t == nullptr) return;

	cAuto_Body *a = new cAuto_Body();
	a->fAutoLength = 0.05f;
	a->pCurrentTrack = t;
	a->pCurrentTrack->listAutos.push_back(a);
	a->pTrackOriginNode = t->node[0];
	a->UpdateAuto(0.0f);
	listAutomata.push_back(a);
}

void cCarCrimeCity::SpawnVehicle(int x, int y)
{
	cCell* cell = pCity->Cell(x, y);

	cAuto_Track *t = ((cCell_Road*)cell)->pSafeCarTrack;
	if (t == nullptr) return;

	cAuto_Body *a = new cAuto_Body();
	a->fAutoLength = 0.2f;
	a->pCurrentTrack = t;
	a->pCurrentTrack->listAutos.push_back(a);
	a->pTrackOriginNode = t->node[0];
	a->UpdateAuto(0.0f);	
	listAutomata.push_back(a);
}

void cCarCrimeCity::DoEditMode(float fElapsedTime)
{
	// Get cell under mouse cursor
	cCell* mcell = pCity->Cell(nMouseX, nMouseY);
	bool bTempCellAdded = false;

	// Left click and drag adds cells
	if (mcell != nullptr && GetMouse(0).bHeld)
		setSelectedCells.emplace(nMouseY * pCity->GetWidth() + nMouseX);

	// Right click clears selection
	if (GetMouse(1).bReleased)
		setSelectedCells.clear();
		
	if (setSelectedCells.empty())
	{
		// If nothing can be edited validly then just exit
		if (mcell == nullptr)
			return;

		// else set is empty, so temporarily add current cell to it
		setSelectedCells.emplace(nMouseY * pCity->GetWidth() + nMouseX);
		bTempCellAdded = true;
	}

	// If the map changes, we will need to update
	// the automata, and adjacency
	bool bMapChanged = false;

	// Press "G" to apply grass
	if (GetKey(olc::Key::G).bPressed)
	{		
		for (auto &c : setSelectedCells)
		{
			int x = c % pCity->GetWidth();
			int y = c / pCity->GetWidth();
			cCell* cell = pCity->Replace(x, y, new cCell_Plane(pCity, x, y, PLANE_GRASS));					
			cell->LinkAssets(mapAssetTextures, mapAssetMeshes, mapAssetTransform);
		}

		bMapChanged = true;
	}

	// Press "P" to apply Pavement
	if (GetKey(olc::Key::P).bPressed)
	{
		for (auto &c : setSelectedCells)
		{
			int x = c % pCity->GetWidth();
			int y = c / pCity->GetWidth();
			cCell* cell = pCity->Replace(x, y, new cCell_Plane(pCity, x, y, PLANE_ASPHALT));
			cell->LinkAssets(mapAssetTextures, mapAssetMeshes, mapAssetTransform);
		}

		bMapChanged = true;
	}

	// Press "W" to apply Water
	if (GetKey(olc::Key::W).bPressed)
	{
		for (auto &c : setSelectedCells)
		{
			int x = c % pCity->GetWidth();
			int y = c / pCity->GetWidth();
			cCell* cell = pCity->Replace(x, y, new cCell_Water(pCity, x, y));
			cell->LinkAssets(mapAssetTextures, mapAssetMeshes, mapAssetTransform);
		}

		bMapChanged = true;
	}

	// Press "R" to apply Roads
	if (GetKey(olc::Key::Q).bPressed)
	{
		for (auto &c : setSelectedCells)
		{
			int x = c % pCity->GetWidth();
			int y = c / pCity->GetWidth();
			cCell* cell = pCity->Replace(x, y, new cCell_Building("Apartments_1", pCity, x, y));
			cell->LinkAssets(mapAssetTextures, mapAssetMeshes, mapAssetTransform);
		}

		bMapChanged = true;
	}


	// Press "R" to apply Roads
	if (GetKey(olc::Key::R).bPressed)
	{
		for (auto &c : setSelectedCells)
		{
			int x = c % pCity->GetWidth();
			int y = c / pCity->GetWidth();
			cCell* cell = pCity->Replace(x, y, new cCell_Road(pCity, x, y));
			cell->LinkAssets(mapAssetTextures, mapAssetMeshes, mapAssetTransform);
		}

		bMapChanged = true;
	}



	if (GetKey(olc::Key::C).bPressed)
	{
		for (auto &c : setSelectedCells)
		{
			int x = c % pCity->GetWidth();
			int y = c / pCity->GetWidth();
			SpawnVehicle(x, y);			
		}
	}


	if (GetKey(olc::Key::V).bPressed)
	{
		for (auto &c : setSelectedCells)
		{
			int x = c % pCity->GetWidth();
			int y = c / pCity->GetWidth();
			SpawnPedestrian(x, y);			
		}		
	}

	if (bMapChanged)
	{
		// The navigation nodes may have tracks attached to them, so get rid of them
		// all. Below we will reconstruct all tracks because city has changed
		pCity->RemoveAllTracks();

		for (auto &a : listAutomata) delete a;
			listAutomata.clear();
		
		for (int x = 0; x < pCity->GetWidth(); x++)
		{
			for (int y = 0; y < pCity->GetHeight(); y++)
			{
				cCell *c = pCity->Cell(x, y);

				// Update adjacency information, i.e. those cells whose
				// state changes based on neighbouring cells
				c->CalculateAdjacency();
			}
		}
	}


	// To facilitate "edit under cursor" we added a temporary cell
	// which needs to be removed now
	if (bTempCellAdded)
		setSelectedCells.clear();
}

olc::vf2d cCarCrimeCity::GetMouseOnGround(const olc::vf2d &vMouseScreen)
{
	olc::GFX3D::vec3d vLookTarget = olc::GFX3D::Math::Vec_Add(vEye, vLookDir);
	olc::GFX3D::mat4x4 matProj = olc::GFX3D::Math::Mat_MakeProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);
	olc::GFX3D::mat4x4 matView = olc::GFX3D::Math::Mat_PointAt(vEye, vLookTarget, vUp);
	olc::GFX3D::vec3d vecMouseDir = {
		2.0f * ((vMouseScreen.x / (float)ScreenWidth()) - 0.5f) / matProj.m[0][0],
		2.0f * ((vMouseScreen.y / (float)ScreenHeight()) - 0.5f) / matProj.m[1][1],
		1.0f,
		0.0f };

	olc::GFX3D::vec3d vecMouseOrigin = { 0.0f, 0.0f, 0.0f };
	vecMouseOrigin = olc::GFX3D::Math::Mat_MultiplyVector(matView, vecMouseOrigin);
	vecMouseDir = olc::GFX3D::Math::Mat_MultiplyVector(matView, vecMouseDir);
	vecMouseDir = olc::GFX3D::Math::Vec_Mul(vecMouseDir, 1000.0f);
	vecMouseDir = olc::GFX3D::Math::Vec_Add(vecMouseOrigin, vecMouseDir);

	// Perform line/plane intersection to determine mouse position in world space
	olc::GFX3D::vec3d plane_p = { 0.0f, 0.0f, 0.0f };
	olc::GFX3D::vec3d plane_n = { 0.0f, 0.0f, 1.0f };
	float t = 0.0f;
	olc::GFX3D::vec3d mouse3d = olc::GFX3D::Math::Vec_IntersectPlane(plane_p, plane_n, vecMouseOrigin, vecMouseDir, t);
	return { mouse3d.x, mouse3d.y };
}

bool cCarCrimeCity::OnUserUpdate(float fElapsedTime)
{
	fGlobalTime += fElapsedTime;

	if (GetKey(olc::Key::TAB).bReleased) bEditMode = !bEditMode;

	if (bEditMode) // Use mouse to pan and zoom, and place objects
	{
		vEye = vCamera;
		olc::vf2d vMouseScreen = { (float)GetMouseX(), (float)GetMouseY() };
		olc::vf2d vMouseOnGroundBeforeZoom = GetMouseOnGround(vMouseScreen);

		vOffset = { 0,0 };

		if (IsFocused())
		{
			if (GetMouse(2).bPressed) { vStartPan = vMouseOnGroundBeforeZoom; }
			if (GetMouse(2).bHeld) { vOffset = (vStartPan - vMouseOnGroundBeforeZoom); };

			if (GetMouseWheel() > 0)
			{
				vCamera.z *= 0.5f;
			}

			if (GetMouseWheel() < 0)
			{
				vCamera.z *= 1.5f;
			}
		}

		vEye = vCamera;
		olc::vf2d vMouseOnGroundAfterZoom = GetMouseOnGround(vMouseScreen);
		vOffset += (vMouseOnGroundBeforeZoom - vMouseOnGroundAfterZoom);
		vCamera.x += vOffset.x;
		vCamera.y += vOffset.y;
		vEye = vCamera;

		// Get Integer versions of mouse coords in world space
		nMouseX = (int)vMouseOnGroundAfterZoom.x;
		nMouseY = (int)vMouseOnGroundAfterZoom.y;
		
		DoEditMode(fElapsedTime);
	}
	else
	{
		// Not in edit mode, so camera follows player
		if (GetKey(olc::Key::LEFT).bHeld) fAngle += -2.5f * fElapsedTime;
		if (GetKey(olc::Key::RIGHT).bHeld) fAngle += 2.5f * fElapsedTime;
		if (GetKey(olc::Key::UP).bHeld)
		{
			carvel = { cos(fAngle), sin(fAngle) };
			carpos += carvel * 2.0f * fElapsedTime;
		}

		vCamera.x = carpos.x;
		vCamera.y = carpos.y;
		vEye = vCamera;
	}

	/*fAngle = 0.0f;
	if (GetKey(olc::Key::LEFT).bHeld) fAngle = -0.8f;
	if (GetKey(olc::Key::RIGHT).bHeld) fAngle = 0.8f;*/


	//car.UpdateDrive(fElapsedTime, 1.0f, GetKey(olc::Key::UP).bHeld, GetKey(olc::Key::SPACE).bHeld, GetKey(olc::Key::DOWN).bHeld, fAngle);
	

	//if (car.bSkidding && fmod(fGlobalTime, 0.05f) < 0.01f)
	//{
	//	listDecalSmoke.push_front({ 0.1f, {car.vPosRear.x, car.vPosRear.y, -0.03f} });
	//}


	//// Update Decals
	//for (auto &d : listDecalSmoke)
	//{
	//	d.fLifetime += fElapsedTime;
	//}

	//listDecalSmoke.remove_if([](const sSmokeDecal &d) {return d.fLifetime > 2.0f; });

	//if (!bEditMode)
	//{
	//	vCamera.x = car.GetOrigin().x;
	//	vCamera.y = car.GetOrigin().y;
	//}


	//float fTargetHeight = -1.0f;
	//int nCarX = vCamera.x;
	//int nCarY = vCamera.y;

	std::vector<cGameObjectQuad> vecNeighbours;

	//// Check surrounding cells height
	//for (int x = nCarX - 1; x < nCarX + 2; x++)
	//	for (int y = nCarY - 1; y < nCarY + 2; y++)
	//	{
	//		if (pCity->Cell(x,y) && pCity->Cell(x, y)->bBuilding)
	//		{
	//			cGameObjectQuad ob(1.0f, 1.0f);
	//			ob.pos = { (float)x + 0.5f, (float)y + 0.5f, 0.0f, 1.0f };
	//			ob.TransformModelToWorld();
	//			vecNeighbours.push_back(ob);
	//			fTargetHeight = -2.0f;
	//		}
	//	}

	//goCar->pos.x = car.GetOrigin().x;
	//goCar->pos.y = car.GetOrigin().y;
	//goCar->fAngle = car.GetRotation();
	//goCar->TransformModelToWorld();

	//for (auto &ob : vecNeighbours)
	//{
	//	if (goCar->StaticCollisionWith(ob, true))
	//	{			
	//		goCar->TransformModelToWorld();
	//		car.vPosRear.x += goCar->pos.x - car.GetOrigin().x;
	//		car.vPosRear.y += goCar->pos.y - car.GetOrigin().y;
	//		car.vPosFront.x += goCar->pos.x - car.GetOrigin().x;
	//		car.vPosFront.y += goCar->pos.y - car.GetOrigin().y;
	//		car.fSpeed = 0.0f;
	//	}
	//}

	//if(!bEditMode)
	//	vCamera.z += (fTargetHeight - vCamera.z) * 10.0f * fElapsedTime;
	

	//car.UpdateTow(fElapsedTime, { mouse3d.x, mouse3d.y });

	

	//for (int v = 1; v<vecTraffic.size(); v++)
	//{
	//	//vecTraffic[v].UpdateTow(fElapsedTime * 10.0f, vecTraffic[v - 1].vPosRear);
	//}

	// Calculate Visible ground plane dimensions
	viewWorldTopLeft     = GetMouseOnGround(olc::vf2d( 0.0f, 0.0f ));
	viewWorldBottomRight = GetMouseOnGround(olc::vf2d((float)ScreenWidth(), (float)ScreenHeight()));

	// Calculate visible world extents
	int nStartX = std::max(0, (int)viewWorldTopLeft.x - 1);
	int nEndX = std::min(pCity->GetWidth(), (int)viewWorldBottomRight.x + 1);
	int nStartY = std::max(0, (int)viewWorldTopLeft.y - 1);
	int nEndY = std::min(pCity->GetHeight(), (int)viewWorldBottomRight.y + 1);

	// Only update automata for cells near player
	int nAutomStartX = std::max(0, (int)viewWorldTopLeft.x - 3);
	int nAutomEndX = std::min(pCity->GetWidth(), (int)viewWorldBottomRight.x + 3);
	int nAutomStartY = std::max(0, (int)viewWorldTopLeft.y - 3);
	int nAutomEndY = std::min(pCity->GetHeight(), (int)viewWorldBottomRight.y + 3);

	int nLocalStartX = std::max(0, (int)vCamera.x - 3);
	int nLocalEndX = std::min(pCity->GetWidth(), (int)vCamera.x + 3);
	int nLocalStartY = std::max(0, (int)vCamera.y - 3);
	int nLocalEndY = std::min(pCity->GetHeight(), (int)vCamera.y + 3);

	
	// Update Cells
	for (int x = nStartX; x < nEndX; x++)
	{
		for (int y = nStartY; y < nEndY; y++)
		{
			pCity->Cell(x, y)->Update(fElapsedTime);
		}
	}

	// Update Automata
	for (auto &a : listAutomata)
	{
		a->UpdateAuto(fElapsedTime);
		
		// If automata is too far from camera, remove it
		if ((a->vAutoPos - olc::vf2d(vCamera.x, vCamera.y)).mag() > 5.0f)
		{
			// Despawn automata

			// 1) Disconnect it from track
			a->pCurrentTrack->listAutos.remove(a);

			// 2) Erase it from memory
			delete a; a = nullptr;			
		}
	}
	
	// Remove dead automata, their pointer has been set to nullptr in the list
	listAutomata.remove(nullptr);

	// Maintain a certain level of automata in vicinty of player
	if (listAutomata.size() < 20)
	{
		bool bSpawnOK = false;
		int nSpawnAttempt = 20;
		while (!bSpawnOK && nSpawnAttempt > 0)
		{
			// Find random cell on edge of vicinty, which is out of view of the player
			float fRandomAngle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
			int nRandomCellX = vCamera.x + cos(fRandomAngle) * 3.0f;
			int nRandomCellY = vCamera.y + sin(fRandomAngle) * 3.0f;

			nSpawnAttempt--;

			if (pCity->Cell(nRandomCellX, nRandomCellY) && pCity->Cell(nRandomCellX, nRandomCellY)->nCellType == CELL_ROAD)
			{
				bSpawnOK = true;

				// Add random automata
				if (rand() % 100 < 50)
				{
					// Spawn Pedestrian
					SpawnPedestrian(nRandomCellX, nRandomCellY);
				}
				else
				{
					// Spawn Vehicle
					SpawnVehicle(nRandomCellX, nRandomCellY);
					// TODO: Get % chance of vehicle spawn from lua script
				}
			}
		}		
	}

	


	// Render Scene
	Clear(olc::BLUE);
	olc::GFX3D::ClearDepth();

	// Create rendering pipeline
	olc::GFX3D::PipeLine pipe;
	pipe.SetProjection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f, 0.0f, 0.0f, (float)ScreenWidth(), (float)ScreenHeight());
	olc::GFX3D::vec3d vLookTarget = olc::GFX3D::Math::Vec_Add(vEye, vLookDir);
	pipe.SetCamera(vEye, vLookTarget, vUp);


	// Add global illumination vector (sunlight)
	olc::GFX3D::vec3d lightdir = { 1.0f, 1.0f, -1.0f };
	pipe.SetLightSource(0, olc::GFX3D::LIGHT_AMBIENT, olc::Pixel(100,100,100), { 0,0,0 }, lightdir);
	pipe.SetLightSource(1, olc::GFX3D::LIGHT_DIRECTIONAL, olc::WHITE, { 0,0,0 }, lightdir);


	// RENDER CELL CONTENTS

	// Render Base Objects (those without alpha components)
	for (int x = nStartX; x < nEndX; x++)
	{
		//omp_set_dynamic(0);
		//omp_set_num_threads(4);
		//#pragma omp parallel for
		for (int y = nStartY; y < nEndY; y++)
		{
			pCity->Cell(x, y)->DrawBase(this, pipe);
		}
		//#pragma omp barrier
	}

	// Render Upper Objects (those with alpha components)
	for (int x = nStartX; x < nEndX; x++)
	{
		for (int y = nStartY; y < nEndY; y++)
		{
			pCity->Cell(x, y)->DrawAlpha(this, pipe);
		}
	}

	if (bEditMode)
	{
		// Render additional per cell debug information
		for (int x = nStartX; x < nEndX; x++)
		{
			for (int y = nStartY; y < nEndY; y++)
			{
				pCity->Cell(x, y)->DrawDebug(this, pipe);
			}
		}
	}
				
	if (bEditMode)
	{
		// Draw Selections
		for (auto &c : setSelectedCells)
		{
			int x = c % pCity->GetWidth();
			int y = c / pCity->GetWidth();
			olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeTranslation((float)x, (float)y, 0.01f);
			pipe.SetTransform(matWorld);
			pipe.Render(mapAssetMeshes["UnitQuad"]->tris, olc::GFX3D::RENDER_WIRE);
		}
	}

	// RENDER AUTOMATA

	std::string test[] = { "Sedan", "SUV", "TruckCab", "TruckTrailer", "UTE", "Wagon" };
	int i = 0;
	for (auto &a : listAutomata)
	{
		olc::GFX3D::vec3d v = { a->vAutoPos.x, a->vAutoPos.y, 0.0f };
		
		/*olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeTranslation(a->vAutoPos.x, a->vAutoPos.y, 0.01f);
		matWorld = olc::GFX3D::Math::Mat_MultiplyMatrix(mapAssetTransform[test[i]], matWorld);
		pipe.SetTransform(matWorld);
		pipe.SetTexture(mapAssetTextures[test[i]]);
		pipe.Render(mapAssetMeshes[test[i]]->tris, olc::GFX3D::RENDER_CULL_CW | olc::GFX3D::RENDER_DEPTH | olc::GFX3D::RENDER_TEXTURED | olc::GFX3D::RENDER_LIGHTS);
		i++;
		i = i % 6;*/
		
		pipe.RenderCircleXZ(v, a->fAutoLength < 0.1f ? 0.05f : 0.07f, a->fAutoLength < 0.1f ? olc::MAGENTA : olc::YELLOW);
	}


	// Draw Player Vehicle
	{
		olc::GFX3D::mat4x4 matRotateZ = olc::GFX3D::Math::Mat_MakeRotationZ(fAngle);
		olc::GFX3D::mat4x4 matTranslate = olc::GFX3D::Math::Mat_MakeTranslation(carpos.x, carpos.y, 0.01f);
		olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MultiplyMatrix(mapAssetTransform["Sedan"], matRotateZ);		
		matWorld = olc::GFX3D::Math::Mat_MultiplyMatrix(matWorld, matTranslate);
		pipe.SetTransform(matWorld);
		pipe.SetTexture(mapAssetTextures["Sedan"]);
		pipe.Render(mapAssetMeshes[test[i]]->tris, olc::GFX3D::RENDER_CULL_CW | olc::GFX3D::RENDER_DEPTH | olc::GFX3D::RENDER_TEXTURED | olc::GFX3D::RENDER_LIGHTS);
	}

	DrawString(10, 10, "Automata: " + std::to_string(listAutomata.size()), olc::WHITE);


	if (GetKey(olc::Key::ESCAPE).bPressed)
		return false;

	return true;
}

bool cCarCrimeCity::OnUserDestroy()
{
	return true;
}
