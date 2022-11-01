/*
	OneLoneCoder - QuadTree v1.01
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	A Dynamic Quad-Tree implementation to store objects in a 2D space
	with a fast retrieval.

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

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021, 2022


	Changes
	~~~~~~~
	v1.01:		Bug fix clear() function

*/


/*
	WARNING! VECTORS OF QUAD-TREES
	~~~~~~~~~~~~~~~~~~~~~~~~~~~
	This quad tree implementation will suffer when stored inside a container
	that can reallocate its contents. Upon reallocation, all of the iterators
	used to link the quad tree to its items will be invalidated.

	Currently, I advise caution when using std::vector of this quad tree, and
	should you choose to, then reserve space in advance so reallocation cannot
	occur.
*/


#pragma once

#include "olcPixelGameEngine.h"
#include "olcUTIL_Geometry2D.h"



namespace olc::utils
{
	template <typename T, typename CTYPE = float>
	struct QuadTreeItemLocation
	{
		typename std::list<std::pair<geom2d::rect<CTYPE>, T>>* container = nullptr;
		typename std::list<std::pair<geom2d::rect<CTYPE>, T>>::iterator iterator;
	};

	template <typename pT, typename CTYPE = float>
	class DynamicQuadTree
	{
	public:
		DynamicQuadTree(const geom2d::rect<CTYPE>& size, const size_t nDepth = 0, const size_t nMaxDepth = 8)
		{
			m_depth = nDepth;
			m_rect = size;
			m_maxdepth = nMaxDepth;
			resize(m_rect);
		}

		// Insert a region into this area
		QuadTreeItemLocation<pT> insert(const pT item, const geom2d::rect<CTYPE>& itemsize)
		{
			for (int i = 0; i < 4; i++)
			{
				if (geom2d::contains(m_rChild[i], itemsize))
				{
					// Have we reached depth limit?
					if (m_depth + 1 < m_maxdepth)
					{
						// No, so does child exist?
						if (!m_pChild[i])
						{
							// No, so create it
							m_pChild[i] = std::make_shared<DynamicQuadTree<pT>>(m_rChild[i], m_depth + 1, m_maxdepth);
						}

						// Yes, so add item to it
						return m_pChild[i]->insert(item, itemsize);
					}
				}
			}

			// It didnt fit, so item must belong to this geom2d::rect<CTYPE>
			m_pItems.push_back({ itemsize, item });
			return { &m_pItems, std::prev(m_pItems.end()) };
		}

		void relocate(pT item, const geom2d::rect<CTYPE>& rArea)
		{
			// Remove it
			remove(item);

			// Reinsert it with new location
			insert(item, rArea);
		}

		size_t size() const
		{
			size_t nCount = m_pItems.size();
			for (int i = 0; i < 4; i++)
				if (m_pChild[i]) nCount += m_pChild[i]->size();
			return nCount;
		}

		void search(const geom2d::rect<CTYPE>& rArea, std::list<pT>& listItems) const
		{
			// First, check for items belonging to this area, add them to the list
			// if there is overlap
			for (const auto& p : m_pItems)
			{
				if (geom2d::overlaps(rArea,p.first))
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
					if (geom2d::contains(rArea,m_rChild[i]))
						m_pChild[i]->items(listItems);

					// If child overlaps with search area then checks need
					// to be made
					else if (geom2d::overlaps(m_rChild[i],rArea))
						m_pChild[i]->search(rArea, listItems);
				}
			}


		}

		void items(std::list<pT>& listItems) const
		{
			// No questions asked, just return child items
			for (const auto& p : m_pItems)
				listItems.push_back(p.second);

			for (int i = 0; i < 4; i++)
				if (m_pChild[i]) m_pChild[i]->items(listItems);
		}

		void clear()
		{
			m_pItems.clear();

			for (int i = 0; i < 4; i++)
			{
				if (m_pChild[i])
					m_pChild[i]->clear();
				else
					m_pChild[i].reset();
			}
		}

		void resize(const geom2d::rect<CTYPE>& rArea)
		{
			clear();
			m_rect = rArea;
			olc::v2d_generic<CTYPE> vChildSize = m_rect.size / CTYPE(2);
			m_rChild =
			{
				geom2d::rect<CTYPE>(m_rect.pos, vChildSize),
				geom2d::rect<CTYPE>({m_rect.pos.x + vChildSize.x, m_rect.pos.y}, vChildSize),
				geom2d::rect<CTYPE>({m_rect.pos.x, m_rect.pos.y + vChildSize.y}, vChildSize),
				geom2d::rect<CTYPE>(m_rect.pos + vChildSize, vChildSize)
			};
		}


		const geom2d::rect<CTYPE>& area()
		{
			return m_rect;
		}

	protected:
		size_t m_depth = 0;
		size_t m_maxdepth = 8;

		// Area of this quadnode
		geom2d::rect<CTYPE> m_rect;

		// 4 child areas of this quadnode
		std::array<geom2d::rect<CTYPE>, 4> m_rChild{};

		// 4 potential children of this quadnode
		std::array<std::shared_ptr<DynamicQuadTree<pT>>, 4> m_pChild{};

		// Items which belong to this quadnode
		std::list<std::pair<geom2d::rect<CTYPE>, pT>> m_pItems;

	};

	template<typename T, typename CTYPE = float>
	struct QuadTreeItem
	{
		// The item Itself
		T item;

		// A "locator" to the container/iterator that points to this item's iterator in the
		// top level list - phew
		QuadTreeItemLocation<typename std::list<QuadTreeItem<T, CTYPE>>::iterator> pItem;
	};

	template <typename T, typename CTYPE = float>
	class QuadTreeContainer
	{
		using IQuadtreeContainer = std::list<QuadTreeItem<T, CTYPE>>;

	public:
		QuadTreeContainer(const geom2d::rect<CTYPE>& size = { {0.0f, 0.0f}, { 100.0f, 100.0f } }, const size_t nDepth = 0, const size_t nMaxDepth = 8) : root(size, nDepth, nMaxDepth)
		{

		}

		// Sets the spatial coverage area of teh quadtree
		void resize(const geom2d::rect<CTYPE>& rArea)
		{
			root.resize(rArea);
		}

		// Inserts an item into the quadtree
		void insert(const T& item, const geom2d::rect<CTYPE>& itemsize)
		{
			QuadTreeItem<T> newItem;
			newItem.item = item;

			// Item i stored in container
			m_allItems.emplace_back(newItem);

			// Pointer/Area of item is stored in geom2d::rect<CTYPE> tree
			m_allItems.back().pItem = root.insert(std::prev(m_allItems.end()), itemsize);
		}

		// Returns a std::list of pointers to items within the search area
		std::list<typename IQuadtreeContainer::iterator> search(const geom2d::rect<CTYPE>& rArea) const
		{
			std::list<typename IQuadtreeContainer::iterator> listItemPointers;
			root.search(rArea, listItemPointers);
			return listItemPointers;
		}

		void remove(typename IQuadtreeContainer::iterator& item)
		{
			// Iterator points to a QuadTreeItem
			item->pItem.container->erase(item->pItem.iterator);

			// Remove actual item from container
			m_allItems.erase(item);
		}

		void relocate(typename IQuadtreeContainer::iterator& item, const geom2d::rect<CTYPE>& itemsize)
		{
			// Remove pointer to item from whichever container its stored in
			item->pItem.container->erase(item->pItem.iterator);

			// Update the items pointer by reinsertion into geom2d::rect<CTYPE> tree
			item->pItem = root.insert(item, itemsize);

		}

		typename IQuadtreeContainer::iterator begin()
		{
			return m_allItems.begin();
		}

		typename IQuadtreeContainer::iterator end()
		{
			return m_allItems.end();
		}

		typename IQuadtreeContainer::const_iterator cbegin()
		{
			return m_allItems.cbegin();
		}

		typename IQuadtreeContainer::const_iterator cend()
		{
			return m_allItems.cend();
		}

		size_t size() const
		{
			return root.size();
		}

		void clear()
		{
			m_allItems.clear();
			root.clear();
		}

		const geom2d::rect<CTYPE>& area()
		{
			return root.area();
		}

	protected:
		DynamicQuadTree<typename IQuadtreeContainer::iterator> root;
		IQuadtreeContainer m_allItems;
	};
}
