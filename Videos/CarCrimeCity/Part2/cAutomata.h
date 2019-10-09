/*
	Top Down City Based Car Crime Game - Part #2
	"Colin, I hope you're shooting 600+ wherever you are buddy. RIP." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

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

	Instructions:
	~~~~~~~~~~~~~
	Scroll with middle mouse wheel, TAB toggle edit mode, R to place road
	P to place pavement, Q to place building, Arrow keys to drive car

	Relevant Video: https://youtu.be/fIV6P1W-wuo

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

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/


#pragma once

#include "olcPixelGameEngine.h"

class cAuto_Track;
class cAuto_Node;
class cAuto_Body;
class cCell;

class cAuto_Node
{
public:
	cAuto_Node();
	cAuto_Node(const olc::vf2d &worldpos);
	olc::vf2d pos;
	bool bBlock = false;
	std::list<cAuto_Track*> listTracks;
};

class cAuto_Track
{
public:
	cAuto_Node* node[2]; // Two end nodes
	cCell* cell; // Pointer to host cell
	olc::vf2d GetPostion(float t, cAuto_Node *pstart);
	std::list<cAuto_Body*> listAutos;
	float fTrackLength = 1.0f;
};

class cAuto_Body
{
public:
	cAuto_Body();
	~cAuto_Body();

public:
	void UpdateAuto(float fElapsedTime);

public:
	olc::vf2d vAutoPos = { 0.0f, 0.0f };
	float fAutoPos = 0.0f; // Location of automata along track
	float fAutoLength = 0.0f; // Physical length of automata
	cAuto_Track *pCurrentTrack = nullptr;
	cAuto_Node *pTrackOriginNode = nullptr;

};
