/*
	olcPGEX_PopUp.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                Retro PopUp Menu 1.0                         |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	a quick and easy to use, flexible, skinnable context pop-up 
	menu system.

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2020 OneLoneCoder.com

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
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020
*/


/*
	Example
	~~~~~~~

	#define OLC_PGEX_POPUPMENU
	#include "olcPGEX_PopUpMenu.h"

	NOTE: Requires a 9-patch sprite, by default each patch is
	8x8 pixels, patches are as follows:

	| PANEL TL | PANEL T | PANEL TR | SCROLL UP   | CURSOR TL | CURSOR TR |
	| PANEL L  | PANEL M | PANEL R  | SUBMENU     | CURSOR BL | CURSOR BR |
	| PANEL BL | PANEL B | PANEL BR | SCROLL DOWN | UNUSED    | UNUSED    |

	You can find an example sprite here:
	https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/RetroMenu.png

	Constructing A Menu
	~~~~~~~~~~~~~~~~~~~

	// Declaration (presumably inside class)
	olc::popup::Menu m;
	
	// Construction (root menu is a 1x5 table)
	m.SetTable(1, 5);

	// Add first item  to root menu (A 1x5 submenu)
	m["Menu1"].SetTable(1, 5);

	// Add items to first item
	m["Menu1"]["Item1"];
	m["Menu1"]["Item2"];

	// Add a 4x3 submenu
	m["Menu1"]["Item3"].SetTable(4, 3);
	m["Menu1"]["Item3"]["Option1"];
	m["Menu1"]["Item3"]["Option2"];

	// Set properties of specific item
	m["Menu1"]["Item3"]["Option3"].Enable(false);
	m["Menu1"]["Item3"]["Option4"];
	m["Menu1"]["Item3"]["Option5"];
	m["Menu1"]["Item4"];

	// Add second item to root menu
	m["Menu2"].SetTable(3, 3);
	m["Menu2"]["Item1"];
	m["Menu2"]["Item2"].SetID(1001).Enable(true);
	m["Menu2"]["Item3"];

	// Construct the menu structure
	m.Build();


	Displaying a Menu
	~~~~~~~~~~~~~~~~~

	// Declaration of menu manager (presumably inside class)
	olc::popup::Manager man;

	// Set the Menu object to the MenuManager (call once per pop)
	man.Open(&m);

	// Draw Menu at position (30, 30), using "patch sprite"
	man.Draw(sprGFX, { 30,30 });


	Interacting with menu
	~~~~~~~~~~~~~~~~~~~~~

	// Send key events to menu	
	if (GetKey(olc::Key::UP).bPressed)    man.OnUp();
	if (GetKey(olc::Key::DOWN).bPressed)  man.OnDown();
	if (GetKey(olc::Key::LEFT).bPressed)  man.OnLeft();
	if (GetKey(olc::Key::RIGHT).bPressed) man.OnRight();
	if (GetKey(olc::Key::Z).bPressed)     man.OnBack();

	// "Confirm/Action" Key does something, if it returns non-null
	// then a menu item has been selected. The specific item will
	// be returned
	olc::popup::Menu* command = nullptr;
	if (GetKey(olc::Key::SPACE).bPressed) command = man.OnConfirm();
	if (command != nullptr)
	{
		std::string sLastAction = 
		"Selected: " + command->GetName() + 
		" ID: " + std::to_string(command->GetID());

		// Optionally close menu?
		man.Close();
	}

*/

#ifndef OLC_PGEX_POPUPMENU_H
#define OLC_PGEX_POPUPMENU_H

#include <cstdint>

namespace olc
{
	namespace popup
	{
		constexpr int32_t nPatch = 8;

		class Menu
		{
		public:
			Menu();
			Menu(const std::string n);

			Menu& SetTable(int32_t nColumns, int32_t nRows);
			Menu& SetID(int32_t id);
			Menu& Enable(bool b);

			int32_t GetID();
			std::string& GetName();			
			bool Enabled();
			bool HasChildren();
			olc::vi2d GetSize();
			olc::vi2d& GetCursorPosition();
			Menu& operator[](const std::string& name);
			void Build();
			void DrawSelf(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset);
			void ClampCursor();
			void OnUp();
			void OnDown();
			void OnLeft();
			void OnRight();
			Menu* OnConfirm();
			Menu* GetSelectedItem();

		protected:
			int32_t nID = -1;
			olc::vi2d vCellTable = { 1, 0 };
			std::unordered_map<std::string, size_t> itemPointer;
			std::vector<olc::popup::Menu> items;
			olc::vi2d vSizeInPatches = { 0, 0 };
			olc::vi2d vCellSize = { 0, 0 };
			olc::vi2d vCellPadding = { 2, 0 };
			olc::vi2d vCellCursor = { 0, 0 };
			int32_t nCursorItem = 0;
			int32_t nTopVisibleRow = 0;
			int32_t nTotalRows = 0;
			const olc::vi2d vPatchSize = { nPatch, nPatch };
			std::string sName;
			olc::vi2d vCursorPos = { 0, 0 };
			bool bEnabled = true;
		};

		class Manager : public olc::PGEX
		{
		public:
			Manager();
			void Open(Menu* mo);
			void Close();
			void OnUp();
			void OnDown();
			void OnLeft();
			void OnRight();
			void OnBack();
			Menu* OnConfirm();
			void Draw(olc::Sprite* sprGFX, olc::vi2d vScreenOffset);

		private:
			std::list<Menu*> panels;
		};

	}
};




#ifdef OLC_PGEX_POPUPMENU
#undef OLC_PGEX_POPUPMENU

namespace olc
{
	namespace popup
	{
		Menu::Menu()
		{
		}

		Menu::Menu(const std::string n)
		{
			sName = n;
		}


		Menu& Menu::SetTable(int32_t nColumns, int32_t nRows)
		{
			vCellTable = { nColumns, nRows };
			return *this;
		}

		Menu& Menu::SetID(int32_t id)
		{
			nID = id;
			return *this;
		}

		Menu& Menu::Enable(bool b)
		{
			bEnabled = b;
			return *this;
		}

		int32_t Menu::GetID()
		{
			return nID;
		}

		std::string& Menu::GetName()
		{
			return sName;
		}

		bool Menu::Enabled()
		{
			return bEnabled;
		}

		bool Menu::HasChildren()
		{
			return !items.empty();
		}

		olc::vi2d Menu::GetSize()
		{
			return { int32_t(sName.size()), 1 };
		}

		olc::vi2d& Menu::GetCursorPosition()
		{
			return vCursorPos;
		}

		Menu& Menu::operator[](const std::string& name)
		{
			if (itemPointer.count(name) == 0)
			{
				itemPointer[name] = items.size();
				items.push_back(Menu(name));
			}

			return items[itemPointer[name]];
		}

		void Menu::Build()
		{
			// Recursively build all children, so they can determine their size, use
			// that size to indicate cell sizes if this object contains more than 
			// one item
				for (auto& m : items)
				{
					if (m.HasChildren())
					{
						m.Build();
					}

					// Longest child name determines cell width
					vCellSize.x = std::max(m.GetSize().x, vCellSize.x);
					vCellSize.y = std::max(m.GetSize().y, vCellSize.y);
				}

			// Adjust size of this object (in patches) if it were rendered as a panel
			vSizeInPatches.x = vCellTable.x * vCellSize.x + (vCellTable.x - 1) * vCellPadding.x + 2;
			vSizeInPatches.y = vCellTable.y * vCellSize.y + (vCellTable.y - 1) * vCellPadding.y + 2;

			// Calculate how many rows this item has to hold
			nTotalRows = (items.size() / vCellTable.x) + (((items.size() % vCellTable.x) > 0) ? 1 : 0);
		}

		void Menu::DrawSelf(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset)
		{
			// === Draw Panel

			// Record current pixel mode user is using
			olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
			pge.SetPixelMode(olc::Pixel::MASK);

			// Draw Panel & Border
			olc::vi2d vPatchPos = { 0,0 };
			for (vPatchPos.x = 0; vPatchPos.x < vSizeInPatches.x; vPatchPos.x++)
			{
				for (vPatchPos.y = 0; vPatchPos.y < vSizeInPatches.y; vPatchPos.y++)
				{
					// Determine position in screen space
					olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;

					// Calculate which patch is needed
					olc::vi2d vSourcePatch = { 0, 0 };
					if (vPatchPos.x > 0) vSourcePatch.x = 1;
					if (vPatchPos.x == vSizeInPatches.x - 1) vSourcePatch.x = 2;
					if (vPatchPos.y > 0) vSourcePatch.y = 1;
					if (vPatchPos.y == vSizeInPatches.y - 1) vSourcePatch.y = 2;

					// Draw Actual Patch
					pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
				}
			}

			// === Draw Panel Contents
			olc::vi2d vCell = { 0,0 };
			vPatchPos = { 1,1 };

			// Work out visible items
			int32_t nTopLeftItem = nTopVisibleRow * vCellTable.x;
			int32_t nBottomRightItem = vCellTable.y * vCellTable.x + nTopLeftItem;

			// Clamp to size of child item vector
			nBottomRightItem = std::min(int32_t(items.size()), nBottomRightItem);
			int32_t nVisibleItems = nBottomRightItem - nTopLeftItem;

			// Draw Scroll Markers (if required)
			if (nTopVisibleRow > 0)
			{
				vPatchPos = { vSizeInPatches.x - 2, 0 };
				olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;
				olc::vi2d vSourcePatch = { 3, 0 };
				pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
			}

			if ((nTotalRows - nTopVisibleRow) > vCellTable.y)
			{
				vPatchPos = { vSizeInPatches.x - 2, vSizeInPatches.y - 1 };
				olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;
				olc::vi2d vSourcePatch = { 3, 2 };
				pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
			}

			// Draw Visible Items
			for (int32_t i = 0; i < nVisibleItems; i++)
			{
				// Cell location
				vCell.x = i % vCellTable.x;
				vCell.y = i / vCellTable.x;

				// Patch location (including border offset and padding)
				vPatchPos.x = vCell.x * (vCellSize.x + vCellPadding.x) + 1;
				vPatchPos.y = vCell.y * (vCellSize.y + vCellPadding.y) + 1;

				// Actual screen location in pixels
				olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;

				// Display Item Header
				pge.DrawString(vScreenLocation, items[nTopLeftItem + i].sName, items[nTopLeftItem + i].bEnabled ? olc::WHITE : olc::DARK_GREY);

				if (items[nTopLeftItem + i].HasChildren())
				{
					// Display Indicator that panel has a sub panel
					vPatchPos.x = vCell.x * (vCellSize.x + vCellPadding.x) + 1 + vCellSize.x;
					vPatchPos.y = vCell.y * (vCellSize.y + vCellPadding.y) + 1;
					olc::vi2d vSourcePatch = { 3, 1 };
					vScreenLocation = vPatchPos * nPatch + vScreenOffset;
					pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
				}
			}

			// Calculate cursor position in screen space in case system draws it
			vCursorPos.x = (vCellCursor.x * (vCellSize.x + vCellPadding.x)) * nPatch + vScreenOffset.x - nPatch;
			vCursorPos.y = ((vCellCursor.y - nTopVisibleRow) * (vCellSize.y + vCellPadding.y)) * nPatch + vScreenOffset.y + nPatch;
		}

		void Menu::ClampCursor()
		{
			// Find item in children
			nCursorItem = vCellCursor.y * vCellTable.x + vCellCursor.x;

			// Clamp Cursor
			if (nCursorItem >= int32_t(items.size()))
			{
				vCellCursor.y = (items.size() / vCellTable.x);
				vCellCursor.x = (items.size() % vCellTable.x) - 1;
				nCursorItem = items.size() - 1;
			}
		}

		void Menu::OnUp()
		{
			vCellCursor.y--;
			if (vCellCursor.y < 0) vCellCursor.y = 0;

			if (vCellCursor.y < nTopVisibleRow)
			{
				nTopVisibleRow--;
				if (nTopVisibleRow < 0) nTopVisibleRow = 0;
			}

			ClampCursor();
		}

		void Menu::OnDown()
		{
			vCellCursor.y++;
			if (vCellCursor.y == nTotalRows) vCellCursor.y = nTotalRows - 1;

			if (vCellCursor.y > (nTopVisibleRow + vCellTable.y - 1))
			{
				nTopVisibleRow++;
				if (nTopVisibleRow > (nTotalRows - vCellTable.y))
					nTopVisibleRow = nTotalRows - vCellTable.y;
			}

			ClampCursor();
		}

		void Menu::OnLeft()
		{
			vCellCursor.x--;
			if (vCellCursor.x < 0) vCellCursor.x = 0;
			ClampCursor();
		}

		void Menu::OnRight()
		{
			vCellCursor.x++;
			if (vCellCursor.x == vCellTable.x) vCellCursor.x = vCellTable.x - 1;
			ClampCursor();
		}

		Menu* Menu::OnConfirm()
		{
			// Check if selected item has children
			if (items[nCursorItem].HasChildren())
			{
				return &items[nCursorItem];
			}
			else
				return this;
		}

		Menu* Menu::GetSelectedItem()
		{
			return &items[nCursorItem];
		}

		// =====================================================================

		Manager::Manager()
		{
		}

		void Manager::Open(Menu* mo) 
		{ 
			Close(); 
			panels.push_back(mo); 
		}

		void Manager::Close()
		{
			panels.clear(); 
		}

		void Manager::OnUp() 
		{ 
			if (!panels.empty()) panels.back()->OnUp(); 
		}

		void Manager::OnDown() 
		{ 
			if (!panels.empty()) panels.back()->OnDown(); 
		}

		void Manager::OnLeft()
		{ 
			if (!panels.empty()) panels.back()->OnLeft();
		}

		void Manager::OnRight()
		{ 
			if (!panels.empty()) panels.back()->OnRight();
		}

		void Manager::OnBack()
		{
			if (!panels.empty()) panels.pop_back(); 
		}

		Menu* Manager::OnConfirm()
		{
			if (panels.empty()) return nullptr;

			Menu* next = panels.back()->OnConfirm();
			if (next == panels.back())
			{
				if (panels.back()->GetSelectedItem()->Enabled())
					return panels.back()->GetSelectedItem();
			}
			else
			{
				if (next->Enabled())
					panels.push_back(next);
			}

			return nullptr;
		}

		void Manager::Draw(olc::Sprite* sprGFX, olc::vi2d vScreenOffset)
		{
			if (panels.empty()) return;

			// Draw Visible Menu System
			for (auto& p : panels)
			{
				p->DrawSelf(*pge, sprGFX, vScreenOffset);
				vScreenOffset += {10, 10};
			}

			// Draw Cursor
			olc::Pixel::Mode currentPixelMode = pge->GetPixelMode();
			pge->SetPixelMode(olc::Pixel::ALPHA);
			pge->DrawPartialSprite(panels.back()->GetCursorPosition(), sprGFX, olc::vi2d(4, 0) * nPatch, { nPatch * 2, nPatch * 2 });
			pge->SetPixelMode(currentPixelMode);
		}
	}
};


#endif
#endif
