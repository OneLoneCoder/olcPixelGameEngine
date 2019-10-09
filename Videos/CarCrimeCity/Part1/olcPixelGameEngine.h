/*
	olcPixelGameEngine.h

	+-------------------------------------------------------------+
	|           OneLoneCoder Pixel Game Engine v1.12              |
	| "Like the command prompt console one, but not..." - javidx9 |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	The olcConsoleGameEngine has been a surprsing and wonderful
	success for me, and I'm delighted how people have reacted so
	positively towards it, so thanks for that.

	However, there are limitations that I simply cannot avoid.
	Firstly, I need to maintain several different versions of
	it to accommodate users on Windows7, 8, 10, Linux, Mac,
	Visual Studio & Code::Blocks. Secondly, this year I've been
	pushing the console to the limits of its graphical capabilities
	and the effect is becoming underwhelming. The engine itself
	is not slow at all, but the process that Windows uses to
	draw the command prompt to the screen is, and worse still,
	it's dynamic based upon the variation of character colours
	and glyphs. Sadly I have no control over this, and recent
	videos that are extremely graphical (for a command prompt :P )
	have been dipping to unacceptable framerates. As the channel
	has been popular with aspiring game developers, I'm concerned
	that the visual appeal of the command prompt is perhaps
	limited to us oldies, and I dont want to alienate younger
	learners. Finally, I'd like to demonstrate many more
	algorithms and image processing that exist in the graphical
	domain, for which the console is insufficient.

	For this reason, I have created olcPixelGameEngine! The look
	and feel to the programmer is almost identical, so all of my
	existing code from the videos is easily portable, and the
	programmer uses this file in exactly the same way. But I've
	decided that rather than just build a command prompt emulator,
	that I would at least harness some modern(ish) portable
	technologies.

	As a result, the olcPixelGameEngine supports 32-bit colour, is
	written in a cross-platform style, uses modern(ish) C++
	conventions and most importantly, renders much much faster. I
	will use this version when my applications are predominantly
	graphics based, but use the console version when they are
	predominantly text based - Don't worry, loads more command
	prompt silliness to come yet, but evolution is important!!

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 OneLoneCoder.com

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
	Patreon:	https://www.patreon.com/javidx9

	Relevant Videos
	~~~~~~~~~~~~~~~
	https://youtu.be/kRH6oJLFYxY Introducing olcPixelGameEngine

	Compiling in Linux
	~~~~~~~~~~~~~~~~~~
	You will need a modern C++ compiler, so update yours!
	To compile use the command:

	g++ -o YourProgName YourSource.cpp -lX11 -lGL -lpthread -lpng

	On some Linux configurations, the frame rate is locked to the refresh
	rate of the monitor. This engine tries to unlock it but may not be
	able to, in which case try launching your program like this:

	vblank_mode=0 ./YourProgName


	Compiling in Code::Blocks on Windows
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Well I wont judge you, but make sure your Code::Blocks installation
	is really up to date - you may even consider updating your C++ toolchain
	to use MinGW32-W64, so google this. You will also need to enable C++14
	in your build options, and add to your linker the following libraries:
	user32 gdi32 opengl32 gdiplus

	Thanks
	~~~~~~
	I'd like to extend thanks to Eremiell, slavka, gurkanctn, Phantim, 
	JackOJC, KrossX, Huhlig, Dragoneye, Appa, JustinRichardsMusic, SliceNDice
	Ralakus, Gorbit99, raoul & MagetzUb for advice, ideas and testing, and I'd like 
	to extend my appreciation to the 23K YouTube followers and 1.5K Discord server
	members who give me the motivation to keep going with all this :D

	Special thanks to those who bring gifts!
	GnarGnarHead.......Domina
	Gorbit99...........Bastion

	Special thanks to my Patreons too - I wont name you on here, but I've
	certainly enjoyed my tea and flapjacks :D

	Author
	~~~~~~ 
	David Barr, aka javidx9, ©OneLoneCoder 2018, 2019
*/

////////////////////////////////////////////////////////////////////////////////////////// 

/* Example Usage (main.cpp)
	#define OLC_PGE_APPLICATION
	#include "olcPixelGameEngine.h"
	// Override base class with your custom functionality
	class Example : public olc::PixelGameEngine
	{
	public:
		Example()
		{
			sAppName = "Example";
		}
	public:
		bool OnUserCreate() override
		{
			// Called once at the start, so create things here
			return true;
		}
		bool OnUserUpdate(float fElapsedTime) override
		{
			// called once per frame, draws random coloured pixels
			for (int x = 0; x < ScreenWidth(); x++)
				for (int y = 0; y < ScreenHeight(); y++)
					Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));
			return true;
		}
	};
	int main()
	{
		Example demo;
		if (demo.Construct(256, 240, 4, 4))
			demo.Start();
		return 0;
	}
*/

#ifndef OLC_PGE_DEF
#define OLC_PGE_DEF

#ifdef _WIN32
	// Link to libraries
#ifndef __MINGW32__
	#pragma comment(lib, "user32.lib")		// Visual Studio Only
	#pragma comment(lib, "gdi32.lib")		// For other Windows Compilers please add
	#pragma comment(lib, "opengl32.lib")	// these libs to your linker input
	#pragma comment(lib, "gdiplus.lib")
#else
	// In Code::Blocks, Select C++14 in your build options, and add the
	// following libs to your linker: user32 gdi32 opengl32 gdiplus
#endif
	// Include WinAPI
	#include <windows.h>
	#include <gdiplus.h>

	// OpenGL Extension
	#include <GL/gl.h>
	typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	static wglSwapInterval_t *wglSwapInterval;
#else
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <X11/X.h>
	#include <X11/Xlib.h>
	#include <png.h>
	typedef int(glSwapInterval_t) (Display *dpy, GLXDrawable drawable, int interval);
	static glSwapInterval_t *glSwapIntervalEXT;
#endif


// Standard includes
#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <map>
#include <functional>

#undef min
#undef max

namespace olc // All OneLoneCoder stuff will now exist in the "olc" namespace
{
	struct Pixel
	{
		union
		{
			uint32_t n = 0xFF000000;
			struct
			{
				uint8_t r;	uint8_t g;	uint8_t b;	uint8_t a;
			};
		};

		Pixel();
		Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
		Pixel(uint32_t p);
		enum Mode { NORMAL, MASK, ALPHA, CUSTOM };
	};

	// Some constants for symbolic naming of Pixels
	static const Pixel
		WHITE(255, 255, 255),
		GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
		RED(255, 0, 0), DARK_RED(128, 0, 0), VERY_DARK_RED(64, 0, 0),
		YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
		GREEN(0, 255, 0), DARK_GREEN(0, 128, 0), VERY_DARK_GREEN(0, 64, 0),
		CYAN(0, 255, 255), DARK_CYAN(0, 128, 128), VERY_DARK_CYAN(0, 64, 64),
		BLUE(0, 0, 255), DARK_BLUE(0, 0, 128), VERY_DARK_BLUE(0, 0, 64),
		MAGENTA(255, 0, 255), DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64),
		BLACK(0, 0, 0),
		BLANK(0, 0, 0, 0);

	enum rcode
	{
		FAIL = 0,
		OK = 1,
		NO_FILE = -1,
	};

	//=============================================================

	struct HWButton
	{
		bool bPressed = false;	// Set once during the frame the event occurs
		bool bReleased = false;	// Set once during the frame the event occurs
		bool bHeld = false;		// Set tru for all frames between pressed and released events
	};

	//=============================================================

	class ResourcePack
	{
	public:
		ResourcePack();
		~ResourcePack();
		struct sEntry : public std::streambuf {
			uint32_t nID, nFileOffset, nFileSize; uint8_t* data; void _config() { this->setg((char*)data, (char*)data, (char*)(data + nFileSize)); }
		};

	public:
		olc::rcode AddToPack(std::string sFile);

	public:
		olc::rcode SavePack(std::string sFile);
		olc::rcode LoadPack(std::string sFile);
		olc::rcode ClearPack();

	public:
		olc::ResourcePack::sEntry GetStreamBuffer(std::string sFile);

	private:

		std::map<std::string, sEntry> mapFiles;
	};

	//=============================================================

	// A bitmap-like structure that stores a 2D array of Pixels
	class Sprite
	{
	public:
		Sprite();
		Sprite(std::string sImageFile);
		Sprite(std::string sImageFile, olc::ResourcePack *pack);
		Sprite(int32_t w, int32_t h);
		~Sprite();

	public:
		olc::rcode LoadFromFile(std::string sImageFile, olc::ResourcePack *pack = nullptr);
		olc::rcode LoadFromPGESprFile(std::string sImageFile, olc::ResourcePack *pack = nullptr);
		olc::rcode SaveToPGESprFile(std::string sImageFile);

	public:
		int32_t width = 0;
		int32_t height = 0;
		enum Mode { NORMAL, PERIODIC };

	public:
		void SetSampleMode(olc::Sprite::Mode mode = olc::Sprite::Mode::NORMAL);
		Pixel GetPixel(int32_t x, int32_t y);
		void  SetPixel(int32_t x, int32_t y, Pixel p);
		Pixel Sample(float x, float y);
		Pixel* GetData();
	
	private:
		Pixel *pColData = nullptr;
		Mode modeSample = Mode::NORMAL;

#ifdef OLC_DBG_OVERDRAW
	public:
		static int nOverdrawCount;
#endif

	};

	//=============================================================

	enum Key
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
		NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL,
	};


	//=============================================================

	class PixelGameEngine
	{
	public:
		PixelGameEngine();

	public:
		olc::rcode	Construct(uint32_t screen_w, uint32_t screen_h, uint32_t pixel_w, uint32_t pixel_h);
		olc::rcode	Start();

	public: // Override Interfaces
		// Called once on application startup, use to load your resources
		virtual bool OnUserCreate();
		// Called every frame, and provides you with a time per frame value
		virtual bool OnUserUpdate(float fElapsedTime);
		// Called once on application termination, so you can be a clean coder
		virtual bool OnUserDestroy();

	public: // Hardware Interfaces
		// Returns true if window is currently in focus
		bool IsFocused();
		// Get the state of a specific keyboard button
		HWButton GetKey(Key k);
		// Get the state of a specific mouse button
		HWButton GetMouse(uint32_t b);
		// Get Mouse X coordinate in "pixel" space
		int32_t GetMouseX();
		// Get Mouse Y coordinate in "pixel" space
		int32_t GetMouseY();

	public: // Utility
		// Returns the width of the screen in "pixels"
		int32_t ScreenWidth();
		// Returns the height of the screen in "pixels"
		int32_t ScreenHeight();
		// Returns the width of the currently selected drawing target in "pixels"
		int32_t GetDrawTargetWidth();
		// Returns the height of the currently selected drawing target in "pixels"
		int32_t GetDrawTargetHeight();
		// Returns the currently active draw target
		Sprite* GetDrawTarget();

	public: // Draw Routines
		// Specify which Sprite should be the target of drawing functions, use nullptr
		// to specify the primary screen
		void SetDrawTarget(Sprite *target);
		// Change the pixel mode for different optimisations
		// olc::Pixel::NORMAL = No transparency
		// olc::Pixel::MASK   = Transparent if alpha is < 255
		// olc::Pixel::ALPHA  = Full transparency
		void SetPixelMode(Pixel::Mode m);
		Pixel::Mode GetPixelMode();
		// Use a custom blend function
		void SetPixelMode(std::function<olc::Pixel(const int x, const int y, const olc::Pixel& pSource, const olc::Pixel& pDest)> pixelMode);
		// Change the blend factor form between 0.0f to 1.0f;
		void SetPixelBlend(float fBlend);
		// Offset texels by sub-pixel amount (advanced, do not use)
		void SetSubPixelOffset(float ox, float oy);

		// Draws a single Pixel
		virtual void Draw(int32_t x, int32_t y, Pixel p = olc::WHITE);
		// Draws a line from (x1,y1) to (x2,y2)
		void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p = olc::WHITE);
		// Draws a circle located at (x,y) with radius
		void DrawCircle(int32_t x, int32_t y, int32_t radius, Pixel p = olc::WHITE);
		// Fills a circle located at (x,y) with radius
		void FillCircle(int32_t x, int32_t y, int32_t radius, Pixel p = olc::WHITE);
		// Draws a rectangle at (x,y) to (x+w,y+h)
		void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = olc::WHITE);
		// Fills a rectangle at (x,y) to (x+w,y+h)
		void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = olc::WHITE);
		// Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
		void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = olc::WHITE);
		// Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
		void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = olc::WHITE);
		// Draws an entire sprite at location (x,y)
		void DrawSprite(int32_t x, int32_t y, Sprite *sprite, uint32_t scale = 1);
		// Draws an area of a sprite at location (x,y), where the
		// selected area is (ox,oy) to (ox+w,oy+h)
		void DrawPartialSprite(int32_t x, int32_t y, Sprite *sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale = 1);
		// Draws a single line of text
		void DrawString(int32_t x, int32_t y, std::string sText, Pixel col = olc::WHITE, uint32_t scale = 1);
		// Clears entire draw target to Pixel
		void Clear(Pixel p);

	public: // Branding
		std::string sAppName;

	private: // Inner mysterious workings
		Sprite		*pDefaultDrawTarget = nullptr;
		Sprite		*pDrawTarget = nullptr;
		Pixel::Mode	nPixelMode = Pixel::NORMAL;
		float		fBlendFactor = 1.0f;
		uint32_t	nScreenWidth = 256;
		uint32_t	nScreenHeight = 240;
		uint32_t	nPixelWidth = 4;
		uint32_t	nPixelHeight = 4;
		int32_t	nMousePosX = 0;
		int32_t	nMousePosY = 0;
		float		fPixelX = 1.0f;
		float		fPixelY = 1.0f;
		float		fSubPixelOffsetX = 0.0f;
		float		fSubPixelOffsetY = 0.0f;
		bool		bHasInputFocus = false;
		bool		bHasMouseFocus = false;
		float		fFrameTimer = 1.0f;
		int			nFrameCount = 0;
		Sprite		*fontSprite = nullptr;
		std::function<olc::Pixel(const int x, const int y, const olc::Pixel&, const olc::Pixel&)> funcPixelMode;

		static std::map<uint16_t, uint8_t> mapKeys;
		bool		pKeyNewState[256]{ 0 };
		bool		pKeyOldState[256]{ 0 };
		HWButton	pKeyboardState[256];

		bool		pMouseNewState[5]{ 0 };
		bool		pMouseOldState[5]{ 0 };
		HWButton	pMouseState[5];

#ifdef _WIN32
		HDC			glDeviceContext = nullptr;
		HGLRC		glRenderContext = nullptr;
#else
		GLXContext	glDeviceContext = nullptr;
		GLXContext	glRenderContext = nullptr;
#endif
		GLuint		glBuffer;

		void		EngineThread();

		// If anything sets this flag to false, the engine
		// "should" shut down gracefully
		static std::atomic<bool> bAtomActive;

		// Common initialisation functions
		void olc_UpdateMouse(int32_t x, int32_t y);
		bool olc_OpenGLCreate();
		void olc_ConstructFontSheet();

#ifdef _WIN32
		// Windows specific window handling
		HWND olc_hWnd = nullptr;
		HWND olc_WindowCreate();
		std::wstring wsAppName;
		static LRESULT CALLBACK olc_WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#else
		// Non-Windows specific window handling
		Display*				olc_Display = nullptr;
		Window					olc_WindowRoot;
		Window					olc_Window;
		XVisualInfo*            olc_VisualInfo;
		Colormap                olc_ColourMap;
		XSetWindowAttributes    olc_SetWindowAttribs;
		Display*				olc_WindowCreate();
#endif

	};


	class PGEX
	{
		friend class olc::PixelGameEngine;
	protected:
		static PixelGameEngine* pge;
	};

	//=============================================================
}

#endif // OLC_PGE_DEF




/*
	Object Oriented Mode
	~~~~~~~~~~~~~~~~~~~~

	If the olcPixelGameEngine.h is called from several sources it can cause
	multiple definitions of objects. To prevent this, ONLY ONE of the pathways
	to including this file must have OLC_PGE_APPLICATION defined before it. This prevents
	the definitions being duplicated.

	Consider the following project structure:

	Class1.h	- Includes olcPixelGameEngine.h, overrides olc::PixelGameEngine
	Class1.cpp	- #define OLC_PGE_APPLICATION #include "Class1.h"
	Class2.h	- Includes Class1.h, which includes olcPixelGameEngine.h
	Class2.cpp	- #define OLC_PGE_APPLICATION #include "Class2.h"
	main.cpp	- Includes Class1.h and Class2.h

	If all of this is a bit too confusing, you can split this file in two!
	Everything below this comment block can go into olcPixelGameEngineOOP.cpp
	and everything above it can go into olcPixelGameEngineOOP.h

*/

#ifdef OLC_PGE_APPLICATION
#undef OLC_PGE_APPLICATION

namespace olc
{
	Pixel::Pixel()
	{
		r = 0; g = 0; b = 0; a = 255;
	}

	Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		r = red; g = green; b = blue; a = alpha;
	}

	Pixel::Pixel(uint32_t p)
	{
		n = p;
	}

	//==========================================================

	std::wstring ConvertS2W(std::string s)
	{
#ifdef _WIN32
		int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
		wchar_t* buffer = new wchar_t[count];
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
		std::wstring w(buffer);
		delete[] buffer;
		return w;
#endif
//#ifdef __MINGW32__
//		wchar_t *buffer = new wchar_t[sImageFile.length() + 1];
//		mbstowcs(buffer, sImageFile.c_str(), sImageFile.length());
//		buffer[sImageFile.length()] = L'\0';
//		wsImageFile = buffer;
//		delete[] buffer;
//#else
	}

	Sprite::Sprite()
	{
		pColData = nullptr;
		width = 0;
		height = 0;
	}

	Sprite::Sprite(std::string sImageFile)
	{
		LoadFromFile(sImageFile);
	}

	Sprite::Sprite(std::string sImageFile, olc::ResourcePack *pack)
	{
		LoadFromPGESprFile(sImageFile, pack);
	}

	Sprite::Sprite(int32_t w, int32_t h)
	{
		if(pColData) delete[] pColData;
		width = w;		height = h;
		pColData = new Pixel[width * height];
		for (int32_t i = 0; i < width*height; i++)
			pColData[i] = Pixel();
	}

	Sprite::~Sprite()
	{
		if (pColData) delete pColData;
	}

	olc::rcode Sprite::LoadFromPGESprFile(std::string sImageFile, olc::ResourcePack *pack)
	{
		if (pColData) delete[] pColData;

		auto ReadData = [&](std::istream &is)
		{
			is.read((char*)&width, sizeof(int32_t));
			is.read((char*)&height, sizeof(int32_t));
			pColData = new Pixel[width * height];
			is.read((char*)pColData, width * height * sizeof(uint32_t));
		};

		// These are essentially Memory Surfaces represented by olc::Sprite
		// which load very fast, but are completely uncompressed
		if (pack == nullptr)
		{
			std::ifstream ifs;
			ifs.open(sImageFile, std::ifstream::binary);
			if (ifs.is_open())
			{
				ReadData(ifs);
				return olc::OK;
			}
			else
				return olc::FAIL;
		}
		else
		{
			auto streamBuffer = pack->GetStreamBuffer(sImageFile);
			std::istream is(&streamBuffer);
			ReadData(is);
		}


		return olc::FAIL;
		}

	olc::rcode Sprite::SaveToPGESprFile(std::string sImageFile)
	{
		if (pColData == nullptr) return olc::FAIL;

		std::ofstream ofs;
		ofs.open(sImageFile, std::ifstream::binary);
		if (ofs.is_open())
		{
			ofs.write((char*)&width, sizeof(int32_t));
			ofs.write((char*)&height, sizeof(int32_t));
			ofs.write((char*)pColData, width*height*sizeof(uint32_t));
			ofs.close();
			return olc::OK;
		}

		return olc::FAIL;
	}

	olc::rcode Sprite::LoadFromFile(std::string sImageFile, olc::ResourcePack *pack)
	{
#ifdef _WIN32
		// Use GDI+
		std::wstring wsImageFile;
#ifdef __MINGW32__
        wchar_t *buffer = new wchar_t[sImageFile.length() + 1];
        mbstowcs(buffer, sImageFile.c_str(), sImageFile.length());
        buffer[sImageFile.length()] = L'\0';
        wsImageFile = buffer;
        delete [] buffer;
#else
		wsImageFile = ConvertS2W(sImageFile);
#endif
        Gdiplus::Bitmap *bmp = Gdiplus::Bitmap::FromFile(wsImageFile.c_str());
		if (bmp == nullptr)
			return olc::NO_FILE;

		width = bmp->GetWidth();
		height = bmp->GetHeight();
		pColData = new Pixel[width * height];

		for(int x=0; x<width; x++)
			for (int y = 0; y < height; y++)
			{
				Gdiplus::Color c;
				bmp->GetPixel(x, y, &c);
				SetPixel(x, y, Pixel(c.GetRed(), c.GetGreen(), c.GetBlue(), c.GetAlpha()));
			}
		delete bmp;
		return olc::OK;
#else
		////////////////////////////////////////////////////////////////////////////
		// Use libpng, Thanks to Guillaume Cottenceau
		// https://gist.github.com/niw/5963798
		png_structp png;
		png_infop info;

		FILE *f = fopen(sImageFile.c_str(), "rb");
		if (!f) return olc::NO_FILE;

		png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) goto fail_load;

		info = png_create_info_struct(png);
		if (!info) goto fail_load;

		if (setjmp(png_jmpbuf(png))) goto fail_load;

		png_init_io(png, f);
		png_read_info(png, info);

		png_byte color_type;
		png_byte bit_depth;
		png_bytep *row_pointers;
		width = png_get_image_width(png, info);
		height = png_get_image_height(png, info);
		color_type = png_get_color_type(png, info);
		bit_depth = png_get_bit_depth(png, info);

#ifdef _DEBUG
		std::cout << "Loading PNG: " << sImageFile << "\n";
		std::cout << "W:" << width << " H:" << height << " D:" << (int)bit_depth << "\n";
#endif

		if (bit_depth == 16) png_set_strip_16(png);
		if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)	png_set_expand_gray_1_2_4_to_8(png);
		if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
		if (color_type == PNG_COLOR_TYPE_RGB ||
			color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
		if (color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);

		png_read_update_info(png, info);
		row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
		for (int y = 0; y < height; y++) {
			row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
		}
		png_read_image(png, row_pointers);
		////////////////////////////////////////////////////////////////////////////

		// Create sprite array
		pColData = new Pixel[width * height];

		// Iterate through image rows, converting into sprite format
		for (int y = 0; y < height; y++)
		{
			png_bytep row = row_pointers[y];
			for (int x = 0; x < width; x++)
			{
				png_bytep px = &(row[x * 4]);
				SetPixel(x, y, Pixel(px[0], px[1], px[2], px[3]));
			}
		}

		fclose(f);
		return olc::OK;

	fail_load:
		width = 0;
		height = 0;
		fclose(f);
		pColData = nullptr;
		return olc::FAIL;
#endif
	}

	void Sprite::SetSampleMode(olc::Sprite::Mode mode)
	{
		modeSample = mode;
	}


	Pixel Sprite::GetPixel(int32_t x, int32_t y)
	{
		if (modeSample == olc::Sprite::Mode::NORMAL)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
				return pColData[y*width + x];
			else
				return Pixel(0, 0, 0, 0);
		}
		else
		{			
			return pColData[abs(y%height)*width + abs(x%width)];
		}
	}

	void Sprite::SetPixel(int32_t x, int32_t y, Pixel p)
	{

#ifdef OLC_DBG_OVERDRAW
		nOverdrawCount++;
#endif

		if (x >= 0 && x < width && y >= 0 && y < height)
			pColData[y*width + x] = p;
	}

	Pixel Sprite::Sample(float x, float y)
	{
		int32_t sx = (int32_t)((x * (float)width) - 0.5f);
		int32_t sy = (int32_t)((y * (float)height) - 0.5f);
		return GetPixel(sx, sy);
	}

	Pixel* Sprite::GetData() { return pColData; }

	//==========================================================

	ResourcePack::ResourcePack()
	{

	}

	ResourcePack::~ResourcePack()
	{
		ClearPack();
	}

	olc::rcode ResourcePack::AddToPack(std::string sFile)
	{
		std::ifstream ifs(sFile, std::ifstream::binary);
		if (!ifs.is_open()) return olc::FAIL;

		// Get File Size
		std::streampos p = 0;
		p = ifs.tellg();
		ifs.seekg(0, std::ios::end);
		p = ifs.tellg() - p;
		ifs.seekg(0, std::ios::beg);

		// Create entry
		sEntry e;
		e.data = nullptr;
		e.nFileSize = (uint32_t)p;

		// Read file into memory
		e.data = new uint8_t[(uint32_t)e.nFileSize];
		ifs.read((char*)e.data, e.nFileSize);
		ifs.close();

		// Add To Map
		mapFiles[sFile] = e;
		return olc::OK;
	}

	olc::rcode ResourcePack::SavePack(std::string sFile)
	{
		std::ofstream ofs(sFile, std::ofstream::binary);
		if (!ofs.is_open()) return olc::FAIL;

		// 1) Write Map
		size_t nMapSize = mapFiles.size();
		ofs.write((char*)&nMapSize, sizeof(size_t));
		for (auto &e : mapFiles)
		{
			size_t nPathSize = e.first.size();
			ofs.write((char*)&nPathSize, sizeof(size_t));
			ofs.write(e.first.c_str(), nPathSize);
			ofs.write((char*)&e.second.nID, sizeof(uint32_t));
			ofs.write((char*)&e.second.nFileSize, sizeof(uint32_t));
			ofs.write((char*)&e.second.nFileOffset, sizeof(uint32_t));
		}

		// 2) Write Data
		std::streampos offset = ofs.tellp();
		for (auto &e : mapFiles)
		{
			e.second.nFileOffset = (uint32_t)offset;
			ofs.write((char*)e.second.data, e.second.nFileSize);
			offset += e.second.nFileSize;
		}

		// 3) Rewrite Map (it has been updated with offsets now)
		ofs.seekp(std::ios::beg);
		ofs.write((char*)&nMapSize, sizeof(size_t));
		for (auto &e : mapFiles)
		{
			size_t nPathSize = e.first.size();
			ofs.write((char*)&nPathSize, sizeof(size_t));
			ofs.write(e.first.c_str(), nPathSize);
			ofs.write((char*)&e.second.nID, sizeof(uint32_t));
			ofs.write((char*)&e.second.nFileSize, sizeof(uint32_t));
			ofs.write((char*)&e.second.nFileOffset, sizeof(uint32_t));
		}
		ofs.close();

		return olc::OK;
	}

	olc::rcode ResourcePack::LoadPack(std::string sFile)
	{
		std::ifstream ifs(sFile, std::ifstream::binary);
		if (!ifs.is_open()) return olc::FAIL;

		// 1) Read Map
		size_t nMapEntries;
		ifs.read((char*)&nMapEntries, sizeof(size_t));
		for (size_t i = 0; i < nMapEntries; i++)
		{
			size_t nFilePathSize = 0;
			ifs.read((char*)&nFilePathSize, sizeof(size_t));

			std::string sFileName(nFilePathSize, ' ');
			for (size_t j = 0; j < nFilePathSize; j++)
				sFileName[j] = ifs.get();

			sEntry e;
			e.data = nullptr;
			ifs.read((char*)&e.nID, sizeof(uint32_t));
			ifs.read((char*)&e.nFileSize, sizeof(uint32_t));
			ifs.read((char*)&e.nFileOffset, sizeof(uint32_t));
			mapFiles[sFileName] = e;
		}

		// 2) Read Data
		for (auto &e : mapFiles)
		{
			e.second.data = new uint8_t[(uint32_t)e.second.nFileSize];
			ifs.seekg(e.second.nFileOffset);
			ifs.read((char*)e.second.data, e.second.nFileSize);
			e.second._config();
		}

		ifs.close();
		return olc::OK;
	}

	olc::ResourcePack::sEntry ResourcePack::GetStreamBuffer(std::string sFile)
	{
		return mapFiles[sFile];
	}

	olc::rcode ResourcePack::ClearPack()
	{
		for (auto &e : mapFiles)
		{
			if (e.second.data != nullptr)
				delete[] e.second.data;
		}

		mapFiles.clear();
		return olc::OK;
	}

	//==========================================================

	PixelGameEngine::PixelGameEngine()
	{
		sAppName = "Undefined";
		olc::PGEX::pge = this;
	}

	olc::rcode PixelGameEngine::Construct(uint32_t screen_w, uint32_t screen_h, uint32_t pixel_w, uint32_t pixel_h)
	{
		nScreenWidth = screen_w;
		nScreenHeight = screen_h;
		nPixelWidth = pixel_w;
		nPixelHeight = pixel_h;

		fPixelX = 2.0f / (float)(nScreenWidth);
		fPixelY = 2.0f / (float)(nScreenHeight);

		if (nPixelWidth == 0 || nPixelHeight == 0 || nScreenWidth == 0 || nScreenHeight == 0)
			return olc::FAIL;
		
#ifdef _WIN32
#ifdef UNICODE
#ifndef __MINGW32__
		wsAppName = ConvertS2W(sAppName);
#endif
#endif
#endif
		// Load the default font sheet
		olc_ConstructFontSheet();
		
		// Create a sprite that represents the primary drawing target
		pDefaultDrawTarget = new Sprite(nScreenWidth, nScreenHeight);
		SetDrawTarget(nullptr);
		return olc::OK;
	}

	olc::rcode PixelGameEngine::Start()
	{
		// Construct the window
		if (!olc_WindowCreate())
			return olc::FAIL;

		// Load libraries required for PNG file interaction
#ifdef _WIN32
		// Windows use GDI+
		Gdiplus::GdiplusStartupInput startupInput;
		ULONG_PTR			token;
		Gdiplus::GdiplusStartup(&token, &startupInput, NULL);
#else
		// Linux use libpng

#endif
		// Start the thread
		bAtomActive = true;
		std::thread t = std::thread(&PixelGameEngine::EngineThread, this);

#ifdef _WIN32
		// Handle Windows Message Loop
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#endif

		// Wait for thread to be exited
		t.join();
		return olc::OK;
	}

	void PixelGameEngine::SetDrawTarget(Sprite *target)
	{
		if (target)
			pDrawTarget = target;
		else
			pDrawTarget = pDefaultDrawTarget;
	}

	Sprite* PixelGameEngine::GetDrawTarget()
	{
		return pDrawTarget;
	}

	int32_t PixelGameEngine::GetDrawTargetWidth()
	{
		if (pDrawTarget)
			return pDrawTarget->width;
		else
			return 0;
	}

	int32_t PixelGameEngine::GetDrawTargetHeight()
	{
		if (pDrawTarget)
			return pDrawTarget->height;
		else
			return 0;
	}

	bool PixelGameEngine::IsFocused()
	{
		return bHasInputFocus;
	}

	HWButton PixelGameEngine::GetKey(Key k)
	{
		return pKeyboardState[k];
	}

	HWButton PixelGameEngine::GetMouse(uint32_t b)
	{
		return pMouseState[b];
	}

	int32_t PixelGameEngine::GetMouseX()
	{
		return nMousePosX;
	}

	int32_t PixelGameEngine::GetMouseY()
	{
		return nMousePosY;
	}

	int32_t PixelGameEngine::ScreenWidth()
	{
		return nScreenWidth;
	}

	int32_t PixelGameEngine::ScreenHeight()
	{
		return nScreenHeight;
	}

	void PixelGameEngine::Draw(int32_t x, int32_t y, Pixel p)
	{
		if (!pDrawTarget) return;


		if (nPixelMode == Pixel::NORMAL)
		{
			pDrawTarget->SetPixel(x, y, p);
			return;
		}

		if (nPixelMode == Pixel::MASK)
		{
			if(p.a == 255)
				pDrawTarget->SetPixel(x, y, p);
			return;
		}

		if (nPixelMode == Pixel::ALPHA)
		{
			Pixel d = pDrawTarget->GetPixel(x, y);
			float a = (float)(p.a / 255.0f) * fBlendFactor;
			float c = 1.0f - a;
			float r = a * (float)p.r + c * (float)d.r;
			float g = a * (float)p.g + c * (float)d.g;
			float b = a * (float)p.b + c * (float)d.b;
			pDrawTarget->SetPixel(x, y, Pixel((uint8_t)r, (uint8_t)g, (uint8_t)b));
			return;
		}

		if (nPixelMode == Pixel::CUSTOM)
		{
			pDrawTarget->SetPixel(x, y, funcPixelMode(x, y, p, pDrawTarget->GetPixel(x, y)));
			return;
		}
	}

	void PixelGameEngine::SetSubPixelOffset(float ox, float oy)
	{
		fSubPixelOffsetX = ox * fPixelX;
		fSubPixelOffsetY = oy * fPixelY;
	}

	void PixelGameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;

		// straight lines idea by gurkanctn
		if (dx == 0) // Line is vertical
		{
			if (y2 < y1) std::swap(y1, y2);
			for (y = y1; y <= y2; y++)
				Draw(x1, y, p);
			return;
		}

		if (dy == 0) // Line is horizontal
		{
			if (x2 < x1) std::swap(x1, x2);
			for (x = x1; x <= x2; x++)
				Draw(x, y1, p);
			return;
		}

		// Line is Funk-aye
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1; y = y1; xe = x2;
			}
			else
			{
				x = x2; y = y2; xe = x1;
			}

			Draw(x, y, p);

			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, p);
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1; y = y1; ye = y2;
			}
			else
			{
				x = x2; y = y2; ye = y1;
			}

			Draw(x, y, p);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				Draw(x, y, p);
			}
		}
	}

	void PixelGameEngine::DrawCircle(int32_t x, int32_t y, int32_t radius, Pixel p)
	{
		int x0 = 0;
		int y0 = radius;
		int d = 3 - 2 * radius;
		if (!radius) return;

		while (y0 >= x0) // only formulate 1/8 of circle
		{
			Draw(x - x0, y - y0, p);//upper left left
			Draw(x - y0, y - x0, p);//upper upper left
			Draw(x + y0, y - x0, p);//upper upper right
			Draw(x + x0, y - y0, p);//upper right right
			Draw(x - x0, y + y0, p);//lower left left
			Draw(x - y0, y + x0, p);//lower lower left
			Draw(x + y0, y + x0, p);//lower lower right
			Draw(x + x0, y + y0, p);//lower right right
			if (d < 0) d += 4 * x0++ + 6;
			else d += 4 * (x0++ - y0--) + 10;
		}
	}

	void PixelGameEngine::FillCircle(int32_t x, int32_t y, int32_t radius, Pixel p)
	{
		// Taken from wikipedia
		int x0 = 0;
		int y0 = radius;
		int d = 3 - 2 * radius;
		if (!radius) return;

		auto drawline = [&](int sx, int ex, int ny)
		{
			for (int i = sx; i <= ex; i++)
				Draw(i, ny, p);
		};

		while (y0 >= x0)
		{
			// Modified to draw scan-lines instead of edges
			drawline(x - x0, x + x0, y - y0);
			drawline(x - y0, x + y0, y - x0);
			drawline(x - x0, x + x0, y + y0);
			drawline(x - y0, x + y0, y + x0);
			if (d < 0) d += 4 * x0++ + 6;
			else d += 4 * (x0++ - y0--) + 10;
		}
	}

	void PixelGameEngine::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p)
	{
		DrawLine(x, y, x+w, y, p);
		DrawLine(x+w, y, x+w, y+h, p);
		DrawLine(x+w, y+h, x, y+h, p);
		DrawLine(x, y+h, x, y, p);
	}

	void PixelGameEngine::Clear(Pixel p)
	{
		int pixels = GetDrawTargetWidth() * GetDrawTargetHeight();
		Pixel* m = GetDrawTarget()->GetData();
		for (int i = 0; i < pixels; i++)
			m[i] = p;
#ifdef OLC_DBG_OVERDRAW
		olc::Sprite::nOverdrawCount += pixels;
#endif
	}

	void PixelGameEngine::FillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p)
	{			
		int32_t x2 = x + w;
		int32_t y2 = y + h;

		if (x < 0) x = 0;
		if (x >= (int32_t)nScreenWidth) x = (int32_t)nScreenWidth;
		if (y < 0) y = 0;
		if (y >= (int32_t)nScreenHeight) y = (int32_t)nScreenHeight;

		if (x2 < 0) x2 = 0;
		if (x2 >= (int32_t)nScreenWidth) x2 = (int32_t)nScreenWidth;
		if (y2 < 0) y2 = 0;
		if (y2 >= (int32_t)nScreenHeight) y2 = (int32_t)nScreenHeight;

		for (int i = x; i < x2; i++)
			for (int j = y; j < y2; j++)
				Draw(i, j, p);
	}

	void PixelGameEngine::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p)
	{
		DrawLine(x1, y1, x2, y2, p);
		DrawLine(x2, y2, x3, y3, p);
		DrawLine(x3, y3, x1, y1, p);
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	void PixelGameEngine::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p)
	{
		auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, p); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1>y2) { SWAP(y1, y2); SWAP(x1, x2); }
		if (y1>y3) { SWAP(y1, y3); SWAP(x1, x3); }
		if (y2>y3) { SWAP(y2, y3); SWAP(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1); if (dx2<0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) {   // swap values
			SWAP(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			SWAP(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										// Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1<0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			SWAP(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x<t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i<dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i<dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:

			if (minx>t1x) minx = t1x; if (minx>t2x) minx = t2x;
			if (maxx<t1x) maxx = t1x; if (maxx<t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y>y3) return;
		}
	}

	void PixelGameEngine::DrawSprite(int32_t x, int32_t y, Sprite *sprite, uint32_t scale)
	{
		if (sprite == nullptr)
			return;

		if (scale > 1)
		{
			for (int32_t i = 0; i < sprite->width; i++)
				for (int32_t j = 0; j < sprite->height; j++)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
							Draw(x + (i*scale) + is, y + (j*scale) + js, sprite->GetPixel(i, j));
		}
		else
		{
			for (int32_t i = 0; i < sprite->width; i++)
				for (int32_t j = 0; j < sprite->height; j++)
					Draw(x + i, y + j, sprite->GetPixel(i, j));
		}
	}

	void PixelGameEngine::DrawPartialSprite(int32_t x, int32_t y, Sprite *sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale)
	{
		if (sprite == nullptr)
			return;

		if (scale > 1)
		{
			for (int32_t i = 0; i < w; i++)
				for (int32_t j = 0; j < h; j++)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
							Draw(x + (i*scale) + is, y + (j*scale) + js, sprite->GetPixel(i + ox, j + oy));
		}
		else
		{
			for (int32_t i = 0; i < w; i++)
				for (int32_t j = 0; j < h; j++)
					Draw(x + i, y + j, sprite->GetPixel(i + ox, j + oy));
		}
	}

	void PixelGameEngine::DrawString(int32_t x, int32_t y, std::string sText, Pixel col, uint32_t scale)
	{
		int32_t sx = 0;
		int32_t sy = 0;
		Pixel::Mode m = nPixelMode;
		if(col.ALPHA != 255)	SetPixelMode(Pixel::ALPHA);
		else					SetPixelMode(Pixel::MASK);
		for (auto c : sText)
		{
			if (c == '\n')
			{
				sx = 0; sy += 8 * scale;
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1)
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (fontSprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								for (uint32_t is = 0; is < scale; is++)
									for (uint32_t js = 0; js < scale; js++)
										Draw(x + sx + (i*scale) + is, y + sy + (j*scale) + js, col);
				}
				else
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (fontSprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)								
								Draw(x + sx + i, y + sy + j, col);
				}										
				sx += 8 * scale;
			}
		}
		SetPixelMode(m);
	}

	void PixelGameEngine::SetPixelMode(Pixel::Mode m)
	{
		nPixelMode = m;
	}

	Pixel::Mode PixelGameEngine::GetPixelMode()
	{
		return nPixelMode;
	}

	void PixelGameEngine::SetPixelMode(std::function<olc::Pixel(const int x, const int y, const olc::Pixel&, const olc::Pixel&)> pixelMode)
	{
		funcPixelMode = pixelMode;
		nPixelMode = Pixel::Mode::CUSTOM;
	}

	void PixelGameEngine::SetPixelBlend(float fBlend)
	{
		fBlendFactor = fBlend;
		if (fBlendFactor < 0.0f) fBlendFactor = 0.0f;
		if (fBlendFactor > 1.0f) fBlendFactor = 1.0f;
	}

	// User must override these functions as required. I have not made
	// them abstract because I do need a default behaviour to occur if
	// they are not overwritten
	bool PixelGameEngine::OnUserCreate()
	{ return false; }
	bool PixelGameEngine::OnUserUpdate(float fElapsedTime)
	{ return false; }
	bool PixelGameEngine::OnUserDestroy()
	{ return true; }
	//////////////////////////////////////////////////////////////////

	void PixelGameEngine::olc_UpdateMouse(int32_t x, int32_t y)
	{
		// Mouse coords come in screen space
		// But leave in pixel space
		nMousePosX = x / (int32_t)nPixelWidth;
		nMousePosY = y / (int32_t)nPixelHeight;

		if (nMousePosX >= (int32_t)nScreenWidth)
			nMousePosX = nScreenWidth - 1;
		if (nMousePosY >= (int32_t)nScreenHeight)
			nMousePosY = nScreenHeight - 1;

		if (nMousePosX < 0)
			nMousePosX = 0;
		if (nMousePosY < 0)
			nMousePosY = 0;
	}

	void PixelGameEngine::EngineThread()
	{
		// Start OpenGL, the context is owned by the game thread
		olc_OpenGLCreate();

		// Create Screen Texture - disable filtering
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &glBuffer);
		glBindTexture(GL_TEXTURE_2D, glBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nScreenWidth, nScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pDefaultDrawTarget->GetData());


		// Create user resources as part of this thread
		if (!OnUserCreate())
			bAtomActive = false;

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();

		while (bAtomActive)
		{
			// Run as fast as possible
			while (bAtomActive)
			{
				// Handle Timing
				tp2 = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = tp2 - tp1;
				tp1 = tp2;

				// Our time per frame coefficient
				float fElapsedTime = elapsedTime.count();

#ifndef _WIN32
				// Handle Xlib Message Loop - we do this in the
				// same thread that OpenGL was created so we dont
				// need to worry too much about multithreading with X11
				XEvent xev;
				while (XPending(olc_Display))
				{
					XNextEvent(olc_Display, &xev);
					if (xev.type == Expose)
					{
						XWindowAttributes gwa;
						XGetWindowAttributes(olc_Display, olc_Window, &gwa);
						glViewport(0, 0, gwa.width, gwa.height);
					}
					else if (xev.type == KeyPress)
					{
						KeySym sym = XLookupKeysym(&xev.xkey, 0);
						pKeyNewState[mapKeys[sym]] = true;
					}
					else if (xev.type == KeyRelease)
					{
						KeySym sym = XLookupKeysym(&xev.xkey, 0);
						pKeyNewState[mapKeys[sym]] = false;
					}
					else if (xev.type == ButtonPress)
					{
						pMouseNewState[xev.xbutton.button-1] = true;
					}
					else if (xev.type == ButtonRelease)
					{
						pMouseNewState[xev.xbutton.button-1] = false;
					}
					else if (xev.type == MotionNotify)
					{
						olc_UpdateMouse(xev.xmotion.x, xev.xmotion.y);
					}
					else if (xev.type == FocusIn)
					{
						bHasInputFocus = true;
					}
					else if (xev.type == FocusOut)
					{
						bHasInputFocus = false;
					}
					else if (xev.type == ClientMessage)
					{
						bAtomActive = false;
					}
				}
#endif

				// Handle User Input - Keyboard
				for (int i = 0; i < 256; i++)
				{
					pKeyboardState[i].bPressed = false;
					pKeyboardState[i].bReleased = false;

					if (pKeyNewState[i] != pKeyOldState[i])
					{
						if (pKeyNewState[i])
						{
							pKeyboardState[i].bPressed = !pKeyboardState[i].bHeld;
							pKeyboardState[i].bHeld = true;
						}
						else
						{
							pKeyboardState[i].bReleased = true;
							pKeyboardState[i].bHeld = false;
						}
					}

					pKeyOldState[i] = pKeyNewState[i];
				}

				// Handle User Input - Mouse
				for (int i = 0; i < 5; i++)
				{
					pMouseState[i].bPressed = false;
					pMouseState[i].bReleased = false;

					if (pMouseNewState[i] != pMouseOldState[i])
					{
						if (pMouseNewState[i])
						{
							pMouseState[i].bPressed = !pMouseState[i].bHeld;
							pMouseState[i].bHeld = true;
						}
						else
						{
							pMouseState[i].bReleased = true;
							pMouseState[i].bHeld = false;
						}
					}

					pMouseOldState[i] = pMouseNewState[i];
				}

#ifdef OLC_DBG_OVERDRAW
				olc::Sprite::nOverdrawCount = 0;
#endif

				// Handle Frame Update
				if (!OnUserUpdate(fElapsedTime))
					bAtomActive = false;

				// Display Graphics

				// TODO: This is a bit slow (especially in debug, but 100x faster in release mode???)
				// Copy pixel array into texture
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nScreenWidth, nScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, pDefaultDrawTarget->GetData());
				
				// Display texture on screen
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f + (fSubPixelOffsetX), -1.0f + (fSubPixelOffsetY), 0.0f);
					glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f + (fSubPixelOffsetX),  1.0f + (fSubPixelOffsetY), 0.0f);
					glTexCoord2f(1.0, 0.0); glVertex3f( 1.0f + (fSubPixelOffsetX),  1.0f + (fSubPixelOffsetY), 0.0f);
					glTexCoord2f(1.0, 1.0); glVertex3f( 1.0f + (fSubPixelOffsetX), -1.0f + (fSubPixelOffsetY), 0.0f);
				glEnd();

				// Present Graphics to screen
#ifdef _WIN32
				SwapBuffers(glDeviceContext);
#else
				glXSwapBuffers(olc_Display, olc_Window);
#endif

				// Update Title Bar
				fFrameTimer += fElapsedTime;
				nFrameCount++;
				if (fFrameTimer >= 1.0f)
				{
					fFrameTimer -= 1.0f;

					std::string sTitle = "OneLoneCoder.com - Pixel Game Engine - " + sAppName + " - FPS: " + std::to_string(nFrameCount);
#ifdef _WIN32
#ifdef UNICODE
					SetWindowText(olc_hWnd, ConvertS2W(sTitle).c_str());
#else
					SetWindowText(olc_hWnd, sTitle.c_str());
#endif
#else					
					XStoreName(olc_Display, olc_Window, sTitle.c_str());
#endif
					nFrameCount = 0;
				}
			}

			// Allow the user to free resources if they have overrided the destroy function
			if (OnUserDestroy())
			{
				// User has permitted destroy, so exit and clean up
			}
			else
			{
				// User denied destroy for some reason, so continue running
				bAtomActive = true;
			}
		}

#ifdef _WIN32
		wglDeleteContext(glRenderContext);
		PostMessage(olc_hWnd, WM_DESTROY, 0, 0);
#else
		glXMakeCurrent(olc_Display, None, NULL);
		glXDestroyContext(olc_Display, glDeviceContext);
		XDestroyWindow(olc_Display, olc_Window);
		XCloseDisplay(olc_Display);
#endif

	}


	void PixelGameEngine::olc_ConstructFontSheet()
	{
		std::string data;
		data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
		data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
		data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
		data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
		data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
		data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
		data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
		data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
		data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
		data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
		data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
		data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
		data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
		data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
		data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
		data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

		fontSprite = new olc::Sprite(128, 48);
		int px = 0, py = 0;
		for (int b = 0; b < 1024; b += 4)
		{
			uint32_t sym1 = (uint32_t)data[b + 0] - 48;
			uint32_t sym2 = (uint32_t)data[b + 1] - 48;
			uint32_t sym3 = (uint32_t)data[b + 2] - 48;
			uint32_t sym4 = (uint32_t)data[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 24; i++)
			{
				int k = r & (1 << i) ? 255 : 0;
				fontSprite->SetPixel(px, py, olc::Pixel(k, k, k, k));
				if (++py == 48) { px++; py = 0; }
			}
		}
	}

#ifdef _WIN32
	HWND PixelGameEngine::olc_WindowCreate()
	{
		WNDCLASS wc;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = olc_WindowEvent;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.lpszMenuName = nullptr;
		wc.hbrBackground = nullptr;
#ifdef UNICODE
		wc.lpszClassName = L"OLC_PIXEL_GAME_ENGINE";
#else
		wc.lpszClassName = "OLC_PIXEL_GAME_ENGINE";
#endif

		RegisterClass(&wc);

		// Define window furniture
		DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
		RECT rWndRect = { 0, 0, (LONG)nScreenWidth * (LONG)nPixelWidth, (LONG)nScreenHeight * (LONG)nPixelHeight };

		// Keep client size as requested
		AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);

		int width = rWndRect.right - rWndRect.left;
		int height = rWndRect.bottom - rWndRect.top;

#ifdef UNICODE
		olc_hWnd = CreateWindowEx(dwExStyle, L"OLC_PIXEL_GAME_ENGINE", L"", dwStyle,
			30, 30, width, height, NULL, NULL, GetModuleHandle(nullptr), this);
#else
		olc_hWnd = CreateWindowEx(dwExStyle, "OLC_PIXEL_GAME_ENGINE", "", dwStyle,
			30, 30, width, height, NULL, NULL, GetModuleHandle(nullptr), this);
#endif

		// Create Keyboard Mapping
		mapKeys[0x41] = Key::A; mapKeys[0x42] = Key::B; mapKeys[0x43] = Key::C; mapKeys[0x44] = Key::D; mapKeys[0x45] = Key::E;
		mapKeys[0x46] = Key::F; mapKeys[0x47] = Key::G; mapKeys[0x48] = Key::H; mapKeys[0x49] = Key::I; mapKeys[0x4A] = Key::J;
		mapKeys[0x4B] = Key::K; mapKeys[0x4C] = Key::L; mapKeys[0x4D] = Key::M; mapKeys[0x4E] = Key::N; mapKeys[0x4F] = Key::O;
		mapKeys[0x50] = Key::P; mapKeys[0x51] = Key::Q; mapKeys[0x52] = Key::R; mapKeys[0x53] = Key::S; mapKeys[0x54] = Key::T;
		mapKeys[0x55] = Key::U; mapKeys[0x56] = Key::V; mapKeys[0x57] = Key::W; mapKeys[0x58] = Key::X; mapKeys[0x59] = Key::Y;
		mapKeys[0x5A] = Key::Z;

		mapKeys[VK_F1] = Key::F1; mapKeys[VK_F2] = Key::F2; mapKeys[VK_F3] = Key::F3; mapKeys[VK_F4] = Key::F4;
		mapKeys[VK_F5] = Key::F5; mapKeys[VK_F6] = Key::F6; mapKeys[VK_F7] = Key::F7; mapKeys[VK_F8] = Key::F8;
		mapKeys[VK_F9] = Key::F9; mapKeys[VK_F10] = Key::F10; mapKeys[VK_F11] = Key::F11; mapKeys[VK_F12] = Key::F12;

		mapKeys[VK_DOWN] = Key::DOWN; mapKeys[VK_LEFT] = Key::LEFT; mapKeys[VK_RIGHT] = Key::RIGHT; mapKeys[VK_UP] = Key::UP;
		mapKeys[VK_RETURN] = Key::ENTER; //mapKeys[VK_RETURN] = Key::RETURN;

		mapKeys[VK_BACK] = Key::BACK; mapKeys[VK_ESCAPE] = Key::ESCAPE; mapKeys[VK_RETURN] = Key::ENTER; mapKeys[VK_PAUSE] = Key::PAUSE;
		mapKeys[VK_SCROLL] = Key::SCROLL; mapKeys[VK_TAB] = Key::TAB; mapKeys[VK_DELETE] = Key::DEL; mapKeys[VK_HOME] = Key::HOME;
		mapKeys[VK_END] = Key::END; mapKeys[VK_PRIOR] = Key::PGUP; mapKeys[VK_NEXT] = Key::PGDN; mapKeys[VK_INSERT] = Key::INS;
		mapKeys[VK_SHIFT] = Key::SHIFT; mapKeys[VK_CONTROL] = Key::CTRL;
		mapKeys[VK_SPACE] = Key::SPACE;

		mapKeys[0x30] = Key::K0; mapKeys[0x31] = Key::K1; mapKeys[0x32] = Key::K2; mapKeys[0x33] = Key::K3; mapKeys[0x34] = Key::K4;
		mapKeys[0x35] = Key::K5; mapKeys[0x36] = Key::K6; mapKeys[0x37] = Key::K7; mapKeys[0x38] = Key::K8; mapKeys[0x39] = Key::K9;

		mapKeys[VK_NUMPAD0] = Key::NP0; mapKeys[VK_NUMPAD1] = Key::NP1; mapKeys[VK_NUMPAD2] = Key::NP2; mapKeys[VK_NUMPAD3] = Key::NP3; mapKeys[VK_NUMPAD4] = Key::NP4;
		mapKeys[VK_NUMPAD5] = Key::NP5; mapKeys[VK_NUMPAD6] = Key::NP6; mapKeys[VK_NUMPAD7] = Key::NP7; mapKeys[VK_NUMPAD8] = Key::NP8; mapKeys[VK_NUMPAD9] = Key::NP9;
		mapKeys[VK_MULTIPLY] = Key::NP_MUL; mapKeys[VK_ADD] = Key::NP_ADD; mapKeys[VK_DIVIDE] = Key::NP_DIV; mapKeys[VK_SUBTRACT] = Key::NP_SUB; mapKeys[VK_DECIMAL] = Key::NP_DECIMAL;

		return olc_hWnd;
	}

	bool PixelGameEngine::olc_OpenGLCreate()
	{
		// Create Device Context
		glDeviceContext = GetDC(olc_hWnd);
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), 1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			PFD_MAIN_PLANE, 0, 0, 0, 0
		};

		int pf = 0;
		if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return false;
		SetPixelFormat(glDeviceContext, pf, &pfd);

		if (!(glRenderContext = wglCreateContext(glDeviceContext))) return false;
		wglMakeCurrent(glDeviceContext, glRenderContext);

		// Remove Frame cap
		wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapInterval(0);
		return true;
	}

	// Windows Event Handler
	LRESULT CALLBACK PixelGameEngine::olc_WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static PixelGameEngine *sge;
		switch (uMsg)
		{
		case WM_CREATE:		sge = (PixelGameEngine*)((LPCREATESTRUCT)lParam)->lpCreateParams;	return 0;
		case WM_MOUSEMOVE:	
		{
			uint16_t x = lParam & 0xFFFF;				// Thanks @ForAbby (Discord)
			uint16_t y = (lParam >> 16) & 0xFFFF;
			int16_t ix = *(int16_t*)&x;
			int16_t iy = *(int16_t*)&y;
			sge->olc_UpdateMouse(ix, iy);	
			return 0;
		}
		case WM_MOUSELEAVE: sge->bHasMouseFocus = false;
		case WM_SETFOCUS:	sge->bHasInputFocus = true;								return 0;
		case WM_KILLFOCUS:	sge->bHasInputFocus = false;							return 0;
		case WM_KEYDOWN:	sge->pKeyNewState[mapKeys[wParam]] = true;				return 0;
		case WM_KEYUP:		sge->pKeyNewState[mapKeys[wParam]] = false;				return 0;
		case WM_LBUTTONDOWN:sge->pMouseNewState[0] = true;							return 0;
		case WM_LBUTTONUP:	sge->pMouseNewState[0] = false;							return 0;
		case WM_RBUTTONDOWN:sge->pMouseNewState[1] = true;							return 0;
		case WM_RBUTTONUP:	sge->pMouseNewState[1] = false;							return 0;
		case WM_MBUTTONDOWN:sge->pMouseNewState[2] = true;							return 0;
		case WM_MBUTTONUP:	sge->pMouseNewState[2] = false;							return 0;
		case WM_CLOSE:		bAtomActive = false;									return 0;
		case WM_DESTROY:	PostQuitMessage(0);										return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
#else
	// Do the Linux stuff!
	Display* PixelGameEngine::olc_WindowCreate()
	{
		XInitThreads();

		// Grab the deafult display and window
		olc_Display		= XOpenDisplay(NULL);
		olc_WindowRoot	= DefaultRootWindow(olc_Display);

		// Based on the display capabilities, configure the appearance of the window
		GLint olc_GLAttribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		olc_VisualInfo	= glXChooseVisual(olc_Display, 0, olc_GLAttribs);
		olc_ColourMap	= XCreateColormap(olc_Display, olc_WindowRoot, olc_VisualInfo->visual, AllocNone);
		olc_SetWindowAttribs.colormap = olc_ColourMap;

		// Register which events we are interested in receiving
		olc_SetWindowAttribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask;

		// Create the window
		olc_Window		= XCreateWindow(olc_Display, olc_WindowRoot, 30, 30, nScreenWidth * nPixelWidth, nScreenHeight * nPixelHeight, 0, olc_VisualInfo->depth, InputOutput, olc_VisualInfo->visual, CWColormap | CWEventMask, &olc_SetWindowAttribs);

		Atom wmDelete = XInternAtom(olc_Display, "WM_DELETE_WINDOW", true);
		XSetWMProtocols(olc_Display, olc_Window, &wmDelete, 1);

		XMapWindow(olc_Display, olc_Window);
		XStoreName(olc_Display, olc_Window, "OneLoneCoder.com - Pixel Game Engine");

		// Create Keyboard Mapping
		mapKeys[0x61] = Key::A; mapKeys[0x62] = Key::B; mapKeys[0x63] = Key::C; mapKeys[0x64] = Key::D; mapKeys[0x65] = Key::E;
		mapKeys[0x66] = Key::F; mapKeys[0x67] = Key::G; mapKeys[0x68] = Key::H; mapKeys[0x69] = Key::I; mapKeys[0x6A] = Key::J;
		mapKeys[0x6B] = Key::K; mapKeys[0x6C] = Key::L; mapKeys[0x6D] = Key::M; mapKeys[0x6E] = Key::N; mapKeys[0x6F] = Key::O;
		mapKeys[0x70] = Key::P; mapKeys[0x71] = Key::Q; mapKeys[0x72] = Key::R; mapKeys[0x73] = Key::S; mapKeys[0x74] = Key::T;
		mapKeys[0x75] = Key::U; mapKeys[0x76] = Key::V; mapKeys[0x77] = Key::W; mapKeys[0x78] = Key::X; mapKeys[0x79] = Key::Y;
		mapKeys[0x7A] = Key::Z;

		mapKeys[XK_F1] = Key::F1; mapKeys[XK_F2] = Key::F2; mapKeys[XK_F3] = Key::F3; mapKeys[XK_F4] = Key::F4;
		mapKeys[XK_F5] = Key::F5; mapKeys[XK_F6] = Key::F6; mapKeys[XK_F7] = Key::F7; mapKeys[XK_F8] = Key::F8;
		mapKeys[XK_F9] = Key::F9; mapKeys[XK_F10] = Key::F10; mapKeys[XK_F11] = Key::F11; mapKeys[XK_F12] = Key::F12;

		mapKeys[XK_Down] = Key::DOWN; mapKeys[XK_Left] = Key::LEFT; mapKeys[XK_Right] = Key::RIGHT; mapKeys[XK_Up] = Key::UP;
		mapKeys[XK_KP_Enter] = Key::ENTER; mapKeys[XK_Return] = Key::ENTER;

		mapKeys[XK_BackSpace] = Key::BACK; mapKeys[XK_Escape] = Key::ESCAPE; mapKeys[XK_Linefeed] = Key::ENTER;	mapKeys[XK_Pause] = Key::PAUSE;
		mapKeys[XK_Scroll_Lock] = Key::SCROLL; mapKeys[XK_Tab] = Key::TAB; mapKeys[XK_Delete] = Key::DEL; mapKeys[XK_Home] = Key::HOME;
		mapKeys[XK_End] = Key::END; mapKeys[XK_Page_Up] = Key::PGUP; mapKeys[XK_Page_Down] = Key::PGDN;	mapKeys[XK_Insert] = Key::INS;
		mapKeys[XK_Shift_L] = Key::SHIFT; mapKeys[XK_Shift_R] = Key::SHIFT; mapKeys[XK_Control_L] = Key::CTRL; mapKeys[XK_Control_R] = Key::CTRL;
		mapKeys[XK_space] = Key::SPACE;

		mapKeys[XK_0] = Key::K0; mapKeys[XK_1] = Key::K1; mapKeys[XK_2] = Key::K2; mapKeys[XK_3] = Key::K3; mapKeys[XK_4] = Key::K4;
		mapKeys[XK_5] = Key::K5; mapKeys[XK_6] = Key::K6; mapKeys[XK_7] = Key::K7; mapKeys[XK_8] = Key::K8; mapKeys[XK_9] = Key::K9;

		mapKeys[XK_KP_0] = Key::NP0; mapKeys[XK_KP_1] = Key::NP1; mapKeys[XK_KP_2] = Key::NP2; mapKeys[XK_KP_3] = Key::NP3; mapKeys[XK_KP_4] = Key::NP4;
		mapKeys[XK_KP_5] = Key::NP5; mapKeys[XK_KP_6] = Key::NP6; mapKeys[XK_KP_7] = Key::NP7; mapKeys[XK_KP_8] = Key::NP8; mapKeys[XK_KP_9] = Key::NP9;
		mapKeys[XK_KP_Multiply] = Key::NP_MUL; mapKeys[XK_KP_Add] = Key::NP_ADD; mapKeys[XK_KP_Divide] = Key::NP_DIV; mapKeys[XK_KP_Subtract] = Key::NP_SUB; mapKeys[XK_KP_Decimal] = Key::NP_DECIMAL;

		return olc_Display;
	}

	bool PixelGameEngine::olc_OpenGLCreate()
	{
		glDeviceContext = glXCreateContext(olc_Display, olc_VisualInfo, nullptr, GL_TRUE);
		glXMakeCurrent(olc_Display, olc_Window, glDeviceContext);
		
		XWindowAttributes gwa;
		XGetWindowAttributes(olc_Display, olc_Window, &gwa);		
		glViewport(0, 0, gwa.width, gwa.height);
		
		glSwapIntervalEXT = nullptr;
		glSwapIntervalEXT = (glSwapInterval_t*)glXGetProcAddress((unsigned char*)"glXSwapIntervalEXT");
		if (glSwapIntervalEXT)
			glSwapIntervalEXT(olc_Display, olc_Window, 0);
		else
		{
			printf("NOTE: Could not disable VSYNC, glXSwapIntervalEXT() was not found!\n");
			printf("      Don't worry though, things will still work, it's just the\n");
			printf("      frame rate will be capped to your monitors refresh rate - javidx9\n");
		}
		
		return true;
	}

#endif

	// Need a couple of statics as these are singleton instances
	// read from multiple locations
	std::atomic<bool> PixelGameEngine::bAtomActive{ false };
	std::map<uint16_t, uint8_t> PixelGameEngine::mapKeys;
	olc::PixelGameEngine* olc::PGEX::pge = nullptr;
#ifdef OLC_DBG_OVERDRAW
	int olc::Sprite::nOverdrawCount = 0;
#endif
	//=============================================================
}

#endif