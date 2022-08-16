/*
	Quirky Quad Trees Part #1 - Static Quad Tree Implementation
	"War... huh... What is it good for? Absolutely nothin..." - javidx9
	
	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018 - 2022 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	
	Video:
	~~~~~~
	https://youtu.be/ASAowY6yJII

	Pan & Zoom with middle mouse, TAB to switch between methods
	
	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com
	
	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021, 2022
*/


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"

namespace olc
{
	struct rect
	{
		olc::vf2d pos;
		olc::vf2d size;

		rect(const olc::vf2d& p = { 0.0f, 0.0f }, const olc::vf2d& s = { 1.0f, 1.0f }) : pos(p), size(s)
		{

		}

		constexpr bool contains(const olc::vf2d& p) const
		{
			return !(p.x < pos.x || p.y < pos.y || p.x >= (pos.x + size.x) || p.y >= (pos.y + size.y));
		}

		constexpr bool contains(const olc::rect& r) const
		{
			return (r.pos.x >= pos.x) && (r.pos.x + r.size.x < pos.x + size.x) &&
				(r.pos.y >= pos.y) && (r.pos.y + r.size.y < pos.y + size.y);
		}

		constexpr bool overlaps(const olc::rect& r) const
		{
			return (pos.x < r.pos.x + r.size.x && pos.x + size.x >= r.pos.x && pos.y < r.pos.y + r.size.y && pos.y + size.y >= r.pos.y);
		}
	};

};


// Constrain depth of Quad Tree. Since its floating point, it could in principle sub-divide for
// a very long time, consuming far more time and memory than is sensible
constexpr size_t MAX_DEPTH = 8;


template <typename OBJECT_TYPE>
class StaticQuadTree
{
public:
	StaticQuadTree(const olc::rect& size = { {0.0f, 0.0f}, {100.0f, 100.0f} }, const size_t nDepth = 0)
	{
		m_depth = nDepth;
		resize(size);
	}

	// Force area change on Tree, invalidates this and all child layers
	void resize(const olc::rect& rArea)
	{
		// Erase this layer
		clear();

		// Recalculate area of children
		m_rect = rArea;
		olc::vf2d vChildSize = m_rect.size / 2.0f;

		// Cache child areas local to this layer
		m_rChild =
		{
			// Top Left
			olc::rect(m_rect.pos, vChildSize),
			// Top Right
			olc::rect({m_rect.pos.x + vChildSize.x, m_rect.pos.y}, vChildSize),
			// Bottom Left
			olc::rect({m_rect.pos.x, m_rect.pos.y + vChildSize.y}, vChildSize),
			// Bottom Right
			olc::rect(m_rect.pos + vChildSize, vChildSize)
		};

	}

	// Clears the contents of this layer, and all child layers
	void clear()
	{
		// Erase any items stored in this layer
		m_pItems.clear();

		// Iterate through children, erase them too
		for (int i = 0; i < 4; i++)
		{
			if (m_pChild[i])
				m_pChild[i]->clear();
			m_pChild[i].reset();
		}
	}

	// Returns a count of how many items are stored in this layer, and all children of this layer
	size_t size() const
	{
		size_t nCount = m_pItems.size();
		for (int i = 0; i < 4; i++)
			if (m_pChild[i]) nCount += m_pChild[i]->size();
		return nCount;
	}

	// Inserts an object into this layer (or appropriate child layer), given the area the item occupies
	void insert(const OBJECT_TYPE& item, const olc::rect& itemsize)
	{
		// Check each child
		for (int i = 0; i < 4; i++)
		{
			// If the child can wholly contain the item being inserted
			if (m_rChild[i].contains(itemsize))
			{
				// Have we reached depth limit?
				if (m_depth + 1 < MAX_DEPTH)
				{
					// No, so does child exist?
					if (!m_pChild[i])
					{
						// No, so create it
						m_pChild[i] = std::make_shared<StaticQuadTree<OBJECT_TYPE>>(m_rChild[i], m_depth + 1);
					}

					// Yes, so add item to it
					m_pChild[i]->insert(item, itemsize);
					return;
				}
			}
		}

		// It didnt fit, so item must belong to this quad
		m_pItems.push_back({ itemsize, item });
	}

	// Returns a list of objects in the given search area
	std::list<OBJECT_TYPE> search(const olc::rect& rArea) const
	{
		std::list<OBJECT_TYPE> listItems;
		search(rArea, listItems);
		return listItems;
	}

	// Returns the objects in the given search area, by adding to supplied list
	void search(const olc::rect& rArea, std::list<OBJECT_TYPE>& listItems) const
	{
		// First, check for items belonging to this area, add them to the list
		// if there is overlap
		for (const auto& p : m_pItems)
		{
			if (rArea.overlaps(p.first))
				listItems.push_back(p.second);
		}

		// Second, recurse through children and see if they can
		// add to the list
		for (int i = 0; i < 4; i++)
		{
			if (m_pChild[i])
			{
				// If child is entirely contained within area, recursively
				// add all of its children, no need to check boundaries
				if (rArea.contains(m_rChild[i]))
					m_pChild[i]->items(listItems);

				// If child overlaps with search area then checks need
				// to be made
				else if (m_rChild[i].overlaps(rArea))
					m_pChild[i]->search(rArea, listItems);
			}
		}
	}

	void items(std::list<OBJECT_TYPE>& listItems) const
	{
		// No questions asked, just return child items
		for (const auto& p : m_pItems)
			listItems.push_back(p.second);

		// Now add children of this layer's items
		for (int i = 0; i < 4; i++)
			if (m_pChild[i]) m_pChild[i]->items(listItems);
	}


	std::list<OBJECT_TYPE> items() const
	{
		// No questions asked, just return child items
		std::list<OBJECT_TYPE> listItems;
		items(listItems);
		return listItems;
	}

	// Returns area of this layer
	const olc::rect& area()
	{
		return m_rect;
	}


protected:
	// Depth of this StaticQuadTree layer
	size_t m_depth = 0;

	// Area of this StaticQuadTree
	olc::rect m_rect;

	// 4 child areas of this StaticQuadTree
	std::array<olc::rect, 4> m_rChild{};

	// 4 potential children of this StaticQuadTree
	std::array<std::shared_ptr<StaticQuadTree<OBJECT_TYPE>>, 4> m_pChild{};

	// Items which belong to this StaticQuadTree
	std::vector<std::pair<olc::rect, OBJECT_TYPE>> m_pItems;
};


template <typename OBJECT_TYPE>
class StaticQuadTreeContainer
{
	// Using a std::list as we dont want pointers to be invalidated to objects stored in the
	// tree should the contents of the tree change
	using QuadTreeContainer = std::list<OBJECT_TYPE>;

protected:
	// The actual container
	QuadTreeContainer m_allItems;

	// Use our StaticQuadTree to store "pointers" instead of objects - this reduces
	// overheads when moving or copying objects 
	StaticQuadTree<typename QuadTreeContainer::iterator> root;

public:
	StaticQuadTreeContainer(const olc::rect& size = { {0.0f, 0.0f}, { 100.0f, 100.0f } }, const size_t nDepth = 0) : root(size, nDepth)
	{

	}

	// Sets the spatial coverage area of the quadtree
	// Invalidates tree
	void resize(const olc::rect& rArea)
	{
		root.resize(rArea);
	}

	// Returns number of items within tree
	size_t size() const
	{
		return m_allItems.size();
	}

	// Returns true if tree is empty
	bool empty() const
	{
		return m_allItems.empty();
	}

	// Removes all items from tree
	void clear()
	{
		root.clear();
		m_allItems.clear();
	}


	// Convenience functions for ranged for loop
	typename QuadTreeContainer::iterator begin()
	{
		return m_allItems.begin();
	}

	typename QuadTreeContainer::iterator end()
	{
		return m_allItems.end();
	}

	typename QuadTreeContainer::const_iterator cbegin()
	{
		return m_allItems.cbegin();
	}

	typename QuadTreeContainer::const_iterator cend()
	{
		return m_allItems.cend();
	}


	// Insert item into tree in specified area
	void insert(const OBJECT_TYPE& item, const olc::rect& itemsize)
	{
		// Item is stored in container
		m_allItems.push_back(item);

		// Pointer/Area of item is stored in quad tree
		root.insert(std::prev(m_allItems.end()), itemsize);
	}

	// Returns a std::list of pointers to items within the search area
	std::list<typename QuadTreeContainer::iterator> search(const olc::rect& rArea) const
	{
		std::list<typename QuadTreeContainer::iterator> listItemPointers;
		root.search(rArea, listItemPointers);
		return listItemPointers;
	}

};




// The Example!
class Example_StaticQuadTree : public olc::PixelGameEngine
{
public:
	Example_StaticQuadTree()
	{
		sAppName = "Static QuadTree";
	}

protected:
	olc::TransformedView tv;

	// An example object of something in 2D space
	struct SomeObjectWithArea
	{
		olc::vf2d vPos;
		olc::vf2d vVel;
		olc::vf2d vSize;
		olc::Pixel colour;
	};

	// A regular list of the objects
	std::list<SomeObjectWithArea> vecObjects;

	// An equivalent quad tree of the objects
	StaticQuadTreeContainer<SomeObjectWithArea> treeObjects;

	// The "length" of one side of the "world" the objects reside in
	float fArea = 100000.0f;

	bool bUseQuadTree = true;

public:
	bool OnUserCreate() override
	{
		// Transform View - enables Pan & Zoom
		tv.Initialise({ ScreenWidth(), ScreenHeight() });

		// Create the tree, and size it to the world
		treeObjects.resize(olc::rect({ 0.0f, 0.0f }, { fArea, fArea }));


		// Dirty random float generator
		auto rand_float = [](const float a, const float b)
		{
			return float(rand()) / float(RAND_MAX) * (b - a) + a;
		};


		// Create 1,000,000 objects, push into both containers (so 2,000,000 I suppose :P )
		for (int i = 0; i < 1000000; i++)
		{
			SomeObjectWithArea ob;
			ob.vPos = { rand_float(0.0f, fArea), rand_float(0.0f, fArea) };
			ob.vSize = { rand_float(0.1f, 100.0f), rand_float(0.1f, 100.0f) };
			ob.colour = olc::Pixel(rand() % 256, rand() % 256, rand() % 256);

			treeObjects.insert(ob, olc::rect(ob.vPos, ob.vSize));
			vecObjects.push_back(ob);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Tab switches between modes
		if (GetKey(olc::Key::TAB).bPressed)
			bUseQuadTree = !bUseQuadTree;

		tv.HandlePanAndZoom();

		// Get rectangle that equates to screen in world space
		olc::rect rScreen = { tv.GetWorldTL(), tv.GetWorldBR() - tv.GetWorldTL() };
		size_t nObjectCount = 0;

		if (bUseQuadTree)
		{
			// QUAD TREE MODE
			auto tpStart = std::chrono::system_clock::now();

			// Use search function to return list of pointers to objects in that area
			for (const auto& object : treeObjects.search(rScreen))
			{
				tv.FillRectDecal(object->vPos, object->vSize, object->colour);
				nObjectCount++;
			}
			std::chrono::duration<float> duration = std::chrono::system_clock::now() - tpStart;


			std::string sOutput = "Quadtree " + std::to_string(nObjectCount) + "/" + std::to_string(vecObjects.size()) + " in " + std::to_string(duration.count());
			DrawStringDecal({ 4, 4 }, sOutput, olc::BLACK, { 4.0f, 8.0f });
			DrawStringDecal({ 2, 2 }, sOutput, olc::WHITE, { 4.0f, 8.0f });

		}
		else
		{
			// LINEAR SEARCH MODE
			auto tpStart = std::chrono::system_clock::now();

			// Blindly check all objects to see if they overlap with screen
			for (const auto& object : vecObjects)
			{
				if (rScreen.overlaps({ object.vPos, object.vSize }))
				{
					tv.FillRectDecal(object.vPos, object.vSize, object.colour);
					nObjectCount++;
				}
			}
			std::chrono::duration<float> duration = std::chrono::system_clock::now() - tpStart;

			std::string sOutput = "Linear " + std::to_string(nObjectCount) + "/" + std::to_string(vecObjects.size()) + " in " + std::to_string(duration.count());
			DrawStringDecal({ 4, 4 }, sOutput, olc::BLACK, { 4.0f, 8.0f });
			DrawStringDecal({ 2, 2 }, sOutput, olc::WHITE, { 4.0f, 8.0f });
		}

		return true;
	}
};


int main()
{
	Example_StaticQuadTree demo;
	if (demo.Construct(1280, 960, 1, 1, false, false))
		demo.Start();
	return 0;
}