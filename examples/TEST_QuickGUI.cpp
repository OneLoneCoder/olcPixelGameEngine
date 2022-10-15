/*
	Example file for olcPGEX_QuickGUI.h

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

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// PGEX Require the presence of olc::PixelGameEngine
#define OLC_PGEX_QUICKGUI
#include "extensions/olcPGEX_QuickGUI.h"


class olcDemo_QuickGUI : public olc::PixelGameEngine
{
public:
	olcDemo_QuickGUI()
	{
		sAppName = "olcDemo_QuickGUI";
	}

protected:
	olc::QuickGUI::Manager guiManager;

	olc::QuickGUI::Slider* guiSlider1 = nullptr;
	olc::QuickGUI::Slider* guiSlider2 = nullptr;
	olc::QuickGUI::Slider* guiSlider3 = nullptr;

	olc::QuickGUI::Button* guiButton1 = nullptr;
	olc::QuickGUI::Button* guiButton2 = nullptr;
	olc::QuickGUI::Button* guiButton3 = nullptr;

	olc::QuickGUI::Slider* guiThemeColourR = nullptr;
	olc::QuickGUI::Slider* guiThemeColourG = nullptr;
	olc::QuickGUI::Slider* guiThemeColourB = nullptr;

	olc::QuickGUI::Label* guiLabelR = nullptr;
	olc::QuickGUI::Label* guiLabelG = nullptr;
	olc::QuickGUI::Label* guiLabelB = nullptr;

	olc::QuickGUI::CheckBox* guiCheckBox1 = nullptr;

	olc::QuickGUI::TextBox* guiTextBox1 = nullptr;
	olc::QuickGUI::TextBox* guiTextBox2 = nullptr;


	std::vector<std::string> listExample;
	olc::QuickGUI::ListBox* guiListBox = nullptr;

public:
	bool OnUserCreate() override
	{
		// Horizontal Slider
		guiSlider1 = new olc::QuickGUI::Slider(guiManager, 
			{ 30.0f, 10.0f }, { 246.0f, 10.0f }, 0, 100, 50);
		// Diagonal Slider!
		guiSlider2 = new olc::QuickGUI::Slider(guiManager, 
			{ 20.0f, 20.0f }, { 120.0f, 120.0f }, 0, 100, 50);
		// Vertical Slider
		guiSlider3 = new olc::QuickGUI::Slider(guiManager, 
			{ 10.0f, 30.0f }, { 10.0f, 230.0f }, 0, 100, 50);

		// Theme colour slider - Red
		guiThemeColourR = new olc::QuickGUI::Slider(guiManager,
			{ 150.0f, 30.0f }, { 246.0f, 30.0f }, 0, 255, 0);
		// Theme colour slider - Green
		guiThemeColourG = new olc::QuickGUI::Slider(guiManager,
			{ 150.0f, 50.0f }, { 246.0f, 50.0f }, 0, 255, 0);
		// Theme colour slider - Blue
		guiThemeColourB = new olc::QuickGUI::Slider(guiManager,
			{ 150.0f, 70.0f }, { 246.0f, 70.0f }, 0, 255, 128);

		// Labels for theme colour sliders
		guiLabelR = new olc::QuickGUI::Label(guiManager,
			"Red:", { 80.0f, 22.0f }, { 50.0f, 16.0f });
		guiLabelG = new olc::QuickGUI::Label(guiManager,
			"Green:", { 80.0f, 42.0f }, { 50.0f, 16.0f });
		guiLabelB = new olc::QuickGUI::Label(guiManager,
			"Blue:", { 80.0f, 62.0f }, { 50.0f, 16.0f });

		// Customize how the labels look
		guiLabelB->nAlign = olc::QuickGUI::Label::Alignment::Right;		
		guiLabelG->nAlign = olc::QuickGUI::Label::Alignment::Right;
		guiLabelG->bHasBorder = true;		
		guiLabelR->nAlign = olc::QuickGUI::Label::Alignment::Right;
		guiLabelR->bHasBorder = true;
		guiLabelR->bHasBackground = true;

		// Some Buttons, 1 is just a thing, 2 has its text updated and 3 resets the theme
		guiButton1 = new olc::QuickGUI::Button(guiManager,
			"Button 1", { 30.0f, 150.0f }, { 100.0f, 16.0f });
		guiButton2 = new olc::QuickGUI::Button(guiManager,
			"Button 2", { 30.0f, 170.0f }, { 100.0f, 16.0f });
		guiButton3 = new olc::QuickGUI::Button(guiManager,
			"Reset Theme", { 30.0f, 190.0f }, { 100.0f, 16.0f });

		// A CheckBox, switches between sprite or decal drawing
		guiCheckBox1 = new olc::QuickGUI::CheckBox(guiManager,
			"Use Decals", false, { 30.0f, 210.0f }, { 100.0f, 16.0f });

		// TextBox, allows basic text entry
		guiTextBox1 = new olc::QuickGUI::TextBox(guiManager,
			"", { 150.0f, 140.0f }, { 100.0f, 16.0f });
		guiTextBox2 = new olc::QuickGUI::TextBox(guiManager,
			"0.04", { 150.0f, 160.0f }, { 100.0f, 16.0f });

		listExample.push_back("Item 1");
		listExample.push_back("Item 2");
		listExample.push_back("Item 3");
		listExample.push_back("Item 4");
		listExample.push_back("Item 5");
		listExample.push_back("Item 6");

		guiListBox = new olc::QuickGUI::ListBox(guiManager,
			listExample, { 150.0f, 180.0f }, { 100.0f, 54.0f });

		// but but but.... waaaaaaaaahaaaaaaaa.... where do I delete these horrible
		// pointers??? I just can't accept that addressable memory exists and it makes
		// me feel really insecure!
		//
		// By default olc::QuickGUI::Manager will delete any Controls added to it, so you
		// dont have to. If you must obfuscate your program with smart pointers, or require
		// that you are in rage-control of your memory at all times, construct the Manager
		// with false as the argument - then its all up to you buddy.

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// We must update the manager at some point each frame. Values of controls
		// are only valid AFTER this call to update()
		guiManager.Update(this);

		// Some silly examples...

		// 001) The "theme colour" can be set from slider values
		guiManager.colNormal = olc::Pixel(
			uint8_t(guiThemeColourR->fValue),
			uint8_t(guiThemeColourG->fValue),
			uint8_t(guiThemeColourB->fValue));

		// 002) Display Slider 1 value on Button 2
		guiButton2->sText = "Button " + std::to_string(int32_t(guiSlider1->fValue));

		// 003) Check if "Reset Theme" button is pressed, if it is, well, err...
		if (guiButton3->bPressed)
		{
			// ...reset the theme! (which also updates the sliders)
			guiThemeColourR->fValue = 0.0f;
			guiThemeColourG->fValue = 0.0f;
			guiThemeColourB->fValue = 128.0f;
		}

		// 004) Link Slider 2 and Slider 3 together
		if(guiSlider2->bHeld)
			guiSlider3->fValue = 100.0f - guiSlider2->fValue;
		if (guiSlider3->bHeld)
			guiSlider2->fValue = 100.0f - guiSlider3->fValue;
		

		// Draw Stuff!
		Clear(olc::BLACK);

		// 005) Use checkbox to determine rendering mode
		if (guiCheckBox1->bChecked)
			guiManager.DrawDecal(this);
		else
			guiManager.Draw(this);
		return true;
	}
};

int main()
{
	olcDemo_QuickGUI demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}