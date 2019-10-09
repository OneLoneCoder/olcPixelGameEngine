#include "cCityMap.h"

#include <fstream>



cCityMap::cCityMap(int w, int h, std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms)
{
	CreateCity(w, h, mapTextures, mapMesh, mapTransforms);
}

cCityMap::~cCityMap()
{
	ReleaseCity();
}

int cCityMap::GetWidth()
{
	return nWidth;
}

int cCityMap::GetHeight()
{
	return nHeight;
}

cCell* cCityMap::Cell(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return pCells[y*nWidth + x];
	else
		return nullptr;
}

cCell* cCityMap::Replace(int x, int y, cCell* cell)
{
	if (cell == nullptr)
		return nullptr;

	if (pCells[y * nWidth + x] != nullptr)
		delete pCells[y * nWidth + x];

	pCells[y * nWidth + x] = cell;
	return cell;
}

void cCityMap::CreateCity(int w, int h, std::map<std::string, olc::Sprite*> &mapTextures, std::map<std::string, olc::GFX3D::mesh*> &mapMesh, std::map<std::string, olc::GFX3D::mat4x4> &mapTransforms)
{
	ReleaseCity();
	nWidth = w;
	nHeight = h;
	pCells = new cCell*[nHeight * nWidth];

	// Create Navigation Node Pool, assumes 5 nodes on east and south
	// side of each cell. The City owns these nodes, and cells in the
	// city borrow them and link to them as required
	pNodes = new cAuto_Node[nHeight * nWidth * 49];

	// The cell has 49 nodes, though some are simply unused. This is less memory
	// efficient certainly, but makes code more intuitive and easier to write

	for (int x = 0; x < nWidth; x++)
	{
		for (int y = 0; y < nHeight; y++)
		{
			// Nodes sit between cells, therefore each create nodes along
			// the east and southern sides of the cell. This assumes that
			// navigation along the top and left boundaries of the map
			// will not occur. And it shouldnt, as its water

			int idx = (y * nWidth + x) * 49;
			
			for (int dx = 0; dx < 7; dx++)
			{
				float off_x = 0.0f;
				switch (dx)
				{
				case 0: off_x = 0.000f; break; 
				case 1: off_x = 0.083f; break;
				case 2: off_x = 0.333f; break;
				case 3: off_x = 0.500f; break;
				case 4: off_x = 0.667f; break;
				case 5: off_x = 0.917f; break;
				case 6: off_x = 1.000f; break;
				}


				for (int dy = 0; dy < 7; dy++)
				{
					float off_y = 0.0f;
					switch (dy)
					{
					case 0: off_y = 0.000f; break;
					case 1: off_y = 0.083f; break;
					case 2: off_y = 0.333f; break;
					case 3: off_y = 0.500f; break;
					case 4: off_y = 0.667f; break;
					case 5: off_y = 0.917f; break;
					case 6: off_y = 1.000f; break;
					}

					pNodes[idx + dy * 7 + dx].pos = { (float)x + off_x, (float)y + off_y };
					pNodes[idx + dy * 7 + dx].bBlock = false;
				}
			}
		}
	}


	// Now create default Cell
	for (int x = 0; x < nWidth; x++)
	{
		for (int y = 0; y < nHeight; y++)
		{
			// Default city, everything is grass
			pCells[y * nWidth + x] = new cCell_Plane(this, x, y, PLANE_GRASS);

			// Give the cell the opportunity to locally reference the resources it needs
			pCells[y * nWidth + x]->LinkAssets(mapTextures, mapMesh, mapTransforms);
		}
	}
			
}

cAuto_Node* cCityMap::GetAutoNodeBase(int x, int y)
{
	return pNodes + (y * nWidth + x) * 49;
}

void cCityMap::RemoveAllTracks()
{
	for (int i = 0; i < nWidth * nHeight * 49; i++)
	{
		pNodes[i].listTracks.clear();
	}
}

void cCityMap::ReleaseCity()
{
	for (int x = 0; x < nWidth; x++)
	{
		for (int y = 0; y < nHeight; y++)
		{
			// Erase any tracks attached to nodes
			for(int i=0; i<49; i++)
				Cell(x, y)->pNaviNodes[i]->listTracks.clear();

			// Release individual cell objects
			delete pCells[y * nWidth + x];
		}
	}

	// Release array of cell pointers
	if (pCells != nullptr) delete pCells;
	
	// Release array of automata navigation nodes
	if (pNodes != nullptr) delete pNodes;

	nWidth = 0;
	nHeight = 0;
}


bool cCityMap::SaveCity(std::string sFilename)
{
	/*std::ofstream file(sFilename, std::ios::out | std::ios::binary);
	if (!file.is_open()) return false;
	
	file.write((char*)&m_nWidth, sizeof(int));
	file.write((char*)&m_nHeight, sizeof(int));

	for (int x = 0; x < m_nWidth; x++)
	{
		for (int y = 0; y < m_nHeight; y++)
		{
			file.write((char*)Cell(x, y), sizeof(cCityCell));
		}
	}*/

	return true;
}

bool cCityMap::LoadCity(std::string sFilename)
{
	/*std::ifstream file(sFilename, std::ios::in | std::ios::binary);
	if (!file.is_open()) return false;

	int w, h;
	file.read((char*)&w, sizeof(int));
	file.read((char*)&h, sizeof(int));
	CreateCity(w, h);
	
	for (int x = 0; x < m_nWidth; x++)
	{
		for (int y = 0; y < m_nHeight; y++)
		{
			file.read((char*)Cell(x, y), sizeof(cCityCell));
		}
	}*/

	return true;
}