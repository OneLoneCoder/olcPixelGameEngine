/*
	Easy To Use "Retro Pop-Up Menu System"
	"There's a storm comin'......" - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2020 OneLoneCoder.com

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

	Relevant Video: https://youtu.be/jde1Jq5dF0E

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com
	
	Community Blog: https://community.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2018, 2019, 2020
*/


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <unordered_map>


constexpr int32_t nPatch = 8;


class menuobject
{
public:
	menuobject()
	{ sName = "root"; }

	menuobject(const std::string& name)
	{ sName = name; }

	menuobject& SetTable(int nColumns, int nRows)
	{ vCellTable = { nColumns, nRows };	return *this; }

	menuobject& SetID(int32_t id)
	{ nID = id; return *this; }

	int32_t GetID()
	{ return nID; }

	std::string& GetName()
	{ return sName; }

	menuobject& Enable(bool b)
	{ bEnabled = b; return *this; }

	bool Enabled()
	{ return bEnabled; }

	bool HasChildren()
	{ return !items.empty(); }

	// For now, cells are simply one line strings
	olc::vi2d GetSize()
	{ return { int32_t(sName.size()), 1 }; }

	olc::vi2d& GetCursorPosition()
	{ return vCursorPos; }

	menuobject& operator[](const std::string& name)
	{
		if (itemPointer.count(name) == 0)
		{
			itemPointer[name] = items.size();
			items.push_back(menuobject(name));
		}

		return items[itemPointer[name]];
	}

	void Build()
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

	void DrawSelf(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset)
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

	void ClampCursor()
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

	void OnUp()
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

	void OnDown()
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

	void OnLeft()
	{
		vCellCursor.x--;
		if (vCellCursor.x < 0) vCellCursor.x = 0;
		ClampCursor();
	}

	void OnRight()
	{
		vCellCursor.x++;
		if (vCellCursor.x == vCellTable.x) vCellCursor.x = vCellTable.x - 1;
		ClampCursor();
	}

	menuobject* OnConfirm()
	{
		// Check if selected item has children
		if (items[nCursorItem].HasChildren())
		{
			return &items[nCursorItem];
		}
		else
			return this;
	}

	menuobject* GetSelectedItem()
	{
		return &items[nCursorItem];
	}


protected:
	int32_t nID = -1;
	olc::vi2d vCellTable = { 1, 0 };
	std::unordered_map<std::string, size_t> itemPointer;
	std::vector<menuobject> items;
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


class menumanager
{	
public:
	menumanager() { }

	void Open(menuobject* mo) { Close(); panels.push_back(mo); }
	void Close() { panels.clear(); }

	void OnUp()    { if (!panels.empty()) panels.back()->OnUp();    }
	void OnDown()  { if (!panels.empty()) panels.back()->OnDown();  }
	void OnLeft()  { if (!panels.empty()) panels.back()->OnLeft();  }
	void OnRight() { if (!panels.empty()) panels.back()->OnRight(); }
	void OnBack()  { if (!panels.empty()) panels.pop_back(); }

	menuobject* OnConfirm() 
	{
		if (panels.empty()) return nullptr;

		menuobject* next = panels.back()->OnConfirm();
		if (next == panels.back())
		{
			if(panels.back()->GetSelectedItem()->Enabled())
				return panels.back()->GetSelectedItem();
		}
		else
		{
			if(next->Enabled())
				panels.push_back(next);
		}

		return nullptr;
	}

	void Draw(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset)
	{
		if (panels.empty()) return;

		// Draw Visible Menu System
		for (auto& p : panels)
		{
			p->DrawSelf(pge, sprGFX, vScreenOffset);
			vScreenOffset += {10, 10};
		}

		// Draw Cursor
		olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
		pge.SetPixelMode(olc::Pixel::ALPHA);
		pge.DrawPartialSprite(panels.back()->GetCursorPosition(), sprGFX, olc::vi2d(4, 0) * nPatch, { nPatch * 2, nPatch * 2 });
		pge.SetPixelMode(currentPixelMode);		
	}

private:
	std::list<menuobject*> panels;
};

// Override base class with your custom functionality
class olcRetroPopUpMenu : public olc::PixelGameEngine
{
public:
	olcRetroPopUpMenu()
	{
		sAppName = "Retro Pop-Up Menu";
	}

	olc::Sprite* sprGFX = nullptr;

	menuobject mo;
	menumanager mm;

public:
	bool OnUserCreate() override
	{
		sprGFX = new olc::Sprite("./RetroMenu.png");

		mo["main"].SetTable(1, 4);
		mo["main"]["Attack"].SetID(101);

		mo["main"]["Magic"].SetTable(1, 2);

		mo["main"]["Magic"]["White"].SetTable(3, 6);
		auto& menuWhiteMagic = mo["main"]["Magic"]["White"];
		menuWhiteMagic["Cure"].SetID(401);
		menuWhiteMagic["Cura"].SetID(402);
		menuWhiteMagic["Curaga"].SetID(403);
		menuWhiteMagic["Esuna"].SetID(404);

		mo["main"]["Magic"]["Black"].SetTable(3, 4);
		auto& menuBlackMagic = mo["main"]["Magic"]["Black"];
		menuBlackMagic["Fire"].SetID(201);
		menuBlackMagic["Fira"].SetID(202);
		menuBlackMagic["Firaga"].SetID(203);
		menuBlackMagic["Blizzard"].SetID(204);
		menuBlackMagic["Blizzara"].SetID(205).Enable(false);
		menuBlackMagic["Blizzaga"].SetID(206).Enable(false);
		menuBlackMagic["Thunder"].SetID(207);
		menuBlackMagic["Thundara"].SetID(208);
		menuBlackMagic["Thundaga"].SetID(209);
		menuBlackMagic["Quake"].SetID(210);
		menuBlackMagic["Quake2"].SetID(211);
		menuBlackMagic["Quake3"].SetID(212); 
		menuBlackMagic["Bio"].SetID(213);
		menuBlackMagic["Bio1"].SetID(214);
		menuBlackMagic["Bio2"].SetID(215);
		menuBlackMagic["Demi"].SetID(216);
		menuBlackMagic["Demi1"].SetID(217);
		menuBlackMagic["Demi2"].SetID(218);

		mo["main"]["Defend"].SetID(102);

		mo["main"]["Items"].SetTable(2, 4).Enable(false);
		mo["main"]["Items"]["Potion"].SetID(301);
		mo["main"]["Items"]["Ether"].SetID(302);
		mo["main"]["Items"]["Elixir"].SetID(303);

		mo["main"]["Escape"].SetID(103);

		mo.Build();

		mm.Open(&mo["main"]);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		menuobject* command = nullptr;
		
		if (GetKey(olc::Key::M).bPressed)    mm.Open(&mo["main"]);

		if (GetKey(olc::Key::UP).bPressed)    mm.OnUp();
		if (GetKey(olc::Key::DOWN).bPressed)  mm.OnDown();
		if (GetKey(olc::Key::LEFT).bPressed)  mm.OnLeft();
		if (GetKey(olc::Key::RIGHT).bPressed) mm.OnRight();
		if (GetKey(olc::Key::SPACE).bPressed) command = mm.OnConfirm();
		if (GetKey(olc::Key::Z).bPressed)     mm.OnBack();

		if (command != nullptr)
		{
			sLastAction = "Selected: " + command->GetName() + " ID: " + std::to_string(command->GetID());
			mm.Close();
		}
		
		Clear(olc::BLACK);
		mm.Draw(*this, sprGFX, { 30,30 });
		DrawString(10, 200, sLastAction);
		return true;
	}

	std::string sLastAction;
};

int main()
{
	olcRetroPopUpMenu demo;
	if (demo.Construct(384, 240, 4, 4))
		demo.Start();
	return 0;
}