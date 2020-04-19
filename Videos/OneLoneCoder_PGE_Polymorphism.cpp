/*
	OLC::CAD - A practical example of Polymorphism
	"Damn Gorbette, you made us giggle..." - javidx9

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
	Press & Hold middle mouse mutton to PAN
	Use Scroll wheel (or Q & A) to zoom in & out
	Press L to start drawing a line
	Press C to start drawing a circle
	Press B to start drawing a box
	Press S to start drawing a curve
	Press M to move node under cursor

	Relevant Video: https://youtu.be/kxKKHKSMGIg

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

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Forward declare shape, since we use it in sNode
struct sShape;

// Define a node
struct sNode
{
	sShape *parent;
	olc::vf2d pos;
};

// Our BASE class, defines the interface for all shapes
struct sShape
{
	// Shapes are defined by the placment of nodes
	std::vector<sNode> vecNodes;
	uint32_t nMaxNodes = 0;

	// The colour of the shape
	olc::Pixel col = olc::GREEN;

	// All shapes share word to screen transformation
	// coefficients, so share them staically
	static float fWorldScale;
	static olc::vf2d vWorldOffset;

	// Convert coordinates from World Space --> Screen Space
	void WorldToScreen(const olc::vf2d &v, int &nScreenX, int &nScreenY)
	{
		nScreenX = (int)((v.x - vWorldOffset.x) * fWorldScale);
		nScreenY = (int)((v.y - vWorldOffset.y) * fWorldScale);
	}

	// This is a PURE function, which makes this class abstract. A sub-class
	// of this class must provide an implementation of this function by
	// overriding it
	virtual void DrawYourself(olc::PixelGameEngine *pge) = 0;

	// Shapes are defined by nodes, the shape is responsible
	// for issuing nodes that get placed by the user. The shape may
	// change depending on how many nodes have been placed. Once the
	// maximum number of nodes for a shape have been placed, it returns
	// nullptr
	sNode* GetNextNode(const olc::vf2d &p)
	{
		if (vecNodes.size() == nMaxNodes)
			return nullptr; // Shape is complete so no new nodes to be issued

		// else create new node and add to shapes node vector
		sNode n;
		n.parent = this;
		n.pos = p;
		vecNodes.push_back(n);

		// Beware! - This normally is bad! But see sub classes
		return &vecNodes[vecNodes.size() - 1];
	}

	// Test to see if supplied coordinate exists at same location
	// as any of the nodes for this shape. Return a pointer to that
	// node if it does
	sNode* HitNode(olc::vf2d &p)
	{
		for (auto &n : vecNodes)
		{
			if ((p - n.pos).mag() < 0.01f)
				return &n;
		}

		return nullptr;
	}

	// Draw all of the nodes that define this shape so far
	void DrawNodes(olc::PixelGameEngine *pge)
	{
		for (auto &n : vecNodes)
		{
			int sx, sy;
			WorldToScreen(n.pos, sx, sy);
			pge->FillCircle(sx, sy, 2, olc::RED);
		}
	}
};

// We must provide an implementation of our static variables
float sShape::fWorldScale = 1.0f;
olc::vf2d sShape::vWorldOffset = { 0,0 };



// LINE sub class, inherits from sShape
struct sLine : public sShape
{
	sLine()
	{
		nMaxNodes = 2;
		vecNodes.reserve(nMaxNodes); // We're gonna be getting pointers to vector elements
		// though we have defined already how much capacity our vector will have. This makes
		// it safe to do this as we know the vector will not be maniupulated as we add nodes
		// to it. Is this bad practice? Possibly, but as with all thing programming, if you
		// know what you are doing, it's ok :D
	}

	// Implements custom DrawYourself Function, meaning the shape
	// is no longer abstract
	void DrawYourself(olc::PixelGameEngine *pge) override
	{
		int sx, sy, ex, ey;
		WorldToScreen(vecNodes[0].pos, sx, sy);
		WorldToScreen(vecNodes[1].pos, ex, ey);
		pge->DrawLine(sx, sy, ex, ey, col);
	}
};


// BOX
struct sBox : public sShape
{
	sBox()
	{
		nMaxNodes = 2;
		vecNodes.reserve(nMaxNodes); 
	}

	void DrawYourself(olc::PixelGameEngine *pge) override
	{
		int sx, sy, ex, ey;
		WorldToScreen(vecNodes[0].pos, sx, sy);
		WorldToScreen(vecNodes[1].pos, ex, ey);
		pge->DrawRect(sx, sy, ex - sx, ey - sy, col);
	}
};


// CIRCLE
struct sCircle : public sShape
{
	sCircle()
	{
		nMaxNodes = 2;
		vecNodes.reserve(nMaxNodes);
	}

	void DrawYourself(olc::PixelGameEngine *pge) override
	{
		float fRadius = (vecNodes[0].pos - vecNodes[1].pos).mag();
		int sx, sy, ex, ey;
		WorldToScreen(vecNodes[0].pos, sx, sy);
		WorldToScreen(vecNodes[1].pos, ex, ey);
		pge->DrawLine(sx, sy, ex, ey, col, 0xFF00FF00);

		// Note the radius is also scaled so it is drawn appropriately
		pge->DrawCircle(sx, sy, (int32_t)(fRadius * fWorldScale), col);
	}
};

// BEZIER SPLINE - requires 3 nodes to be defined fully
struct sCurve : public sShape
{
	sCurve()
	{
		nMaxNodes = 3;
		vecNodes.reserve(nMaxNodes);
	}

	void DrawYourself(olc::PixelGameEngine *pge) override
	{
		int sx, sy, ex, ey;

		if (vecNodes.size() < 3)
		{
			// Can only draw line from first to second
			WorldToScreen(vecNodes[0].pos, sx, sy);
			WorldToScreen(vecNodes[1].pos, ex, ey);
			pge->DrawLine(sx, sy, ex, ey, col, 0xFF00FF00);
		}

		if (vecNodes.size() == 3)
		{
			// Can draw line from first to second
			WorldToScreen(vecNodes[0].pos, sx, sy);
			WorldToScreen(vecNodes[1].pos, ex, ey);
			pge->DrawLine(sx, sy, ex, ey, col, 0xFF00FF00);

			// Can draw second structural line
			WorldToScreen(vecNodes[1].pos, sx, sy);
			WorldToScreen(vecNodes[2].pos, ex, ey);
			pge->DrawLine(sx, sy, ex, ey, col, 0xFF00FF00);

			// And bezier curve
			olc::vf2d op = vecNodes[0].pos;
			olc::vf2d np = op;
			for (float t = 0; t < 1.0f; t += 0.01f)
			{
				np = (1 - t)*(1 - t)*vecNodes[0].pos + 2 * (1 - t)*t*vecNodes[1].pos + t * t * vecNodes[2].pos;
				WorldToScreen(op, sx, sy);
				WorldToScreen(np, ex, ey);
				pge->DrawLine(sx, sy, ex, ey, col);
				op = np;
			}
		}

	}
};



// APPLICATION STARTS HERE

class Polymorphism : public olc::PixelGameEngine
{
public:
	Polymorphism()
	{
		sAppName = "Polymorphism";
	}

private:
	// Pan & Zoom variables
	olc::vf2d vOffset = { 0.0f, 0.0f };
	olc::vf2d vStartPan = { 0.0f, 0.0f };
	float fScale = 10.0f;
	float fGrid = 1.0f;

	// Convert coordinates from World Space --> Screen Space
	void WorldToScreen(const olc::vf2d &v, int &nScreenX, int &nScreenY)
	{
		nScreenX = (int)((v.x - vOffset.x) * fScale);
		nScreenY = (int)((v.y - vOffset.y) * fScale);
	}

	// Convert coordinates from Screen Space --> World Space
	void ScreenToWorld(int nScreenX, int nScreenY, olc::vf2d &v)
	{
		v.x = (float)(nScreenX) / fScale + vOffset.x;
		v.y = (float)(nScreenY) / fScale + vOffset.y;
	}


	// A pointer to a shape that is currently being defined
	// by the placment of nodes
	sShape* tempShape = nullptr;

	// A list of pointers to all shapes which have been drawn
	// so far
	std::list<sShape*> listShapes;

	// A pointer to a node that is currently selected. Selected 
	// nodes follow the mouse cursor
	sNode *selectedNode = nullptr;

	// "Snapped" mouse location
	olc::vf2d vCursor = { 0, 0 };

	// NOTE! No direct instances of lines, circles, boxes or curves,
	// the application is only aware of the existence of shapes!
	// THIS IS THE POWER OF POLYMORPHISM!

public:
	bool OnUserCreate() override
	{
		// Configure world space (0,0) to be middle of screen space
		vOffset = { (float)(-ScreenWidth() / 2) / fScale, (float)(-ScreenHeight() / 2) / fScale };
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Get mouse location this frame
		olc::vf2d vMouse = { (float)GetMouseX(), (float)GetMouseY() };


		// Handle Pan & Zoom
		if (GetMouse(2).bPressed)
		{
			vStartPan = vMouse;
		}

		if (GetMouse(2).bHeld)
		{
			vOffset -= (vMouse - vStartPan) / fScale;
			vStartPan = vMouse;
		}

		olc::vf2d vMouseBeforeZoom;
		ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseBeforeZoom);

		if (GetKey(olc::Key::Q).bHeld || GetMouseWheel() > 0)
		{
			fScale *= 1.1f;
		}

		if (GetKey(olc::Key::A).bHeld || GetMouseWheel() < 0)
		{
			fScale *= 0.9f;
		}

		olc::vf2d vMouseAfterZoom;
		ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseAfterZoom);
		vOffset += (vMouseBeforeZoom - vMouseAfterZoom);


		// Snap mouse cursor to nearest grid interval
		vCursor.x = floorf((vMouseAfterZoom.x + 0.5f) * fGrid);
		vCursor.y = floorf((vMouseAfterZoom.y + 0.5f) * fGrid);


		if (GetKey(olc::Key::L).bPressed)
		{
			tempShape = new sLine();

			// Place first node at location of keypress
			selectedNode = tempShape->GetNextNode(vCursor);

			// Get Second node
			selectedNode = tempShape->GetNextNode(vCursor);
		}


		if (GetKey(olc::Key::B).bPressed)
		{
			tempShape = new sBox();

			// Place first node at location of keypress
			selectedNode = tempShape->GetNextNode(vCursor);

			// Get Second node
			selectedNode = tempShape->GetNextNode(vCursor);
		}

		if (GetKey(olc::Key::C).bPressed)
		{
			// Create new shape as a temporary
			tempShape = new sCircle();

			// Place first node at location of keypress
			selectedNode = tempShape->GetNextNode(vCursor);

			// Get Second node
			selectedNode = tempShape->GetNextNode(vCursor);
		}

		if (GetKey(olc::Key::S).bPressed)
		{
			// Create new shape as a temporary
			tempShape = new sCurve();

			// Place first node at location of keypress
			selectedNode = tempShape->GetNextNode(vCursor);

			// Get Second node
			selectedNode = tempShape->GetNextNode(vCursor);
		}

		// Search for any node that exists under the cursor, if one
		// is found then select it
		if (GetKey(olc::Key::M).bPressed)
		{
			selectedNode = nullptr;
			for (auto &shape : listShapes)
			{
				selectedNode = shape->HitNode(vCursor);
				if (selectedNode != nullptr)
					break;
			}
		}


		// If a node is selected, make it follow the mouse cursor
		// by updating its position
		if (selectedNode != nullptr)
		{
			selectedNode->pos = vCursor;
		}


		// As the user left clicks to place nodes, the shape can grow
		// until it requires no more nodes, at which point it is completed
		// and added to the list of completed shapes.
		if (GetMouse(0).bReleased)
		{
			if (tempShape != nullptr)
			{
				selectedNode = tempShape->GetNextNode(vCursor);
				if (selectedNode == nullptr)
				{
					tempShape->col = olc::WHITE;
					listShapes.push_back(tempShape);
					tempShape = nullptr; // Thanks @howlevergreen /Disord
				}
				
			}
			else
			{
				selectedNode = nullptr;
			}
		}



		// Clear Screen
		Clear(olc::VERY_DARK_BLUE);

		int sx, sy;
		int ex, ey;

		// Get visible world
		olc::vf2d vWorldTopLeft, vWorldBottomRight;
		ScreenToWorld(0, 0, vWorldTopLeft);
		ScreenToWorld(ScreenWidth(), ScreenHeight(), vWorldBottomRight);

		// Get values just beyond screen boundaries
		vWorldTopLeft.x = floor(vWorldTopLeft.x);
		vWorldTopLeft.y = floor(vWorldTopLeft.y);
		vWorldBottomRight.x = ceil(vWorldBottomRight.x);
		vWorldBottomRight.y = ceil(vWorldBottomRight.y);

		// Draw Grid dots
		for (float x = vWorldTopLeft.x; x < vWorldBottomRight.x; x += fGrid)
		{
			for (float y = vWorldTopLeft.y; y < vWorldBottomRight.y; y += fGrid)
			{
				WorldToScreen({ x, y }, sx, sy);
				Draw(sx, sy, olc::BLUE);
			}
		}

		// Draw World Axis
		WorldToScreen({ 0,vWorldTopLeft.y }, sx, sy);
		WorldToScreen({ 0,vWorldBottomRight.y }, ex, ey);
		DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);
		WorldToScreen({ vWorldTopLeft.x,0 }, sx, sy);
		WorldToScreen({ vWorldBottomRight.x,0 }, ex, ey);
		DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);

		// Update shape translation coefficients
		sShape::fWorldScale = fScale;
		sShape::vWorldOffset = vOffset;

		// Draw All Existing Shapes
		for (auto &shape : listShapes)
		{
			shape->DrawYourself(this);
			shape->DrawNodes(this);
		}

		// Draw shape currently being defined
		if (tempShape != nullptr)
		{
			tempShape->DrawYourself(this);
			tempShape->DrawNodes(this);
		}

		// Draw "Snapped" Cursor
		WorldToScreen(vCursor, sx, sy);
		DrawCircle(sx, sy, 3, olc::YELLOW);

		// Draw Cursor Position
		DrawString(10, 10, "X=" + std::to_string(vCursor.x) + ", Y=" + std::to_string(vCursor.x), olc::YELLOW, 2);
		return true;
	}
};


int main()
{
	Polymorphism demo;
	if (demo.Construct(800, 480, 1, 1, false))
		demo.Start();
	return 0;
}




