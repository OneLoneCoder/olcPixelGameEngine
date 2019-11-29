#pragma once
#include "olcPixelGameEngine.h"

#include <algorithm>
#undef min
#undef max

namespace olc
{
	
	class TILE : public olc::PGEX
	{
		
	public:
		

		struct Edge
		{
			float sx, sy;
			float ex, ey;
		};

	public:
		class Atlas
		{
		public:
			Atlas();
			void Create(olc::Sprite *tileSheet);
			olc::rcode LoadFromFile(std::string filename);
			olc::rcode SaveToFile(std::string filename);

		public:
			olc::Sprite *sprTileSheet;
			std::vector<std::tuple<int32_t, int32_t, int32_t, int32_t>> location;
		};

	public:

		template <class T>
		class Layer
		{
		public:
			Layer();
			void Create(int32_t w, int32_t h, int32_t tw, int32_t th);
			olc::rcode LoadFromFile(std::string filename);
			olc::rcode SaveToFile(std::string filename);
			T* GetTile(int32_t x, int32_t y);

		public:
			int32_t nLayerWidth;
			int32_t nLayerHeight;
			int32_t nTileWidth;
			int32_t nTileHeight;
		
		private:
			T		*pTiles;
			
		};

		class BasicTile
		{
		public:
			BasicTile();

		public:
			int32_t id;
			bool exist;

			int  edge_id[4];
			bool edge_exist[4];
		};

	public:
		template<typename T>
		static void DrawLayer(olc::TILE::Layer<T> &layer, olc::TILE::Atlas &atlas, float cam_x, float cam_y, int tiles_x, int tiles_y, int nScale = 1);
		
		template<typename T>
		static olc::Pixel GetLayerPixel(olc::TILE::Layer<T> &layer, olc::TILE::Atlas &atlas, float x, float y);

		template<typename T>
		static std::vector<olc::TILE::Edge> ExtractEdgesFromLayer(olc::TILE::Layer<T> &layer, int sx, int sy, int width, int height);

	};
}




namespace olc
{
	TILE::BasicTile::BasicTile()
	{
		exist = false;
		id = 0;

		for (int i = 0; i < 4; i++)
		{
			edge_exist[i] = false;
			edge_id[i] = 0;
		}
	}

	template <class T>
	TILE::Layer<T>::Layer()
	{

	}



	template <class T>
	void TILE::Layer<T>::Create(int32_t w, int32_t h, int32_t tw, int32_t th)
	{
		nLayerWidth = w;
		nLayerHeight = h;
		nTileWidth = tw;
		nTileHeight = th;

		pTiles = new T[nLayerWidth * nLayerHeight];
		for (int i = 0; i < nLayerWidth*nLayerHeight; i++)
		{
			pTiles[i].id = 0;
		}
	}

	template <class T>
	olc::rcode TILE::Layer<T>::LoadFromFile(std::string filename)
	{
		return olc::FAIL;
	}

	template <class T>
	olc::rcode TILE::Layer<T>::SaveToFile(std::string filename)
	{
		return olc::FAIL;
	}

	template <class T>
	T* TILE::Layer<T>::GetTile(int32_t x, int32_t y)
	{
		if (x < 0 || x >= nLayerWidth || y < 0 || y >= nLayerHeight)
			return nullptr;
		else
			return &pTiles[y*nLayerWidth + x];
	}

	template<typename T>
	void TILE::DrawLayer(olc::TILE::Layer<T> &layer, olc::TILE::Atlas &atlas, float cam_x, float cam_y, int32_t tiles_x, int32_t tiles_y, int nScale)
	{
		float fOffsetX = cam_x - (int)cam_x;
		float fOffsetY = cam_y - (int)cam_y;
		
		for (int32_t x = 0; x < tiles_x; x++)
		{
			for (int32_t y = 0; y < tiles_y; y++)
			{
				olc::TILE::BasicTile *t = layer.GetTile(x + (int)cam_x, y + (int)cam_y);
				if (t != nullptr && t->exist)
				{
					float fx = (int)(((float)x - fOffsetX) * (float)(layer.nTileWidth));
					float fy = (int)(((float)y - fOffsetY) * (float)(layer.nTileHeight));

					pge->DrawPartialSprite(
						fx + 0.5f - (fx < 0.0f),
						fy + 0.5f - (fy < 0.0f),
						atlas.sprTileSheet,
						std::get<0>(atlas.location[t->id]),
						std::get<1>(atlas.location[t->id]),
						std::get<2>(atlas.location[t->id]),
						std::get<3>(atlas.location[t->id]), 
						nScale);						
				}
			}
		}
	}

	template<typename T>
	olc::Pixel TILE::GetLayerPixel(olc::TILE::Layer<T> &layer, olc::TILE::Atlas &atlas, float x, float y)
	{
		olc::TILE::BasicTile *t = layer.GetTile((int32_t)x, (int32_t)y);
		if (t != nullptr)
		{
			float fOffsetX = x - (int)x;
			float fOffsetY = y - (int)y;
			return atlas.sprTileSheet->GetPixel(std::get<0>(atlas.location[t->id]) + fOffsetX * std::get<2>(atlas.location[t->id]),
				std::get<1>(atlas.location[t->id]) + fOffsetX * std::get<3>(atlas.location[t->id]));
		}
		else
			return olc::BLANK;
	}

	template<typename T>
	std::vector<olc::TILE::Edge> TILE::ExtractEdgesFromLayer(olc::TILE::Layer<T> &layer, int sx, int sy, int width, int height)
	{
		enum
		{
			NORTH = 0,
			EAST = 1,
			SOUTH = 2,
			WEST = 3
		};

		std::vector<olc::TILE::Edge> vecEdges;

		for (int x = -1; x < width + 1; x++)
			for (int y = -1; y < height + 1; y++)
				for (int j = 0; j < 4; j++)
				{
					if ((x + sx) >= 0 && (y + sy) >= 0 && (x + sx) < (layer.nLayerWidth - 1) && (y + sy) < (layer.nLayerHeight - 1))
					{
						layer.GetTile(x + sx, y + sy)->edge_exist[j] = false;
						layer.GetTile(x + sx, y + sy)->edge_id[j] = 0;
					}
				}

		// Add boundary edges
		vecEdges.push_back({ (float)(sx)* layer.nTileWidth,			(float)(sy)*layer.nTileHeight,				(float)(sx + width)*layer.nTileWidth,	(float)(sy)*layer.nTileHeight });
		vecEdges.push_back({ (float)(sx + width)* layer.nTileWidth,	(float)(sy)*layer.nTileHeight,				(float)(sx + width)*layer.nTileWidth,	(float)(sy + height)*layer.nTileHeight });
		vecEdges.push_back({ (float)(sx + width)* layer.nTileWidth,	(float)(sy + height)*layer.nTileHeight,		(float)(sx)*layer.nTileWidth,			(float)(sy + height)*layer.nTileHeight });
		vecEdges.push_back({ (float)(sx)* layer.nTileWidth,			(float)(sy + height)*layer.nTileHeight,		(float)(sx)*layer.nTileWidth,			(float)(sy)*layer.nTileHeight });


		// Iterate through region from top left to bottom right
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				T* i = layer.GetTile(x + sx, y + sy);		//This
				T* n = layer.GetTile(x + sx, y + sy - 1);
				T* s = layer.GetTile(x + sx, y + sy + 1);
				T* w = layer.GetTile(x + sx - 1, y + sy);
				T* e = layer.GetTile(x + sx + 1, y + sy);

				// If this cell exists, check if it needs edges
				if (i->exist)
				{
					// If this cell has no western neighbour, it needs a western edge
					if (w && !w->exist)
					{
						// It can either extend it from its northern neighbour if they have
						// one, or It can start a new one.
						if (n && n->edge_exist[WEST])
						{
							// Northern neighbour has a western edge, so grow it downwards
							vecEdges[n->edge_id[WEST]].ey += layer.nTileHeight;
							i->edge_id[WEST] = n->edge_id[WEST];
							i->edge_exist[WEST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							olc::TILE::Edge edge;
							edge.sx = (sx + x) * layer.nTileWidth; edge.sy = (sy + y) * layer.nTileHeight;
							edge.ex = edge.sx; edge.ey = edge.sy + layer.nTileHeight;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							i->edge_id[WEST] = edge_id;
							i->edge_exist[WEST] = true;
						}
					}


					// If this cell dont have an eastern neignbour, It needs a eastern edge
					if (e && !e->exist)
					{
						// It can either extend it from its northern neighbour if they have
						// one, or It can start a new one.
						if (n && n->edge_exist[EAST])
						{
							// Northern neighbour has one, so grow it downwards
							vecEdges[n->edge_id[EAST]].ey += layer.nTileHeight;
							i->edge_id[EAST] = n->edge_id[EAST];
							i->edge_exist[EAST] = true;
						}
						else
						{
							// Northern neighbour does not have one, so create one
							olc::TILE::Edge edge;
							edge.sx = (sx + x + 1) * layer.nTileWidth; edge.sy = (sy + y) * layer.nTileHeight;
							edge.ex = edge.sx; edge.ey = edge.sy + layer.nTileHeight;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							i->edge_id[EAST] = edge_id;
							i->edge_exist[EAST] = true;
						}
					}

					// If this cell doesnt have a northern neignbour, It needs a northern edge
					if (n && !n->exist)
					{
						// It can either extend it from its western neighbour if they have
						// one, or It can start a new one.
						if (w && w->edge_exist[NORTH])
						{
							// Western neighbour has one, so grow it eastwards
							vecEdges[w->edge_id[NORTH]].ex += layer.nTileWidth;
							i->edge_id[NORTH] = w->edge_id[NORTH];
							i->edge_exist[NORTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so create one
							olc::TILE::Edge edge;
							edge.sx = (sx + x) * layer.nTileWidth; edge.sy = (sy + y) * layer.nTileHeight;
							edge.ex = edge.sx + layer.nTileWidth; edge.ey = edge.sy;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							i->edge_id[NORTH] = edge_id;
							i->edge_exist[NORTH] = true;
						}
					}

					// If this cell doesnt have a southern neignbour, It needs a southern edge
					if (s && !s->exist)
					{
						// It can either extend it from its western neighbour if they have
						// one, or It can start a new one.
						if (w && w->edge_exist[SOUTH])
						{
							// Western neighbour has one, so grow it eastwards
							vecEdges[w->edge_id[SOUTH]].ex += layer.nTileWidth;
							i->edge_id[SOUTH] = w->edge_id[SOUTH];
							i->edge_exist[SOUTH] = true;
						}
						else
						{
							// Western neighbour does not have one, so I need to create one
							olc::TILE::Edge edge;
							edge.sx = (sx + x) * layer.nTileWidth; edge.sy = (sy + y + 1) * layer.nTileHeight;
							edge.ex = edge.sx + layer.nTileWidth; edge.ey = edge.sy;

							// Add edge to Polygon Pool
							int edge_id = vecEdges.size();
							vecEdges.push_back(edge);

							// Update tile information with edge information
							i->edge_id[SOUTH] = edge_id;
							i->edge_exist[SOUTH] = true;
						}
					}
				}
			}

			return vecEdges;
	}



	TILE::Atlas::Atlas()
	{
	}

	void TILE::Atlas::Create(olc::Sprite *tileSheet)
	{
		sprTileSheet = tileSheet;
		location.clear();
		
	}
	
	olc::rcode TILE::Atlas::LoadFromFile(std::string filename)
	{
		return olc::FAIL;
	}

	olc::rcode TILE::Atlas::SaveToFile(std::string filename)
	{
		return olc::FAIL;
	}

}
