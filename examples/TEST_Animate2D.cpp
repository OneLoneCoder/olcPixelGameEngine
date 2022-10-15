/*
	Example file for olcUTIL_Animate2D.h

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

#include "utilities/olcUTIL_Animate2D.h"

class TEST_Animate2D : public olc::PixelGameEngine
{
public:
	TEST_Animate2D()
	{		
		sAppName = "Animate2D Utility Test";
	}

	// These are the states the dude can exist in, we'll need these for physics
	// and animation. The physics side of things moves the dude around according to state,
	// while the animator chooses the frames based upon state and time
	enum class DudeState: uint8_t
	{
		WALK_N, WALK_S, WALK_E, WALK_W, IDLE_STAND,	LAUGH, CHEER, YES, NO
	};

	// !! - IMPORTANT - !!
	// The animation set
	olc::utils::Animate2D::Animation<DudeState> animDude;

	// One big sprite containing all the graphics
	olc::Renderable gfxAll;

	// Small object to reprsent a dude walking around doing things
	struct sWalkingDude
	{
		// Which dude overall out of graophic?
		int32_t id = 0;
		// Where are they?
		olc::vf2d pos;
		// What are they doing?
		DudeState UserState = DudeState::IDLE_STAND;
		// For how long should they do it?
		float fTick = 0.0f;

		// !! - IMPORTANT - !!
		// Animation Token - links this object's state to animator. Note
		// there is no 'ownership' or memory issues here, this is a token
		// that the animator can use to quickly get to where it needs
		// in order to return frames upon request for this object.
		olc::utils::Animate2D::AnimationState animstate;
	};

	// Introducing.... The dudes!
	size_t nDudes = 500;
	std::vector<sWalkingDude> vDudes;

public:
	bool OnUserCreate() override
	{	
		// For this appliaction I have a single image that contains
		// 28x2 unique characters, each character contains 8 animations of 3
		// frames each. Each frame is 26x36 pixels
		gfxAll.Load("./assets/MegaSprite1.png");

		// Thats A LOT of individual graphics, but they all follow a similar pattern
		// because the asset was created usefully (take note certain popular asset creators)
		// which means we can reuse the animation without needing to define it
		// individually for all the "dudes" - the "cookie cutter" approach

		// Manually construct sequences - gfxAll could in fact be nullptr for this
		// application, but ive kept it here for convenience
		olc::utils::Animate2D::FrameSequence anim_fs_walk_s;
		anim_fs_walk_s.AddFrame({ &gfxAll, {{0,0}, {26,36}} });
		anim_fs_walk_s.AddFrame({ &gfxAll, {{26,0}, {26,36}} });
		anim_fs_walk_s.AddFrame({ &gfxAll, {{52,0}, {26,36}} });

		olc::utils::Animate2D::FrameSequence anim_fs_walk_w;
		anim_fs_walk_w.AddFrame({ &gfxAll, {{ 0,36}, {26,36}} });
		anim_fs_walk_w.AddFrame({ &gfxAll, {{26,36}, {26,36}} });
		anim_fs_walk_w.AddFrame({ &gfxAll, {{52,36}, {26,36}} });

		olc::utils::Animate2D::FrameSequence anim_fs_walk_e;
		anim_fs_walk_e.AddFrame({ &gfxAll, {{ 0,72}, {26,36}} });
		anim_fs_walk_e.AddFrame({ &gfxAll, {{26,72}, {26,36}} });
		anim_fs_walk_e.AddFrame({ &gfxAll, {{52,72}, {26,36}} });

		olc::utils::Animate2D::FrameSequence anim_fs_walk_n;
		anim_fs_walk_n.AddFrame({ &gfxAll, {{ 0,108}, {26,36}} });
		anim_fs_walk_n.AddFrame({ &gfxAll, {{26,108}, {26,36}} });
		anim_fs_walk_n.AddFrame({ &gfxAll, {{52,108}, {26,36}} });

		olc::utils::Animate2D::FrameSequence anim_fs_yes;
		anim_fs_yes.AddFrame({ &gfxAll, {{ 0,144}, {26,36}} });
		anim_fs_yes.AddFrame({ &gfxAll, {{26,144}, {26,36}} });
		anim_fs_yes.AddFrame({ &gfxAll, {{52,144}, {26,36}} });

		olc::utils::Animate2D::FrameSequence anim_fs_no;
		anim_fs_no.AddFrame({ &gfxAll, {{ 0,180}, {26,36}} });
		anim_fs_no.AddFrame({ &gfxAll, {{26,180}, {26,36}} });
		anim_fs_no.AddFrame({ &gfxAll, {{52,180}, {26,36}} });

		olc::utils::Animate2D::FrameSequence anim_fs_laugh;
		anim_fs_laugh.AddFrame({ &gfxAll, {{ 0,216}, {26,36}} });
		anim_fs_laugh.AddFrame({ &gfxAll, {{26,216}, {26,36}} });
		anim_fs_laugh.AddFrame({ &gfxAll, {{52,216}, {26,36}} });

		olc::utils::Animate2D::FrameSequence anim_fs_cheer;
		anim_fs_cheer.AddFrame({ &gfxAll, {{ 0,252}, {26,36}} });
		anim_fs_cheer.AddFrame({ &gfxAll, {{26,252}, {26,36}} });
		anim_fs_cheer.AddFrame({ &gfxAll, {{52,252}, {26,36}} });

		// A special "idle" animation just consists of a single frame
		olc::utils::Animate2D::FrameSequence anim_fs_idle;
		anim_fs_idle.AddFrame({ &gfxAll, {{26,0}, {26,36}} });

		// We have constructed teh individual sequences, now its time
		// to add them to an animation, along with a state name/enum.
		//
		// I have chosen to use the enum shown earlier. You could use
		// std::string too, which is conveninent if you need to display
		// the states, though potentially far less performant

		animDude.AddState(DudeState::WALK_S, anim_fs_walk_s);
		animDude.AddState(DudeState::WALK_W, anim_fs_walk_w);
		animDude.AddState(DudeState::WALK_E, anim_fs_walk_e);
		animDude.AddState(DudeState::WALK_N, anim_fs_walk_n);
		animDude.AddState(DudeState::IDLE_STAND, anim_fs_idle);
		animDude.AddState(DudeState::YES, anim_fs_yes);
		animDude.AddState(DudeState::NO, anim_fs_no);
		animDude.AddState(DudeState::LAUGH, anim_fs_laugh);
		animDude.AddState(DudeState::CHEER, anim_fs_cheer);
		
		// Initialise the dudes
		for (size_t n = 0; n < nDudes; n++)
		{
			sWalkingDude dude;

			// Random dude
			dude.id = rand() % (28 * 2);

			// Begin in idle state, at random location
			dude.UserState = DudeState::IDLE_STAND;
			dude.pos = { float(rand() % ScreenWidth()), float(rand() % ScreenHeight()) };

			// The animation token needs to be updated too
			animDude.ChangeState(dude.animstate, dude.UserState);

			vDudes.push_back(dude);
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{	
		// Update Dudes		
		float fSpeed = 32.0f;
		
		for (auto& dude : vDudes)
		{
			// If a dude's tick reaches 0, it will select a new state
			dude.fTick -= fElapsedTime;
			if (dude.fTick < 0.0f)
			{
				// Choose one out of the 9 randomly
				int nAction = rand() % 9;

				// Choose for how long it should do it in seconds
				dude.fTick = (float(rand()) / float(RAND_MAX)) * 5.0f;

				// Assign the state depending on the dice roll - (since enum
				// we could cast here too, but, well, meh...)
				if (nAction == 0) dude.UserState = DudeState::IDLE_STAND;
				if (nAction == 1) dude.UserState = DudeState::WALK_S;
				if (nAction == 2) dude.UserState = DudeState::WALK_N;
				if (nAction == 3) dude.UserState = DudeState::WALK_E;
				if (nAction == 4) dude.UserState = DudeState::WALK_W;
				if (nAction == 5) dude.UserState = DudeState::YES;
				if (nAction == 6) dude.UserState = DudeState::NO;
				if (nAction == 7) dude.UserState = DudeState::LAUGH;
				if (nAction == 8) dude.UserState = DudeState::CHEER;

				// State has changed, so update animation token
				// !! - IMPORTANT - !!
				animDude.ChangeState(dude.animstate, dude.UserState);
			}

			// Update "physics", if walking move in that direction at speed
			if (dude.UserState == DudeState::WALK_S) dude.pos += olc::vf2d(0, +1) * fSpeed * fElapsedTime;
			if (dude.UserState == DudeState::WALK_N) dude.pos += olc::vf2d(0, -1) * fSpeed * fElapsedTime;
			if (dude.UserState == DudeState::WALK_E) dude.pos += olc::vf2d(+1, 0) * fSpeed * fElapsedTime;
			if (dude.UserState == DudeState::WALK_W) dude.pos += olc::vf2d(-1, 0) * fSpeed * fElapsedTime;

			// If walk off screen, wrap around to other side
			if (dude.pos.x > ScreenWidth()) dude.pos.x -= ScreenWidth();
			if (dude.pos.y > ScreenHeight()) dude.pos.x -= ScreenHeight();
			if (dude.pos.x < 0) dude.pos.x += ScreenWidth();
			if (dude.pos.y < 0) dude.pos.y += ScreenHeight();

			// Update animation token every frame
			// !! - IMPORTANT - !!
			animDude.UpdateState(dude.animstate, fElapsedTime);
		}
		
		// Render Dudes
		for (const auto& dude : vDudes)
		{
			// Get the frame, this contains both source image and source location rectangle
			// !! - IMPORTANT - !!
			const auto& frame = animDude.GetFrame(dude.animstate);

			// Thats cool, but there are 28x2 dudes on the sprite sheet, so using the ID, construct
			// an offset to the correct dude
			olc::vi2d vOffset = { (dude.id % 28) * 78, (dude.id / 28) * 288 };

			// Use DrawPartialDecal to chop out the correct dude frm the image source
			DrawPartialDecal(dude.pos, frame.GetSourceImage()->Decal(), frame.GetSourceRect().pos + vOffset, frame.GetSourceRect().size);
		}

		// That's it
		return true;
	}
};

int main()
{
	TEST_Animate2D demo;
	if (demo.Construct(640, 480, 2, 2))
		demo.Start();
	return 0;
}