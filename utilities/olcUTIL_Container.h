/*
	OneLoneCoder - Container v1.00
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Assortment of std::container like objects with access specific mechanisms


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

*/

/*
	SingleSelection
	~~~~~~~~~~~~~~~
	This container behaves like a std::vector in all circumstances but features
	additional methods that allow it to surve as a list of items, one of which
	can be selected, and the items can be manipulated.

	An example of this would be the image layers in Photoshop.

	For convenience, all container operations operate on an item index rather
	than an iterator
*/

#pragma once

#include <vector>
#include <algorithm>

namespace olc::utils::Container
{
	template<typename T>
	class SingleSelection : public std::vector<T>
	{
	public:

		SingleSelection() : std::vector<T>() 
		{}
		

		SingleSelection(std::initializer_list<T> list) : std::vector<T>(list)
		{}

		// Returns selected item in container
		size_t selection() const
		{
			return m_nSelectedItem;
		}

		// Return the item actually selected
		const T& selected() const
		{
			return this->operator[](m_nSelectedItem);
		}

		// Return the item actually selected
		T& selected()
		{
			return this->operator[](m_nSelectedItem);
		}

		// Select item in container
		void select(const size_t item)
		{
			m_nSelectedItem = std::clamp(item, size_t(0), this->size() - size_t(1));
		}

		// Move selected item positively
		void move_up()
		{
			if(move_up(m_nSelectedItem))
				m_nSelectedItem++;
		}

		// Move selected item negatively
		void move_down()
		{
			if(move_down(m_nSelectedItem))
				m_nSelectedItem--;
		}

		// Move specified item negatively
		bool move_down(const size_t item)
		{
			// Are there at least two items and not first one selected?
			if (this->size() >= 2 && item > 0)
			{
				std::swap(this->operator[](item - 1), this->operator[](item));
				return true;
			}
			return false;
		}

		// Move specified item positively
		bool move_up(const size_t item)
		{
			// Are there at least two items and not last one selected?
			if (this->size() >= 2 && item < this->size() - 1)
			{
				std::swap(this->operator[](item + 1), this->operator[](item));
				return true;
			}
			return false;
		}

		void insert_after(const size_t idx, const T& value)
		{
			this->insert(idx, value);
		}


	protected:
		size_t m_nSelectedItem = 0;
	};
}