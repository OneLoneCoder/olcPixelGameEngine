#pragma region license_and_help
/*
	olcPixelGameEngine.h

	+-------------------------------------------------------------+
	|           OneLoneCoder Pixel Game Engine v2.28              |
	|  "What do you need? Pixels... Lots of Pixels..." - javidx9  |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	olc::PixelGameEngine is a single file, cross platform graphics and userinput
	framework used for games, visualisations, algorithm exploration and learning.
	It was developed by YouTuber "javidx9" as an assistive tool for many of his
	videos. The goal of this project is to provide high speed graphics with
	minimal project setup complexity, to encourage new programmers, younger people,
	and anyone else that wants to make fun things.

	However, olc::PixelGameEngine is not a toy! It is a powerful and fast utility
	capable of delivering high resolution, high speed, high quality applications
	which behave the same way regardless of the operating system or platform.

	This file provides the core utility set of the olc::PixelGameEngine, including
	window creation, keyboard/mouse input, main game thread, timing, pixel drawing
	routines, image/sprite loading and drawing routines, and a bunch of utility
	types to make rapid development of games/visualisations possible.


	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2024 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	1. Redistributions or derivations of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce the above
	copyright notice. This list of conditions and the following	disclaimer must be
	reproduced in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may
	be used to endorse or promote products derived from this software without specific
	prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	"AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL,	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com
	Patreon:	https://www.patreon.com/javidx9
	Community:  https://community.onelonecoder.com



	Compiling in Linux
	~~~~~~~~~~~~~~~~~~
	You will need a modern C++ compiler, so update yours!
	To compile use the command:

	g++ -o YourProgName YourSource.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

	On some Linux configurations, the frame rate is locked to the refresh
	rate of the monitor. This engine tries to unlock it but may not be
	able to, in which case try launching your program like this:

	vblank_mode=0 ./YourProgName



	Compiling in Code::Blocks on Windows
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Well I wont judge you, but make sure your Code::Blocks installation
	is really up to date - you may even consider updating your C++ toolchain
	to use MinGW32-W64.

	Guide for installing recent GCC for Windows:
	https://www.msys2.org/
	Guide for configuring code::blocks:
	https://solarianprogrammer.com/2019/11/05/install-gcc-windows/
	https://solarianprogrammer.com/2019/11/16/install-codeblocks-gcc-windows-build-c-cpp-fortran-programs/

	Add these libraries to "Linker Options":
	user32 gdi32 opengl32 gdiplus Shlwapi dwmapi stdc++fs

	Set these compiler options: -std=c++17



	Compiling on Mac - EXPERIMENTAL! PROBABLY HAS BUGS
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Yes yes, people use Macs for C++ programming! Who knew? Anyway, enough
	arguing, thanks to Mumflr the PGE is now supported on Mac. Now I know nothing
	about Mac, so if you need support, I suggest checking out the instructions
	here: https://github.com/MumflrFumperdink/olcPGEMac

	clang++ -arch x86_64 -std=c++17 -mmacosx-version-min=10.15 -Wall -framework OpenGL 
		-framework GLUT -framework Carbon -lpng YourSource.cpp -o YourProgName



	Compiling with Emscripten (New & Experimental)
	~~~~~~~~~~~~~~~~~~~~~~~~~
	Emscripten compiler will turn your awesome C++ PixelGameEngine project into WASM!
	This means you can run your application in teh browser, great for distributing
	and submission in to jams and things! It's a bit new at the moment. 

	em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 ./YourSource.cpp -o pge.html



	Using stb_image.h
	~~~~~~~~~~~~~~~~~
	The PGE will load png images by default (with help from libpng on non-windows systems).
	However, the excellent "stb_image.h" can be used instead, supporting a variety of
	image formats, and has no library dependence - something we like at OLC studios ;)
	To use stb_image.h, make sure it's in your code base, and simply:

	#define OLC_IMAGE_STB

	Before including the olcPixelGameEngine.h header file. stb_image.h works on many systems
	and can be downloaded here: https://github.com/nothings/stb/blob/master/stb_image.h



	Multiple cpp file projects?
	~~~~~~~~~~~~~~~~~~~~~~~~~~~
	As a single header solution, the OLC_PGE_APPLICATION definition is used to
	insert the engine implementation at a project location of your choosing.
	The simplest way to setup multifile projects is to create a file called
	"olcPixelGameEngine.cpp" which includes the following:

	#define OLC_PGE_APPLICATION
	#include "olcPixelGameEngine.h"

	That's all it should include. You can also include PGEX includes and 
	defines in here too. With this in place, you dont need to 
	#define OLC_PGE_APPLICATION anywhere, and can simply include this 
	header file as an when you need to.



	Ports
	~~~~~
	olc::PixelGameEngine has been ported and tested with varying degrees of
	success to: WinXP, Win7, Win8, Win10, Various Linux, Raspberry Pi,
	Chromebook, Playstation Portable (PSP) and Nintendo Switch. If you are
	interested in the details of these ports, come and visit the Discord!



	Thanks
	~~~~~~
	I'd like to extend thanks to Ian McKay, Bispoo, Eremiell, slavka, Kwizatz77, gurkanctn, Phantim,
	IProgramInCPP, JackOJC, KrossX, Huhlig, Dragoneye, Appa, JustinRichardsMusic, SliceNDice, 
	dandistine,	Ralakus, Gorbit99, raoul, joshinils, benedani, Moros1138, Alexio, SaladinAkara 
	& MagetzUb for advice, ideas and testing, and I'd like to extend my appreciation to the
	250K YouTube followers,	80+ Patreons, 4.8K Twitch followers and 10K Discord server members 
	who give me	the motivation to keep going with all this :D

	Significant Contributors: @Moros1138, @SaladinAkara, @MaGetzUb, @slavka,
							  @Dragoneye, @Gorbit99, @dandistine & @Mumflr

	Special thanks to those who bring gifts!
	GnarGnarHead.......Domina
	Gorbit99...........Bastion, Ori & The Blind Forest, Terraria, Spelunky 2, Skully
	Marti Morta........Gris
	Danicron...........Terraria
	SaladinAkara.......Aseprite, Inside, Quern: Undying Thoughts, Outer Wilds
	AlterEgo...........Final Fantasy XII - The Zodiac Age
	SlicEnDicE.........Noita, Inside
	TGD................Voucher Gift
	Dragoneye..........Lucas Arts Adventure Game Pack
	Anonymous Pirate...Return To Monkey Island

	Special thanks to my Patreons too - I wont name you on here, but I've
	certainly enjoyed my tea and flapjacks :D

	- In Memory of SaladinAkara 25.06.2023 -

	Author
	~~~~~~
	David Barr, aka javidx9, (c) OneLoneCoder 2018, 2019, 2020, 2021, 2022, 2023, 2024
*/
#pragma endregion

#pragma region version_history
/*
	2.01: Made renderer and platform static for multifile projects
	2.02: Added Decal destructor, optimised Pixel constructor
	2.03: Added FreeBSD flags, Added DrawStringDecal()
	2.04: Windows Full-Screen bug fixed
	2.05: +DrawPartialWarpedDecal() - draws a warped decal from a subset image
		  +DrawPartialRotatedDecal() - draws a rotated decal from a subset image
	2.06: +GetTextSize() - returns area occupied by multiline string
		  +GetWindowSize() - returns actual window size
		  +GetElapsedTime() - returns last calculated fElapsedTime
		  +GetWindowMouse() - returns actual mouse location in window
		  +DrawExplicitDecal() - bow-chikka-bow-bow
		  +DrawPartialDecal(pos, size) - draws a partial decal to specified area
		  +FillRectDecal() - draws a flat shaded rectangle as a decal
		  +GradientFillRectDecal() - draws a rectangle, with unique colour corners
		  +Modified DrawCircle() & FillCircle() - Thanks IanM-Matrix1 (#PR121)
		  +Gone someway to appeasing pedants
	2.07: +GetPixelSize() - returns user specified pixel size
		  +GetScreenPixelSize() - returns actual size in monitor pixels
		  +Pixel Cohesion Mode (flag in Construct()) - disallows arbitrary window scaling
		  +Working VSYNC in Windows windowed application - now much smoother
		  +Added string conversion for olc::vectors
		  +Added comparator operators for olc::vectors
		  +Added DestroyWindow() on windows platforms for serial PGE launches
		  +Added GetMousePos() to stop TarriestPython whinging
	2.08: Fix SetScreenSize() aspect ratio pre-calculation
		  Fix DrawExplicitDecal() - stupid oversight with multiple decals
		  Disabled olc::Sprite copy constructor
		  +olc::Sprite Duplicate() - produces a new clone of the sprite
		  +olc::Sprite Duplicate(pos, size) - produces a new sprite from the region defined
		  +Unary operators for vectors
		  +More pedant mollification - Thanks TheLandfill
		  +ImageLoader modules - user selectable image handling core, gdi+, libpng, stb_image
		  +Mac Support via GLUT - thanks Mumflr!
	2.09: Fix olc::Renderable Image load error - Thanks MaGetzUb & Zij-IT for finding and moaning about it
		  Fix file rejection in image loaders when using resource packs
		  Tidied Compiler defines per platform - Thanks slavka
		  +Pedant fixes, const correctness in parts
		  +DecalModes - Normal, Additive, Multiplicative blend modes
		  +Pixel Operators & Lerping
		  +Filtered Decals - If you hate pixels, then erase this file
		  +DrawStringProp(), GetTextSizeProp(), DrawStringPropDecal() - Draws non-monospaced font
	2.10: Fix PixelLerp() - oops my bad, lerped the wrong way :P
		  Fix "Shader" support for strings - thanks Megarev for crying about it
		  Fix GetTextSizeProp() - Height was just plain wrong...
		  +vec2d operator overloads (element wise *=, /=)
		  +vec2d comparison operators... :| yup... hmmmm...
		  +vec2d ceil(), floor(), min(), max() functions - surprising how often I do it manually
		  +DrawExplicitDecal(... uint32_t elements) - complete control over convex polygons and lines
		  +DrawPolygonDecal() - to keep Bispoo happy, required significant rewrite of EVERYTHING, but hey ho
		  +Complete rewrite of decal renderer
		  +OpenGL 3.3 Renderer (also supports Raspberry Pi)
		  +PGEX Break-In Hooks - with a push from Dandistine
		  +Wireframe Decal Mode - For debug overlays
	2.11: Made PGEX hooks optional - (provide true to super constructor)
	2.12: Fix for MinGW compiler non-compliance :( - why is its sdk structure different?? why???
	2.13: +GetFontSprite() - allows access to font data	 
	2.14: Fix WIN32 Definition reshuffle
		  Fix DrawPartialDecal() - messed up dimension during renderer experiment, didnt remove junk code, thanks Alexio
		  Fix? Strange error regarding GDI+ Image Loader not knowing about COM, SDK change?
	2.15: Big Reformat
		  +WASM Platform (via Emscripten) - Big Thanks to OLC Community - See Platform for details		  
		  +Sample Mode for Decals
		  +Made olc_ConfigureSystem() accessible
		  +Added OLC_----_CUSTOM_EX for externalised platforms, renderers and image loaders
		  =Refactored olc::Sprite pixel data store
		  -Deprecating LoadFromPGESprFile()
		  -Deprecating SaveToPGESprFile()
		  Fix Pixel -= operator (thanks Au Lit)
	2.16: FIX Emscripten JS formatting in VS IDE (thanks Moros)
		  +"Headless" Mode
		  +DrawLineDecal()
		  +Mouse Button Constants
		  +Move Constructor for olc::Renderable
		  +Polar/Cartesian conversion for v2d_generic
		  +DrawRotatedStringDecal()/DrawRotatedStringPropDecal() (thanks Oso-Grande/Sopadeoso (PR #209))
		  =Using olc::Renderable for layer surface
		  +Major Mac and GLUT Update (thanks Mumflr)
	2.17: +Clipping for DrawLine() functions
		  +Reintroduced sub-pixel decals
		  +Modified DrawPartialDecal() to quantise and correctly sample from tile atlasses
		  +olc::Sprite::GetPixel() - Clamp Mode
	2.18: +Option to not "dirty" layers with SetDrawTarget() - Thanks TerasKasi!
		  =Detection for Mac M1, fix for scroll wheel interrogation - Thanks ruarq!
	2.19: Textual Input(of)course Edition!
	      =Built in font is now olc::Renderable
		  +EnablePixelTransfer() - Gate if layer content transfers occur (speedup in decal only apps)
		  +TextEntryEnable() - Enables/Disables text entry mode
		  +TextEntryGetString() - Gets the current accumulated string in text entry mode
		  +TextEntryGetCursor() - Gets the current cursor position in text entry mode
		  +IsTextEntryEnabled() - Returns true if text entry mode is activated
		  +OnTextEntryComplete() - Override is called when user presses "ENTER" in text entry mode
		  +Potential for regional keyboard mappings - needs volunteers to do this
		  +ConsoleShow() - Opens built in command console
		  +ConsoleClear() - Clears built in command console output
		  +ConsoleOut() - Stream strings to command console output
		  +ConsoleCaptureStdOut() - Capture std::cout by redirecting to built-in console
		  +OnConsoleCommand() - Override is called when command is entered into built in console
	2.20: +DrawRectDecal() - Keeps OneSketchyGuy quiet
	      +GetScreenSize()
		  +olc::Sprite::Size() - returns size of sprite in vector format
	2.21: Emscripten Overhaul - Thanks Moros!
		  +DrawPolygonDecal() tint overload, can now tint a polygon accounting for vertex colours
		  +Multiplicative Pixel overload
		  +v2d_generic clamp()
		  +v2d_generic lerp()
		  +GetDroppedFiles() - returns files dropped onto engine window for that frame (MSW only)
		  +GetDroppedFilesPoint() - returns location of dropped files (MSW only)
		  +Exposed OpenGL33 Loader interface so the typedefs can be shared with PGEX & user
		  +Fix OGL33 DecalStructure types - wow, how did that one get missed?? lol
		  +FillTexturedTriangle() - Software rasterizes a textured, coloured, triangle
		  +FillTexturedPolygon() - Hijacks DecalStructure for configuration
		  +olc::vf2d arguments for Sprite::Sample() functions
	2.22: = Fix typo on dragged file buffers for unicode builds
	2.23: Fixed Emscripten host sizing errors - Thanks Moros
		  Fixed v2d_generic.clamp() function
	2.24: Fix FillTexturedTriangle() to remove const-ref
	2.25: +DrawPolygonDecal(pos, tex, w, col)
	2.26: Window Manipulation Update! (MS Windows only right now... need volunteers!)
		  +1:1 "Real Window" mode which follows resizing - Construct(...)
		  +SetWindowSize() - Sets Position/Size of window
		  +ShowWindowFrame() - Enables/Disables window furniture
		  +olc_UpdateWindowPos() - Break in to set position of window
		  +adv_ManualRenderEnable() - [ADVANCED] To be PGE or not be PGE...
		  +adv_HardwareClip() - [ADVANCED] Specify target clip region
		  +adv_FlushLayer() - [ADVANCED] Force layer update to buffer
		  +adv_FlushLayerDecals() - [ADVANCED] Force layer's decal render to buffer
		  +FillTriangleDecal() - Draws a triangle decal
		  +GradientTriangleDecal() - Draws a triangle decal with unique vertex colours
	2.27: Restructuring of connected files (post-jam clean up)
		  +Guarding olc::v_2d with less faff
		  +Updated olcUTIL_Geometry2D.h
		  +Updated olcUTIL_QuadTree.h
		  +Updated olcUTIL_Animate2D.h
		  +Updated olcUTIL_SplashScreen.h
		  +File Resolution for PGEtinker.com
	2.28: Brought olc::v_2d inline with other sources
		  
    !! Apple Platforms will not see these updates immediately - Sorry, I dont have a mac to test... !!
	!!   Volunteers willing to help appreciated, though PRs are manually integrated with credit     !!
*/
#pragma endregion

#pragma region hello_world_example
// O------------------------------------------------------------------------------O
// | Example "Hello World" Program (main.cpp)                                     |
// O------------------------------------------------------------------------------O
/*

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
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
		// Called once per frame, draws random coloured pixels
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
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
#pragma endregion

#ifndef OLC_PGE_DEF
#define OLC_PGE_DEF

#pragma region std_includes
// O------------------------------------------------------------------------------O
// | STANDARD INCLUDES                                                            |
// O------------------------------------------------------------------------------O
#include <cmath>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>
#include <functional>
#include <algorithm>
#include <array>
#include <cstring>
#pragma endregion

#define PGE_VER 228

// O------------------------------------------------------------------------------O
// | COMPILER CONFIGURATION ODDITIES                                              |
// O------------------------------------------------------------------------------O
#pragma region compiler_config
#define USE_EXPERIMENTAL_FS
#if defined(_WIN32)
	#if _MSC_VER >= 1920 && _MSVC_LANG >= 201703L
		#undef USE_EXPERIMENTAL_FS
	#endif
#endif
#if defined(__linux__) || defined(__MINGW32__) || defined(__EMSCRIPTEN__) || defined(__FreeBSD__) || defined(__APPLE__)
	#if __cplusplus >= 201703L
		#undef USE_EXPERIMENTAL_FS
	#endif
#endif

#if !defined(OLC_KEYBOARD_UK)
	#define OLC_KEYBOARD_UK
#endif


#if defined(USE_EXPERIMENTAL_FS) || defined(FORCE_EXPERIMENTAL_FS)
	// C++14
	#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
	#include <experimental/filesystem>
	namespace _gfs = std::experimental::filesystem::v1;
#else
	// C++17
	#include <filesystem>
	namespace _gfs = std::filesystem;
#endif

#if defined(UNICODE) || defined(_UNICODE)
	#define olcT(s) L##s
#else
	#define olcT(s) s
#endif

#define UNUSED(x) (void)(x)

// O------------------------------------------------------------------------------O
// | PLATFORM SELECTION CODE, Thanks slavka!                                      |
// O------------------------------------------------------------------------------O

#if defined(OLC_PGE_HEADLESS)
	#define OLC_PLATFORM_HEADLESS
	#define OLC_GFX_HEADLESS
	#if !defined(OLC_IMAGE_STB) && !defined(OLC_IMAGE_GDI) && !defined(OLC_IMAGE_LIBPNG)
		#define OLC_IMAGE_HEADLESS
	#endif
#endif

// Platform
#if !defined(OLC_PLATFORM_WINAPI) && !defined(OLC_PLATFORM_X11) && !defined(OLC_PLATFORM_GLUT) && !defined(OLC_PLATFORM_EMSCRIPTEN) && !defined(OLC_PLATFORM_HEADLESS)
	#if !defined(OLC_PLATFORM_CUSTOM_EX)
		#if defined(_WIN32)
			#define OLC_PLATFORM_WINAPI
		#endif
		#if defined(__linux__) || defined(__FreeBSD__)
			#define OLC_PLATFORM_X11
		#endif
		#if defined(__APPLE__)
			#define GL_SILENCE_DEPRECATION
			#define OLC_PLATFORM_GLUT
		#endif
		#if defined(__EMSCRIPTEN__)
			#define OLC_PLATFORM_EMSCRIPTEN
		#endif
	#endif
#endif

// Start Situation
#if defined(OLC_PLATFORM_GLUT) || defined(OLC_PLATFORM_EMSCRIPTEN)
	#define PGE_USE_CUSTOM_START
#endif



// Renderer
#if !defined(OLC_GFX_OPENGL10) && !defined(OLC_GFX_OPENGL33) && !defined(OLC_GFX_DIRECTX10) && !defined(OLC_GFX_HEADLESS)
	#if !defined(OLC_GFX_CUSTOM_EX)
		#if defined(OLC_PLATFORM_EMSCRIPTEN)
			#define OLC_GFX_OPENGL33
		#else
			#define OLC_GFX_OPENGL10
		#endif
	#endif
#endif

// Image loader
#if !defined(OLC_IMAGE_STB) && !defined(OLC_IMAGE_GDI) && !defined(OLC_IMAGE_LIBPNG) && !defined(OLC_IMAGE_HEADLESS)
	#if !defined(OLC_IMAGE_CUSTOM_EX)
		#if defined(_WIN32)
			#define	OLC_IMAGE_GDI
		#endif
		#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__EMSCRIPTEN__)
			#define	OLC_IMAGE_LIBPNG
		#endif
	#endif
#endif

// File resolver for runtime FS access of emscripten builds
#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#define FILE_RESOLVE(url, file) emscripten_wget(url, file); emscripten_sleep(0)
#else
#define FILE_RESOLVE(url, file)
#endif

// O------------------------------------------------------------------------------O
// | PLATFORM-SPECIFIC DEPENDENCIES                                               |
// O------------------------------------------------------------------------------O
#if !defined(OLC_PGE_HEADLESS)
#if defined(OLC_PLATFORM_WINAPI)	
	#define _WINSOCKAPI_ // Thanks Cornchipss
		#if !defined(VC_EXTRALEAN)
		#define VC_EXTRALEAN
	#endif
	#if !defined(NOMINMAX)
		#define NOMINMAX
	#endif

	// In Code::Blocks
	#if !defined(_WIN32_WINNT)
		#ifdef HAVE_MSMF
			#define _WIN32_WINNT 0x0600 // Windows Vista
		#else
			#define _WIN32_WINNT 0x0500 // Windows 2000
		#endif
	#endif

	#include <windows.h>
	#undef _WINSOCKAPI_
#endif

#if defined(OLC_PLATFORM_X11)
	namespace X11
	{
		#include <X11/X.h>
		#include <X11/Xlib.h>
	}
#endif

#if defined(OLC_PLATFORM_GLUT)
	#if defined(__linux__)
		#include <GL/glut.h>
		#include <GL/freeglut_ext.h>
	#endif
	#if defined(__APPLE__)
		#include <GLUT/glut.h>
	#include <objc/message.h>
	#include <objc/NSObjCRuntime.h>
	#endif
#endif
#endif

#if defined(OLC_PGE_HEADLESS)
#if defined max
#undef max
#endif
#if defined min
#undef min
#endif
#endif
#pragma endregion



#if !defined(OLC_VECTOR2D_DEFINED)
	namespace olc
	{
		/*
			A complete 2D geometric vector structure, with a variety
			of useful utility functions and operator overloads
		*/
		template<class T>
		struct v_2d
		{
			static_assert(std::is_arithmetic<T>::value, "olc::v_2d<type> must be numeric");

			// x-axis component
			T x = 0;
			// y-axis component
			T y = 0;

			// Default constructor
			inline constexpr v_2d() = default;

			// Specific constructor
			inline constexpr v_2d(T _x, T _y) : x(_x), y(_y)
			{}

			// Copy constructor
			inline constexpr v_2d(const v_2d& v) = default;

			// Assignment operator
			inline constexpr v_2d& operator=(const v_2d& v) = default;


			// Returns rectangular area of vector
			inline constexpr auto area() const
			{
				return x * y;
			}

			// Returns magnitude of vector
			inline auto mag() const
			{
				return std::sqrt(x * x + y * y);
			}

			// Returns magnitude squared of vector (useful for fast comparisons)
			inline constexpr T mag2() const
			{
				return x * x + y * y;
			}

			// Returns normalised version of vector
			inline v_2d norm() const
			{
				auto r = 1 / mag();
				return v_2d(x * r, y * r);
			}

			// Returns vector at 90 degrees to this one
			inline constexpr v_2d perp() const
			{
				return v_2d(-y, x);
			}

			// Rounds both components down
			inline constexpr v_2d floor() const
			{
				return v_2d(std::floor(x), std::floor(y));
			}

			// Rounds both components up
			inline constexpr v_2d ceil() const
			{
				return v_2d(std::ceil(x), std::ceil(y));
			}

			// Returns 'element-wise' max of this and another vector
			inline constexpr v_2d max(const v_2d& v) const
			{
				return v_2d(std::max(x, v.x), std::max(y, v.y));
			}

			// Returns 'element-wise' min of this and another vector
			inline constexpr v_2d min(const v_2d& v) const
			{
				return v_2d(std::min(x, v.x), std::min(y, v.y));
			}

			// Calculates scalar dot product between this and another vector
			inline constexpr auto dot(const v_2d& rhs) const
			{
				return this->x * rhs.x + this->y * rhs.y;
			}

			// Calculates 'scalar' cross product between this and another vector (useful for winding orders)
			inline constexpr auto cross(const v_2d& rhs) const
			{
				return this->x * rhs.y - this->y * rhs.x;
			}

			// Treat this as polar coordinate (R, Theta), return cartesian equivalent (X, Y)
			inline constexpr v_2d cart() const
			{
				return v_2d(std::cos(y) * x, std::sin(y) * x);
			}

			// Treat this as cartesian coordinate (X, Y), return polar equivalent (R, Theta)
			inline constexpr v_2d polar() const
			{
				return v_2d(mag(), std::atan2(y, x));
			}

			// Clamp the components of this vector in between the 'element-wise' minimum and maximum of 2 other vectors
			inline constexpr v_2d clamp(const v_2d& v1, const v_2d& v2) const
			{
				return this->max(v1).min(v2);
			}

			// Linearly interpolate between this vector, and another vector, given normalised parameter 't'
			inline constexpr v_2d lerp(const v_2d& v1, const double t) const
			{
				return (*this) * (T(1.0 - t)) + (v1 * T(t));
			}

			// Compare if this vector is numerically equal to another
			inline constexpr bool operator == (const v_2d& rhs) const
			{
				return (this->x == rhs.x && this->y == rhs.y);
			}

			// Compare if this vector is not numerically equal to another
			inline constexpr bool operator != (const v_2d& rhs) const
			{
				return (this->x != rhs.x || this->y != rhs.y);
			}

			// Return this vector as a std::string, of the form "(x,y)"
			inline std::string str() const
			{
				return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + ")";
			}

			// Assuming this vector is incident, given a normal, return the reflection
			inline constexpr v_2d reflect(const v_2d& n) const
			{
				return (*this) - 2.0 * (this->dot(n) * n);
			}

			// Allow 'casting' from other v_2d types
			template<class F>
			inline constexpr operator v_2d<F>() const
			{
				return { static_cast<F>(this->x), static_cast<F>(this->y) };
			}
		};

		// Multiplication operator overloads between vectors and scalars, and vectors and vectors
		template<class TL, class TR>
		inline constexpr auto operator * (const TL& lhs, const v_2d<TR>& rhs)
		{
			return v_2d(lhs * rhs.x, lhs * rhs.y);
		}

		template<class TL, class TR>
		inline constexpr auto operator * (const v_2d<TL>& lhs, const TR& rhs)
		{
			return v_2d(lhs.x * rhs, lhs.y * rhs);
		}

		template<class TL, class TR>
		inline constexpr auto operator * (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
		{
			return v_2d(lhs.x * rhs.x, lhs.y * rhs.y);
		}

		template<class TL, class TR>
		inline constexpr auto operator *= (v_2d<TL>& lhs, const TR& rhs)
		{
			lhs = lhs * rhs;
			return lhs;
		}

		// Division operator overloads between vectors and scalars, and vectors and vectors
		template<class TL, class TR>
		inline constexpr auto operator / (const TL& lhs, const v_2d<TR>& rhs)
		{
			return v_2d(lhs / rhs.x, lhs / rhs.y);
		}

		template<class TL, class TR>
		inline constexpr auto operator / (const v_2d<TL>& lhs, const TR& rhs)
		{
			return v_2d(lhs.x / rhs, lhs.y / rhs);
		}

		template<class TL, class TR>
		inline constexpr auto operator / (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
		{
			return v_2d(lhs.x / rhs.x, lhs.y / rhs.y);
		}

		template<class TL, class TR>
		inline constexpr auto operator /= (v_2d<TL>& lhs, const TR& rhs)
		{
			lhs = lhs / rhs;
			return lhs;
		}

		// Unary Addition operator (pointless but i like the platinum trophies)
		template<class T>
		inline constexpr auto operator + (const v_2d<T>& lhs)
		{
			return v_2d(+lhs.x, +lhs.y);
		}

		// Addition operator overloads between vectors and scalars, and vectors and vectors
		template<class TL, class TR>
		inline constexpr auto operator + (const TL& lhs, const v_2d<TR>& rhs)
		{
			return v_2d(lhs + rhs.x, lhs + rhs.y);
		}

		template<class TL, class TR>
		inline constexpr auto operator + (const v_2d<TL>& lhs, const TR& rhs)
		{
			return v_2d(lhs.x + rhs, lhs.y + rhs);
		}

		template<class TL, class TR>
		inline constexpr auto operator + (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
		{
			return v_2d(lhs.x + rhs.x, lhs.y + rhs.y);
		}

		template<class TL, class TR>
		inline constexpr auto operator += (v_2d<TL>& lhs, const TR& rhs)
		{
			lhs = lhs + rhs;
			return lhs;
		}

		template<class TL, class TR>
		inline constexpr auto operator += (v_2d<TL>& lhs, const v_2d<TR>& rhs)
		{
			lhs = lhs + rhs;
			return lhs;
		}

		// Unary negation operator overoad for inverting a vector
		template<class T>
		inline constexpr auto operator - (const v_2d<T>& lhs)
		{
			return v_2d(-lhs.x, -lhs.y);
		}

		// Subtraction operator overloads between vectors and scalars, and vectors and vectors
		template<class TL, class TR>
		inline constexpr auto operator - (const TL& lhs, const v_2d<TR>& rhs)
		{
			return v_2d(lhs - rhs.x, lhs - rhs.y);
		}

		template<class TL, class TR>
		inline constexpr auto operator - (const v_2d<TL>& lhs, const TR& rhs)
		{
			return v_2d(lhs.x - rhs, lhs.y - rhs);
		}

		template<class TL, class TR>
		inline constexpr auto operator - (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
		{
			return v_2d(lhs.x - rhs.x, lhs.y - rhs.y);
		}

		template<class TL, class TR>
		inline constexpr auto operator -= (v_2d<TL>& lhs, const TR& rhs)
		{
			lhs = lhs - rhs;
			return lhs;
		}

		// Greater/Less-Than Operator overloads - mathematically useless, but handy for "sorted" container storage
		template<class TL, class TR>
		inline constexpr bool operator < (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
		{
			return (lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x);
		}

		template<class TL, class TR>
		inline constexpr bool operator > (const v_2d<TL>& lhs, const v_2d<TR>& rhs)
		{
			return (lhs.y > rhs.y) || (lhs.y == rhs.y && lhs.x > rhs.x);
		}

		// Allow olc::v_2d to play nicely with std::cout
		template<class T>
		inline constexpr std::ostream& operator << (std::ostream& os, const v_2d<T>& rhs)
		{
			os << rhs.str();
			return os;
		}

		// Convenient types ready-to-go
		typedef v_2d<int32_t> vi2d;
		typedef v_2d<uint32_t> vu2d;
		typedef v_2d<float> vf2d;
		typedef v_2d<double> vd2d;
	}
#define OLC_VECTOR2D_DEFINED 1
#endif



// O------------------------------------------------------------------------------O
// | olcPixelGameEngine INTERFACE DECLARATION                                     |
// O------------------------------------------------------------------------------O
#pragma region pge_declaration
namespace olc
{
	class PixelGameEngine;
	class Sprite;

	// Pixel Game Engine Advanced Configuration
	constexpr uint8_t  nMouseButtons = 5;
	constexpr uint8_t  nDefaultAlpha = 0xFF;
	constexpr uint32_t nDefaultPixel = uint32_t(nDefaultAlpha << 24);
	constexpr uint8_t  nTabSizeInSpaces = 4;
	constexpr size_t OLC_MAX_VERTS = 128;
	enum rcode { FAIL = 0, OK = 1, NO_FILE = -1 };

	// O------------------------------------------------------------------------------O
	// | olc::Pixel - Represents a 32-Bit RGBA colour                                 |
	// O------------------------------------------------------------------------------O
#if !defined(OLC_IGNORE_PIXEL)
	struct Pixel
	{
		union
		{
			uint32_t n = nDefaultPixel;
			struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
		};

		enum Mode { NORMAL, MASK, ALPHA, CUSTOM };

		Pixel();
		Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = nDefaultAlpha);
		Pixel(uint32_t p);
		Pixel& operator = (const Pixel& v) = default;
		bool   operator ==(const Pixel& p) const;
		bool   operator !=(const Pixel& p) const;
		Pixel  operator * (const float i) const;
		Pixel  operator / (const float i) const;
		Pixel& operator *=(const float i);
		Pixel& operator /=(const float i);
		Pixel  operator + (const Pixel& p) const;
		Pixel  operator - (const Pixel& p) const;
		Pixel& operator +=(const Pixel& p);
		Pixel& operator -=(const Pixel& p);
		Pixel  operator * (const Pixel& p) const;
		Pixel& operator *=(const Pixel& p);
		Pixel  inv() const;
	};

	Pixel PixelF(float red, float green, float blue, float alpha = 1.0f);
	Pixel PixelLerp(const olc::Pixel& p1, const olc::Pixel& p2, float t);


	// O------------------------------------------------------------------------------O
	// | USEFUL CONSTANTS                                                             |
	// O------------------------------------------------------------------------------O
	static const Pixel
		GREY(192, 192, 192), DARK_GREY(128, 128, 128), VERY_DARK_GREY(64, 64, 64),
		RED(255, 0, 0), DARK_RED(128, 0, 0), VERY_DARK_RED(64, 0, 0),
		YELLOW(255, 255, 0), DARK_YELLOW(128, 128, 0), VERY_DARK_YELLOW(64, 64, 0),
		GREEN(0, 255, 0), DARK_GREEN(0, 128, 0), VERY_DARK_GREEN(0, 64, 0),
		CYAN(0, 255, 255), DARK_CYAN(0, 128, 128), VERY_DARK_CYAN(0, 64, 64),
		BLUE(0, 0, 255), DARK_BLUE(0, 0, 128), VERY_DARK_BLUE(0, 0, 64),
		MAGENTA(255, 0, 255), DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64),
		WHITE(255, 255, 255), BLACK(0, 0, 0), BLANK(0, 0, 0, 0);
#endif
	// Thanks to scripticuk and others for updating the key maps
	// NOTE: The GLUT platform will need updating, open to contributions ;)
	enum Key
	{
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
		NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL, PERIOD,
		EQUALS, COMMA, MINUS,
		OEM_1, OEM_2, OEM_3, OEM_4, OEM_5, OEM_6, OEM_7, OEM_8,
		CAPS_LOCK, ENUM_END
	};

	namespace Mouse
	{
		static constexpr int32_t LEFT = 0;
		static constexpr int32_t RIGHT = 1;
		static constexpr int32_t MIDDLE = 2;
	};

	// O------------------------------------------------------------------------------O
	// | olc::HWButton - Represents the state of a hardware button (mouse/key/joy)    |
	// O------------------------------------------------------------------------------O
	struct HWButton
	{
		bool bPressed = false;	// Set once during the frame the event occurs
		bool bReleased = false;	// Set once during the frame the event occurs
		bool bHeld = false;		// Set true for all frames between pressed and released events
	};


	// O------------------------------------------------------------------------------O
	// | olc::ResourcePack - A virtual scrambled filesystem to pack your assets into  |
	// O------------------------------------------------------------------------------O
	struct ResourceBuffer : public std::streambuf
	{
		ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size);
		std::vector<char> vMemory;
	};

	class ResourcePack : public std::streambuf
	{
	public:
		ResourcePack();
		~ResourcePack();
		bool AddFile(const std::string& sFile);
		bool LoadPack(const std::string& sFile, const std::string& sKey);
		bool SavePack(const std::string& sFile, const std::string& sKey);
		ResourceBuffer GetFileBuffer(const std::string& sFile);
		bool Loaded();
	private:
		struct sResourceFile { uint32_t nSize; uint32_t nOffset; };
		std::map<std::string, sResourceFile> mapFiles;
		std::ifstream baseFile;
		std::vector<char> scramble(const std::vector<char>& data, const std::string& key);
		std::string makeposix(const std::string& path);
	};


	class ImageLoader
	{
	public:
		ImageLoader() = default;
		virtual ~ImageLoader() = default;
		virtual olc::rcode LoadImageResource(olc::Sprite* spr, const std::string& sImageFile, olc::ResourcePack* pack) = 0;
		virtual olc::rcode SaveImageResource(olc::Sprite* spr, const std::string& sImageFile) = 0;
	};


	// O------------------------------------------------------------------------------O
	// | olc::Sprite - An image represented by a 2D array of olc::Pixel               |
	// O------------------------------------------------------------------------------O
	class Sprite
	{
	public:
		Sprite();
		Sprite(const std::string& sImageFile, olc::ResourcePack* pack = nullptr);
		Sprite(int32_t w, int32_t h);
		Sprite(const olc::Sprite&) = delete;
		~Sprite();

	public:
		olc::rcode LoadFromFile(const std::string& sImageFile, olc::ResourcePack* pack = nullptr);

	public:
		int32_t width = 0;
		int32_t height = 0;
		enum Mode { NORMAL, PERIODIC, CLAMP };
		enum Flip { NONE = 0, HORIZ = 1, VERT = 2 };

	public:
		void SetSampleMode(olc::Sprite::Mode mode = olc::Sprite::Mode::NORMAL);
		Pixel GetPixel(int32_t x, int32_t y) const;
		bool  SetPixel(int32_t x, int32_t y, Pixel p);
		Pixel GetPixel(const olc::vi2d& a) const;
		bool  SetPixel(const olc::vi2d& a, Pixel p);
		Pixel Sample(float x, float y) const;
		Pixel Sample(const olc::vf2d& uv) const;
		Pixel SampleBL(float u, float v) const;
		Pixel SampleBL(const olc::vf2d& uv) const;
		Pixel* GetData();
		olc::Sprite* Duplicate();
		olc::Sprite* Duplicate(const olc::vi2d& vPos, const olc::vi2d& vSize);
		olc::vi2d Size() const;
		void SetSize(int32_t w, int32_t h);
		std::vector<olc::Pixel> pColData;
		Mode modeSample = Mode::NORMAL;

		static std::unique_ptr<olc::ImageLoader> loader;
	};

	// O------------------------------------------------------------------------------O
	// | olc::Decal - A GPU resident storage of an olc::Sprite                        |
	// O------------------------------------------------------------------------------O
	class Decal
	{
	public:
		Decal(olc::Sprite* spr, bool filter = false, bool clamp = true);
		Decal(const uint32_t nExistingTextureResource, olc::Sprite* spr);
		virtual ~Decal();
		void Update();
		void UpdateSprite();

	public: // But dont touch
		int32_t id = -1;
		olc::Sprite* sprite = nullptr;
		olc::vf2d vUVScale = { 1.0f, 1.0f };
	};

	enum class DecalMode
	{
		NORMAL,
		ADDITIVE,
		MULTIPLICATIVE,
		STENCIL,
		ILLUMINATE,
		WIREFRAME,
	};

	enum class DecalStructure
	{
		LINE,
		FAN,
		STRIP,
		LIST
	};

	// O------------------------------------------------------------------------------O
	// | olc::Renderable - Convenience class to keep a sprite and decal together      |
	// O------------------------------------------------------------------------------O
	class Renderable
	{
	public:
		Renderable() = default;		
		Renderable(Renderable&& r) = default; 
		Renderable& operator=(Renderable&& r) = default;
		Renderable(const Renderable&) = delete;
		olc::rcode Load(const std::string& sFile, ResourcePack* pack = nullptr, bool filter = false, bool clamp = true);
		void Create(uint32_t width, uint32_t height, bool filter = false, bool clamp = true);
		olc::Decal* Decal() const;
		olc::Sprite* Sprite() const;


	private:
		std::unique_ptr<olc::Sprite> pSprite = nullptr;
		std::unique_ptr<olc::Decal> pDecal = nullptr;
	};


	// O------------------------------------------------------------------------------O
	// | Auxilliary components internal to engine                                     |
	// O------------------------------------------------------------------------------O

	struct DecalInstance
	{
		olc::Decal* decal = nullptr;
		std::vector<olc::vf2d> pos;
		std::vector<olc::vf2d> uv;
		std::vector<float> w;
		std::vector<float> z;
		std::vector<olc::Pixel> tint;
		olc::DecalMode mode = olc::DecalMode::NORMAL;
		olc::DecalStructure structure = olc::DecalStructure::FAN;
		uint32_t points = 0;
		bool depth = false;
	};

	struct LayerDesc
	{
		olc::vf2d vOffset = { 0, 0 };
		olc::vf2d vScale = { 1, 1 };
		bool bShow = false;
		bool bUpdate = false;
		olc::Renderable pDrawTarget;
		uint32_t nResID = 0;
		std::vector<DecalInstance> vecDecalInstance;
		olc::Pixel tint = olc::WHITE;
		std::function<void()> funcHook = nullptr;
	};

	class Renderer
	{
	public:
		virtual ~Renderer() = default;
		virtual void       PrepareDevice() = 0;
		virtual olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) = 0;
		virtual olc::rcode DestroyDevice() = 0;
		virtual void       DisplayFrame() = 0;
		virtual void       PrepareDrawing() = 0;
		virtual void	   SetDecalMode(const olc::DecalMode& mode) = 0;
		virtual void       DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) = 0;
		virtual void       DrawDecal(const olc::DecalInstance& decal) = 0;
		virtual uint32_t   CreateTexture(const uint32_t width, const uint32_t height, const bool filtered = false, const bool clamp = true) = 0;
		virtual void       UpdateTexture(uint32_t id, olc::Sprite* spr) = 0;
		virtual void       ReadTexture(uint32_t id, olc::Sprite* spr) = 0;
		virtual uint32_t   DeleteTexture(const uint32_t id) = 0;
		virtual void       ApplyTexture(uint32_t id) = 0;
		virtual void       UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) = 0;
		virtual void       ClearBuffer(olc::Pixel p, bool bDepth) = 0;
		static olc::PixelGameEngine* ptrPGE;
	};

	class Platform
	{
	public:
		virtual ~Platform() = default;
		virtual olc::rcode ApplicationStartUp() = 0;
		virtual olc::rcode ApplicationCleanUp() = 0;
		virtual olc::rcode ThreadStartUp() = 0;
		virtual olc::rcode ThreadCleanUp() = 0;
		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) = 0;
		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) = 0;
		virtual olc::rcode SetWindowTitle(const std::string& s) = 0;
		virtual olc::rcode ShowWindowFrame(const bool bShowFrame = true) = 0;
		virtual olc::rcode SetWindowSize(const olc::vi2d& vWindowPos, const olc::vi2d& vWindowSize) = 0;
		virtual olc::rcode StartSystemEventLoop() = 0;
		virtual olc::rcode HandleSystemEvent() = 0;
		static olc::PixelGameEngine* ptrPGE;
	};

	class PGEX;

	// The Static Twins (plus one)
	static std::unique_ptr<Renderer> renderer;
	static std::unique_ptr<Platform> platform;
	static std::map<size_t, uint8_t> mapKeys;

	// O------------------------------------------------------------------------------O
	// | olc::PixelGameEngine - The main BASE class for your application              |
	// O------------------------------------------------------------------------------O
	class PixelGameEngine
	{
	public:
		PixelGameEngine();
		virtual ~PixelGameEngine();
	public:
		olc::rcode Construct(int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h,
			bool full_screen = false, bool vsync = false, bool cohesion = false, bool realwindow = false);
		olc::rcode Start();

	public: // User Override Interfaces
		// Called once on application startup, use to load your resources
		virtual bool OnUserCreate();
		// Called every frame, and provides you with a time per frame value
		virtual bool OnUserUpdate(float fElapsedTime);
		// Called once on application termination, so you can be one clean coder
		virtual bool OnUserDestroy();

		// Called when a text entry is confirmed with "enter" key
		virtual void OnTextEntryComplete(const std::string& sText);
		// Called when a console command is executed
		virtual bool OnConsoleCommand(const std::string& sCommand);


	public: // Hardware Interfaces
		// Returns true if window is currently in focus
		bool IsFocused() const;
		// Get the state of a specific keyboard button
		HWButton GetKey(Key k) const;
		// Get the state of a specific mouse button
		HWButton GetMouse(uint32_t b) const;
		// Get Mouse X coordinate in "pixel" space
		int32_t GetMouseX() const;
		// Get Mouse Y coordinate in "pixel" space
		int32_t GetMouseY() const;
		// Get Mouse Wheel Delta
		int32_t GetMouseWheel() const;
		// Get the mouse in window space
		const olc::vi2d& GetWindowMouse() const;
		// Gets the mouse as a vector to keep Tarriest happy
		const olc::vi2d& GetMousePos() const;

		static const std::map<size_t, uint8_t>& GetKeyMap() { return mapKeys; }

		// Muck about with the GUI
		olc::rcode SetWindowSize(const olc::vi2d& vPos, const olc::vi2d& vSize);
		olc::rcode ShowWindowFrame(const bool bShowFrame);

	public: // Utility
		// Returns the width of the screen in "pixels"
		int32_t ScreenWidth() const;
		// Returns the height of the screen in "pixels"
		int32_t ScreenHeight() const;
		// Returns the width of the currently selected drawing target in "pixels"
		int32_t GetDrawTargetWidth() const;
		// Returns the height of the currently selected drawing target in "pixels"
		int32_t GetDrawTargetHeight() const;
		// Returns the currently active draw target
		olc::Sprite* GetDrawTarget() const;
		// Resize the primary screen sprite
		void SetScreenSize(int w, int h);
		// Specify which Sprite should be the target of drawing functions, use nullptr
		// to specify the primary screen
		void SetDrawTarget(Sprite* target);
		// Gets the current Frames Per Second
		uint32_t GetFPS() const;
		// Gets last update of elapsed time
		float GetElapsedTime() const;
		// Gets Actual Window size
		const olc::vi2d& GetWindowSize() const;
		// Gets Actual Window position
		const olc::vi2d& GetWindowPos() const;
		// Gets pixel scale
		const olc::vi2d& GetPixelSize() const;
		// Gets actual pixel scale
		const olc::vi2d& GetScreenPixelSize() const;
		// Gets "screen" size
		const olc::vi2d& GetScreenSize() const;
		// Gets any files dropped this frame
		const std::vector<std::string>& GetDroppedFiles() const;
		const olc::vi2d& GetDroppedFilesPoint() const;

	public: // CONFIGURATION ROUTINES
		// Layer targeting functions
		void SetDrawTarget(uint8_t layer, bool bDirty = true);
		void EnableLayer(uint8_t layer, bool b);
		void SetLayerOffset(uint8_t layer, const olc::vf2d& offset);
		void SetLayerOffset(uint8_t layer, float x, float y);
		void SetLayerScale(uint8_t layer, const olc::vf2d& scale);
		void SetLayerScale(uint8_t layer, float x, float y);
		void SetLayerTint(uint8_t layer, const olc::Pixel& tint);
		void SetLayerCustomRenderFunction(uint8_t layer, std::function<void()> f);

		std::vector<LayerDesc>& GetLayers();
		uint32_t CreateLayer();

		// Change the pixel mode for different optimisations
		// olc::Pixel::NORMAL = No transparency
		// olc::Pixel::MASK   = Transparent if alpha is < 255
		// olc::Pixel::ALPHA  = Full transparency
		void SetPixelMode(Pixel::Mode m);
		Pixel::Mode GetPixelMode();
		// Use a custom blend function
		void SetPixelMode(std::function<olc::Pixel(const int x, const int y, const olc::Pixel& pSource, const olc::Pixel& pDest)> pixelMode);
		// Change the blend factor from between 0.0f to 1.0f;
		void SetPixelBlend(float fBlend);

		// [ADVANCED] For those that really want to dick about with PGE :P
		// Note: Normal use of olc::PGE does not require you use these functions
		void adv_ManualRenderEnable(const bool bEnable);
		void adv_HardwareClip(const bool bScale, const olc::vi2d& viewPos, const olc::vi2d& viewSize, const bool bClear = false);
		void adv_FlushLayer(const size_t nLayerID);
		void adv_FlushLayerDecals(const size_t nLayerID);

	public: // DRAWING ROUTINES
		// Draws a single Pixel
		virtual bool Draw(int32_t x, int32_t y, Pixel p = olc::WHITE);
		bool Draw(const olc::vi2d& pos, Pixel p = olc::WHITE);
		// Draws a line from (x1,y1) to (x2,y2)
		void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF);
		void DrawLine(const olc::vi2d& pos1, const olc::vi2d& pos2, Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF);
		// Draws a circle located at (x,y) with radius
		void DrawCircle(int32_t x, int32_t y, int32_t radius, Pixel p = olc::WHITE, uint8_t mask = 0xFF);
		void DrawCircle(const olc::vi2d& pos, int32_t radius, Pixel p = olc::WHITE, uint8_t mask = 0xFF);
		// Fills a circle located at (x,y) with radius
		void FillCircle(int32_t x, int32_t y, int32_t radius, Pixel p = olc::WHITE);
		void FillCircle(const olc::vi2d& pos, int32_t radius, Pixel p = olc::WHITE);
		// Draws a rectangle at (x,y) to (x+w,y+h)
		void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = olc::WHITE);
		void DrawRect(const olc::vi2d& pos, const olc::vi2d& size, Pixel p = olc::WHITE);
		// Fills a rectangle at (x,y) to (x+w,y+h)
		void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = olc::WHITE);
		void FillRect(const olc::vi2d& pos, const olc::vi2d& size, Pixel p = olc::WHITE);
		// Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
		void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = olc::WHITE);
		void DrawTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, Pixel p = olc::WHITE);
		// Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
		void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = olc::WHITE);
		void FillTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, Pixel p = olc::WHITE);
		// Fill a textured and coloured triangle
		void FillTexturedTriangle(std::vector<olc::vf2d> vPoints, std::vector<olc::vf2d> vTex, std::vector<olc::Pixel> vColour, olc::Sprite* sprTex);
		void FillTexturedPolygon(const std::vector<olc::vf2d>& vPoints, const std::vector<olc::vf2d>& vTex, const std::vector<olc::Pixel>& vColour, olc::Sprite* sprTex, olc::DecalStructure structure = olc::DecalStructure::LIST);
		// Draws an entire sprite at location (x,y)
		void DrawSprite(int32_t x, int32_t y, Sprite* sprite, uint32_t scale = 1, uint8_t flip = olc::Sprite::NONE);
		void DrawSprite(const olc::vi2d& pos, Sprite* sprite, uint32_t scale = 1, uint8_t flip = olc::Sprite::NONE);
		// Draws an area of a sprite at location (x,y), where the
		// selected area is (ox,oy) to (ox+w,oy+h)
		void DrawPartialSprite(int32_t x, int32_t y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale = 1, uint8_t flip = olc::Sprite::NONE);
		void DrawPartialSprite(const olc::vi2d& pos, Sprite* sprite, const olc::vi2d& sourcepos, const olc::vi2d& size, uint32_t scale = 1, uint8_t flip = olc::Sprite::NONE);
		// Draws a single line of text - traditional monospaced
		void DrawString(int32_t x, int32_t y, const std::string& sText, Pixel col = olc::WHITE, uint32_t scale = 1);
		void DrawString(const olc::vi2d& pos, const std::string& sText, Pixel col = olc::WHITE, uint32_t scale = 1);
		olc::vi2d GetTextSize(const std::string& s);
		// Draws a single line of text - non-monospaced
		void DrawStringProp(int32_t x, int32_t y, const std::string& sText, Pixel col = olc::WHITE, uint32_t scale = 1);
		void DrawStringProp(const olc::vi2d& pos, const std::string& sText, Pixel col = olc::WHITE, uint32_t scale = 1);
		olc::vi2d GetTextSizeProp(const std::string& s);

		// Decal Quad functions
		void SetDecalMode(const olc::DecalMode& mode);
		void SetDecalStructure(const olc::DecalStructure& structure);
		// Draws a whole decal, with optional scale and tinting
		void DrawDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE);
		// Draws a region of a decal, with optional scale and tinting
		void DrawPartialDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialDecal(const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		// Draws fully user controlled 4 vertices, pos(pixels), uv(pixels), colours
		void DrawExplicitDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d* uv, const olc::Pixel* col, uint32_t elements = 4);
		// Draws a decal with 4 arbitrary points, warping the texture to look "correct"
		void DrawWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::Pixel& tint = olc::WHITE);
		void DrawWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::Pixel& tint = olc::WHITE);
		void DrawWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::Pixel& tint = olc::WHITE);
		// As above, but you can specify a region of a decal source sprite
		void DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		// Draws a decal rotated to specified angle, wit point of rotation offset
		void DrawRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center = { 0.0f, 0.0f }, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE);
		// Draws a multiline string as a decal, with tiniting and scaling
		void DrawStringDecal(const olc::vf2d& pos, const std::string& sText, const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
		void DrawStringPropDecal(const olc::vf2d& pos, const std::string& sText, const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
		// Draws a single shaded filled rectangle as a decal
		void DrawRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col = olc::WHITE);
		void FillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col = olc::WHITE);
		// Draws a corner shaded rectangle as a decal
		void GradientFillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel colTL, const olc::Pixel colBL, const olc::Pixel colBR, const olc::Pixel colTR);		
		// Draws a single shaded filled triangle as a decal
		void FillTriangleDecal(const olc::vf2d& p0, const olc::vf2d& p1, const olc::vf2d& p2, const olc::Pixel col = olc::WHITE);
		// Draws a corner shaded triangle as a decal
		void GradientTriangleDecal(const olc::vf2d& p0, const olc::vf2d& p1, const olc::vf2d& p2, const olc::Pixel c0, const olc::Pixel c1, const olc::Pixel c2);
		// Draws an arbitrary convex textured polygon using GPU
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const olc::Pixel tint = olc::WHITE);
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<float>& depth, const std::vector<olc::vf2d>& uv, const olc::Pixel tint = olc::WHITE);
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& tint);
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& colours, const olc::Pixel tint);
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<float>& depth, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& colours, const olc::Pixel tint);

		// Draws a line in Decal Space
		void DrawLineDecal(const olc::vf2d& pos1, const olc::vf2d& pos2, Pixel p = olc::WHITE);
		void DrawRotatedStringDecal(const olc::vf2d& pos, const std::string& sText, const float fAngle, const olc::vf2d& center = { 0.0f, 0.0f }, const olc::Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
		void DrawRotatedStringPropDecal(const olc::vf2d& pos, const std::string& sText, const float fAngle, const olc::vf2d& center = { 0.0f, 0.0f }, const olc::Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f });
		// Clears entire draw target to Pixel
		void Clear(Pixel p);
		// Clears the rendering back buffer
		void ClearBuffer(Pixel p, bool bDepth = true);
		// Returns the font image
		olc::Sprite* GetFontSprite();

		// Clip a line segment to visible area
		bool ClipLineToScreen(olc::vi2d& in_p1, olc::vi2d& in_p2);

		// Dont allow PGE to mark layers as dirty, so pixel graphics don't update
		void EnablePixelTransfer(const bool bEnable = true);

		// Command Console Routines
		void ConsoleShow(const olc::Key &keyExit, bool bSuspendTime = true);
		bool IsConsoleShowing() const;
		void ConsoleClear();
		std::stringstream& ConsoleOut();
		void ConsoleCaptureStdOut(const bool bCapture);

		// Text Entry Routines
		void TextEntryEnable(const bool bEnable, const std::string& sText = "");
		std::string TextEntryGetString() const;
		int32_t TextEntryGetCursor() const;
		bool IsTextEntryEnabled() const;



	private:
		void UpdateTextEntry();
		void UpdateConsole();

	public:

		// Experimental Lightweight 3D Routines ================
#ifdef OLC_ENABLE_EXPERIMENTAL
		// Set Manual View Matrix
		void LW3D_View(const std::array<float, 16>& m);
		// Set Manual World Matrix
		void LW3D_World(const std::array<float, 16>& m);
		// Set Manual Projection Matrix
		void LW3D_Projection(const std::array<float, 16>& m);
		
		// Draws a vector of vertices, interprted as individual triangles
		void LW3D_DrawTriangles(olc::Decal* decal, const std::vector<std::array<float,3>>& pos, const std::vector<olc::vf2d>& tex, const std::vector<olc::Pixel>& col);
		void LW3D_DrawWarpedDecal(olc::Decal* decal, const std::vector<std::array<float, 3>>& pos, const olc::Pixel& tint);

		void LW3D_ModelTranslate(const float x, const float y, const float z);
		
		// Camera convenience functions
		void LW3D_SetCameraAtTarget(const float fEyeX, const float fEyeY, const float fEyeZ,
			const float fTargetX, const float fTargetY, const float fTargetZ,
			const float fUpX = 0.0f, const float fUpY = 1.0f, const float fUpZ = 0.0f);
		void LW3D_SetCameraAlongDirection(const float fEyeX, const float fEyeY, const float fEyeZ,
			const float fDirX, const float fDirY, const float fDirZ,
			const float fUpX = 0.0f, const float fUpY = 1.0f, const float fUpZ = 0.0f);

		// 3D Rendering Flags
		void LW3D_EnableDepthTest(const bool bEnableDepth);
		void LW3D_EnableBackfaceCulling(const bool bEnableCull);
#endif
	public: // Branding
		std::string sAppName;

	private: // Inner mysterious workings
		olc::Sprite*     pDrawTarget = nullptr;
		Pixel::Mode	nPixelMode = Pixel::NORMAL;
		float		fBlendFactor = 1.0f;
		olc::vi2d	vScreenSize = { 256, 240 };
		olc::vf2d	vInvScreenSize = { 1.0f / 256.0f, 1.0f / 240.0f };
		olc::vi2d	vPixelSize = { 4, 4 };
		olc::vi2d   vScreenPixelSize = { 4, 4 };
		olc::vi2d	vMousePos = { 0, 0 };
		int32_t		nMouseWheelDelta = 0;
		olc::vi2d	vMousePosCache = { 0, 0 };
		olc::vi2d   vMouseWindowPos = { 0, 0 };
		int32_t		nMouseWheelDeltaCache = 0;
		olc::vi2d	vWindowPos = { 0, 0 };
		olc::vi2d	vWindowSize = { 0, 0 };
		olc::vi2d	vViewPos = { 0, 0 };
		olc::vi2d	vViewSize = { 0,0 };
		bool		bFullScreen = false;
		olc::vf2d	vPixel = { 1.0f, 1.0f };
		bool		bHasInputFocus = false;
		bool		bHasMouseFocus = false;
		bool		bEnableVSYNC = false;
		bool		bRealWindowMode = false;
		bool		bResizeRequested = false;
		olc::vi2d	vResizeRequested = { 0, 0 };
		float		fFrameTimer = 1.0f;
		float		fLastElapsed = 0.0f;
		int			nFrameCount = 0;		
		bool bSuspendTextureTransfer = false;
		Renderable  fontRenderable;
		std::vector<LayerDesc> vLayers;
		uint8_t		nTargetLayer = 0;
		uint32_t	nLastFPS = 0;
		bool		bManualRenderEnable = false;
		bool        bPixelCohesion = false;
		DecalMode   nDecalMode = DecalMode::NORMAL;
		DecalStructure nDecalStructure = DecalStructure::FAN;
		std::function<olc::Pixel(const int x, const int y, const olc::Pixel&, const olc::Pixel&)> funcPixelMode;
		std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
		std::vector<olc::vi2d> vFontSpacing;
		std::vector<std::string> vDroppedFiles;
		std::vector<std::string> vDroppedFilesCache;
		olc::vi2d vDroppedFilesPoint;
		olc::vi2d vDroppedFilesPointCache;

		// Command Console Specific
		bool bConsoleShow = false;
		bool bConsoleSuspendTime = false;
		olc::Key keyConsoleExit = olc::Key::F1;
		std::stringstream ssConsoleOutput;
		std::streambuf* sbufOldCout = nullptr;
		olc::vi2d vConsoleSize;
		olc::vi2d vConsoleCursor = { 0,0 };
		olc::vf2d vConsoleCharacterScale = { 1.0f, 2.0f };
		std::vector<std::string> sConsoleLines;
		std::list<std::string> sCommandHistory;
		std::list<std::string>::iterator sCommandHistoryIt;

		// Text Entry Specific
		bool bTextEntryEnable = false;
		std::string sTextEntryString = "";
		int32_t nTextEntryCursor = 0;
		std::vector<std::tuple<olc::Key, std::string, std::string>> vKeyboardMap;



		// State of keyboard		
		bool		pKeyNewState[256] = { 0 };
		bool		pKeyOldState[256] = { 0 };
		HWButton	pKeyboardState[256] = { 0 };

		// State of mouse
		bool		pMouseNewState[nMouseButtons] = { 0 };
		bool		pMouseOldState[nMouseButtons] = { 0 };
		HWButton	pMouseState[nMouseButtons] = { 0 };

		// The main engine thread
		void		EngineThread();


		// If anything sets this flag to false, the engine
		// "should" shut down gracefully
		static std::atomic<bool> bAtomActive;

	public:
		// "Break In" Functions
		void olc_UpdateMouse(int32_t x, int32_t y);
		void olc_UpdateMouseWheel(int32_t delta);
		void olc_UpdateWindowPos(int32_t x, int32_t y);
		void olc_UpdateWindowSize(int32_t x, int32_t y);
		void olc_UpdateViewport();
		void olc_ConstructFontSheet();
		void olc_CoreUpdate();
		void olc_PrepareEngine();
		void olc_UpdateMouseState(int32_t button, bool state);
		void olc_UpdateKeyState(int32_t key, bool state);
		void olc_UpdateMouseFocus(bool state);
		void olc_UpdateKeyFocus(bool state);
		void olc_Terminate();
		void olc_DropFiles(int32_t x, int32_t y, const std::vector<std::string>& vFiles);
		void olc_Reanimate();
		bool olc_IsRunning();

		// At the very end of this file, chooses which
		// components to compile
		virtual void olc_ConfigureSystem();

		// NOTE: Items Here are to be deprecated, I have left them in for now
		// in case you are using them, but they will be removed.
		// olc::vf2d	vSubPixelOffset = { 0.0f, 0.0f };

	public: // PGEX Stuff
		friend class PGEX;
		void pgex_Register(olc::PGEX* pgex);

	private:
		std::vector<olc::PGEX*> vExtensions;
	};



	// O------------------------------------------------------------------------------O
	// | PGE EXTENSION BASE CLASS - Permits access to PGE functions from extension    |
	// O------------------------------------------------------------------------------O
	class PGEX
	{
		friend class olc::PixelGameEngine;
	public:
		PGEX(bool bHook = false);

	protected:
		virtual void OnBeforeUserCreate();
		virtual void OnAfterUserCreate();
		virtual bool OnBeforeUserUpdate(float &fElapsedTime);
		virtual void OnAfterUserUpdate(float fElapsedTime);

	protected:
		static PixelGameEngine* pge;
	};
}

#pragma endregion


#pragma region opengl33_iface
// In order to facilitate more advanced graphics features, some PGEX
// will rely on shaders. Instead of having each PGEX responsible for
// managing this, for convenience, this interface exists.

#if defined(OLC_GFX_OPENGL33)

	#if defined(OLC_PLATFORM_WINAPI)
		#include <gl/GL.h>
		#define CALLSTYLE __stdcall
	#endif

	#if defined(__linux__) || defined(__FreeBSD__)
		#include <GL/gl.h>
	#endif

	#if defined(OLC_PLATFORM_X11)
		namespace X11 {
			#include <GL/glx.h>
		}
		#define CALLSTYLE 
	#endif

	#if defined(__APPLE__)
		#define GL_SILENCE_DEPRECATION
		#include <OpenGL/OpenGL.h>
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
	#endif

	#if defined(OLC_PLATFORM_EMSCRIPTEN)
		#include <EGL/egl.h>
		#include <GLES2/gl2.h>
		#define GL_GLEXT_PROTOTYPES
		#include <GLES2/gl2ext.h>
		#include <emscripten/emscripten.h>
		#define CALLSTYLE
		#define GL_CLAMP GL_CLAMP_TO_EDGE
	#endif

namespace olc
{
	typedef char GLchar;
	typedef ptrdiff_t GLsizeiptr;

	typedef GLuint CALLSTYLE locCreateShader_t(GLenum type);
	typedef GLuint CALLSTYLE locCreateProgram_t(void);
	typedef void CALLSTYLE locDeleteShader_t(GLuint shader);
	typedef void CALLSTYLE locCompileShader_t(GLuint shader);
	typedef void CALLSTYLE locLinkProgram_t(GLuint program);
	typedef void CALLSTYLE locDeleteProgram_t(GLuint program);
	typedef void CALLSTYLE locAttachShader_t(GLuint program, GLuint shader);
	typedef void CALLSTYLE locBindBuffer_t(GLenum target, GLuint buffer);
	typedef void CALLSTYLE locBufferData_t(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	typedef void CALLSTYLE locGenBuffers_t(GLsizei n, GLuint* buffers);
	typedef void CALLSTYLE locVertexAttribPointer_t(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	typedef void CALLSTYLE locEnableVertexAttribArray_t(GLuint index);
	typedef void CALLSTYLE locUseProgram_t(GLuint program);
	typedef void CALLSTYLE locBindVertexArray_t(GLuint array);
	typedef void CALLSTYLE locGenVertexArrays_t(GLsizei n, GLuint* arrays);
	typedef void CALLSTYLE locGetShaderInfoLog_t(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
	typedef GLint CALLSTYLE locGetUniformLocation_t(GLuint program, const GLchar* name);
	typedef void CALLSTYLE locUniform1f_t(GLint location, GLfloat v0);
	typedef void CALLSTYLE locUniform1i_t(GLint location, GLint v0);
	typedef void CALLSTYLE locUniform2fv_t(GLint location, GLsizei count, const GLfloat* value);
	typedef void CALLSTYLE locActiveTexture_t(GLenum texture);
	typedef void CALLSTYLE locGenFrameBuffers_t(GLsizei n, GLuint* ids);
	typedef void CALLSTYLE locBindFrameBuffer_t(GLenum target, GLuint fb);
	typedef GLenum CALLSTYLE locCheckFrameBufferStatus_t(GLenum target);
	typedef void CALLSTYLE locDeleteFrameBuffers_t(GLsizei n, const GLuint* fbs);
	typedef void CALLSTYLE locFrameBufferTexture2D_t(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void CALLSTYLE locDrawBuffers_t(GLsizei n, const GLenum* bufs);
	typedef void CALLSTYLE locBlendFuncSeparate_t(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

#if defined(OLC_PLATFORM_WINAPI)
	typedef void __stdcall locSwapInterval_t(GLsizei n);
#endif

#if defined(OLC_PLATFORM_X11)
	typedef int(locSwapInterval_t)(X11::Display* dpy, X11::GLXDrawable drawable, int interval);
#endif

#if defined(OLC_PLATFORM_EMSCRIPTEN)
	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
	typedef EGLBoolean(locSwapInterval_t)(EGLDisplay display, EGLint interval);
#else
	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
#endif

} // olc namespace
#endif // OpenGL33 Definitions
#pragma endregion


#endif // OLC_PGE_DEF


// O------------------------------------------------------------------------------O
// | START OF OLC_PGE_APPLICATION                                                 |
// O------------------------------------------------------------------------------O
#ifdef OLC_PGE_APPLICATION
#undef OLC_PGE_APPLICATION

// O------------------------------------------------------------------------------O
// | olcPixelGameEngine INTERFACE IMPLEMENTATION (CORE)                           |
// | Note: The core implementation is platform independent                        |
// O------------------------------------------------------------------------------O
#pragma region pge_implementation
namespace olc
{
	// O------------------------------------------------------------------------------O
	// | olc::Pixel IMPLEMENTATION                                                    |
	// O------------------------------------------------------------------------------O
#if !defined(OLC_IGNORE_PIXEL)
	Pixel::Pixel()
	{ r = 0; g = 0; b = 0; a = nDefaultAlpha; }

	Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{ n = red | (green << 8) | (blue << 16) | (alpha << 24); } // Thanks jarekpelczar 

	Pixel::Pixel(uint32_t p)
	{ n = p; }

	bool Pixel::operator==(const Pixel& p) const
	{ return n == p.n; }

	bool Pixel::operator!=(const Pixel& p) const
	{ return n != p.n; }

	Pixel  Pixel::operator * (const float i) const
	{
		float fR = std::min(255.0f, std::max(0.0f, float(r) * i));
		float fG = std::min(255.0f, std::max(0.0f, float(g) * i));
		float fB = std::min(255.0f, std::max(0.0f, float(b) * i));
		return Pixel(uint8_t(fR), uint8_t(fG), uint8_t(fB), a);
	}

	Pixel  Pixel::operator / (const float i) const
	{
		float fR = std::min(255.0f, std::max(0.0f, float(r) / i));
		float fG = std::min(255.0f, std::max(0.0f, float(g) / i));
		float fB = std::min(255.0f, std::max(0.0f, float(b) / i));
		return Pixel(uint8_t(fR), uint8_t(fG), uint8_t(fB), a);
	}

	Pixel& Pixel::operator *=(const float i)
	{
		this->r = uint8_t(std::min(255.0f, std::max(0.0f, float(r) * i)));
		this->g = uint8_t(std::min(255.0f, std::max(0.0f, float(g) * i)));
		this->b = uint8_t(std::min(255.0f, std::max(0.0f, float(b) * i)));
		return *this;
	}

	Pixel& Pixel::operator /=(const float i)
	{
		this->r = uint8_t(std::min(255.0f, std::max(0.0f, float(r) / i)));
		this->g = uint8_t(std::min(255.0f, std::max(0.0f, float(g) / i)));
		this->b = uint8_t(std::min(255.0f, std::max(0.0f, float(b) / i)));
		return *this;
	}

	Pixel  Pixel::operator + (const Pixel& p) const
	{
		uint8_t nR = uint8_t(std::min(255, std::max(0, int(r) + int(p.r))));
		uint8_t nG = uint8_t(std::min(255, std::max(0, int(g) + int(p.g))));
		uint8_t nB = uint8_t(std::min(255, std::max(0, int(b) + int(p.b))));
		return Pixel(nR, nG, nB, a);
	}

	Pixel  Pixel::operator - (const Pixel& p) const
	{
		uint8_t nR = uint8_t(std::min(255, std::max(0, int(r) - int(p.r))));
		uint8_t nG = uint8_t(std::min(255, std::max(0, int(g) - int(p.g))));
		uint8_t nB = uint8_t(std::min(255, std::max(0, int(b) - int(p.b))));
		return Pixel(nR, nG, nB, a);
	}

	Pixel& Pixel::operator += (const Pixel& p)
	{
		this->r = uint8_t(std::min(255, std::max(0, int(r) + int(p.r))));
		this->g = uint8_t(std::min(255, std::max(0, int(g) + int(p.g))));
		this->b = uint8_t(std::min(255, std::max(0, int(b) + int(p.b))));
		return *this;
	}

	Pixel& Pixel::operator -= (const Pixel& p) // Thanks Au Lit
	{
		this->r = uint8_t(std::min(255, std::max(0, int(r) - int(p.r))));
		this->g = uint8_t(std::min(255, std::max(0, int(g) - int(p.g))));
		this->b = uint8_t(std::min(255, std::max(0, int(b) - int(p.b))));
		return *this;
	}

	Pixel Pixel::operator * (const Pixel& p) const
	{
		uint8_t nR = uint8_t(std::min(255.0f, std::max(0.0f, float(r) * float(p.r) / 255.0f)));
		uint8_t nG = uint8_t(std::min(255.0f, std::max(0.0f, float(g) * float(p.g) / 255.0f)));
		uint8_t nB = uint8_t(std::min(255.0f, std::max(0.0f, float(b) * float(p.b) / 255.0f)));
		uint8_t nA = uint8_t(std::min(255.0f, std::max(0.0f, float(a) * float(p.a) / 255.0f)));
		return Pixel(nR, nG, nB, nA);
	}

	Pixel& Pixel::operator *=(const Pixel& p)
	{
		this->r = uint8_t(std::min(255.0f, std::max(0.0f, float(r) * float(p.r) / 255.0f)));
		this->g = uint8_t(std::min(255.0f, std::max(0.0f, float(g) * float(p.g) / 255.0f)));
		this->b = uint8_t(std::min(255.0f, std::max(0.0f, float(b) * float(p.b) / 255.0f)));
		this->a = uint8_t(std::min(255.0f, std::max(0.0f, float(a) * float(p.a) / 255.0f)));
		return *this;
	}

	Pixel Pixel::inv() const
	{
		uint8_t nR = uint8_t(std::min(255, std::max(0, 255 - int(r))));
		uint8_t nG = uint8_t(std::min(255, std::max(0, 255 - int(g))));
		uint8_t nB = uint8_t(std::min(255, std::max(0, 255 - int(b))));
		return Pixel(nR, nG, nB, a);
	}

	Pixel PixelF(float red, float green, float blue, float alpha)
	{ return Pixel(uint8_t(red * 255.0f), uint8_t(green * 255.0f), uint8_t(blue * 255.0f), uint8_t(alpha * 255.0f)); }

	Pixel PixelLerp(const olc::Pixel& p1, const olc::Pixel& p2, float t)
	{ return (p2 * t) + p1 * (1.0f - t); }
#endif
	// O------------------------------------------------------------------------------O
	// | olc::Sprite IMPLEMENTATION                                                   |
	// O------------------------------------------------------------------------------O
	Sprite::Sprite()
	{ width = 0; height = 0; }

	Sprite::Sprite(const std::string& sImageFile, olc::ResourcePack* pack)
	{ LoadFromFile(sImageFile, pack); }

	Sprite::Sprite(int32_t w, int32_t h)
	{		
		SetSize(w, h);
	}

	void Sprite::SetSize(int32_t w, int32_t h)
	{
		width = w;		height = h;
		pColData.resize(width * height, nDefaultPixel);
	}

	Sprite::~Sprite()
	{ pColData.clear();	}

	void Sprite::SetSampleMode(olc::Sprite::Mode mode)
	{ modeSample = mode; }

	Pixel Sprite::GetPixel(const olc::vi2d& a) const
	{ return GetPixel(a.x, a.y); }

	bool Sprite::SetPixel(const olc::vi2d& a, Pixel p)
	{ return SetPixel(a.x, a.y, p); }

	Pixel Sprite::GetPixel(int32_t x, int32_t y) const
	{
		if (modeSample == olc::Sprite::Mode::NORMAL)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
				return pColData[y * width + x];
			else
				return Pixel(0, 0, 0, 0);
		}
		else
		{
			if (modeSample == olc::Sprite::Mode::PERIODIC)
				return pColData[abs(y % height) * width + abs(x % width)];
			else
				return pColData[std::max(0, std::min(y, height-1)) * width + std::max(0, std::min(x, width-1))];
		}
	}

	bool Sprite::SetPixel(int32_t x, int32_t y, Pixel p)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			pColData[y * width + x] = p;
			return true;
		}
		else
			return false;
	}

	Pixel Sprite::Sample(float x, float y) const
	{
		int32_t sx = std::min((int32_t)((x * (float)width)), width - 1);
		int32_t sy = std::min((int32_t)((y * (float)height)), height - 1);
		return GetPixel(sx, sy);
	}

	Pixel Sprite::Sample(const olc::vf2d& uv) const
	{
		return Sample(uv.x, uv.y);
	}

	Pixel Sprite::SampleBL(float u, float v) const
	{
		u = u * width - 0.5f;
		v = v * height - 0.5f;
		int x = (int)floor(u); // cast to int rounds toward zero, not downward
		int y = (int)floor(v); // Thanks @joshinils
		float u_ratio = u - x;
		float v_ratio = v - y;
		float u_opposite = 1 - u_ratio;
		float v_opposite = 1 - v_ratio;

		olc::Pixel p1 = GetPixel(std::max(x, 0), std::max(y, 0));
		olc::Pixel p2 = GetPixel(std::min(x + 1, (int)width - 1), std::max(y, 0));
		olc::Pixel p3 = GetPixel(std::max(x, 0), std::min(y + 1, (int)height - 1));
		olc::Pixel p4 = GetPixel(std::min(x + 1, (int)width - 1), std::min(y + 1, (int)height - 1));

		return olc::Pixel(
			(uint8_t)((p1.r * u_opposite + p2.r * u_ratio) * v_opposite + (p3.r * u_opposite + p4.r * u_ratio) * v_ratio),
			(uint8_t)((p1.g * u_opposite + p2.g * u_ratio) * v_opposite + (p3.g * u_opposite + p4.g * u_ratio) * v_ratio),
			(uint8_t)((p1.b * u_opposite + p2.b * u_ratio) * v_opposite + (p3.b * u_opposite + p4.b * u_ratio) * v_ratio));
	}

	Pixel Sprite::SampleBL(const olc::vf2d& uv) const
	{
		return SampleBL(uv.x, uv.y);
	}

	Pixel* Sprite::GetData()
	{ return pColData.data(); }


	olc::rcode Sprite::LoadFromFile(const std::string& sImageFile, olc::ResourcePack* pack)
	{
		UNUSED(pack);
		return loader->LoadImageResource(this, sImageFile, pack);
	}

	olc::Sprite* Sprite::Duplicate()
	{
		olc::Sprite* spr = new olc::Sprite(width, height);
		std::memcpy(spr->GetData(), GetData(), width * height * sizeof(olc::Pixel));
		spr->modeSample = modeSample;
		return spr;
	}

	olc::Sprite* Sprite::Duplicate(const olc::vi2d& vPos, const olc::vi2d& vSize)
	{
		olc::Sprite* spr = new olc::Sprite(vSize.x, vSize.y);
		for (int y = 0; y < vSize.y; y++)
			for (int x = 0; x < vSize.x; x++)
				spr->SetPixel(x, y, GetPixel(vPos.x + x, vPos.y + y));
		return spr;
	}

	olc::vi2d olc::Sprite::Size() const
	{
		return { width, height };
	}

	// O------------------------------------------------------------------------------O
	// | olc::Decal IMPLEMENTATION                                                    |
	// O------------------------------------------------------------------------------O
	Decal::Decal(olc::Sprite* spr, bool filter, bool clamp)
	{
		id = -1;
		if (spr == nullptr) return;
		sprite = spr;
		id = renderer->CreateTexture(sprite->width, sprite->height, filter, clamp);
		Update();
	}

	Decal::Decal(const uint32_t nExistingTextureResource, olc::Sprite* spr)
	{
		if (spr == nullptr) return;
		id = nExistingTextureResource;
	}

	void Decal::Update()
	{
		if (sprite == nullptr) return;
		vUVScale = { 1.0f / float(sprite->width), 1.0f / float(sprite->height) };
		renderer->ApplyTexture(id);
		renderer->UpdateTexture(id, sprite);
	}

	void Decal::UpdateSprite()
	{
		if (sprite == nullptr) return;
		renderer->ApplyTexture(id);
		renderer->ReadTexture(id, sprite);
	}

	Decal::~Decal()
	{
		if (id != -1)
		{
			renderer->DeleteTexture(id);
			id = -1;
		}
	}

	void Renderable::Create(uint32_t width, uint32_t height, bool filter, bool clamp)
	{
		pSprite = std::make_unique<olc::Sprite>(width, height);
		pDecal = std::make_unique<olc::Decal>(pSprite.get(), filter, clamp);
	}

	olc::rcode Renderable::Load(const std::string& sFile, ResourcePack* pack, bool filter, bool clamp)
	{
		pSprite = std::make_unique<olc::Sprite>();
		if (pSprite->LoadFromFile(sFile, pack) == olc::rcode::OK)
		{
			pDecal = std::make_unique<olc::Decal>(pSprite.get(), filter, clamp);
			return olc::rcode::OK;
		}
		else
		{
			pSprite.release();
			pSprite = nullptr;
			return olc::rcode::NO_FILE;
		}
	}

	olc::Decal* Renderable::Decal() const
	{ return pDecal.get(); }

	olc::Sprite* Renderable::Sprite() const
	{ return pSprite.get(); }

	// O------------------------------------------------------------------------------O
	// | olc::ResourcePack IMPLEMENTATION                                             |
	// O------------------------------------------------------------------------------O


	//=============================================================
	// Resource Packs - Allows you to store files in one large 
	// scrambled file - Thanks MaGetzUb for debugging a null char in std::stringstream bug
	ResourceBuffer::ResourceBuffer(std::ifstream& ifs, uint32_t offset, uint32_t size)
	{
		vMemory.resize(size);
		ifs.seekg(offset); ifs.read(vMemory.data(), vMemory.size());
		setg(vMemory.data(), vMemory.data(), vMemory.data() + size);
	}

	ResourcePack::ResourcePack() { }
	ResourcePack::~ResourcePack() { baseFile.close(); }

	bool ResourcePack::AddFile(const std::string& sFile)
	{
		const std::string file = makeposix(sFile);

		if (_gfs::exists(file))
		{
			sResourceFile e;
			e.nSize = (uint32_t)_gfs::file_size(file);
			e.nOffset = 0; // Unknown at this stage			
			mapFiles[file] = e;
			return true;
		}
		return false;
	}

	bool ResourcePack::LoadPack(const std::string& sFile, const std::string& sKey)
	{
		// Open the resource file
		baseFile.open(sFile, std::ifstream::binary);
		if (!baseFile.is_open()) return false;

		// 1) Read Scrambled index
		uint32_t nIndexSize = 0;
		baseFile.read((char*)&nIndexSize, sizeof(uint32_t));

		std::vector<char> buffer(nIndexSize);
		for (uint32_t j = 0; j < nIndexSize; j++)
			buffer[j] = baseFile.get();

		std::vector<char> decoded = scramble(buffer, sKey);
		size_t pos = 0;
		auto read = [&decoded, &pos](char* dst, size_t size) {
			memcpy((void*)dst, (const void*)(decoded.data() + pos), size);
			pos += size;
		};

		auto get = [&read]() -> int { char c; read(&c, 1); return c; };

		// 2) Read Map
		uint32_t nMapEntries = 0;
		read((char*)&nMapEntries, sizeof(uint32_t));
		for (uint32_t i = 0; i < nMapEntries; i++)
		{
			uint32_t nFilePathSize = 0;
			read((char*)&nFilePathSize, sizeof(uint32_t));

			std::string sFileName(nFilePathSize, ' ');
			for (uint32_t j = 0; j < nFilePathSize; j++)
				sFileName[j] = get();

			sResourceFile e;
			read((char*)&e.nSize, sizeof(uint32_t));
			read((char*)&e.nOffset, sizeof(uint32_t));
			mapFiles[sFileName] = e;
		}

		// Don't close base file! we will provide a stream
		// pointer when the file is requested
		return true;
	}

	bool ResourcePack::SavePack(const std::string& sFile, const std::string& sKey)
	{
		// Create/Overwrite the resource file
		std::ofstream ofs(sFile, std::ofstream::binary);
		if (!ofs.is_open()) return false;

		// Iterate through map
		uint32_t nIndexSize = 0; // Unknown for now
		ofs.write((char*)&nIndexSize, sizeof(uint32_t));
		uint32_t nMapSize = uint32_t(mapFiles.size());
		ofs.write((char*)&nMapSize, sizeof(uint32_t));
		for (auto& e : mapFiles)
		{
			// Write the path of the file
			size_t nPathSize = e.first.size();
			ofs.write((char*)&nPathSize, sizeof(uint32_t));
			ofs.write(e.first.c_str(), nPathSize);

			// Write the file entry properties
			ofs.write((char*)&e.second.nSize, sizeof(uint32_t));
			ofs.write((char*)&e.second.nOffset, sizeof(uint32_t));
		}

		// 2) Write the individual Data
		std::streampos offset = ofs.tellp();
		nIndexSize = (uint32_t)offset;
		for (auto& e : mapFiles)
		{
			// Store beginning of file offset within resource pack file
			e.second.nOffset = (uint32_t)offset;

			// Load the file to be added
			std::vector<uint8_t> vBuffer(e.second.nSize);
			std::ifstream i(e.first, std::ifstream::binary);
			i.read((char*)vBuffer.data(), e.second.nSize);
			i.close();

			// Write the loaded file into resource pack file
			ofs.write((char*)vBuffer.data(), e.second.nSize);
			offset += e.second.nSize;
		}

		// 3) Scramble Index
		std::vector<char> stream;
		auto write = [&stream](const char* data, size_t size) {
			size_t sizeNow = stream.size();
			stream.resize(sizeNow + size);
			memcpy(stream.data() + sizeNow, data, size);
		};

		// Iterate through map
		write((char*)&nMapSize, sizeof(uint32_t));
		for (auto& e : mapFiles)
		{
			// Write the path of the file
			size_t nPathSize = e.first.size();
			write((char*)&nPathSize, sizeof(uint32_t));
			write(e.first.c_str(), nPathSize);

			// Write the file entry properties
			write((char*)&e.second.nSize, sizeof(uint32_t));
			write((char*)&e.second.nOffset, sizeof(uint32_t));
		}
		std::vector<char> sIndexString = scramble(stream, sKey);
		uint32_t nIndexStringLen = uint32_t(sIndexString.size());
		// 4) Rewrite Map (it has been updated with offsets now)
		// at start of file
		ofs.seekp(0, std::ios::beg);
		ofs.write((char*)&nIndexStringLen, sizeof(uint32_t));
		ofs.write(sIndexString.data(), nIndexStringLen);
		ofs.close();
		return true;
	}

	ResourceBuffer ResourcePack::GetFileBuffer(const std::string& sFile)
	{ return ResourceBuffer(baseFile, mapFiles[sFile].nOffset, mapFiles[sFile].nSize); }

	bool ResourcePack::Loaded()
	{ return baseFile.is_open(); }

	std::vector<char> ResourcePack::scramble(const std::vector<char>& data, const std::string& key)
	{
		if (key.empty()) return data;
		std::vector<char> o;
		size_t c = 0;
		for (auto s : data)	o.push_back(s ^ key[(c++) % key.size()]);
		return o;
	};

	std::string ResourcePack::makeposix(const std::string& path)
	{
		std::string o;
		for (auto s : path) o += std::string(1, s == '\\' ? '/' : s);
		return o;
	};

	// O------------------------------------------------------------------------------O
	// | olc::PixelGameEngine IMPLEMENTATION                                          |
	// O------------------------------------------------------------------------------O
	PixelGameEngine::PixelGameEngine()
	{
		sAppName = "Undefined";
		olc::PGEX::pge = this;

		// Bring in relevant Platform & Rendering systems depending
		// on compiler parameters
		olc_ConfigureSystem();
	}

	PixelGameEngine::~PixelGameEngine()
	{}


	olc::rcode PixelGameEngine::Construct(int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, bool full_screen, bool vsync, bool cohesion, bool realwindow)
	{
		bPixelCohesion = cohesion;
		bRealWindowMode = realwindow;
		vScreenSize = { screen_w, screen_h };
		vInvScreenSize = { 1.0f / float(screen_w), 1.0f / float(screen_h) };
		vPixelSize = { pixel_w, pixel_h };
		vWindowSize = vScreenSize * vPixelSize;
		bFullScreen = full_screen;
		bEnableVSYNC = vsync;
		vPixel = 2.0f / vScreenSize;

		if (vPixelSize.x <= 0 || vPixelSize.y <= 0 || vScreenSize.x <= 0 || vScreenSize.y <= 0)
			return olc::FAIL;
		return olc::OK;
	}


	void PixelGameEngine::SetScreenSize(int w, int h)
	{
		vScreenSize = { w, h };
		vInvScreenSize = { 1.0f / float(w), 1.0f / float(h) };
		for (auto& layer : vLayers)
		{
			layer.pDrawTarget.Create(vScreenSize.x, vScreenSize.y);
			layer.bUpdate = true;
		}
		SetDrawTarget(nullptr);
		if (!bRealWindowMode)
		{
			// Flush backbuffer
			renderer->ClearBuffer(olc::BLACK, true);
			renderer->DisplayFrame();
			renderer->ClearBuffer(olc::BLACK, true);
		}
		renderer->UpdateViewport(vViewPos, vViewSize);
	}

#if !defined(PGE_USE_CUSTOM_START)
	olc::rcode PixelGameEngine::Start()
	{
		if (platform->ApplicationStartUp() != olc::OK) return olc::FAIL;

		// Construct the window
		if (platform->CreateWindowPane({ 30,30 }, vWindowSize, bFullScreen) != olc::OK) return olc::FAIL;
		olc_UpdateWindowSize(vWindowSize.x, vWindowSize.y);

		// Start the thread
		bAtomActive = true;
		std::thread t = std::thread(&PixelGameEngine::EngineThread, this);

		// Some implementations may form an event loop here
		platform->StartSystemEventLoop();

		// Wait for thread to be exited
		t.join();

		if (platform->ApplicationCleanUp() != olc::OK) return olc::FAIL;

		return olc::OK;
	}
#endif

	void PixelGameEngine::SetDrawTarget(Sprite* target)
	{
		if (target)
		{
			pDrawTarget = target;
		}
		else
		{
			nTargetLayer = 0;
			if(!vLayers.empty())
				pDrawTarget = vLayers[0].pDrawTarget.Sprite();
		}
	}

	void PixelGameEngine::SetDrawTarget(uint8_t layer, bool bDirty)
	{
		if (layer < vLayers.size())
		{
			pDrawTarget = vLayers[layer].pDrawTarget.Sprite();
			vLayers[layer].bUpdate = bDirty;
			nTargetLayer = layer;
		}
	}

	void PixelGameEngine::EnableLayer(uint8_t layer, bool b)
	{ if (layer < vLayers.size()) vLayers[layer].bShow = b; }

	void PixelGameEngine::SetLayerOffset(uint8_t layer, const olc::vf2d& offset)
	{ SetLayerOffset(layer, offset.x, offset.y); }

	void PixelGameEngine::SetLayerOffset(uint8_t layer, float x, float y)
	{ if (layer < vLayers.size()) vLayers[layer].vOffset = { x, y }; }

	void PixelGameEngine::SetLayerScale(uint8_t layer, const olc::vf2d& scale)
	{ SetLayerScale(layer, scale.x, scale.y); }

	void PixelGameEngine::SetLayerScale(uint8_t layer, float x, float y)
	{ if (layer < vLayers.size()) vLayers[layer].vScale = { x, y }; }

	void PixelGameEngine::SetLayerTint(uint8_t layer, const olc::Pixel& tint)
	{ if (layer < vLayers.size()) vLayers[layer].tint = tint; }

	void PixelGameEngine::SetLayerCustomRenderFunction(uint8_t layer, std::function<void()> f)
	{ if (layer < vLayers.size()) vLayers[layer].funcHook = f; }

	std::vector<LayerDesc>& PixelGameEngine::GetLayers()
	{ return vLayers; }

	uint32_t PixelGameEngine::CreateLayer()
	{
		LayerDesc ld;
		ld.pDrawTarget.Create(vScreenSize.x, vScreenSize.y);
		vLayers.push_back(std::move(ld));
		return uint32_t(vLayers.size()) - 1;
	}

	Sprite* PixelGameEngine::GetDrawTarget() const
	{ return pDrawTarget; }

	int32_t PixelGameEngine::GetDrawTargetWidth() const
	{
		if (pDrawTarget)
			return pDrawTarget->width;
		else
			return 0;
	}

	int32_t PixelGameEngine::GetDrawTargetHeight() const
	{
		if (pDrawTarget)
			return pDrawTarget->height;
		else
			return 0;
	}

	uint32_t PixelGameEngine::GetFPS() const
	{ return nLastFPS; }

	bool PixelGameEngine::IsFocused() const
	{ return bHasInputFocus; }

	HWButton PixelGameEngine::GetKey(Key k) const
	{ return pKeyboardState[k];	}

	HWButton PixelGameEngine::GetMouse(uint32_t b) const
	{ return pMouseState[b]; }

	int32_t PixelGameEngine::GetMouseX() const
	{ return vMousePos.x; }

	int32_t PixelGameEngine::GetMouseY() const
	{ return vMousePos.y; }

	const olc::vi2d& PixelGameEngine::GetMousePos() const
	{ return vMousePos; }

	int32_t PixelGameEngine::GetMouseWheel() const
	{ return nMouseWheelDelta; }

	int32_t PixelGameEngine::ScreenWidth() const
	{ return vScreenSize.x; }

	int32_t PixelGameEngine::ScreenHeight() const
	{ return vScreenSize.y; }

	float PixelGameEngine::GetElapsedTime() const
	{ return fLastElapsed; }

	const olc::vi2d& PixelGameEngine::GetWindowSize() const
	{ return vWindowSize; }

	const olc::vi2d& PixelGameEngine::GetWindowPos() const
	{ return vWindowPos; }

	const olc::vi2d& PixelGameEngine::GetPixelSize() const
	{ return vPixelSize; }

	const olc::vi2d& PixelGameEngine::GetScreenPixelSize() const
	{ return vScreenPixelSize; }

	const olc::vi2d& PixelGameEngine::GetScreenSize() const
	{ return vScreenSize;	}

	const olc::vi2d& PixelGameEngine::GetWindowMouse() const
	{ return vMouseWindowPos; }

	bool PixelGameEngine::Draw(const olc::vi2d& pos, Pixel p)
	{ return Draw(pos.x, pos.y, p); }

	// This is it, the critical function that plots a pixel
	bool PixelGameEngine::Draw(int32_t x, int32_t y, Pixel p)
	{
		if (!pDrawTarget) return false;

		if (nPixelMode == Pixel::NORMAL)
		{
			return pDrawTarget->SetPixel(x, y, p);
		}

		if (nPixelMode == Pixel::MASK)
		{
			if (p.a == 255)
				return pDrawTarget->SetPixel(x, y, p);
		}

		if (nPixelMode == Pixel::ALPHA)
		{
			Pixel d = pDrawTarget->GetPixel(x, y);
			float a = (float)(p.a / 255.0f) * fBlendFactor;
			float c = 1.0f - a;
			float r = a * (float)p.r + c * (float)d.r;
			float g = a * (float)p.g + c * (float)d.g;
			float b = a * (float)p.b + c * (float)d.b;
			return pDrawTarget->SetPixel(x, y, Pixel((uint8_t)r, (uint8_t)g, (uint8_t)b/*, (uint8_t)(p.a * fBlendFactor)*/));
		}

		if (nPixelMode == Pixel::CUSTOM)
		{
			return pDrawTarget->SetPixel(x, y, funcPixelMode(x, y, p, pDrawTarget->GetPixel(x, y)));
		}

		return false;
	}


	void PixelGameEngine::DrawLine(const olc::vi2d& pos1, const olc::vi2d& pos2, Pixel p, uint32_t pattern)
	{ DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, p, pattern); }

	void PixelGameEngine::DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p, uint32_t pattern)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;

		auto rol = [&](void) { pattern = (pattern << 1) | (pattern >> 31); return pattern & 1; };

		olc::vi2d p1(x1, y1), p2(x2, y2);
		if (!ClipLineToScreen(p1, p2))
			return;
		x1 = p1.x; y1 = p1.y;
		x2 = p2.x; y2 = p2.y;

		// straight lines idea by gurkanctn
		if (dx == 0) // Line is vertical
		{
			if (y2 < y1) std::swap(y1, y2);
			for (y = y1; y <= y2; y++) if (rol()) Draw(x1, y, p);
			return;
		}

		if (dy == 0) // Line is horizontal
		{
			if (x2 < x1) std::swap(x1, x2);
			for (x = x1; x <= x2; x++) if (rol()) Draw(x, y1, p);
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

			if (rol()) Draw(x, y, p);

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				if (rol()) Draw(x, y, p);
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

			if (rol()) Draw(x, y, p);

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				if (rol()) Draw(x, y, p);
			}
		}
	}

	void PixelGameEngine::DrawCircle(const olc::vi2d& pos, int32_t radius, Pixel p, uint8_t mask)
	{ DrawCircle(pos.x, pos.y, radius, p, mask); }

	void PixelGameEngine::DrawCircle(int32_t x, int32_t y, int32_t radius, Pixel p, uint8_t mask)
	{ // Thanks to IanM-Matrix1 #PR121
		if (radius < 0 || x < -radius || y < -radius || x - GetDrawTargetWidth() > radius || y - GetDrawTargetHeight() > radius)
			return;

		if (radius > 0)
		{
			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;

			while (y0 >= x0) // only formulate 1/8 of circle
			{
				// Draw even octants
				if (mask & 0x01) Draw(x + x0, y - y0, p);// Q6 - upper right right
				if (mask & 0x04) Draw(x + y0, y + x0, p);// Q4 - lower lower right
				if (mask & 0x10) Draw(x - x0, y + y0, p);// Q2 - lower left left
				if (mask & 0x40) Draw(x - y0, y - x0, p);// Q0 - upper upper left
				if (x0 != 0 && x0 != y0)
				{
					if (mask & 0x02) Draw(x + y0, y - x0, p);// Q7 - upper upper right
					if (mask & 0x08) Draw(x + x0, y + y0, p);// Q5 - lower right right
					if (mask & 0x20) Draw(x - y0, y + x0, p);// Q3 - lower lower left
					if (mask & 0x80) Draw(x - x0, y - y0, p);// Q1 - upper left left
				}

				if (d < 0)
					d += 4 * x0++ + 6;
				else
					d += 4 * (x0++ - y0--) + 10;
			}
		}
		else
			Draw(x, y, p);
	}

	void PixelGameEngine::FillCircle(const olc::vi2d& pos, int32_t radius, Pixel p)
	{ FillCircle(pos.x, pos.y, radius, p); }

	void PixelGameEngine::FillCircle(int32_t x, int32_t y, int32_t radius, Pixel p)
	{ // Thanks to IanM-Matrix1 #PR121
		if (radius < 0 || x < -radius || y < -radius || x - GetDrawTargetWidth() > radius || y - GetDrawTargetHeight() > radius)
			return;

		if (radius > 0)
		{
			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;

			auto drawline = [&](int sx, int ex, int y)
			{
				for (int x = sx; x <= ex; x++)
					Draw(x, y, p);
			};

			while (y0 >= x0)
			{
				drawline(x - y0, x + y0, y - x0);
				if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

				if (d < 0)
					d += 4 * x0++ + 6;
				else
				{
					if (x0 != y0)
					{
						drawline(x - x0, x + x0, y - y0);
						drawline(x - x0, x + x0, y + y0);
					}
					d += 4 * (x0++ - y0--) + 10;
				}
			}
		}
		else
			Draw(x, y, p);
	}

	void PixelGameEngine::DrawRect(const olc::vi2d& pos, const olc::vi2d& size, Pixel p)
	{ DrawRect(pos.x, pos.y, size.x, size.y, p); }

	void PixelGameEngine::DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p)
	{
		DrawLine(x, y, x + w, y, p);
		DrawLine(x + w, y, x + w, y + h, p);
		DrawLine(x + w, y + h, x, y + h, p);
		DrawLine(x, y + h, x, y, p);
	}

	void PixelGameEngine::Clear(Pixel p)
	{
		int pixels = GetDrawTargetWidth() * GetDrawTargetHeight();
		Pixel* m = GetDrawTarget()->GetData();
		for (int i = 0; i < pixels; i++) m[i] = p;
	}

	void PixelGameEngine::ClearBuffer(Pixel p, bool bDepth)
	{ renderer->ClearBuffer(p, bDepth);	}

	olc::Sprite* PixelGameEngine::GetFontSprite()
	{ return fontRenderable.Sprite(); }

	bool PixelGameEngine::ClipLineToScreen(olc::vi2d& in_p1, olc::vi2d& in_p2)
	{
		// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
		static constexpr int SEG_I = 0b0000, SEG_L = 0b0001, SEG_R = 0b0010, SEG_B = 0b0100, SEG_T = 0b1000;
		auto Segment = [&vScreenSize = vScreenSize](const olc::vi2d& v)
		{
			int i = SEG_I;
			if (v.x < 0) i |= SEG_L; else if (v.x > vScreenSize.x) i |= SEG_R;
			if (v.y < 0) i |= SEG_B; else if (v.y > vScreenSize.y) i |= SEG_T;
			return i;
		};

		int s1 = Segment(in_p1), s2 = Segment(in_p2);

		while (true)
		{
			if (!(s1 | s2))	  return true;
			else if (s1 & s2) return false;
			else
			{
				int s3 = s2 > s1 ? s2 : s1;
				olc::vi2d n;
				if (s3 & SEG_T) { n.x = in_p1.x + (in_p2.x - in_p1.x) * (vScreenSize.y - in_p1.y) / (in_p2.y - in_p1.y); n.y = vScreenSize.y; }
				else if (s3 & SEG_B) { n.x = in_p1.x + (in_p2.x - in_p1.x) * (0 - in_p1.y) / (in_p2.y - in_p1.y); n.y = 0; }
				else if (s3 & SEG_R) { n.x = vScreenSize.x; n.y = in_p1.y + (in_p2.y - in_p1.y) * (vScreenSize.x - in_p1.x) / (in_p2.x - in_p1.x); }
				else if (s3 & SEG_L) { n.x = 0; n.y = in_p1.y + (in_p2.y - in_p1.y) * (0 - in_p1.x) / (in_p2.x - in_p1.x); }
				if (s3 == s1) { in_p1 = n; s1 = Segment(in_p1); }
				else { in_p2 = n; s2 = Segment(in_p2); }
			}
		}
		return true;
	}

	void PixelGameEngine::EnablePixelTransfer(const bool bEnable)
	{
		bSuspendTextureTransfer = !bEnable;
	}


	void PixelGameEngine::FillRect(const olc::vi2d& pos, const olc::vi2d& size, Pixel p)
	{ FillRect(pos.x, pos.y, size.x, size.y, p); }

	void PixelGameEngine::FillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p)
	{
		int32_t x2 = x + w;
		int32_t y2 = y + h;

		if (x < 0) x = 0;
		if (x >= (int32_t)GetDrawTargetWidth()) x = (int32_t)GetDrawTargetWidth();
		if (y < 0) y = 0;
		if (y >= (int32_t)GetDrawTargetHeight()) y = (int32_t)GetDrawTargetHeight();

		if (x2 < 0) x2 = 0;
		if (x2 >= (int32_t)GetDrawTargetWidth()) x2 = (int32_t)GetDrawTargetWidth();
		if (y2 < 0) y2 = 0;
		if (y2 >= (int32_t)GetDrawTargetHeight()) y2 = (int32_t)GetDrawTargetHeight();

		for (int i = x; i < x2; i++)
			for (int j = y; j < y2; j++)
				Draw(i, j, p);
	}

	void PixelGameEngine::DrawTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, Pixel p)
	{ DrawTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p); }

	void PixelGameEngine::DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p)
	{
		DrawLine(x1, y1, x2, y2, p);
		DrawLine(x2, y2, x3, y3, p);
		DrawLine(x3, y3, x1, y1, p);
	}

	void PixelGameEngine::FillTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, Pixel p)
	{ FillTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p); }

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	void PixelGameEngine::FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p)
	{
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, p); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1);
		if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1);
		if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
		if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i < dx1) {
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
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;
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
		dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i < dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i < dx1) i++;
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

			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y > y3) return;
		}
	}

	void PixelGameEngine::FillTexturedTriangle(std::vector<olc::vf2d> vPoints, std::vector<olc::vf2d> vTex, std::vector<olc::Pixel> vColour, olc::Sprite* sprTex)
	{
		olc::vi2d p1 = vPoints[0];
		olc::vi2d p2 = vPoints[1];
		olc::vi2d p3 = vPoints[2];

		if (p2.y < p1.y){std::swap(p1.y, p2.y); std::swap(p1.x, p2.x); std::swap(vTex[0].x, vTex[1].x); std::swap(vTex[0].y, vTex[1].y); std::swap(vColour[0], vColour[1]);}
		if (p3.y < p1.y){std::swap(p1.y, p3.y); std::swap(p1.x, p3.x); std::swap(vTex[0].x, vTex[2].x); std::swap(vTex[0].y, vTex[2].y); std::swap(vColour[0], vColour[2]);}
		if (p3.y < p2.y){std::swap(p2.y, p3.y); std::swap(p2.x, p3.x); std::swap(vTex[1].x, vTex[2].x); std::swap(vTex[1].y, vTex[2].y); std::swap(vColour[1], vColour[2]);}

		olc::vi2d dPos1 = p2 - p1;
		olc::vf2d dTex1 = vTex[1] - vTex[0];
		int dcr1 = vColour[1].r - vColour[0].r;
		int dcg1 = vColour[1].g - vColour[0].g;
		int dcb1 = vColour[1].b - vColour[0].b;
		int dca1 = vColour[1].a - vColour[0].a;

		olc::vi2d dPos2 = p3 - p1;
		olc::vf2d dTex2 = vTex[2] - vTex[0];
		int dcr2 = vColour[2].r - vColour[0].r;
		int dcg2 = vColour[2].g - vColour[0].g;
		int dcb2 = vColour[2].b - vColour[0].b;
		int dca2 = vColour[2].a - vColour[0].a;

		float dax_step = 0, dbx_step = 0, dcr1_step = 0, dcr2_step = 0,	dcg1_step = 0, dcg2_step = 0, dcb1_step = 0, dcb2_step = 0,	dca1_step = 0, dca2_step = 0;
		olc::vf2d vTex1Step, vTex2Step;

		if (dPos1.y)
		{
			dax_step = dPos1.x / (float)abs(dPos1.y);
			vTex1Step = dTex1 / (float)abs(dPos1.y);
			dcr1_step = dcr1 / (float)abs(dPos1.y);
			dcg1_step = dcg1 / (float)abs(dPos1.y);
			dcb1_step = dcb1 / (float)abs(dPos1.y);
			dca1_step = dca1 / (float)abs(dPos1.y);
		}

		if (dPos2.y)
		{
			dbx_step = dPos2.x / (float)abs(dPos2.y);
			vTex2Step = dTex2 / (float)abs(dPos2.y);
			dcr2_step = dcr2 / (float)abs(dPos2.y);
			dcg2_step = dcg2 / (float)abs(dPos2.y);
			dcb2_step = dcb2 / (float)abs(dPos2.y);
			dca2_step = dca2 / (float)abs(dPos2.y);
		}

		olc::vi2d vStart;
		olc::vi2d vEnd;
		int vStartIdx;

		for (int pass = 0; pass < 2; pass++)
		{
			if (pass == 0)
			{
				vStart = p1; vEnd = p2;	vStartIdx = 0;
			}
			else
			{
				dPos1 = p3 - p2;
				dTex1 = vTex[2] - vTex[1];
				dcr1 = vColour[2].r - vColour[1].r;
				dcg1 = vColour[2].g - vColour[1].g;
				dcb1 = vColour[2].b - vColour[1].b;
				dca1 = vColour[2].a - vColour[1].a;
				dcr1_step = 0; dcg1_step = 0; dcb1_step = 0; dca1_step = 0;

				if (dPos2.y) dbx_step = dPos2.x / (float)abs(dPos2.y);
				if (dPos1.y)
				{
					dax_step = dPos1.x / (float)abs(dPos1.y);
					vTex1Step = dTex1 / (float)abs(dPos1.y);
					dcr1_step = dcr1 / (float)abs(dPos1.y);
					dcg1_step = dcg1 / (float)abs(dPos1.y);
					dcb1_step = dcb1 / (float)abs(dPos1.y);
					dca1_step = dca1 / (float)abs(dPos1.y);
				}

				vStart = p2; vEnd = p3; vStartIdx = 1;
			}

			if (dPos1.y)
			{
				for (int i = vStart.y; i <= vEnd.y; i++)
				{
					int ax = int(vStart.x + (float)(i - vStart.y) * dax_step);
					int bx = int(p1.x + (float)(i - p1.y) * dbx_step);

					olc::vf2d tex_s(vTex[vStartIdx].x + (float)(i - vStart.y) * vTex1Step.x, vTex[vStartIdx].y + (float)(i - vStart.y) * vTex1Step.y);
					olc::vf2d tex_e(vTex[0].x + (float)(i - p1.y) * vTex2Step.x, vTex[0].y + (float)(i - p1.y) * vTex2Step.y);

					olc::Pixel col_s(vColour[vStartIdx].r + uint8_t((float)(i - vStart.y) * dcr1_step), vColour[vStartIdx].g + uint8_t((float)(i - vStart.y) * dcg1_step),
						vColour[vStartIdx].b + uint8_t((float)(i - vStart.y) * dcb1_step), vColour[vStartIdx].a + uint8_t((float)(i - vStart.y) * dca1_step));

					olc::Pixel col_e(vColour[0].r + uint8_t((float)(i - p1.y) * dcr2_step), vColour[0].g + uint8_t((float)(i - p1.y) * dcg2_step),
						vColour[0].b + uint8_t((float)(i - p1.y) * dcb2_step), vColour[0].a + uint8_t((float)(i - p1.y) * dca2_step));

					if (ax > bx) { std::swap(ax, bx); std::swap(tex_s, tex_e); std::swap(col_s, col_e); }

					float tstep = 1.0f / ((float)(bx - ax));
					float t = 0.0f;

					for (int j = ax; j < bx; j++)
					{
						olc::Pixel pixel = PixelLerp(col_s, col_e, t);
						if (sprTex != nullptr) pixel *= sprTex->Sample(tex_s.lerp(tex_e, t));
						Draw(j, i, pixel);
						t += tstep;
					}
				}
			}
		}			
	}

	void PixelGameEngine::FillTexturedPolygon(const std::vector<olc::vf2d>& vPoints, const std::vector<olc::vf2d>& vTex, const std::vector<olc::Pixel>& vColour, olc::Sprite* sprTex, olc::DecalStructure structure)
	{
		if (structure == olc::DecalStructure::LINE)
		{
			return; // Meaningless, so do nothing
		}

		if (vPoints.size() < 3 || vTex.size() < 3 || vColour.size() < 3)
			return;

		if (structure == olc::DecalStructure::LIST)
		{			
			for (size_t tri = 0; tri < vPoints.size() / 3; tri++)
			{
				std::vector<olc::vf2d> vP = { vPoints[tri * 3 + 0], vPoints[tri * 3 + 1], vPoints[tri * 3 + 2] };
				std::vector<olc::vf2d> vT = { vTex[tri * 3 + 0], vTex[tri * 3 + 1], vTex[tri * 3 + 2] };
				std::vector<olc::Pixel> vC = { vColour[tri * 3 + 0], vColour[tri * 3 + 1], vColour[tri * 3 + 2] };
				FillTexturedTriangle(vP, vT, vC, sprTex);
			}
			return;
		}

		if (structure == olc::DecalStructure::STRIP)
		{
			for (size_t tri = 2; tri < vPoints.size(); tri++)
			{
				std::vector<olc::vf2d> vP = { vPoints[tri - 2], vPoints[tri-1], vPoints[tri] };
				std::vector<olc::vf2d> vT = { vTex[tri - 2], vTex[tri - 1], vTex[tri] };
				std::vector<olc::Pixel> vC = { vColour[tri - 2], vColour[tri - 1], vColour[tri] };
				FillTexturedTriangle(vP, vT, vC, sprTex);
			}
			return;
		}

		if (structure == olc::DecalStructure::FAN)
		{
			for (size_t tri = 2; tri < vPoints.size(); tri++)
			{
				std::vector<olc::vf2d> vP = { vPoints[0], vPoints[tri - 1], vPoints[tri] };
				std::vector<olc::vf2d> vT = { vTex[0], vTex[tri - 1], vTex[tri] };
				std::vector<olc::Pixel> vC = { vColour[0], vColour[tri - 1], vColour[tri] };
				FillTexturedTriangle(vP, vT, vC, sprTex);
			}
			return;
		}
	}


	void PixelGameEngine::DrawSprite(const olc::vi2d& pos, Sprite* sprite, uint32_t scale, uint8_t flip)
	{ DrawSprite(pos.x, pos.y, sprite, scale, flip); }

	void PixelGameEngine::DrawSprite(int32_t x, int32_t y, Sprite* sprite, uint32_t scale, uint8_t flip)
	{
		if (sprite == nullptr)
			return;

		int32_t fxs = 0, fxm = 1, fx = 0;
		int32_t fys = 0, fym = 1, fy = 0;
		if (flip & olc::Sprite::Flip::HORIZ) { fxs = sprite->width - 1; fxm = -1; }
		if (flip & olc::Sprite::Flip::VERT) { fys = sprite->height - 1; fym = -1; }

		if (scale > 1)
		{
			fx = fxs;
			for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
			{
				fy = fys;
				for (int32_t j = 0; j < sprite->height; j++, fy += fym)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
							Draw(x + (i * scale) + is, y + (j * scale) + js, sprite->GetPixel(fx, fy));
			}
		}
		else
		{
			fx = fxs;
			for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
			{
				fy = fys;
				for (int32_t j = 0; j < sprite->height; j++, fy += fym)
					Draw(x + i, y + j, sprite->GetPixel(fx, fy));
			}
		}
	}

	void PixelGameEngine::DrawPartialSprite(const olc::vi2d& pos, Sprite* sprite, const olc::vi2d& sourcepos, const olc::vi2d& size, uint32_t scale, uint8_t flip)
	{ DrawPartialSprite(pos.x, pos.y, sprite, sourcepos.x, sourcepos.y, size.x, size.y, scale, flip); }

	void PixelGameEngine::DrawPartialSprite(int32_t x, int32_t y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale, uint8_t flip)
	{
		if (sprite == nullptr)
			return;

		int32_t fxs = 0, fxm = 1, fx = 0;
		int32_t fys = 0, fym = 1, fy = 0;
		if (flip & olc::Sprite::Flip::HORIZ) { fxs = w - 1; fxm = -1; }
		if (flip & olc::Sprite::Flip::VERT) { fys = h - 1; fym = -1; }

		if (scale > 1)
		{
			fx = fxs;
			for (int32_t i = 0; i < w; i++, fx += fxm)
			{
				fy = fys;
				for (int32_t j = 0; j < h; j++, fy += fym)
					for (uint32_t is = 0; is < scale; is++)
						for (uint32_t js = 0; js < scale; js++)
							Draw(x + (i * scale) + is, y + (j * scale) + js, sprite->GetPixel(fx + ox, fy + oy));
			}
		}
		else
		{
			fx = fxs;
			for (int32_t i = 0; i < w; i++, fx += fxm)
			{
				fy = fys;
				for (int32_t j = 0; j < h; j++, fy += fym)
					Draw(x + i, y + j, sprite->GetPixel(fx + ox, fy + oy));
			}
		}
	}

	void PixelGameEngine::SetDecalMode(const olc::DecalMode& mode)
	{ nDecalMode = mode; }

	void PixelGameEngine::SetDecalStructure(const olc::DecalStructure& structure)
	{ nDecalStructure = structure; }

	void PixelGameEngine::DrawPartialDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale, const olc::Pixel& tint)
	{
		olc::vf2d vScreenSpacePos =
		{
			  (pos.x * vInvScreenSize.x) * 2.0f - 1.0f,
			-((pos.y * vInvScreenSize.y) * 2.0f - 1.0f)
		};

		
		olc::vf2d vScreenSpaceDim =
		{
			  ((pos.x + source_size.x * scale.x) * vInvScreenSize.x) * 2.0f - 1.0f,
			-(((pos.y + source_size.y * scale.y) * vInvScreenSize.y) * 2.0f - 1.0f)
		};

		olc::vf2d vWindow = olc::vf2d(vViewSize);
		olc::vf2d vQuantisedPos = ((vScreenSpacePos * vWindow) + olc::vf2d(0.5f, 0.5f)).floor() / vWindow;
		olc::vf2d vQuantisedDim = ((vScreenSpaceDim * vWindow) + olc::vf2d(0.5f, -0.5f)).ceil() / vWindow;

		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.pos = { { vQuantisedPos.x, vQuantisedPos.y }, { vQuantisedPos.x, vQuantisedDim.y }, { vQuantisedDim.x, vQuantisedDim.y }, { vQuantisedDim.x, vQuantisedPos.y } };
		olc::vf2d uvtl = (source_pos + olc::vf2d(0.0001f, 0.0001f)) * decal->vUVScale;
		olc::vf2d uvbr = (source_pos + source_size - olc::vf2d(0.0001f, 0.0001f)) * decal->vUVScale;
		di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
		di.w = { 1,1,1,1 };
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

	void PixelGameEngine::DrawPartialDecal(const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{
		olc::vf2d vScreenSpacePos =
		{
			(pos.x * vInvScreenSize.x) * 2.0f - 1.0f,
			((pos.y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f
		};

		olc::vf2d vScreenSpaceDim =
		{
			vScreenSpacePos.x + (2.0f * size.x * vInvScreenSize.x),
			vScreenSpacePos.y - (2.0f * size.y * vInvScreenSize.y)
		};

		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.pos = { { vScreenSpacePos.x, vScreenSpacePos.y }, { vScreenSpacePos.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpacePos.y } };
		olc::vf2d uvtl = (source_pos) * decal->vUVScale;
		olc::vf2d uvbr = uvtl + ((source_size) * decal->vUVScale);
		di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
		di.w = { 1,1,1,1 };
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}


	void PixelGameEngine::DrawDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& scale, const olc::Pixel& tint)
	{
		olc::vf2d vScreenSpacePos =
		{
			(pos.x * vInvScreenSize.x) * 2.0f - 1.0f,
			((pos.y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f
		};

		olc::vf2d vScreenSpaceDim =
		{
			vScreenSpacePos.x + (2.0f * (float(decal->sprite->width) * vInvScreenSize.x)) * scale.x,
			vScreenSpacePos.y - (2.0f * (float(decal->sprite->height) * vInvScreenSize.y)) * scale.y
		};

		DecalInstance di;
		di.decal = decal;
		di.points = 4;
		di.tint = { tint, tint, tint, tint };
		di.pos = { { vScreenSpacePos.x, vScreenSpacePos.y }, { vScreenSpacePos.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpacePos.y } };
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		di.w = { 1, 1, 1, 1 };
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

	void PixelGameEngine::DrawExplicitDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d* uv, const olc::Pixel* col, uint32_t elements)
	{
		DecalInstance di;
		di.decal = decal;
		di.pos.resize(elements);
		di.uv.resize(elements);
		di.w.resize(elements);
		di.tint.resize(elements);
		di.points = elements;
		for (uint32_t i = 0; i < elements; i++)
		{
			di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = col[i];
			di.w[i] = 1.0f;
		}
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

	void PixelGameEngine::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const olc::Pixel tint)
	{
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);
		for (uint32_t i = 0; i < di.points; i++)
		{
			di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = tint;
			di.w[i] = 1.0f;
		}
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

	void PixelGameEngine::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel> &tint)
	{
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);
		for (uint32_t i = 0; i < di.points; i++)
		{
			di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = tint[i];
			di.w[i] = 1.0f;
		}
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

	void PixelGameEngine::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& colours, const olc::Pixel tint)
	{
		std::vector<olc::Pixel> newColours(colours.size(), olc::WHITE);
		std::transform(colours.begin(), colours.end(), newColours.begin(),
			[&tint](const olc::Pixel pin) {	return pin * tint; });
		DrawPolygonDecal(decal, pos, uv, newColours);
	}


	void PixelGameEngine::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<float>& depth, const std::vector<olc::vf2d>& uv, const olc::Pixel tint)
	{
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);
		for (uint32_t i = 0; i < di.points; i++)
		{
			di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = tint;
			di.w[i] = depth[i];
		}
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

	void PixelGameEngine::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<float>& depth, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& colours, const olc::Pixel tint)
	{
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);
		for (uint32_t i = 0; i < di.points; i++)
		{
			di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[i] = uv[i];
			di.tint[i] = colours[i] * tint;
			di.w[i] = depth[i];
		}
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

#ifdef OLC_ENABLE_EXPERIMENTAL
	// Lightweight 3D
	void PixelGameEngine::LW3D_DrawTriangles(olc::Decal* decal, const std::vector<std::array<float, 3>>& pos, const std::vector<olc::vf2d>& tex, const std::vector<olc::Pixel>& col)
	{
		DecalInstance di;
		di.decal = decal;
		di.points = uint32_t(pos.size());
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.z.resize(di.points);
		di.tint.resize(di.points);
		for (uint32_t i = 0; i < di.points; i++)
		{
			di.pos[i] = { pos[i][0], pos[i][1] };
			di.w[i] = pos[i][2];
			di.z[i] = pos[i][2];
			di.uv[i] = tex[i];
			di.tint[i] = col[i];			
		}
		di.mode = nDecalMode;
		di.structure = DecalStructure::LIST;
		di.depth = true;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

	void PixelGameEngine::LW3D_DrawWarpedDecal(olc::Decal* decal, const std::vector<std::array<float, 3>>& pos, const olc::Pixel& tint)
	{
		// Thanks Nathan Reed, a brilliant article explaining whats going on here
		// http://www.reedbeta.com/blog/quadrilateral-interpolation-part-1/
		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.w = { 1, 1, 1, 1 };
		di.z = { 1, 1, 1, 1 };
		di.pos.resize(4);
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		olc::vf2d center;
		float rd = ((pos[2][0] - pos[0][0]) * (pos[3][1] - pos[1][1]) - (pos[3][0] - pos[1][0]) * (pos[2][1] - pos[0][1]));
		if (rd != 0)
		{
			rd = 1.0f / rd;
			float rn = ((pos[3][0] - pos[1][0]) * (pos[0][1] - pos[1][1]) - (pos[3][1] - pos[1][1]) * (pos[0][0] - pos[1][0])) * rd;
			float sn = ((pos[2][0] - pos[0][0]) * (pos[0][1] - pos[1][1]) - (pos[2][1] - pos[0][1]) * (pos[0][0] - pos[1][0])) * rd;
			if (!(rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f))
			{
				center.x = pos[0][0] + rn * (pos[2][0] - pos[0][0]);
				center.y = pos[0][1] + rn * (pos[2][1] - pos[0][1]);
			}
			float d[4];
			for (int i = 0; i < 4; i++)
				d[i] = std::sqrt((pos[i][0] - center.x) * (pos[i][0] - center.x) + (pos[i][1] - center.y) * (pos[i][1] - center.y));

			for (int i = 0; i < 4; i++)
			{
				float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
				di.uv[i] *= q; 
				di.w[i] *= q;
				di.z[i] = pos[i][2];
				di.pos[i] = { (pos[i][0] * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i][1] * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			}
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			di.depth = true;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}
	}
#endif

	void PixelGameEngine::DrawLineDecal(const olc::vf2d& pos1, const olc::vf2d& pos2, Pixel p)
	{
		auto m = nDecalMode;
		nDecalMode = olc::DecalMode::WIREFRAME;
		DrawPolygonDecal(nullptr, { pos1, pos2 }, { {0, 0}, {0,0} }, p);
		nDecalMode = m;

		/*DecalInstance di;
		di.decal = nullptr;
		di.points = uint32_t(2);
		di.pos.resize(di.points);
		di.uv.resize(di.points);
		di.w.resize(di.points);
		di.tint.resize(di.points);
		di.pos[0] = { (pos1.x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos1.y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
		di.uv[0] = { 0.0f, 0.0f };
		di.tint[0] = p;
		di.w[0] = 1.0f;
		di.pos[1] = { (pos2.x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos2.y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
		di.uv[1] = { 0.0f, 0.0f };
		di.tint[1] = p;
		di.w[1] = 1.0f;
		di.mode = olc::DecalMode::WIREFRAME;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);*/
	}

	void PixelGameEngine::DrawRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col)
	{
		auto m = nDecalMode;
		SetDecalMode(olc::DecalMode::WIREFRAME);
		olc::vf2d vNewSize = size;// (size - olc::vf2d(0.375f, 0.375f)).ceil();
		std::array<olc::vf2d, 4> points = { { {pos}, {pos.x, pos.y + vNewSize.y}, {pos + vNewSize}, {pos.x + vNewSize.x, pos.y} } };
		std::array<olc::vf2d, 4> uvs = { {{0,0},{0,0},{0,0},{0,0}} };
		std::array<olc::Pixel, 4> cols = { {col, col, col, col} };
		DrawExplicitDecal(nullptr, points.data(), uvs.data(), cols.data(), 4);
		SetDecalMode(m);

	}

	void PixelGameEngine::FillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col)
	{
		olc::vf2d vNewSize = size;// (size - olc::vf2d(0.375f, 0.375f)).ceil();
		std::array<olc::vf2d, 4> points = { { {pos}, {pos.x, pos.y + vNewSize.y}, {pos + vNewSize}, {pos.x + vNewSize.x, pos.y} } };
		std::array<olc::vf2d, 4> uvs = { {{0,0},{0,0},{0,0},{0,0}} };
		std::array<olc::Pixel, 4> cols = { {col, col, col, col} };
		DrawExplicitDecal(nullptr, points.data(), uvs.data(), cols.data(), 4);
	}

	void PixelGameEngine::GradientFillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel colTL, const olc::Pixel colBL, const olc::Pixel colBR, const olc::Pixel colTR)
	{
		std::array<olc::vf2d, 4> points = { { {pos}, {pos.x, pos.y + size.y}, {pos + size}, {pos.x + size.x, pos.y} } };
		std::array<olc::vf2d, 4> uvs = { {{0,0},{0,0},{0,0},{0,0}} };
		std::array<olc::Pixel, 4> cols = { {colTL, colBL, colBR, colTR} };
		DrawExplicitDecal(nullptr, points.data(), uvs.data(), cols.data(), 4);
	}

	void PixelGameEngine::FillTriangleDecal(const olc::vf2d& p0, const olc::vf2d& p1, const olc::vf2d& p2, const olc::Pixel col)
	{		
		std::array<olc::vf2d, 4> points = { { p0, p1, p2 } };
		std::array<olc::vf2d, 4> uvs = { {{0,0},{0,0},{0,0}} };
		std::array<olc::Pixel, 4> cols = { {col, col, col} };
		DrawExplicitDecal(nullptr, points.data(), uvs.data(), cols.data(), 3);
	}

	void PixelGameEngine::GradientTriangleDecal(const olc::vf2d& p0, const olc::vf2d& p1, const olc::vf2d& p2, const olc::Pixel c0, const olc::Pixel c1, const olc::Pixel c2)
	{
		std::array<olc::vf2d, 4> points = { { p0, p1, p2 } };
		std::array<olc::vf2d, 4> uvs = { {{0,0},{0,0},{0,0}} };
		std::array<olc::Pixel, 4> cols = { {c0, c1, c2} };
		DrawExplicitDecal(nullptr, points.data(), uvs.data(), cols.data(), 3);
	}

	void PixelGameEngine::DrawRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center, const olc::vf2d& scale, const olc::Pixel& tint)
	{
		DecalInstance di;
		di.decal = decal;
		di.pos.resize(4);
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		di.w = { 1, 1, 1, 1 };
		di.tint = { tint, tint, tint, tint };
		di.points = 4;
		di.pos[0] = (olc::vf2d(0.0f, 0.0f) - center) * scale;
		di.pos[1] = (olc::vf2d(0.0f, float(decal->sprite->height)) - center) * scale;
		di.pos[2] = (olc::vf2d(float(decal->sprite->width), float(decal->sprite->height)) - center) * scale;
		di.pos[3] = (olc::vf2d(float(decal->sprite->width), 0.0f) - center) * scale;
		float c = cos(fAngle), s = sin(fAngle);
		for (int i = 0; i < 4; i++)
		{
			di.pos[i] = pos + olc::vf2d(di.pos[i].x * c - di.pos[i].y * s, di.pos[i].x * s + di.pos[i].y * c);
			di.pos[i] = di.pos[i] * vInvScreenSize * 2.0f - olc::vf2d(1.0f, 1.0f);
			di.pos[i].y *= -1.0f;
			di.w[i] = 1;
		}
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}


	void PixelGameEngine::DrawPartialRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale, const olc::Pixel& tint)
	{
		DecalInstance di;
		di.decal = decal;
		di.points = 4;
		di.tint = { tint, tint, tint, tint };
		di.w = { 1, 1, 1, 1 };
		di.pos.resize(4);
		di.pos[0] = (olc::vf2d(0.0f, 0.0f) - center) * scale;
		di.pos[1] = (olc::vf2d(0.0f, source_size.y) - center) * scale;
		di.pos[2] = (olc::vf2d(source_size.x, source_size.y) - center) * scale;
		di.pos[3] = (olc::vf2d(source_size.x, 0.0f) - center) * scale;
		float c = cos(fAngle), s = sin(fAngle);
		for (int i = 0; i < 4; i++)
		{
			di.pos[i] = pos + olc::vf2d(di.pos[i].x * c - di.pos[i].y * s, di.pos[i].x * s + di.pos[i].y * c);
			di.pos[i] = di.pos[i] * vInvScreenSize * 2.0f - olc::vf2d(1.0f, 1.0f);
			di.pos[i].y *= -1.0f;
		}

		olc::vf2d uvtl = source_pos * decal->vUVScale;
		olc::vf2d uvbr = uvtl + (source_size * decal->vUVScale);
		di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
		di.mode = nDecalMode;
		di.structure = nDecalStructure;
		vLayers[nTargetLayer].vecDecalInstance.push_back(di);
	}

	void PixelGameEngine::DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{
		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.w = { 1, 1, 1, 1 };
		di.pos.resize(4);
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		olc::vf2d center;
		float rd = ((pos[2].x - pos[0].x) * (pos[3].y - pos[1].y) - (pos[3].x - pos[1].x) * (pos[2].y - pos[0].y));
		if (rd != 0)
		{
			olc::vf2d uvtl = source_pos * decal->vUVScale;
			olc::vf2d uvbr = uvtl + (source_size * decal->vUVScale);
			di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };

			rd = 1.0f / rd;
			float rn = ((pos[3].x - pos[1].x) * (pos[0].y - pos[1].y) - (pos[3].y - pos[1].y) * (pos[0].x - pos[1].x)) * rd;
			float sn = ((pos[2].x - pos[0].x) * (pos[0].y - pos[1].y) - (pos[2].y - pos[0].y) * (pos[0].x - pos[1].x)) * rd;
			if (!(rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f)) center = pos[0] + rn * (pos[2] - pos[0]);
			float d[4];	for (int i = 0; i < 4; i++)	d[i] = (pos[i] - center).mag();
			for (int i = 0; i < 4; i++)
			{
				float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
				di.uv[i] *= q; di.w[i] *= q;
				di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			}
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}
	}

	void PixelGameEngine::DrawWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::Pixel& tint)
	{
		// Thanks Nathan Reed, a brilliant article explaining whats going on here
		// http://www.reedbeta.com/blog/quadrilateral-interpolation-part-1/
		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.w = { 1, 1, 1, 1 };
		di.pos.resize(4);
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		olc::vf2d center;
		float rd = ((pos[2].x - pos[0].x) * (pos[3].y - pos[1].y) - (pos[3].x - pos[1].x) * (pos[2].y - pos[0].y));
		if (rd != 0)
		{
			rd = 1.0f / rd;
			float rn = ((pos[3].x - pos[1].x) * (pos[0].y - pos[1].y) - (pos[3].y - pos[1].y) * (pos[0].x - pos[1].x)) * rd;
			float sn = ((pos[2].x - pos[0].x) * (pos[0].y - pos[1].y) - (pos[2].y - pos[0].y) * (pos[0].x - pos[1].x)) * rd;
			if (!(rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f)) center = pos[0] + rn * (pos[2] - pos[0]);
			float d[4];	for (int i = 0; i < 4; i++)	d[i] = (pos[i] - center).mag();
			for (int i = 0; i < 4; i++)
			{
				float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
				di.uv[i] *= q; di.w[i] *= q;
				di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			}
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}
	}

	void PixelGameEngine::DrawWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::Pixel& tint)
	{ DrawWarpedDecal(decal, pos.data(), tint); }

	void PixelGameEngine::DrawWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::Pixel& tint)
	{ DrawWarpedDecal(decal, &pos[0], tint); }

	void PixelGameEngine::DrawPartialWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{ DrawPartialWarpedDecal(decal, pos.data(), source_pos, source_size, tint); }

	void PixelGameEngine::DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{ DrawPartialWarpedDecal(decal, &pos[0], source_pos, source_size, tint); }

	void PixelGameEngine::DrawStringDecal(const olc::vf2d& pos, const std::string& sText, const Pixel col, const olc::vf2d& scale)
	{
		olc::vf2d spos = { 0.0f, 0.0f };
		for (auto c : sText)
		{
			if (c == '\n')
			{
				spos.x = 0; spos.y += 8.0f * scale.y;
			}
			else if (c == '\t')
			{
				spos.x += 8.0f * float(nTabSizeInSpaces) * scale.x;
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;
				DrawPartialDecal(pos + spos, fontRenderable.Decal(), {float(ox) * 8.0f, float(oy) * 8.0f}, {8.0f, 8.0f}, scale, col);
				spos.x += 8.0f * scale.x;
			}
		}
	}

	void PixelGameEngine::DrawStringPropDecal(const olc::vf2d& pos, const std::string& sText, const Pixel col, const olc::vf2d& scale)
	{
		olc::vf2d spos = { 0.0f, 0.0f };
		for (auto c : sText)
		{
			if (c == '\n')
			{
				spos.x = 0; spos.y += 8.0f * scale.y;
			}
			else if (c == '\t')
			{
				spos.x += 8.0f * float(nTabSizeInSpaces) * scale.x;
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;
				DrawPartialDecal(pos + spos, fontRenderable.Decal(), { float(ox) * 8.0f + float(vFontSpacing[c - 32].x), float(oy) * 8.0f }, { float(vFontSpacing[c - 32].y), 8.0f }, scale, col);
				spos.x += float(vFontSpacing[c - 32].y) * scale.x;
			}
		}
	}
	// Thanks Oso-Grande/Sopadeoso For these awesom and stupidly clever Text Rotation routines... duh XD
	void PixelGameEngine::DrawRotatedStringDecal(const olc::vf2d& pos, const std::string& sText, const float fAngle, const olc::vf2d& center, const Pixel col, const olc::vf2d& scale)
	{
		olc::vf2d spos = center;
		for (auto c : sText)
		{
			if (c == '\n')
			{
				spos.x = center.x; spos.y -= 8.0f;
			}
			else if (c == '\t')
			{
				spos.x += 8.0f * float(nTabSizeInSpaces) * scale.x;
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;
				DrawPartialRotatedDecal(pos, fontRenderable.Decal(), fAngle, spos, { float(ox) * 8.0f, float(oy) * 8.0f }, { 8.0f, 8.0f }, scale, col);
				spos.x -= 8.0f;
			}
		}
	}

	void PixelGameEngine::DrawRotatedStringPropDecal(const olc::vf2d& pos, const std::string& sText, const float fAngle, const olc::vf2d& center, const Pixel col, const olc::vf2d& scale)
	{
		olc::vf2d spos = center;
		for (auto c : sText)
		{
			if (c == '\n')
			{
				spos.x = center.x; spos.y -= 8.0f;
			}
			else if (c == '\t')
			{
				spos.x += 8.0f * float(nTabSizeInSpaces) * scale.x;
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;
				DrawPartialRotatedDecal(pos, fontRenderable.Decal(), fAngle, spos, { float(ox) * 8.0f + float(vFontSpacing[c - 32].x), float(oy) * 8.0f }, { float(vFontSpacing[c - 32].y), 8.0f }, scale, col);
				spos.x -= float(vFontSpacing[c - 32].y);
			}
		}
	}

	olc::vi2d PixelGameEngine::GetTextSize(const std::string& s)
	{
		olc::vi2d size = { 0,1 };
		olc::vi2d pos = { 0,1 };
		for (auto c : s)
		{
			if (c == '\n') { pos.y++;  pos.x = 0; }
			else if (c == '\t') { pos.x += nTabSizeInSpaces; }
			else pos.x++;
			size.x = std::max(size.x, pos.x);
			size.y = std::max(size.y, pos.y);
		}
		return size * 8;
	}

	void PixelGameEngine::DrawString(const olc::vi2d& pos, const std::string& sText, Pixel col, uint32_t scale)
	{ DrawString(pos.x, pos.y, sText, col, scale); }

	void PixelGameEngine::DrawString(int32_t x, int32_t y, const std::string& sText, Pixel col, uint32_t scale)
	{
		int32_t sx = 0;
		int32_t sy = 0;
		Pixel::Mode m = nPixelMode;
		// Thanks @tucna, spotted bug with col.ALPHA :P
		if (m != Pixel::CUSTOM) // Thanks @Megarev, required for "shaders"
		{
			if (col.a != 255)		SetPixelMode(Pixel::ALPHA);
			else					SetPixelMode(Pixel::MASK);
		}
		for (auto c : sText)
		{
			if (c == '\n')
			{
				sx = 0; sy += 8 * scale;
			}
			else if (c == '\t')
			{
				sx += 8 * nTabSizeInSpaces * scale;
			}
			else			
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1)
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (fontRenderable.Sprite()->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								for (uint32_t is = 0; is < scale; is++)
									for (uint32_t js = 0; js < scale; js++)
										Draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, col);
				}
				else
				{
					for (uint32_t i = 0; i < 8; i++)
						for (uint32_t j = 0; j < 8; j++)
							if (fontRenderable.Sprite()->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								Draw(x + sx + i, y + sy + j, col);
				}
				sx += 8 * scale;
			}
		}
		SetPixelMode(m);
	}

	olc::vi2d PixelGameEngine::GetTextSizeProp(const std::string& s)
	{
		olc::vi2d size = { 0,1 };
		olc::vi2d pos = { 0,1 };
		for (auto c : s)
		{
			if (c == '\n') { pos.y += 1;  pos.x = 0; }
			else if (c == '\t') { pos.x += nTabSizeInSpaces * 8; }
			else pos.x += vFontSpacing[c - 32].y;
			size.x = std::max(size.x, pos.x);
			size.y = std::max(size.y, pos.y);
		}

		size.y *= 8;
		return size;
	}

	void PixelGameEngine::DrawStringProp(const olc::vi2d& pos, const std::string& sText, Pixel col, uint32_t scale)
	{ DrawStringProp(pos.x, pos.y, sText, col, scale); }

	void PixelGameEngine::DrawStringProp(int32_t x, int32_t y, const std::string& sText, Pixel col, uint32_t scale)
	{
		int32_t sx = 0;
		int32_t sy = 0;
		Pixel::Mode m = nPixelMode;

		if (m != Pixel::CUSTOM)
		{
			if (col.a != 255)		SetPixelMode(Pixel::ALPHA);
			else					SetPixelMode(Pixel::MASK);
		}
		for (auto c : sText)
		{
			if (c == '\n')
			{
				sx = 0; sy += 8 * scale;
			}
			else if (c == '\t')
			{
				sx += 8 * nTabSizeInSpaces * scale;
			}
			else
			{
				int32_t ox = (c - 32) % 16;
				int32_t oy = (c - 32) / 16;

				if (scale > 1)
				{
					for (int32_t i = 0; i < vFontSpacing[c - 32].y; i++)
						for (int32_t j = 0; j < 8; j++)
							if (fontRenderable.Sprite()->GetPixel(i + ox * 8 + vFontSpacing[c - 32].x, j + oy * 8).r > 0)
								for (int32_t is = 0; is < int(scale); is++)
									for (int32_t js = 0; js < int(scale); js++)
										Draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, col);
				}
				else
				{
					for (int32_t i = 0; i < vFontSpacing[c - 32].y; i++)
						for (int32_t j = 0; j < 8; j++)
							if (fontRenderable.Sprite()->GetPixel(i + ox * 8 + vFontSpacing[c - 32].x, j + oy * 8).r > 0)
								Draw(x + sx + i, y + sy + j, col);
				}
				sx += vFontSpacing[c - 32].y * scale;
			}
		}
		SetPixelMode(m);
	}

	void PixelGameEngine::SetPixelMode(Pixel::Mode m)
	{ nPixelMode = m; }

	Pixel::Mode PixelGameEngine::GetPixelMode()
	{ return nPixelMode; }

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

	std::stringstream& PixelGameEngine::ConsoleOut()
	{ return ssConsoleOutput; }

	bool PixelGameEngine::IsConsoleShowing() const
	{ return bConsoleShow; }

	void PixelGameEngine::ConsoleShow(const olc::Key& keyExit, bool bSuspendTime)
	{
		if (bConsoleShow)
			return;

		bConsoleShow = true;		
		bConsoleSuspendTime = bSuspendTime;
		TextEntryEnable(true);
		keyConsoleExit = keyExit;
		pKeyboardState[keyConsoleExit].bHeld = false;
		pKeyboardState[keyConsoleExit].bPressed = false;
		pKeyboardState[keyConsoleExit].bReleased = true;
	}
	
	void PixelGameEngine::ConsoleClear()
	{ sConsoleLines.clear(); }

	void PixelGameEngine::ConsoleCaptureStdOut(const bool bCapture)
	{
		if(bCapture)
			sbufOldCout = std::cout.rdbuf(ssConsoleOutput.rdbuf());
		else
			std::cout.rdbuf(sbufOldCout);
	}

	void PixelGameEngine::UpdateConsole()
	{
		if (GetKey(keyConsoleExit).bPressed)
		{
			TextEntryEnable(false);
			bConsoleSuspendTime = false;
			bConsoleShow = false;
			return;
		}

		// Keep Console sizes based in real screen dimensions
		vConsoleCharacterScale = olc::vf2d(1.0f, 2.0f) / (olc::vf2d(vViewSize) * vInvScreenSize);
		vConsoleSize = (vViewSize / olc::vi2d(8, 16)) - olc::vi2d(2, 4);

		// If console has changed size, simply reset it
		if (vConsoleSize.y != sConsoleLines.size())
		{
			vConsoleCursor = { 0,0 };
			sConsoleLines.clear();
			sConsoleLines.resize(vConsoleSize.y);
		}

		auto TypeCharacter = [&](const char c)
		{
			if (c >= 32 && c < 127)
			{
				sConsoleLines[vConsoleCursor.y].append(1, c);
				vConsoleCursor.x++;
			}

			if( c == '\n' || vConsoleCursor.x >= vConsoleSize.x)
			{
				vConsoleCursor.y++; vConsoleCursor.x = 0;				
			}			

			if (vConsoleCursor.y >= vConsoleSize.y)
			{
				vConsoleCursor.y = vConsoleSize.y - 1;
				for (int i = 1; i < vConsoleSize.y; i++)
					sConsoleLines[i - 1] = sConsoleLines[i];
				sConsoleLines[vConsoleCursor.y].clear();
			}
		};

		// Empty out "std::cout", parsing as we go
		while (ssConsoleOutput.rdbuf()->sgetc() != -1)
		{
			char c = ssConsoleOutput.rdbuf()->sbumpc();
			TypeCharacter(c);
		}

		// Draw Shadow
		GradientFillRectDecal({ 0,0 }, olc::vf2d(vScreenSize), olc::PixelF(0, 0, 0.5f, 0.5f), olc::PixelF(0, 0, 0.25f, 0.5f), olc::PixelF(0, 0, 0.25f, 0.5f), olc::PixelF(0, 0, 0.25f, 0.5f));
				
		// Draw the console buffer
		SetDecalMode(olc::DecalMode::NORMAL);
		for (int32_t nLine = 0; nLine < vConsoleSize.y; nLine++)
			DrawStringDecal(olc::vf2d( 1, 1 + float(nLine) ) * vConsoleCharacterScale * 8.0f, sConsoleLines[nLine], olc::WHITE, vConsoleCharacterScale);

		// Draw Input State
		FillRectDecal(olc::vf2d(1 + float((TextEntryGetCursor() + 1)), 1 + float((vConsoleSize.y - 1))) * vConsoleCharacterScale * 8.0f, olc::vf2d(8, 8) * vConsoleCharacterScale, olc::DARK_CYAN);
		DrawStringDecal(olc::vf2d(1, 1 + float((vConsoleSize.y - 1))) * vConsoleCharacterScale * 8.0f, std::string(">") + TextEntryGetString(), olc::YELLOW, vConsoleCharacterScale);		
	}


	const std::vector<std::string>& PixelGameEngine::GetDroppedFiles() const
	{ return vDroppedFiles;	}

	const olc::vi2d& PixelGameEngine::GetDroppedFilesPoint() const
	{ return vDroppedFilesPoint; }


	void PixelGameEngine::TextEntryEnable(const bool bEnable, const std::string& sText)
	{
		if (bEnable)
		{
			nTextEntryCursor = int32_t(sText.size());
			sTextEntryString = sText;
			bTextEntryEnable = true;
		}
		else
		{
			bTextEntryEnable = false;
		}
	}

	std::string PixelGameEngine::TextEntryGetString() const
	{ return sTextEntryString; }

	int32_t PixelGameEngine::TextEntryGetCursor() const
	{ return nTextEntryCursor; }

	bool PixelGameEngine::IsTextEntryEnabled() const
	{ return bTextEntryEnable; }


	void PixelGameEngine::UpdateTextEntry()
	{
		// Check for typed characters
		for (const auto& key : vKeyboardMap)
			if (GetKey(std::get<0>(key)).bPressed)
			{
				sTextEntryString.insert(nTextEntryCursor, GetKey(olc::Key::SHIFT).bHeld ? std::get<2>(key) : std::get<1>(key));
				nTextEntryCursor++;
			}

		// Check for command characters
		if (GetKey(olc::Key::LEFT).bPressed)
			nTextEntryCursor = std::max(0, nTextEntryCursor - 1);
		if (GetKey(olc::Key::RIGHT).bPressed)
			nTextEntryCursor = std::min(int32_t(sTextEntryString.size()), nTextEntryCursor + 1);
		if (GetKey(olc::Key::BACK).bPressed && nTextEntryCursor > 0)
		{
			sTextEntryString.erase(nTextEntryCursor-1, 1);
			nTextEntryCursor = std::max(0, nTextEntryCursor - 1);
		}
		if (GetKey(olc::Key::DEL).bPressed && size_t(nTextEntryCursor) < sTextEntryString.size())
			sTextEntryString.erase(nTextEntryCursor, 1);	

		if (GetKey(olc::Key::UP).bPressed)
		{
			if (!sCommandHistory.empty())
			{
				if (sCommandHistoryIt != sCommandHistory.begin())
					sCommandHistoryIt--;

				nTextEntryCursor = int32_t(sCommandHistoryIt->size());
				sTextEntryString = *sCommandHistoryIt;
			}
		}

		if (GetKey(olc::Key::DOWN).bPressed)
		{	
			if (!sCommandHistory.empty())
			{
				if (sCommandHistoryIt != sCommandHistory.end())
				{
					sCommandHistoryIt++;
					if (sCommandHistoryIt != sCommandHistory.end())
					{
						nTextEntryCursor = int32_t(sCommandHistoryIt->size());
						sTextEntryString = *sCommandHistoryIt;
					}
					else
					{
						nTextEntryCursor = 0;
						sTextEntryString = "";
					}
				}
			}
		}

		if (GetKey(olc::Key::ENTER).bPressed)
		{
			if (bConsoleShow)
			{
				std::cout << ">" + sTextEntryString + "\n";
				if (OnConsoleCommand(sTextEntryString))
				{
					sCommandHistory.push_back(sTextEntryString);
					sCommandHistoryIt = sCommandHistory.end();
				}
				sTextEntryString.clear();
				nTextEntryCursor = 0;
			}
			else
			{
				OnTextEntryComplete(sTextEntryString);
				TextEntryEnable(false);
			}
		}
	}

	// User must override these functions as required. I have not made
	// them abstract because I do need a default behaviour to occur if
	// they are not overwritten

	bool PixelGameEngine::OnUserCreate()
	{ return false;	}

	bool PixelGameEngine::OnUserUpdate(float fElapsedTime)
	{ UNUSED(fElapsedTime);  return false; }

	bool PixelGameEngine::OnUserDestroy()
	{ return true; }

	void PixelGameEngine::OnTextEntryComplete(const std::string& sText) { UNUSED(sText); }
	bool PixelGameEngine::OnConsoleCommand(const std::string& sCommand) { UNUSED(sCommand); return false; }
	

	olc::rcode PixelGameEngine::SetWindowSize(const olc::vi2d& vPos, const olc::vi2d& vSize)
	{
		if (platform)
			return platform->SetWindowSize(vPos, vSize);
		else
			return olc::FAIL;
	}
	
	olc::rcode PixelGameEngine::ShowWindowFrame(const bool bShowFrame)
	{
		if (platform)
			return platform->ShowWindowFrame(bShowFrame);
		else
			return olc::FAIL;
	}


	// Externalised API
	void PixelGameEngine::olc_UpdateViewport()
	{
		if (bRealWindowMode)
		{
			vPixelSize = { 1,1 };
			vViewSize = vScreenSize;
			vViewPos = { 0,0 };
			return;
		}

		int32_t ww = vScreenSize.x * vPixelSize.x;
		int32_t wh = vScreenSize.y * vPixelSize.y;
		float wasp = (float)ww / (float)wh;

		if (bPixelCohesion)
		{
			vScreenPixelSize = (vWindowSize / vScreenSize);
			vViewSize = (vWindowSize / vScreenSize) * vScreenSize;
		}
		else
		{
			vViewSize.x = (int32_t)vWindowSize.x;
			vViewSize.y = (int32_t)((float)vViewSize.x / wasp);

			if (vViewSize.y > vWindowSize.y)
			{
				vViewSize.y = vWindowSize.y;
				vViewSize.x = (int32_t)((float)vViewSize.y * wasp);
			}
		}

		vViewPos = (vWindowSize - vViewSize) / 2;
	}

	void PixelGameEngine::olc_UpdateWindowPos(int32_t x, int32_t y)
	{
		vWindowPos = { x, y };	
		olc_UpdateViewport();
	}

	void PixelGameEngine::olc_UpdateWindowSize(int32_t x, int32_t y)
	{
		vWindowSize = { x, y };

		if (bRealWindowMode)
		{
			vResizeRequested = vWindowSize;
			bResizeRequested = true;			
		}

		olc_UpdateViewport();
	}

	void PixelGameEngine::olc_UpdateMouseWheel(int32_t delta)
	{ nMouseWheelDeltaCache += delta; }

	void PixelGameEngine::olc_UpdateMouse(int32_t x, int32_t y)
	{
		// Mouse coords come in screen space
		// But leave in pixel space
		bHasMouseFocus = true;
		vMouseWindowPos = { x, y };
		// Full Screen mode may have a weird viewport we must clamp to
		x -= vViewPos.x;
		y -= vViewPos.y;
		vMousePosCache.x = (int32_t)(((float)x / (float)(vWindowSize.x - (vViewPos.x * 2)) * (float)vScreenSize.x));
		vMousePosCache.y = (int32_t)(((float)y / (float)(vWindowSize.y - (vViewPos.y * 2)) * (float)vScreenSize.y));
		if (vMousePosCache.x >= (int32_t)vScreenSize.x)	vMousePosCache.x = vScreenSize.x - 1;
		if (vMousePosCache.y >= (int32_t)vScreenSize.y)	vMousePosCache.y = vScreenSize.y - 1;
		if (vMousePosCache.x < 0) vMousePosCache.x = 0;
		if (vMousePosCache.y < 0) vMousePosCache.y = 0;
	}

	void PixelGameEngine::olc_UpdateMouseState(int32_t button, bool state)
	{ pMouseNewState[button] = state; }

	void PixelGameEngine::olc_UpdateKeyState(int32_t key, bool state)
	{ pKeyNewState[key] = state; }

	void PixelGameEngine::olc_UpdateMouseFocus(bool state)
	{ bHasMouseFocus = state; }

	void PixelGameEngine::olc_UpdateKeyFocus(bool state)
	{ bHasInputFocus = state; }

	void PixelGameEngine::olc_DropFiles(int32_t x, int32_t y, const std::vector<std::string>& vFiles)
	{ 
		x -= vViewPos.x;
		y -= vViewPos.y;
		vDroppedFilesPointCache.x = (int32_t)(((float)x / (float)(vWindowSize.x - (vViewPos.x * 2)) * (float)vScreenSize.x));
		vDroppedFilesPointCache.y = (int32_t)(((float)y / (float)(vWindowSize.y - (vViewPos.y * 2)) * (float)vScreenSize.y));
		if (vDroppedFilesPointCache.x >= (int32_t)vScreenSize.x)	vDroppedFilesPointCache.x = vScreenSize.x - 1;
		if (vDroppedFilesPointCache.y >= (int32_t)vScreenSize.y)	vDroppedFilesPointCache.y = vScreenSize.y - 1;
		if (vDroppedFilesPointCache.x < 0) vDroppedFilesPointCache.x = 0;
		if (vDroppedFilesPointCache.y < 0) vDroppedFilesPointCache.y = 0;
		vDroppedFilesCache = vFiles; 
	}

	void PixelGameEngine::olc_Reanimate()
	{ bAtomActive = true; }

	bool PixelGameEngine::olc_IsRunning()
	{ return bAtomActive; }

	void PixelGameEngine::olc_Terminate()
	{ bAtomActive = false; }

	void PixelGameEngine::EngineThread()
	{
		// Allow platform to do stuff here if needed, since its now in the
		// context of this thread
		if (platform->ThreadStartUp() == olc::FAIL)	return;

		// Do engine context specific initialisation
		olc_PrepareEngine();

		// Create user resources as part of this thread
		for (auto& ext : vExtensions) ext->OnBeforeUserCreate();
		if (!OnUserCreate()) bAtomActive = false;
		for (auto& ext : vExtensions) ext->OnAfterUserCreate();

		while (bAtomActive)
		{
			// Run as fast as possible
			while (bAtomActive) { olc_CoreUpdate(); }

			// Allow the user to free resources if they have overrided the destroy function
			if (!OnUserDestroy())
			{
				// User denied destroy for some reason, so continue running
				bAtomActive = true;
			}
		}

		platform->ThreadCleanUp();
	}

	void PixelGameEngine::olc_PrepareEngine()
	{
		// Start OpenGL, the context is owned by the game thread
		if (platform->CreateGraphics(bFullScreen, bEnableVSYNC, vViewPos, vViewSize) == olc::FAIL) return;

		// Construct default font sheet
		olc_ConstructFontSheet();

		// Create Primary Layer "0"
		CreateLayer();
		vLayers[0].bUpdate = true;
		vLayers[0].bShow = true;
		SetDrawTarget(nullptr);

		m_tp1 = std::chrono::system_clock::now();
		m_tp2 = std::chrono::system_clock::now();
	}


	void PixelGameEngine::adv_ManualRenderEnable(const bool bEnable)
	{
		bManualRenderEnable = bEnable;
	}

	void PixelGameEngine::adv_HardwareClip(const bool bClipAndScale, const olc::vi2d & viewPos, const olc::vi2d & viewSize, const bool bClear)
	{
		olc::vf2d vNewSize = olc::vf2d(viewSize) / olc::vf2d(vScreenSize);
		olc::vf2d vNewPos = olc::vf2d(viewPos) / olc::vf2d(vScreenSize);
		renderer->UpdateViewport(vViewPos + vNewPos * vViewSize, vNewSize * vViewSize);

		if (bClear)
			renderer->ClearBuffer(olc::BLACK, true);

		SetDecalMode(DecalMode::NORMAL);
		renderer->PrepareDrawing();

		if(!bClipAndScale)
			vInvScreenSize = 1.0f / olc::vf2d(viewSize);
		else
			vInvScreenSize = 1.0f / olc::vf2d(vScreenSize);
	}

	void PixelGameEngine::adv_FlushLayer(const size_t nLayerID)
	{
		auto& layer = vLayers[nLayerID];

		if (layer.bShow)
		{
			if (layer.funcHook == nullptr)
			{
				renderer->ApplyTexture(layer.pDrawTarget.Decal()->id);
				if (!bSuspendTextureTransfer)
				{
					layer.pDrawTarget.Decal()->Update();
					layer.bUpdate = false;
				}

				// Can't use this as it assumes full screen coords
				// renderer->DrawLayerQuad(layer.vOffset, layer.vScale, layer.tint);			
				// Instead, render a textured decal

				olc::vf2d vScreenSpacePos =
				{
					(layer.vOffset.x  * vInvScreenSize.x) * 2.0f - 1.0f,
					((layer.vOffset.y  * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f
				};

				olc::vf2d vScreenSpaceDim =
				{
					vScreenSpacePos.x + (2.0f * (float(layer.pDrawTarget.Sprite()->width) * vInvScreenSize.x)) * layer.vScale.x,
					vScreenSpacePos.y - (2.0f * (float(layer.pDrawTarget.Sprite()->height) * vInvScreenSize.y)) * layer.vScale.y
				};

				DecalInstance di;
				di.decal = layer.pDrawTarget.Decal();
				di.points = 4;
				di.tint = { olc::WHITE, olc::WHITE, olc::WHITE, olc::WHITE };
				di.pos = { { vScreenSpacePos.x, vScreenSpacePos.y }, { vScreenSpacePos.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpacePos.y } };
				di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
				di.w = { 1, 1, 1, 1 };
				di.mode = DecalMode::NORMAL;
				di.structure = DecalStructure::FAN;
				renderer->DrawDecal(di);
			}
			else
			{
				// Mwa ha ha.... Have Fun!!!
				layer.funcHook();
			}
		}
	}

	void PixelGameEngine::adv_FlushLayerDecals(const size_t nLayerID)
	{
		// Display Decals in order for this layer
		auto& layer = vLayers[nLayerID];
		for (auto& decal : layer.vecDecalInstance)
			renderer->DrawDecal(decal);
		layer.vecDecalInstance.clear();
	}



	void PixelGameEngine::olc_CoreUpdate()
	{
		// Handle Timing
		m_tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
		m_tp1 = m_tp2;

		// Our time per frame coefficient
		float fElapsedTime = elapsedTime.count();
		fLastElapsed = fElapsedTime;

		if (bConsoleSuspendTime)
			fElapsedTime = 0.0f;

		// Some platforms will need to check for events
		platform->HandleSystemEvent();

		// Compare hardware input states from previous frame
		auto ScanHardware = [&](HWButton* pKeys, bool* pStateOld, bool* pStateNew, uint32_t nKeyCount)
		{
			for (uint32_t i = 0; i < nKeyCount; i++)
			{
				pKeys[i].bPressed = false;
				pKeys[i].bReleased = false;
				if (pStateNew[i] != pStateOld[i])
				{
					if (pStateNew[i])
					{
						pKeys[i].bPressed = !pKeys[i].bHeld;
						pKeys[i].bHeld = true;
					}
					else
					{
						pKeys[i].bReleased = true;
						pKeys[i].bHeld = false;
					}
				}
				pStateOld[i] = pStateNew[i];
			}
		};

		ScanHardware(pKeyboardState, pKeyOldState, pKeyNewState, 256);
		ScanHardware(pMouseState, pMouseOldState, pMouseNewState, nMouseButtons);

		// Cache mouse coordinates so they remain consistent during frame
		vMousePos = vMousePosCache;
		nMouseWheelDelta = nMouseWheelDeltaCache;
		nMouseWheelDeltaCache = 0;

		vDroppedFiles = vDroppedFilesCache;
		vDroppedFilesPoint = vDroppedFilesPointCache;
		vDroppedFilesCache.clear();

		if (bTextEntryEnable)
		{
			UpdateTextEntry();
		}

		// Handle Frame Update
		bool bExtensionBlockFrame = false;		
		for (auto& ext : vExtensions) bExtensionBlockFrame |= ext->OnBeforeUserUpdate(fElapsedTime);
		if (!bExtensionBlockFrame)
		{
			if (!OnUserUpdate(fElapsedTime)) bAtomActive = false;
			
		}
		for (auto& ext : vExtensions) ext->OnAfterUserUpdate(fElapsedTime);

		

		if (bRealWindowMode)
		{
			vPixelSize = { 1,1 };
			vViewSize = vScreenSize;
			vViewPos = { 0,0 };
		}

		if (!bManualRenderEnable)
		{
			if (bConsoleShow)
			{
				SetDrawTarget((uint8_t)0);
				UpdateConsole();
			}

			// Display Frame
			renderer->UpdateViewport(vViewPos, vViewSize);
			renderer->ClearBuffer(olc::BLACK, true);

			// Layer 0 must always exist
			vLayers[0].bUpdate = true;
			vLayers[0].bShow = true;
			SetDecalMode(DecalMode::NORMAL);
			renderer->PrepareDrawing();

			for (auto layer = vLayers.rbegin(); layer != vLayers.rend(); ++layer)
			{
				if (layer->bShow)
				{
					if (layer->funcHook == nullptr)
					{
						renderer->ApplyTexture(layer->pDrawTarget.Decal()->id);
						if (!bSuspendTextureTransfer && layer->bUpdate)
						{
							layer->pDrawTarget.Decal()->Update();
							layer->bUpdate = false;
						}

						renderer->DrawLayerQuad(layer->vOffset, layer->vScale, layer->tint);

						// Display Decals in order for this layer
						for (auto& decal : layer->vecDecalInstance)
							renderer->DrawDecal(decal);
						layer->vecDecalInstance.clear();
					}
					else
					{
						// Mwa ha ha.... Have Fun!!!
						layer->funcHook();
					}
				}
			}
		}	

		// Present Graphics to screen
		renderer->DisplayFrame();

		if (bResizeRequested)
		{
			bResizeRequested = false;
			SetScreenSize(vWindowSize.x, vWindowSize.y);
			renderer->UpdateViewport({ 0,0 }, vWindowSize);
		}

		// Update Title Bar
		fFrameTimer += fElapsedTime;
		nFrameCount++;
		if (fFrameTimer >= 1.0f)
		{
			nLastFPS = nFrameCount;
			fFrameTimer -= 1.0f;
			std::string sTitle = "OneLoneCoder.com - Pixel Game Engine - " + sAppName + " - FPS: " + std::to_string(nFrameCount);
			platform->SetWindowTitle(sTitle);
			nFrameCount = 0;
		}
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

		fontRenderable.Create(128, 48);

		int px = 0, py = 0;
		for (size_t b = 0; b < 1024; b += 4)
		{
			uint32_t sym1 = (uint32_t)data[b + 0] - 48;
			uint32_t sym2 = (uint32_t)data[b + 1] - 48;
			uint32_t sym3 = (uint32_t)data[b + 2] - 48;
			uint32_t sym4 = (uint32_t)data[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 24; i++)
			{
				int k = r & (1 << i) ? 255 : 0;
				fontRenderable.Sprite()->SetPixel(px, py, olc::Pixel(k, k, k, k));
				if (++py == 48) { px++; py = 0; }
			}
		}

		fontRenderable.Decal()->Update();

		constexpr std::array<uint8_t, 96> vSpacing = { {
			0x03,0x25,0x16,0x08,0x07,0x08,0x08,0x04,0x15,0x15,0x08,0x07,0x15,0x07,0x24,0x08,
			0x08,0x17,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x24,0x15,0x06,0x07,0x16,0x17,
			0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x17,0x08,0x08,0x17,0x08,0x08,0x08,
			0x08,0x08,0x08,0x08,0x17,0x08,0x08,0x08,0x08,0x17,0x08,0x15,0x08,0x15,0x08,0x08,
			0x24,0x18,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x33,0x17,0x17,0x33,0x18,0x17,0x17,
			0x17,0x17,0x17,0x17,0x07,0x17,0x17,0x18,0x18,0x17,0x17,0x07,0x33,0x07,0x08,0x00, } };

		for (auto c : vSpacing) vFontSpacing.push_back({ c >> 4, c & 15 });

		// UK Standard Layout
#ifdef OLC_KEYBOARD_UK
		vKeyboardMap =
		{
			{olc::Key::A, "a", "A"}, {olc::Key::B, "b", "B"}, {olc::Key::C, "c", "C"}, {olc::Key::D, "d", "D"}, {olc::Key::E, "e", "E"},
			{olc::Key::F, "f", "F"}, {olc::Key::G, "g", "G"}, {olc::Key::H, "h", "H"}, {olc::Key::I, "i", "I"}, {olc::Key::J, "j", "J"},
			{olc::Key::K, "k", "K"}, {olc::Key::L, "l", "L"}, {olc::Key::M, "m", "M"}, {olc::Key::N, "n", "N"}, {olc::Key::O, "o", "O"},
			{olc::Key::P, "p", "P"}, {olc::Key::Q, "q", "Q"}, {olc::Key::R, "r", "R"}, {olc::Key::S, "s", "S"}, {olc::Key::T, "t", "T"},
			{olc::Key::U, "u", "U"}, {olc::Key::V, "v", "V"}, {olc::Key::W, "w", "W"}, {olc::Key::X, "x", "X"}, {olc::Key::Y, "y", "Y"},
			{olc::Key::Z, "z", "Z"},

			{olc::Key::K0, "0", ")"}, {olc::Key::K1, "1", "!"}, {olc::Key::K2, "2", "\""}, {olc::Key::K3, "3", "#"},	{olc::Key::K4, "4", "$"},
			{olc::Key::K5, "5", "%"}, {olc::Key::K6, "6", "^"}, {olc::Key::K7, "7", "&"}, {olc::Key::K8, "8", "*"},	{olc::Key::K9, "9", "("},

			{olc::Key::NP0, "0", "0"}, {olc::Key::NP1, "1", "1"}, {olc::Key::NP2, "2", "2"}, {olc::Key::NP3, "3", "3"},	{olc::Key::NP4, "4", "4"},
			{olc::Key::NP5, "5", "5"}, {olc::Key::NP6, "6", "6"}, {olc::Key::NP7, "7", "7"}, {olc::Key::NP8, "8", "8"},	{olc::Key::NP9, "9", "9"},
			{olc::Key::NP_MUL, "*", "*"}, {olc::Key::NP_DIV, "/", "/"}, {olc::Key::NP_ADD, "+", "+"}, {olc::Key::NP_SUB, "-", "-"},	{olc::Key::NP_DECIMAL, ".", "."},

			{olc::Key::PERIOD, ".", ">"}, {olc::Key::EQUALS, "=", "+"}, {olc::Key::COMMA, ",", "<"}, {olc::Key::MINUS, "-", "_"}, {olc::Key::SPACE, " ", " "},

			{olc::Key::OEM_1, ";", ":"}, {olc::Key::OEM_2, "/", "?"}, {olc::Key::OEM_3, "\'", "@"}, {olc::Key::OEM_4, "[", "{"},
			{olc::Key::OEM_5, "\\", "|"}, {olc::Key::OEM_6, "]", "}"}, {olc::Key::OEM_7, "#", "~"}, 
			
			// {olc::Key::TAB, "\t", "\t"}
		};
#endif
	}

	void PixelGameEngine::pgex_Register(olc::PGEX* pgex)
	{
		if (std::find(vExtensions.begin(), vExtensions.end(), pgex) == vExtensions.end())
			vExtensions.push_back(pgex);			
	}


	PGEX::PGEX(bool bHook) { if(bHook) pge->pgex_Register(this); }
	void PGEX::OnBeforeUserCreate() {}
	void PGEX::OnAfterUserCreate()	{}
	bool PGEX::OnBeforeUserUpdate(float& fElapsedTime) { return false; }
	void PGEX::OnAfterUserUpdate(float fElapsedTime) {}

	// Need a couple of statics as these are singleton instances
	// read from multiple locations
	std::atomic<bool> PixelGameEngine::bAtomActive{ false };
	olc::PixelGameEngine* olc::PGEX::pge = nullptr;
	olc::PixelGameEngine* olc::Platform::ptrPGE = nullptr;
	olc::PixelGameEngine* olc::Renderer::ptrPGE = nullptr;
	std::unique_ptr<ImageLoader> olc::Sprite::loader = nullptr;
};
#pragma endregion 


#pragma region platform_headless
namespace olc
{
#if defined(OLC_GFX_HEADLESS)
	class Renderer_Headless : public olc::Renderer
	{
	public:
		virtual void       PrepareDevice() {};
		virtual olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) { return olc::rcode::OK;		}
		virtual olc::rcode DestroyDevice() { return olc::rcode::OK; }
		virtual void       DisplayFrame() {}
		virtual void       PrepareDrawing() {}
		virtual void	   SetDecalMode(const olc::DecalMode& mode) {}
		virtual void       DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) {}
		virtual void       DrawDecal(const olc::DecalInstance& decal) {}
		virtual uint32_t   CreateTexture(const uint32_t width, const uint32_t height, const bool filtered = false, const bool clamp = true) {return 1;};
		virtual void       UpdateTexture(uint32_t id, olc::Sprite* spr) {}
		virtual void       ReadTexture(uint32_t id, olc::Sprite* spr) {}
		virtual uint32_t   DeleteTexture(const uint32_t id) {return 1;}
		virtual void       ApplyTexture(uint32_t id) {}
		virtual void       UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) {}
		virtual void       ClearBuffer(olc::Pixel p, bool bDepth) {}
	};
#endif
#if defined(OLC_PLATFORM_HEADLESS)
	class Platform_Headless : public olc::Platform
	{
	public:
		virtual olc::rcode SetWindowSize(const olc::vi2d& vPos, const olc::vi2d& vSize) override
		{ return olc::rcode::OK; }

		virtual olc::rcode ShowWindowFrame(const bool bShowFrame = true) override
		{ return olc::rcode::OK; }

		virtual olc::rcode ApplicationStartUp() { return olc::rcode::OK; }
		virtual olc::rcode ApplicationCleanUp() { return olc::rcode::OK; }
		virtual olc::rcode ThreadStartUp() { return olc::rcode::OK; }
		virtual olc::rcode ThreadCleanUp() { return olc::rcode::OK; }
		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) { return olc::rcode::OK; }
		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) { return olc::rcode::OK; }
		virtual olc::rcode SetWindowTitle(const std::string& s) { return olc::rcode::OK; }
		virtual olc::rcode StartSystemEventLoop() { return olc::rcode::OK; }
		virtual olc::rcode HandleSystemEvent() { return olc::rcode::OK; }
	};
#endif
}
#pragma endregion

// O------------------------------------------------------------------------------O
// | olcPixelGameEngine Renderers - the draw-y bits                               |
// O------------------------------------------------------------------------------O

#pragma region image_stb
// O------------------------------------------------------------------------------O
// | START IMAGE LOADER: stb_image.h, all systems, very fast                      |
// O------------------------------------------------------------------------------O
// Thanks to Sean Barrett - https://github.com/nothings/stb/blob/master/stb_image.h
// MIT License - Copyright(c) 2017 Sean Barrett

// Note you need to download the above file into your project folder, and
// #define OLC_IMAGE_STB 
// #define OLC_PGE_APPLICATION
// #include "olcPixelGameEngine.h"

#if defined(OLC_IMAGE_STB)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace olc
{
	class ImageLoader_STB : public olc::ImageLoader
	{
	public:
		ImageLoader_STB() : ImageLoader()
		{}

		olc::rcode LoadImageResource(olc::Sprite* spr, const std::string& sImageFile, olc::ResourcePack* pack) override
		{
			UNUSED(pack);
			// clear out existing sprite
			spr->pColData.clear();
			// Open file
			stbi_uc* bytes = nullptr;
			int w = 0, h = 0, cmp = 0;
			if (pack != nullptr)
			{
				ResourceBuffer rb = pack->GetFileBuffer(sImageFile);
				bytes = stbi_load_from_memory((unsigned char*)rb.vMemory.data(), rb.vMemory.size(), &w, &h, &cmp, 4);
			}
			else
			{
				// Check file exists
				if (!_gfs::exists(sImageFile)) return olc::rcode::NO_FILE;
				bytes = stbi_load(sImageFile.c_str(), &w, &h, &cmp, 4);
			}

			if (!bytes) return olc::rcode::FAIL;
			spr->width = w; spr->height = h;
			spr->pColData.resize(spr->width * spr->height);
			std::memcpy(spr->pColData.data(), bytes, spr->width * spr->height * 4);
			delete[] bytes;
			return olc::rcode::OK;
		}

		olc::rcode SaveImageResource(olc::Sprite* spr, const std::string& sImageFile) override
		{
			return olc::rcode::OK;
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | START IMAGE LOADER: stb_image.h                                              |
// O------------------------------------------------------------------------------O
#pragma endregion



#if !defined(OLC_PGE_HEADLESS)

#pragma region renderer_ogl10
// O------------------------------------------------------------------------------O
// | START RENDERER: OpenGL 1.0 (the original, the best...)                       |
// O------------------------------------------------------------------------------O
#if defined(OLC_GFX_OPENGL10)

#if defined(OLC_PLATFORM_WINAPI)
	#include <dwmapi.h>
	#include <GL/gl.h>
	#if !defined(__MINGW32__)
		#pragma comment(lib, "Dwmapi.lib")
	#endif
	typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
	static wglSwapInterval_t* wglSwapInterval = nullptr;
	typedef HDC glDeviceContext_t;
	typedef HGLRC glRenderContext_t;
#endif

#if defined(__linux__) || defined(__FreeBSD__)
	#include <GL/gl.h>
#endif

#if defined(OLC_PLATFORM_X11)
	namespace X11
	{
		#include <GL/glx.h>
	}
	typedef int(glSwapInterval_t)(X11::Display* dpy, X11::GLXDrawable drawable, int interval);
	static glSwapInterval_t* glSwapIntervalEXT;
	typedef X11::GLXContext glDeviceContext_t;
	typedef X11::GLXContext glRenderContext_t;
#endif

#if defined(__APPLE__)
	#define GL_SILENCE_DEPRECATION
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#endif

namespace olc
{
	class Renderer_OGL10 : public olc::Renderer
	{
	private:
#if defined(OLC_PLATFORM_GLUT)
		bool mFullScreen = false;
#else
		glDeviceContext_t glDeviceContext = 0;
		glRenderContext_t glRenderContext = 0;
#endif

		bool bSync = false;
		olc::DecalMode nDecalMode = olc::DecalMode(-1); // Thanks Gusgo & Bispoo
		olc::DecalStructure nDecalStructure = olc::DecalStructure(-1);
#if defined(OLC_PLATFORM_X11)
		X11::Display* olc_Display = nullptr;
		X11::Window* olc_Window = nullptr;
		X11::XVisualInfo* olc_VisualInfo = nullptr;
#endif

	public:
		void PrepareDevice() override
		{
#if defined(OLC_PLATFORM_GLUT)
			//glutInit has to be called with main() arguments, make fake ones
			int argc = 0;
			char* argv[1] = { (char*)"" };
			glutInit(&argc, argv);
			glutInitWindowPosition(0, 0);
			glutInitWindowSize(512, 512);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
			// Creates the window and the OpenGL context for it
			glutCreateWindow("OneLoneCoder.com - Pixel Game Engine");
			glEnable(GL_TEXTURE_2D); // Turn on texturing
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
		}

		olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) override
		{
#if defined(OLC_PLATFORM_WINAPI)
			// Create Device Context
			glDeviceContext = GetDC((HWND)(params[0]));
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};

			int pf = 0;
			if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return olc::FAIL;
			SetPixelFormat(glDeviceContext, pf, &pfd);

			if (!(glRenderContext = wglCreateContext(glDeviceContext))) return olc::FAIL;
			wglMakeCurrent(glDeviceContext, glRenderContext);

			// Remove Frame cap
			wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");
			if (wglSwapInterval && !bVSYNC) wglSwapInterval(0);
			bSync = bVSYNC;
#endif

#if defined(OLC_PLATFORM_X11)
			using namespace X11;
			// Linux has tighter coupling between OpenGL and X11, so we store
			// various "platform" handles in the renderer
			olc_Display = (X11::Display*)(params[0]);
			olc_Window = (X11::Window*)(params[1]);
			olc_VisualInfo = (X11::XVisualInfo*)(params[2]);

			glDeviceContext = glXCreateContext(olc_Display, olc_VisualInfo, nullptr, GL_TRUE);
			glXMakeCurrent(olc_Display, *olc_Window, glDeviceContext);

			XWindowAttributes gwa;
			XGetWindowAttributes(olc_Display, *olc_Window, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);

			glSwapIntervalEXT = nullptr;
			glSwapIntervalEXT = (glSwapInterval_t*)glXGetProcAddress((unsigned char*)"glXSwapIntervalEXT");

			if (glSwapIntervalEXT == nullptr && !bVSYNC)
			{
				printf("NOTE: Could not disable VSYNC, glXSwapIntervalEXT() was not found!\n");
				printf("      Don't worry though, things will still work, it's just the\n");
				printf("      frame rate will be capped to your monitors refresh rate - javidx9\n");
			}

			if (glSwapIntervalEXT != nullptr && !bVSYNC)
				glSwapIntervalEXT(olc_Display, *olc_Window, 0);
#endif		

#if defined(OLC_PLATFORM_GLUT)
			mFullScreen = bFullScreen;
			if (!bVSYNC)
			{
#if defined(__APPLE__)
				GLint sync = 0;
				CGLContextObj ctx = CGLGetCurrentContext();
				if (ctx) CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
#endif
			}
#else
			glEnable(GL_TEXTURE_2D); // Turn on texturing
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
			return olc::rcode::OK;
		}

		olc::rcode DestroyDevice() override
		{
#if defined(OLC_PLATFORM_WINAPI)
			wglDeleteContext(glRenderContext);
#endif

#if defined(OLC_PLATFORM_X11)
			glXMakeCurrent(olc_Display, None, NULL);
			glXDestroyContext(olc_Display, glDeviceContext);
#endif

#if defined(OLC_PLATFORM_GLUT)
			glutDestroyWindow(glutGetWindow());
#endif
			return olc::rcode::OK;
		}

		void DisplayFrame() override
		{
#if defined(OLC_PLATFORM_WINAPI)
			SwapBuffers(glDeviceContext);
			if (bSync) DwmFlush(); // Woooohooooooo!!!! SMOOOOOOOTH!
#endif	

#if defined(OLC_PLATFORM_X11)
			X11::glXSwapBuffers(olc_Display, *olc_Window);
#endif		

#if defined(OLC_PLATFORM_GLUT)
			glutSwapBuffers();
#endif
		}

		void PrepareDrawing() override
		{
			
			//ClearBuffer(olc::GREEN, true);
			glEnable(GL_BLEND);
			nDecalMode = DecalMode::NORMAL;
			nDecalStructure = DecalStructure::FAN;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		void SetDecalMode(const olc::DecalMode& mode)
		{
			if (mode != nDecalMode)
			{
				switch (mode)
				{
				case olc::DecalMode::NORMAL:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case olc::DecalMode::ADDITIVE:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					break;
				case olc::DecalMode::MULTIPLICATIVE:
					glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case olc::DecalMode::STENCIL:
					glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
					break;
				case olc::DecalMode::ILLUMINATE:
					glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
					break;
				case olc::DecalMode::WIREFRAME:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				}

				nDecalMode = mode;
			}
		}

		void DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) override
		{
			glBegin(GL_QUADS);
			glColor4ub(tint.r, tint.g, tint.b, tint.a);
			glTexCoord2f(0.0f * scale.x + offset.x, 1.0f * scale.y + offset.y);
			glVertex3f(-1.0f /*+ vSubPixelOffset.x*/, -1.0f /*+ vSubPixelOffset.y*/, 0.0f);
			glTexCoord2f(0.0f * scale.x + offset.x, 0.0f * scale.y + offset.y);
			glVertex3f(-1.0f /*+ vSubPixelOffset.x*/, 1.0f /*+ vSubPixelOffset.y*/, 0.0f);
			glTexCoord2f(1.0f * scale.x + offset.x, 0.0f * scale.y + offset.y);
			glVertex3f(1.0f /*+ vSubPixelOffset.x*/, 1.0f /*+ vSubPixelOffset.y*/, 0.0f);
			glTexCoord2f(1.0f * scale.x + offset.x, 1.0f * scale.y + offset.y);
			glVertex3f(1.0f /*+ vSubPixelOffset.x*/, -1.0f /*+ vSubPixelOffset.y*/, 0.0f);
			glEnd();
		}

		void DrawDecal(const olc::DecalInstance& decal) override
		{
			SetDecalMode(decal.mode);

			if (decal.decal == nullptr)
				glBindTexture(GL_TEXTURE_2D, 0);
			else
				glBindTexture(GL_TEXTURE_2D, decal.decal->id);
			
			if (decal.depth)
			{
				glEnable(GL_DEPTH_TEST);
			}

			if (nDecalMode == DecalMode::WIREFRAME)
				glBegin(GL_LINE_LOOP);
			else
			{
				if(decal.structure == olc::DecalStructure::FAN)
					glBegin(GL_TRIANGLE_FAN);
				else if(decal.structure == olc::DecalStructure::STRIP)
					glBegin(GL_TRIANGLE_STRIP);
				else if(decal.structure == olc::DecalStructure::LIST)
					glBegin(GL_TRIANGLES);
			}

			if (decal.depth)
			{

				// Render as 3D Spatial Entity
				for (uint32_t n = 0; n < decal.points; n++)
				{
					glColor4ub(decal.tint[n].r, decal.tint[n].g, decal.tint[n].b, decal.tint[n].a);
					glTexCoord4f(decal.uv[n].x, decal.uv[n].y, 0.0f, decal.w[n]);
					glVertex3f(decal.pos[n].x, decal.pos[n].y, decal.z[n]);
				}
			}
			else
			{
				// Render as 2D Spatial entity
				for (uint32_t n = 0; n < decal.points; n++)
				{
					glColor4ub(decal.tint[n].r, decal.tint[n].g, decal.tint[n].b, decal.tint[n].a);
					glTexCoord4f(decal.uv[n].x, decal.uv[n].y, 0.0f, decal.w[n]);
					glVertex2f(decal.pos[n].x, decal.pos[n].y);
				}
			}

			glEnd();

			if (decal.depth)
			{
				glDisable(GL_DEPTH_TEST);
			}
		
		}

		uint32_t CreateTexture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp) override
		{
			UNUSED(width);
			UNUSED(height);
			uint32_t id = 0;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			if (filtered)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}

			if (clamp)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			return id;
		}

		uint32_t DeleteTexture(const uint32_t id) override
		{
			glDeleteTextures(1, &id);
			return id;
		}

		void UpdateTexture(uint32_t id, olc::Sprite* spr) override
		{
			UNUSED(id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spr->width, spr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
		}

		void ReadTexture(uint32_t id, olc::Sprite* spr) override
		{
			glReadPixels(0, 0, spr->width, spr->height, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
		}

		void ApplyTexture(uint32_t id) override
		{
			glBindTexture(GL_TEXTURE_2D, id);
		}

		void ClearBuffer(olc::Pixel p, bool bDepth) override
		{
			glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			if (bDepth) glClear(GL_DEPTH_BUFFER_BIT);
		}

		void UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) override
		{
			glViewport(pos.x, pos.y, size.x, size.y);
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END RENDERER: OpenGL 1.0 (the original, the best...)                         |
// O------------------------------------------------------------------------------O
#pragma endregion

#pragma region renderer_ogl33
// O------------------------------------------------------------------------------O
// | START RENDERER: OpenGL 3.3 (3.0 es) (sh-sh-sh-shaders....)                   |
// O------------------------------------------------------------------------------O
#if defined(OLC_GFX_OPENGL33)

#if defined(OLC_PLATFORM_WINAPI)
	#include <dwmapi.h>
	//#include <gl/GL.h>
	#if !defined(__MINGW32__)
		#pragma comment(lib, "Dwmapi.lib")
	#endif		
	//typedef void __stdcall locSwapInterval_t(GLsizei n);
	typedef HDC glDeviceContext_t;
	typedef HGLRC glRenderContext_t;
	//#define CALLSTYLE __stdcall
	#define OGL_LOAD(t, n) (t*)wglGetProcAddress(#n)
#endif
//
//#if defined(__linux__) || defined(__FreeBSD__)
//	#include <GL/gl.h>
//#endif

#if defined(OLC_PLATFORM_X11)
	/*namespace X11
	{
		#include <GL/glx.h>
	}
	typedef int(locSwapInterval_t)(X11::Display* dpy, X11::GLXDrawable drawable, int interval);*/
	typedef X11::GLXContext glDeviceContext_t;
	typedef X11::GLXContext glRenderContext_t;
	//#define CALLSTYLE 
	#define OGL_LOAD(t, n) (t*)glXGetProcAddress((unsigned char*)#n);
#endif

//#if defined(__APPLE__)
//	#define GL_SILENCE_DEPRECATION
//	#include <OpenGL/OpenGL.h>
//	#include <OpenGL/gl.h>
//	#include <OpenGL/glu.h>
//#endif

#if defined(OLC_PLATFORM_EMSCRIPTEN)
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>
	#define GL_GLEXT_PROTOTYPES
	#include <GLES2/gl2ext.h>
	#include <emscripten/emscripten.h>
	#define CALLSTYLE
	typedef EGLBoolean(locSwapInterval_t)(EGLDisplay display, EGLint interval);
	#define GL_CLAMP GL_CLAMP_TO_EDGE
	#define OGL_LOAD(t, n) n;
#endif

namespace olc
{
//	typedef char GLchar;
//	typedef ptrdiff_t GLsizeiptr;
//	typedef GLuint CALLSTYLE locCreateShader_t(GLenum type);
//	typedef GLuint CALLSTYLE locCreateProgram_t(void);
//	typedef void CALLSTYLE locDeleteShader_t(GLuint shader);
//#if defined(OLC_PLATFORM_EMSCRIPTEN)
//	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
//#else
//	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
//#endif
//	typedef void CALLSTYLE locCompileShader_t(GLuint shader);
//	typedef void CALLSTYLE locLinkProgram_t(GLuint program);
//	typedef void CALLSTYLE locDeleteProgram_t(GLuint program);
//	typedef void CALLSTYLE locAttachShader_t(GLuint program, GLuint shader);
//	typedef void CALLSTYLE locBindBuffer_t(GLenum target, GLuint buffer);
//	typedef void CALLSTYLE locBufferData_t(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
//	typedef void CALLSTYLE locGenBuffers_t(GLsizei n, GLuint* buffers);
//	typedef void CALLSTYLE locVertexAttribPointer_t(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
//	typedef void CALLSTYLE locEnableVertexAttribArray_t(GLuint index);
//	typedef void CALLSTYLE locUseProgram_t(GLuint program);
//	typedef void CALLSTYLE locBindVertexArray_t(GLuint array);
//	typedef void CALLSTYLE locGenVertexArrays_t(GLsizei n, GLuint* arrays);
//	typedef void CALLSTYLE locGetShaderInfoLog_t(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
//	typedef GLint CALLSTYLE locGetUniformLocation_t(GLuint program, const GLchar* name);
//	typedef void CALLSTYLE locUniform1f_t(GLint location, GLfloat v0);
//	typedef void CALLSTYLE locUniform1i_t(GLint location, GLint v0);
//	typedef void CALLSTYLE locUniform2fv_t(GLint location, GLsizei count, const GLfloat* value);
//	typedef void CALLSTYLE locActiveTexture_t(GLenum texture);
//	typedef void CALLSTYLE locGenFrameBuffers_t(GLsizei n, GLuint* ids);
//	typedef void CALLSTYLE locBindFrameBuffer_t(GLenum target, GLuint fb);
//	typedef GLenum CALLSTYLE locCheckFrameBufferStatus_t(GLenum target);
//	typedef void CALLSTYLE locDeleteFrameBuffers_t(GLsizei n, const GLuint* fbs);
//	typedef void CALLSTYLE locFrameBufferTexture2D_t(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
//	typedef void CALLSTYLE locDrawBuffers_t(GLsizei n, const GLenum* bufs);
//	typedef void CALLSTYLE locBlendFuncSeparate_t(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

	

	class Renderer_OGL33 : public olc::Renderer
	{
	private:
#if defined(OLC_PLATFORM_EMSCRIPTEN)
		EGLDisplay olc_Display;
		EGLConfig olc_Config;
		EGLContext olc_Context;
		EGLSurface olc_Surface;
#endif

#if defined(OLC_PLATFORM_GLUT)
		bool mFullScreen = false;
#else
	#if !defined(OLC_PLATFORM_EMSCRIPTEN)
		glDeviceContext_t glDeviceContext = 0;
		glRenderContext_t glRenderContext = 0;
	#endif
#endif
		bool bSync = false;
		olc::DecalMode nDecalMode = olc::DecalMode(-1); // Thanks Gusgo & Bispoo
#if defined(OLC_PLATFORM_X11)
		X11::Display* olc_Display = nullptr;
		X11::Window* olc_Window = nullptr;
		X11::XVisualInfo* olc_VisualInfo = nullptr;
#endif

	private:
		locCreateShader_t* locCreateShader = nullptr;
		locShaderSource_t* locShaderSource = nullptr;
		locCompileShader_t* locCompileShader = nullptr;
		locDeleteShader_t* locDeleteShader = nullptr;
		locCreateProgram_t* locCreateProgram = nullptr;
		locDeleteProgram_t* locDeleteProgram = nullptr;
		locLinkProgram_t* locLinkProgram = nullptr;
		locAttachShader_t* locAttachShader = nullptr;
		locBindBuffer_t* locBindBuffer = nullptr;
		locBufferData_t* locBufferData = nullptr;
		locGenBuffers_t* locGenBuffers = nullptr;
		locVertexAttribPointer_t* locVertexAttribPointer = nullptr;
		locEnableVertexAttribArray_t* locEnableVertexAttribArray = nullptr;
		locUseProgram_t* locUseProgram = nullptr;
		locBindVertexArray_t* locBindVertexArray = nullptr;
		locGenVertexArrays_t* locGenVertexArrays = nullptr;
		locSwapInterval_t* locSwapInterval = nullptr;
		locGetShaderInfoLog_t* locGetShaderInfoLog = nullptr;

		uint32_t m_nFS = 0;
		uint32_t m_nVS = 0;
		uint32_t m_nQuadShader = 0;
		uint32_t m_vbQuad = 0;
		uint32_t m_vaQuad = 0;

		struct locVertex
		{
			float pos[3];
			olc::vf2d tex;
			olc::Pixel col;
		};

		locVertex pVertexMem[OLC_MAX_VERTS];

		olc::Renderable rendBlankQuad;

	public:
		void PrepareDevice() override
		{
#if defined(OLC_PLATFORM_GLUT)
			//glutInit has to be called with main() arguments, make fake ones
			int argc = 0;
			char* argv[1] = { (char*)"" };
			glutInit(&argc, argv);
			glutInitWindowPosition(0, 0);
			glutInitWindowSize(512, 512);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
			// Creates the window and the OpenGL context for it
			glutCreateWindow("OneLoneCoder.com - Pixel Game Engine");
			glEnable(GL_TEXTURE_2D); // Turn on texturing
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
		}

		olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) override
		{
			// Create OpenGL Context
#if defined(OLC_PLATFORM_WINAPI)
			// Create Device Context
			glDeviceContext = GetDC((HWND)(params[0]));
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};

			int pf = 0;
			if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return olc::FAIL;
			SetPixelFormat(glDeviceContext, pf, &pfd);

			if (!(glRenderContext = wglCreateContext(glDeviceContext))) return olc::FAIL;
			wglMakeCurrent(glDeviceContext, glRenderContext);

			// Set Vertical Sync
			locSwapInterval = OGL_LOAD(locSwapInterval_t, wglSwapIntervalEXT);
			if (locSwapInterval && !bVSYNC) locSwapInterval(0);
			bSync = bVSYNC;
#endif

#if defined(OLC_PLATFORM_X11)
			using namespace X11;
			// Linux has tighter coupling between OpenGL and X11, so we store
			// various "platform" handles in the renderer
			olc_Display = (X11::Display*)(params[0]);
			olc_Window = (X11::Window*)(params[1]);
			olc_VisualInfo = (X11::XVisualInfo*)(params[2]);

			glDeviceContext = glXCreateContext(olc_Display, olc_VisualInfo, nullptr, GL_TRUE);
			glXMakeCurrent(olc_Display, *olc_Window, glDeviceContext);

			XWindowAttributes gwa;
			XGetWindowAttributes(olc_Display, *olc_Window, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);

			locSwapInterval = OGL_LOAD(locSwapInterval_t, glXSwapIntervalEXT);

			if (locSwapInterval == nullptr && !bVSYNC)
			{
				printf("NOTE: Could not disable VSYNC, glXSwapIntervalEXT() was not found!\n");
				printf("      Don't worry though, things will still work, it's just the\n");
				printf("      frame rate will be capped to your monitors refresh rate - javidx9\n");
			}

			if (locSwapInterval != nullptr && !bVSYNC)
				locSwapInterval(olc_Display, *olc_Window, 0);
#endif		

#if defined(OLC_PLATFORM_EMSCRIPTEN)
			EGLint const attribute_list[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
			EGLint const context_config[] = { EGL_CONTEXT_CLIENT_VERSION , 2, EGL_NONE };
			EGLint num_config;

			olc_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			eglInitialize(olc_Display, nullptr, nullptr);
			eglChooseConfig(olc_Display, attribute_list, &olc_Config, 1, &num_config);

			/* create an EGL rendering context */
			olc_Context = eglCreateContext(olc_Display, olc_Config, EGL_NO_CONTEXT, context_config);
			olc_Surface = eglCreateWindowSurface(olc_Display, olc_Config, NULL, nullptr);
			eglMakeCurrent(olc_Display, olc_Surface, olc_Surface, olc_Context);
			//eglSwapInterval is currently a NOP, plement anyways in case it becomes supported
			locSwapInterval = &eglSwapInterval;
			locSwapInterval(olc_Display, bVSYNC ? 1 : 0);
#endif

#if defined(OLC_PLATFORM_GLUT)
			mFullScreen = bFullScreen;
			if (!bVSYNC)
			{
#if defined(__APPLE__)
				GLint sync = 0;
				CGLContextObj ctx = CGLGetCurrentContext();
				if (ctx) CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
#endif
			}
#else
	#if !defined(OLC_PLATFORM_EMSCRIPTEN)
			glEnable(GL_TEXTURE_2D); // Turn on texturing
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	#endif
#endif
			// Load External OpenGL Functions
			locCreateShader = OGL_LOAD(locCreateShader_t, glCreateShader);
			locCompileShader = OGL_LOAD(locCompileShader_t, glCompileShader);
			locShaderSource = OGL_LOAD(locShaderSource_t, glShaderSource);
			locDeleteShader = OGL_LOAD(locDeleteShader_t, glDeleteShader);
			locCreateProgram = OGL_LOAD(locCreateProgram_t, glCreateProgram);
			locDeleteProgram = OGL_LOAD(locDeleteProgram_t, glDeleteProgram);
			locLinkProgram = OGL_LOAD(locLinkProgram_t, glLinkProgram);
			locAttachShader = OGL_LOAD(locAttachShader_t, glAttachShader);
			locBindBuffer = OGL_LOAD(locBindBuffer_t, glBindBuffer);
			locBufferData = OGL_LOAD(locBufferData_t, glBufferData);
			locGenBuffers = OGL_LOAD(locGenBuffers_t, glGenBuffers);
			locVertexAttribPointer = OGL_LOAD(locVertexAttribPointer_t, glVertexAttribPointer);
			locEnableVertexAttribArray = OGL_LOAD(locEnableVertexAttribArray_t, glEnableVertexAttribArray);
			locUseProgram = OGL_LOAD(locUseProgram_t, glUseProgram);
			locGetShaderInfoLog = OGL_LOAD(locGetShaderInfoLog_t, glGetShaderInfoLog);
#if !defined(OLC_PLATFORM_EMSCRIPTEN)
			locBindVertexArray = OGL_LOAD(locBindVertexArray_t, glBindVertexArray);
			locGenVertexArrays = OGL_LOAD(locGenVertexArrays_t, glGenVertexArrays);
#else
			locBindVertexArray = glBindVertexArrayOES;
			locGenVertexArrays = glGenVertexArraysOES;
#endif

			// Load & Compile Quad Shader - assumes no errors
			m_nFS = locCreateShader(0x8B30);
			const GLchar* strFS =
#if defined(__arm__) || defined(OLC_PLATFORM_EMSCRIPTEN)
				"#version 300 es\n"
				"precision mediump float;"
#else
				"#version 330 core\n"
#endif
				"out vec4 pixel;\n""in vec2 oTex;\n"
				"in vec4 oCol;\n""uniform sampler2D sprTex;\n""void main(){pixel = texture(sprTex, oTex) * oCol;}";
			locShaderSource(m_nFS, 1, &strFS, NULL);
			locCompileShader(m_nFS);

			m_nVS = locCreateShader(0x8B31);
			const GLchar* strVS =
#if defined(__arm__) || defined(OLC_PLATFORM_EMSCRIPTEN)
				"#version 300 es\n"
				"precision mediump float;"
#else
				"#version 330 core\n"
#endif
				"layout(location = 0) in vec3 aPos;\n""layout(location = 1) in vec2 aTex;\n"
				"layout(location = 2) in vec4 aCol;\n""out vec2 oTex;\n""out vec4 oCol;\n"
				"void main(){ float p = 1.0 / aPos.z; gl_Position = p * vec4(aPos.x, aPos.y, 0.0, 1.0); oTex = p * aTex; oCol = aCol;}";
			locShaderSource(m_nVS, 1, &strVS, NULL);
			locCompileShader(m_nVS);

			m_nQuadShader = locCreateProgram();
			locAttachShader(m_nQuadShader, m_nFS);
			locAttachShader(m_nQuadShader, m_nVS);
			locLinkProgram(m_nQuadShader);

			// Create Quad
			locGenBuffers(1, &m_vbQuad);
			locGenVertexArrays(1, &m_vaQuad);
			locBindVertexArray(m_vaQuad);
			locBindBuffer(0x8892, m_vbQuad);

			locVertex verts[OLC_MAX_VERTS];
			locBufferData(0x8892, sizeof(locVertex) * OLC_MAX_VERTS, verts, 0x88E0);
			locVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(locVertex), 0); locEnableVertexAttribArray(0);
			locVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(locVertex), (void*)(3 * sizeof(float))); locEnableVertexAttribArray(1);
			locVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(locVertex), (void*)(5 * sizeof(float)));	locEnableVertexAttribArray(2);
			locBindBuffer(0x8892, 0);
			locBindVertexArray(0);

			// Create blank texture for spriteless decals
			rendBlankQuad.Create(1, 1);
			rendBlankQuad.Sprite()->GetData()[0] = olc::WHITE;
			rendBlankQuad.Decal()->Update();
			return olc::rcode::OK;
		}

		olc::rcode DestroyDevice() override
		{
#if defined(OLC_PLATFORM_WINAPI)
			wglDeleteContext(glRenderContext);
#endif

#if defined(OLC_PLATFORM_X11)
			glXMakeCurrent(olc_Display, None, NULL);
			glXDestroyContext(olc_Display, glDeviceContext);
#endif

#if defined(OLC_PLATFORM_GLUT)
			glutDestroyWindow(glutGetWindow());
#endif

#if defined(OLC_PLATFORM_EMSCRIPTEN)
			eglMakeCurrent(olc_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			eglDestroyContext(olc_Display, olc_Context);
			eglDestroySurface(olc_Display, olc_Surface);
			eglTerminate(olc_Display);
			olc_Display = EGL_NO_DISPLAY;
			olc_Surface = EGL_NO_SURFACE;
			olc_Context = EGL_NO_CONTEXT;
#endif
			return olc::rcode::OK;
		}

		void DisplayFrame() override
		{
#if defined(OLC_PLATFORM_WINAPI)
			SwapBuffers(glDeviceContext);
			if (bSync) DwmFlush(); // Woooohooooooo!!!! SMOOOOOOOTH!
#endif	

#if defined(OLC_PLATFORM_X11)
			X11::glXSwapBuffers(olc_Display, *olc_Window);
#endif		

#if defined(OLC_PLATFORM_GLUT)
			glutSwapBuffers();
#endif

#if defined(OLC_PLATFORM_EMSCRIPTEN)
			eglSwapBuffers(olc_Display, olc_Surface);
#endif
		}

		void PrepareDrawing() override
		{
			glEnable(GL_BLEND);
			nDecalMode = DecalMode::NORMAL;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			locUseProgram(m_nQuadShader);
			locBindVertexArray(m_vaQuad);

#if defined(OLC_PLATFORM_EMSCRIPTEN)
			locVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(locVertex), 0); locEnableVertexAttribArray(0);
			locVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(locVertex), (void*)(3 * sizeof(float))); locEnableVertexAttribArray(1);
			locVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(locVertex), (void*)(5 * sizeof(float)));	locEnableVertexAttribArray(2);
#endif
		}

		void SetDecalMode(const olc::DecalMode& mode) override
		{
			if (mode != nDecalMode)
			{
				switch (mode)
				{
				case olc::DecalMode::NORMAL: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
				case olc::DecalMode::ADDITIVE: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
				case olc::DecalMode::MULTIPLICATIVE: glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);	break;
				case olc::DecalMode::STENCIL: glBlendFunc(GL_ZERO, GL_SRC_ALPHA); break;
				case olc::DecalMode::ILLUMINATE: glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);	break;
				case olc::DecalMode::WIREFRAME: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
				}

				nDecalMode = mode;
			}
		}

		void DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) override
		{
			locBindBuffer(0x8892, m_vbQuad);
			locVertex verts[4] = {
				{{-1.0f, -1.0f, 1.0}, {0.0f * scale.x + offset.x, 1.0f * scale.y + offset.y}, tint},
				{{+1.0f, -1.0f, 1.0}, {1.0f * scale.x + offset.x, 1.0f * scale.y + offset.y}, tint},
				{{-1.0f, +1.0f, 1.0}, {0.0f * scale.x + offset.x, 0.0f * scale.y + offset.y}, tint},
				{{+1.0f, +1.0f, 1.0}, {1.0f * scale.x + offset.x, 0.0f * scale.y + offset.y}, tint},
			};

			locBufferData(0x8892, sizeof(locVertex) * 4, verts, 0x88E0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		void DrawDecal(const olc::DecalInstance& decal) override
		{
			SetDecalMode(decal.mode);
			if (decal.decal == nullptr)
				glBindTexture(GL_TEXTURE_2D, rendBlankQuad.Decal()->id);
			else
				glBindTexture(GL_TEXTURE_2D, decal.decal->id);

			locBindBuffer(0x8892, m_vbQuad);

			for (uint32_t i = 0; i < decal.points; i++)
				pVertexMem[i] = { { decal.pos[i].x, decal.pos[i].y, decal.w[i] }, { decal.uv[i].x, decal.uv[i].y }, decal.tint[i] };

			locBufferData(0x8892, sizeof(locVertex) * decal.points, pVertexMem, 0x88E0);

			if (nDecalMode == DecalMode::WIREFRAME)
				glDrawArrays(GL_LINE_LOOP, 0, decal.points);
			else
			{
				if (decal.structure == olc::DecalStructure::FAN)
					glDrawArrays(GL_TRIANGLE_FAN, 0, decal.points);
				else if (decal.structure == olc::DecalStructure::STRIP)
					glDrawArrays(GL_TRIANGLE_STRIP, 0, decal.points);
				else if (decal.structure == olc::DecalStructure::LIST)
					glDrawArrays(GL_TRIANGLES, 0, decal.points);
			}
		}

		uint32_t CreateTexture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp) override
		{
			UNUSED(width);
			UNUSED(height);
			uint32_t id = 0;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			if (filtered)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}

			if (clamp)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
#if !defined(OLC_PLATFORM_EMSCRIPTEN)
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#endif
			return id;
		}

		uint32_t DeleteTexture(const uint32_t id) override
		{
			glDeleteTextures(1, &id);
			return id;
		}

		void UpdateTexture(uint32_t id, olc::Sprite* spr) override
		{
			UNUSED(id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spr->width, spr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
		}

		void ReadTexture(uint32_t id, olc::Sprite* spr) override
		{
			glReadPixels(0, 0, spr->width, spr->height, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
		}

		void ApplyTexture(uint32_t id) override
		{
			glBindTexture(GL_TEXTURE_2D, id);
		}

		void ClearBuffer(olc::Pixel p, bool bDepth) override
		{
			glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			if (bDepth) glClear(GL_DEPTH_BUFFER_BIT);
		}

		void UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) override
		{
			glViewport(pos.x, pos.y, size.x, size.y);
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END RENDERER: OpenGL 3.3 (3.0 es) (sh-sh-sh-shaders....)                     |
// O------------------------------------------------------------------------------O
#pragma endregion

// O------------------------------------------------------------------------------O
// | olcPixelGameEngine Image loaders                                             |
// O------------------------------------------------------------------------------O

#pragma region image_gdi
// O------------------------------------------------------------------------------O
// | START IMAGE LOADER: GDI+, Windows Only, always exists, a little slow         |
// O------------------------------------------------------------------------------O
#if defined(OLC_IMAGE_GDI)

#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)
#include <objidl.h>
#include <gdiplus.h>
#if defined(__MINGW32__) // Thanks Gusgo & Dandistine, but c'mon mingw!! wtf?!
	#include <gdiplus/gdiplusinit.h>
#else
	#include <gdiplusinit.h>
#endif
#include <shlwapi.h>
#undef min
#undef max

#if !defined(__MINGW32__)
	#pragma comment(lib, "gdiplus.lib")
	#pragma comment(lib, "Shlwapi.lib")
#endif

namespace olc
{
	// Thanks @MaGetzUb for this, which allows sprites to be defined
	// at construction, by initialising the GDI subsystem
	static class GDIPlusStartup
	{
	public:
		GDIPlusStartup()
		{			
			Gdiplus::GdiplusStartupInput startupInput;
			GdiplusStartup(&token, &startupInput, NULL);
		}

		ULONG_PTR	token;
		
		~GDIPlusStartup()
		{
			// Well, MarcusTU thought this was important :D
			Gdiplus::GdiplusShutdown(token);
		}
	} gdistartup;

	class ImageLoader_GDIPlus : public olc::ImageLoader
	{
	private:
		std::wstring ConvertS2W(std::string s)
		{
#ifdef __MINGW32__
			wchar_t* buffer = new wchar_t[s.length() + 1];
			mbstowcs(buffer, s.c_str(), s.length());
			buffer[s.length()] = L'\0';
#else
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
#endif
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}

	public:
		ImageLoader_GDIPlus() : ImageLoader()
		{}

		olc::rcode LoadImageResource(olc::Sprite* spr, const std::string& sImageFile, olc::ResourcePack* pack) override
		{
			// clear out existing sprite
			spr->pColData.clear();

			// Open file
			UNUSED(pack);
			Gdiplus::Bitmap* bmp = nullptr;
			if (pack != nullptr)
			{
				// Load sprite from input stream
				ResourceBuffer rb = pack->GetFileBuffer(sImageFile);
				bmp = Gdiplus::Bitmap::FromStream(SHCreateMemStream((BYTE*)rb.vMemory.data(), UINT(rb.vMemory.size())));
			}
			else
			{
				// Check file exists
				if (!_gfs::exists(sImageFile)) return olc::rcode::NO_FILE;

				// Load sprite from file
				bmp = Gdiplus::Bitmap::FromFile(ConvertS2W(sImageFile).c_str());
			}

			if (bmp->GetLastStatus() != Gdiplus::Ok) return olc::rcode::FAIL;
			spr->width = bmp->GetWidth();
			spr->height = bmp->GetHeight();

			spr->pColData.resize(spr->width * spr->height);

			for (int y = 0; y < spr->height; y++)
				for (int x = 0; x < spr->width; x++)
				{
					Gdiplus::Color c;
					bmp->GetPixel(x, y, &c);
					spr->SetPixel(x, y, olc::Pixel(c.GetRed(), c.GetGreen(), c.GetBlue(), c.GetAlpha()));
				}
			delete bmp;
			return olc::rcode::OK;
		}

		olc::rcode SaveImageResource(olc::Sprite* spr, const std::string& sImageFile) override
		{
			return olc::rcode::OK;
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END IMAGE LOADER: GDI+                                                       |
// O------------------------------------------------------------------------------O
#pragma endregion

#pragma region image_libpng
// O------------------------------------------------------------------------------O
// | START IMAGE LOADER: libpng, default on linux, requires -lpng  (libpng-dev)   |
// O------------------------------------------------------------------------------O
#if defined(OLC_IMAGE_LIBPNG)
#include <png.h>
namespace olc
{
	void pngReadStream(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		png_voidp a = png_get_io_ptr(pngPtr);
		((std::istream*)a)->read((char*)data, length);
	}

	class ImageLoader_LibPNG : public olc::ImageLoader
	{
	public:
		ImageLoader_LibPNG() : ImageLoader()
		{}

		olc::rcode LoadImageResource(olc::Sprite* spr, const std::string& sImageFile, olc::ResourcePack* pack) override
		{
			UNUSED(pack);

			// clear out existing sprite
			spr->pColData.clear();

			////////////////////////////////////////////////////////////////////////////
			// Use libpng, Thanks to Guillaume Cottenceau
			// https://gist.github.com/niw/5963798
			// Also reading png from streams
			// http://www.piko3d.net/tutorials/libpng-tutorial-loading-png-files-from-streams/
			png_structp png;
			png_infop info;

			auto loadPNG = [&]()
			{
				png_read_info(png, info);
				png_byte color_type;
				png_byte bit_depth;
				png_bytep* row_pointers;
				spr->width = png_get_image_width(png, info);
				spr->height = png_get_image_height(png, info);
				color_type = png_get_color_type(png, info);
				bit_depth = png_get_bit_depth(png, info);
				if (bit_depth == 16) png_set_strip_16(png);
				if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
				if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)	png_set_expand_gray_1_2_4_to_8(png);
				if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
				if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
					png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
				if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
					png_set_gray_to_rgb(png);
				png_read_update_info(png, info);
				row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * spr->height);
				for (int y = 0; y < spr->height; y++) {
					row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
				}
				png_read_image(png, row_pointers);
				////////////////////////////////////////////////////////////////////////////
				// Create sprite array
				spr->pColData.resize(spr->width * spr->height);
				// Iterate through image rows, converting into sprite format
				for (int y = 0; y < spr->height; y++)
				{
					png_bytep row = row_pointers[y];
					for (int x = 0; x < spr->width; x++)
					{
						png_bytep px = &(row[x * 4]);
						spr->SetPixel(x, y, Pixel(px[0], px[1], px[2], px[3]));
					}
				}

				for (int y = 0; y < spr->height; y++) // Thanks maksym33
					free(row_pointers[y]);
				free(row_pointers);
				png_destroy_read_struct(&png, &info, nullptr);
			};

			png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png) goto fail_load;

			info = png_create_info_struct(png);
			if (!info) goto fail_load;

			if (setjmp(png_jmpbuf(png))) goto fail_load;

			if (pack == nullptr)
			{
				FILE* f = fopen(sImageFile.c_str(), "rb");
				if (!f) return olc::rcode::NO_FILE;
				png_init_io(png, f);
				loadPNG();
				fclose(f);
			}
			else
			{
				ResourceBuffer rb = pack->GetFileBuffer(sImageFile);
				std::istream is(&rb);
				png_set_read_fn(png, (png_voidp)&is, pngReadStream);
				loadPNG();
			}

			return olc::rcode::OK;

		fail_load:
			spr->width = 0;
			spr->height = 0;
			spr->pColData.clear();
			return olc::rcode::FAIL;
		}

		olc::rcode SaveImageResource(olc::Sprite* spr, const std::string& sImageFile) override
		{
			return olc::rcode::OK;
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END IMAGE LOADER:                                                            |
// O------------------------------------------------------------------------------O
#pragma endregion


// O------------------------------------------------------------------------------O
// | olcPixelGameEngine Platforms                                                 |
// O------------------------------------------------------------------------------O

#pragma region platform_windows
// O------------------------------------------------------------------------------O
// | START PLATFORM: MICROSOFT WINDOWS XP, VISTA, 7, 8, 10                        |
// O------------------------------------------------------------------------------O
#if defined(OLC_PLATFORM_WINAPI)

#if defined(_WIN32) && !defined(__MINGW32__)
	#pragma comment(lib, "user32.lib")		// Visual Studio Only
	#pragma comment(lib, "gdi32.lib")		// For other Windows Compilers please add
	#pragma comment(lib, "opengl32.lib")	// these libs to your linker input
#endif

namespace olc
{
	class Platform_Windows : public olc::Platform
	{
	private:
		HWND olc_hWnd = nullptr;
		std::wstring wsAppName;
		inline static olc::vi2d vWinPos;
		inline static olc::vi2d vWinSize;

		std::wstring ConvertS2W(std::string s)
		{
#ifdef __MINGW32__
			wchar_t* buffer = new wchar_t[s.length() + 1];
			mbstowcs(buffer, s.c_str(), s.length());
			buffer[s.length()] = L'\0';
#else
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
#endif
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}



	public:
		virtual olc::rcode ApplicationStartUp() override { return olc::rcode::OK; }
		virtual olc::rcode ApplicationCleanUp() override { return olc::rcode::OK; }
		virtual olc::rcode ThreadStartUp() override { return olc::rcode::OK; }

		virtual olc::rcode ThreadCleanUp() override
		{
			renderer->DestroyDevice();
			PostMessage(olc_hWnd, WM_DESTROY, 0, 0);
			return olc::OK;
		}

		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) override
		{
			if (renderer->CreateDevice({ olc_hWnd }, bFullScreen, bEnableVSYNC) == olc::rcode::OK)
			{
				renderer->UpdateViewport(vViewPos, vViewSize);
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}

		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) override
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
			wc.lpszClassName = olcT("OLC_PIXEL_GAME_ENGINE");
			RegisterClass(&wc);

			vWinPos = vWindowPos;
			vWinSize = vWindowSize;

			// Define window furniture
			DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;

			olc::vi2d vTopLeft = vWindowPos;

			// Handle Fullscreen
			if (bFullScreen)
			{
				dwExStyle = 0;
				dwStyle = WS_VISIBLE | WS_POPUP;
				HMONITOR hmon = MonitorFromWindow(olc_hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };
				if (!GetMonitorInfo(hmon, &mi)) return olc::rcode::FAIL;
				vWindowSize = { mi.rcMonitor.right, mi.rcMonitor.bottom };
				vTopLeft.x = 0;
				vTopLeft.y = 0;
			}

			// Keep client size as requested
			RECT rWndRect = { 0, 0, vWindowSize.x, vWindowSize.y };
			AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
			int width = rWndRect.right - rWndRect.left;
			int height = rWndRect.bottom - rWndRect.top;

			olc_hWnd = CreateWindowEx(dwExStyle, olcT("OLC_PIXEL_GAME_ENGINE"), olcT(""), dwStyle,
				vTopLeft.x, vTopLeft.y, width, height, NULL, NULL, GetModuleHandle(nullptr), this);

			DragAcceptFiles(olc_hWnd, true);

			// Create Keyboard Mapping
			mapKeys[0x00] = Key::NONE;
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
			//mapKeys[VK_RETURN] = Key::ENTER;// mapKeys[VK_RETURN] = Key::RETURN;
			
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

			// Thanks scripticuk
			mapKeys[VK_OEM_1] = Key::OEM_1;			// On US and UK keyboards this is the ';:' key
			mapKeys[VK_OEM_2] = Key::OEM_2;			// On US and UK keyboards this is the '/?' key
			mapKeys[VK_OEM_3] = Key::OEM_3;			// On US keyboard this is the '~' key
			mapKeys[VK_OEM_4] = Key::OEM_4;			// On US and UK keyboards this is the '[{' key
			mapKeys[VK_OEM_5] = Key::OEM_5;			// On US keyboard this is '\|' key.
			mapKeys[VK_OEM_6] = Key::OEM_6;			// On US and UK keyboards this is the ']}' key
			mapKeys[VK_OEM_7] = Key::OEM_7;			// On US keyboard this is the single/double quote key. On UK, this is the single quote/@ symbol key
			mapKeys[VK_OEM_8] = Key::OEM_8;			// miscellaneous characters. Varies by keyboard
			mapKeys[VK_OEM_PLUS] = Key::EQUALS;		// the '+' key on any keyboard
			mapKeys[VK_OEM_COMMA] = Key::COMMA;		// the comma key on any keyboard
			mapKeys[VK_OEM_MINUS] = Key::MINUS;		// the minus key on any keyboard
			mapKeys[VK_OEM_PERIOD] = Key::PERIOD;	// the period key on any keyboard
			mapKeys[VK_CAPITAL] = Key::CAPS_LOCK;
			return olc::OK;
		}

		virtual olc::rcode SetWindowTitle(const std::string& s) override
		{
#ifdef UNICODE
			SetWindowText(olc_hWnd, ConvertS2W(s).c_str());
#else
			SetWindowText(olc_hWnd, s.c_str());
#endif
			return olc::OK;
		}

		olc::rcode ShowWindowFrame(const bool bShowFrame)
		{
			// Oooooooof... yeah....
			DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;
			
			RECT rWndRect, rWndRectNow;
			GetWindowRect(olc_hWnd, &rWndRectNow);
			
			if (!bShowFrame)
			{
				LONG_PTR lp = GetWindowLongPtr(olc_hWnd, GWL_STYLE);
				SetWindowLongPtr(olc_hWnd, GWL_STYLE, lp & ~(WS_CAPTION | WS_SYSMENU | WS_POPUPWINDOW | WS_THICKFRAME));								
				lp = GetWindowLongPtr(olc_hWnd, GWL_EXSTYLE);
				SetWindowLongPtr(olc_hWnd, GWL_EXSTYLE, lp & ~(WS_EX_WINDOWEDGE));
				dwExStyle = WS_EX_APPWINDOW;
				dwStyle = 0;			
			}
			else
			{
				LONG_PTR lp = GetWindowLongPtr(olc_hWnd, GWL_STYLE);
				SetWindowLongPtr(olc_hWnd, GWL_STYLE, lp | (WS_CAPTION | WS_SYSMENU | WS_POPUPWINDOW|WS_THICKFRAME));
				lp = GetWindowLongPtr(olc_hWnd, GWL_EXSTYLE);
				SetWindowLongPtr(olc_hWnd, GWL_EXSTYLE, lp | (WS_EX_WINDOWEDGE));
			}

			
			rWndRectNow.right = rWndRectNow.left + vWinSize.x;
			rWndRectNow.bottom = rWndRectNow.top + vWinSize.y;
			rWndRect = rWndRectNow;
			AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
			int width = rWndRect.right - rWndRect.left;
			int height = rWndRect.bottom - rWndRect.top;
			vWinPos = { rWndRect.left, rWndRect.top };
			vWinSize = { width, height };
			SetWindowPos(olc_hWnd, NULL, rWndRectNow.left, rWndRectNow.top, width, height, SWP_SHOWWINDOW);


			return olc::OK;
		}

		olc::rcode SetWindowSize(const olc::vi2d& vWindowPos, const olc::vi2d& vWindowSize)
		{
			vWinPos = vWindowPos;
			vWinSize = vWindowSize;
			RECT rWndRect;
			rWndRect.left = vWinPos.x;
			rWndRect.top = vWinPos.y;
			rWndRect.right = rWndRect.left + vWinSize.x;
			rWndRect.bottom = rWndRect.top + vWinSize.y;
			rWndRect = rWndRect;
			DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;
			AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
			int width = rWndRect.right - rWndRect.left;
			int height = rWndRect.bottom - rWndRect.top;
			vWinPos = { rWndRect.left, rWndRect.top };
			vWinSize = { width, height };
			SetWindowPos(olc_hWnd, NULL, vWinPos.x, vWinPos.y, width, height, SWP_SHOWWINDOW);
			return olc::OK;
		}

		virtual olc::rcode StartSystemEventLoop() override
		{
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return olc::OK;
		}

		virtual olc::rcode HandleSystemEvent() override { return olc::rcode::FAIL; }

		// Windows Event Handler - this is statically connected to the windows event system
		static LRESULT CALLBACK olc_WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch (uMsg)
			{
			case WM_MOUSEMOVE:
			{
				// Thanks @ForAbby (Discord)
				uint16_t x = lParam & 0xFFFF; uint16_t y = (lParam >> 16) & 0xFFFF;
				int16_t ix = *(int16_t*)&x;   int16_t iy = *(int16_t*)&y;
				ptrPGE->olc_UpdateMouse(ix, iy);
				return 0;
			}
			case WM_MOVE:       vWinPos = olc::vi2d(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);  ptrPGE->olc_UpdateWindowPos(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);	return 0;
			case WM_SIZE:       vWinSize = olc::vi2d(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);  ptrPGE->olc_UpdateWindowSize(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);	return 0;
			case WM_MOUSEWHEEL:	ptrPGE->olc_UpdateMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));           return 0;
			case WM_MOUSELEAVE: ptrPGE->olc_UpdateMouseFocus(false);                                    return 0;
			case WM_SETFOCUS:	ptrPGE->olc_UpdateKeyFocus(true);                                       return 0;
			case WM_KILLFOCUS:	ptrPGE->olc_UpdateKeyFocus(false);                                      return 0;
			case WM_KEYDOWN:	ptrPGE->olc_UpdateKeyState(mapKeys[wParam], true);                      return 0;
			case WM_KEYUP:		ptrPGE->olc_UpdateKeyState(mapKeys[wParam], false);                     return 0;
			case WM_SYSKEYDOWN: ptrPGE->olc_UpdateKeyState(mapKeys[wParam], true);						return 0;
			case WM_SYSKEYUP:	ptrPGE->olc_UpdateKeyState(mapKeys[wParam], false);						return 0;
			case WM_LBUTTONDOWN:ptrPGE->olc_UpdateMouseState(0, true);                                  return 0;
			case WM_LBUTTONUP:	ptrPGE->olc_UpdateMouseState(0, false);                                 return 0;
			case WM_RBUTTONDOWN:ptrPGE->olc_UpdateMouseState(1, true);                                  return 0;
			case WM_RBUTTONUP:	ptrPGE->olc_UpdateMouseState(1, false);                                 return 0;
			case WM_MBUTTONDOWN:ptrPGE->olc_UpdateMouseState(2, true);                                  return 0;
			case WM_MBUTTONUP:	ptrPGE->olc_UpdateMouseState(2, false);                                 return 0;
			case WM_DROPFILES:
			{
				// This is all eww...
				HDROP drop = (HDROP)wParam;
				
				uint32_t nFiles = DragQueryFile(drop, 0xFFFFFFFF, nullptr, 0);
				std::vector<std::string> vFiles;
				for (uint32_t i = 0; i < nFiles; i++)
				{
					TCHAR dfbuffer[256]{};
					uint32_t len = DragQueryFile(drop, i, nullptr, 0);
					DragQueryFile(drop, i, dfbuffer, 256);
#ifdef UNICODE
	#ifdef __MINGW32__
					char* buffer = new char[len + 1];
					wcstombs(buffer, dfbuffer, len);
					buffer[len] = '\0';
	#else
					int count = WideCharToMultiByte(CP_UTF8, 0, dfbuffer, -1, NULL, 0, NULL, NULL);
					char* buffer = new char[count];
					WideCharToMultiByte(CP_UTF8, 0, dfbuffer, -1, buffer, count, NULL, NULL);
	#endif				
					vFiles.push_back(std::string(buffer));
					delete[] buffer;
#else
					vFiles.push_back(std::string(dfbuffer));
#endif
				}

				// Even more eww...
				POINT p; DragQueryPoint(drop, &p);
				ptrPGE->olc_DropFiles(p.x, p.y, vFiles);
				DragFinish(drop);
				return 0;
			} 
			break;


			case WM_CLOSE:		ptrPGE->olc_Terminate();                                                return 0;
			case WM_DESTROY:	PostQuitMessage(0); DestroyWindow(hWnd);								return 0;
			}
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END PLATFORM: MICROSOFT WINDOWS XP, VISTA, 7, 8, 10                          |
// O------------------------------------------------------------------------------O
#pragma endregion 

#pragma region platform_linux
// O------------------------------------------------------------------------------O
// | START PLATFORM: LINUX                                                        |
// O------------------------------------------------------------------------------O
#if defined(OLC_PLATFORM_X11)
namespace olc
{
	class Platform_Linux : public olc::Platform
	{
	private:
		X11::Display* olc_Display = nullptr;
		X11::Window					 olc_WindowRoot;
		X11::Window					 olc_Window;
		X11::XVisualInfo* olc_VisualInfo;
		X11::Colormap                olc_ColourMap;
		X11::XSetWindowAttributes    olc_SetWindowAttribs;

	public:
		virtual olc::rcode ApplicationStartUp() override
		{
			return olc::rcode::OK;
		}

		virtual olc::rcode ApplicationCleanUp() override
		{
			XDestroyWindow(olc_Display, olc_Window);
			return olc::rcode::OK;
		}

		virtual olc::rcode ThreadStartUp() override
		{
			return olc::rcode::OK;
		}

		virtual olc::rcode ThreadCleanUp() override
		{
			renderer->DestroyDevice();
			return olc::OK;
		}

		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) override
		{
			if (renderer->CreateDevice({ olc_Display, &olc_Window, olc_VisualInfo }, bFullScreen, bEnableVSYNC) == olc::rcode::OK)
			{
				renderer->UpdateViewport(vViewPos, vViewSize);
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}

		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) override
		{
			using namespace X11;
			XInitThreads();

			// Grab the deafult display and window
			olc_Display = XOpenDisplay(NULL);
			olc_WindowRoot = DefaultRootWindow(olc_Display);

			// Based on the display capabilities, configure the appearance of the window
			GLint olc_GLAttribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			olc_VisualInfo = glXChooseVisual(olc_Display, 0, olc_GLAttribs);
			olc_ColourMap = XCreateColormap(olc_Display, olc_WindowRoot, olc_VisualInfo->visual, AllocNone);
			olc_SetWindowAttribs.colormap = olc_ColourMap;

			// Register which events we are interested in receiving
			olc_SetWindowAttribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | StructureNotifyMask;

			// Create the window
			olc_Window = XCreateWindow(olc_Display, olc_WindowRoot, vWindowPos.x, vWindowPos.y,
				vWindowSize.x, vWindowSize.y,
				0, olc_VisualInfo->depth, InputOutput, olc_VisualInfo->visual,
				CWColormap | CWEventMask, &olc_SetWindowAttribs);

			Atom wmDelete = XInternAtom(olc_Display, "WM_DELETE_WINDOW", true);
			XSetWMProtocols(olc_Display, olc_Window, &wmDelete, 1);

			XMapWindow(olc_Display, olc_Window);
			XStoreName(olc_Display, olc_Window, "OneLoneCoder.com - Pixel Game Engine");

			if (bFullScreen) // Thanks DragonEye, again :D
			{
				Atom wm_state;
				Atom fullscreen;
				wm_state = XInternAtom(olc_Display, "_NET_WM_STATE", False);
				fullscreen = XInternAtom(olc_Display, "_NET_WM_STATE_FULLSCREEN", False);
				XEvent xev{ 0 };
				xev.type = ClientMessage;
				xev.xclient.window = olc_Window;
				xev.xclient.message_type = wm_state;
				xev.xclient.format = 32;
				xev.xclient.data.l[0] = (bFullScreen ? 1 : 0);   // the action (0: off, 1: on, 2: toggle)
				xev.xclient.data.l[1] = fullscreen;             // first property to alter
				xev.xclient.data.l[2] = 0;                      // second property to alter
				xev.xclient.data.l[3] = 0;                      // source indication
				XMapWindow(olc_Display, olc_Window);
				XSendEvent(olc_Display, DefaultRootWindow(olc_Display), False,
					SubstructureRedirectMask | SubstructureNotifyMask, &xev);
				XFlush(olc_Display);
				XWindowAttributes gwa;
				XGetWindowAttributes(olc_Display, olc_Window, &gwa);
				vWindowSize.x = gwa.width;
				vWindowSize.y = gwa.height;
			}

			// Create Keyboard Mapping
			mapKeys[0x00] = Key::NONE;
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
			mapKeys[XK_space] = Key::SPACE; mapKeys[XK_period] = Key::PERIOD;

			mapKeys[XK_0] = Key::K0; mapKeys[XK_1] = Key::K1; mapKeys[XK_2] = Key::K2; mapKeys[XK_3] = Key::K3; mapKeys[XK_4] = Key::K4;
			mapKeys[XK_5] = Key::K5; mapKeys[XK_6] = Key::K6; mapKeys[XK_7] = Key::K7; mapKeys[XK_8] = Key::K8; mapKeys[XK_9] = Key::K9;

			mapKeys[XK_KP_0] = Key::NP0; mapKeys[XK_KP_1] = Key::NP1; mapKeys[XK_KP_2] = Key::NP2; mapKeys[XK_KP_3] = Key::NP3; mapKeys[XK_KP_4] = Key::NP4;
			mapKeys[XK_KP_5] = Key::NP5; mapKeys[XK_KP_6] = Key::NP6; mapKeys[XK_KP_7] = Key::NP7; mapKeys[XK_KP_8] = Key::NP8; mapKeys[XK_KP_9] = Key::NP9;
			mapKeys[XK_KP_Multiply] = Key::NP_MUL; mapKeys[XK_KP_Add] = Key::NP_ADD; mapKeys[XK_KP_Divide] = Key::NP_DIV; mapKeys[XK_KP_Subtract] = Key::NP_SUB; mapKeys[XK_KP_Decimal] = Key::NP_DECIMAL;

			// These keys vary depending on the keyboard. I've included comments for US and UK keyboard layouts
			mapKeys[XK_semicolon] = Key::OEM_1;		// On US and UK keyboards this is the ';:' key
			mapKeys[XK_slash] = Key::OEM_2;			// On US and UK keyboards this is the '/?' key
			mapKeys[XK_asciitilde] = Key::OEM_3;	// On US keyboard this is the '~' key
			mapKeys[XK_bracketleft] = Key::OEM_4;	// On US and UK keyboards this is the '[{' key
			mapKeys[XK_backslash] = Key::OEM_5;		// On US keyboard this is '\|' key.
			mapKeys[XK_bracketright] = Key::OEM_6;	// On US and UK keyboards this is the ']}' key
			mapKeys[XK_apostrophe] = Key::OEM_7;	// On US keyboard this is the single/double quote key. On UK, this is the single quote/@ symbol key
			mapKeys[XK_numbersign] = Key::OEM_8;	// miscellaneous characters. Varies by keyboard. I believe this to be the '#~' key on UK keyboards
			mapKeys[XK_equal] = Key::EQUALS;		// the '+' key on any keyboard
			mapKeys[XK_comma] = Key::COMMA;			// the comma key on any keyboard
			mapKeys[XK_minus] = Key::MINUS;			// the minus key on any keyboard			

			mapKeys[XK_Caps_Lock] = Key::CAPS_LOCK;

			return olc::OK;
		}

		virtual olc::rcode SetWindowTitle(const std::string& s) override
		{
			X11::XStoreName(olc_Display, olc_Window, s.c_str());
			return olc::OK;
		}

		virtual olc::rcode SetWindowSize(const olc::vi2d& vPos, const olc::vi2d& vSize) override
		{
			return olc::rcode::OK;
		}

		virtual olc::rcode ShowWindowFrame(const bool bShowFrame = true) override
		{
			return olc::rcode::OK;
		}


		virtual olc::rcode StartSystemEventLoop() override
		{
			return olc::OK;
		}

		virtual olc::rcode HandleSystemEvent() override
		{
			using namespace X11;
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
					ptrPGE->olc_UpdateWindowSize(gwa.width, gwa.height);
				}
				else if (xev.type == ConfigureNotify)
				{
					XConfigureEvent xce = xev.xconfigure;
					ptrPGE->olc_UpdateWindowSize(xce.width, xce.height);
				}
				else if (xev.type == KeyPress)
				{
					KeySym sym = XLookupKeysym(&xev.xkey, 0);
					ptrPGE->olc_UpdateKeyState(mapKeys[sym], true);
					XKeyEvent* e = (XKeyEvent*)&xev; // Because DragonEye loves numpads
					XLookupString(e, NULL, 0, &sym, NULL);
					ptrPGE->olc_UpdateKeyState(mapKeys[sym], true);
				}
				else if (xev.type == KeyRelease)
				{
					KeySym sym = XLookupKeysym(&xev.xkey, 0);
					ptrPGE->olc_UpdateKeyState(mapKeys[sym], false);
					XKeyEvent* e = (XKeyEvent*)&xev;
					XLookupString(e, NULL, 0, &sym, NULL);
					ptrPGE->olc_UpdateKeyState(mapKeys[sym], false);
				}
				else if (xev.type == ButtonPress)
				{
					switch (xev.xbutton.button)
					{
					case 1:	ptrPGE->olc_UpdateMouseState(0, true); break;
					case 2:	ptrPGE->olc_UpdateMouseState(2, true); break;
					case 3:	ptrPGE->olc_UpdateMouseState(1, true); break;
					case 4:	ptrPGE->olc_UpdateMouseWheel(120); break;
					case 5:	ptrPGE->olc_UpdateMouseWheel(-120); break;
					default: break;
					}
				}
				else if (xev.type == ButtonRelease)
				{
					switch (xev.xbutton.button)
					{
					case 1:	ptrPGE->olc_UpdateMouseState(0, false); break;
					case 2:	ptrPGE->olc_UpdateMouseState(2, false); break;
					case 3:	ptrPGE->olc_UpdateMouseState(1, false); break;
					default: break;
					}
				}
				else if (xev.type == MotionNotify)
				{
					ptrPGE->olc_UpdateMouse(xev.xmotion.x, xev.xmotion.y);
				}
				else if (xev.type == FocusIn)
				{
					ptrPGE->olc_UpdateKeyFocus(true);
				}
				else if (xev.type == FocusOut)
				{
					ptrPGE->olc_UpdateKeyFocus(false);
				}
				else if (xev.type == ClientMessage)
				{
					ptrPGE->olc_Terminate();
				}
			}
			return olc::OK;
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END PLATFORM: LINUX                                                          |
// O------------------------------------------------------------------------------O
#pragma endregion

#pragma region platform_glut
// O------------------------------------------------------------------------------O
// | START PLATFORM: GLUT (used to make it simple for Apple)                      |
// O------------------------------------------------------------------------------O
//
// VERY IMPORTANT!!! The Apple port was originally created by @Mumflr (discord)
// and the repo for the development of this project can be found here:
// https://github.com/MumflrFumperdink/olcPGEMac which contains maccy goodness
// and support on how to setup your build environment.
//
// "MASSIVE MASSIVE THANKS TO MUMFLR" - Javidx9
#if defined(OLC_PLATFORM_GLUT)
namespace olc {

	class Platform_GLUT : public olc::Platform
	{
	public:
		static std::atomic<bool>* bActiveRef;

		virtual olc::rcode SetWindowSize(const olc::vi2d& vPos, const olc::vi2d& vSize) override
		{
			return olc::rcode::OK;
		}

		virtual olc::rcode ShowWindowFrame(const bool bShowFrame = true) override
		{
			return olc::rcode::OK;
		}


		virtual olc::rcode ApplicationStartUp() override {
			return olc::rcode::OK;
		}

		virtual olc::rcode ApplicationCleanUp() override
		{
			return olc::rcode::OK;
		}

		virtual olc::rcode ThreadStartUp() override
		{
			return olc::rcode::OK;
		}

		virtual olc::rcode ThreadCleanUp() override
		{
			renderer->DestroyDevice();
			return olc::OK;
		}

		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) override
		{
			if (renderer->CreateDevice({}, bFullScreen, bEnableVSYNC) == olc::rcode::OK)
			{
				renderer->UpdateViewport(vViewPos, vViewSize);
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}

		static void ExitMainLoop() {
			if (!ptrPGE->OnUserDestroy()) {
				*bActiveRef = true;
				return;
			}
			platform->ThreadCleanUp();
			platform->ApplicationCleanUp();
			exit(0);
		}

#if defined(__APPLE__)
		static void scrollWheelUpdate(id selff, SEL _sel, id theEvent) {
			static const SEL deltaYSel = sel_registerName("deltaY");

#if defined(__aarch64__) // Thanks ruarq!
			double deltaY = ((double (*)(id, SEL))objc_msgSend)(theEvent, deltaYSel);
#else
			double deltaY = ((double (*)(id, SEL))objc_msgSend_fpret)(theEvent, deltaYSel);
#endif

			for (int i = 0; i < abs(deltaY); i++) {
				if (deltaY > 0) {
					ptrPGE->olc_UpdateMouseWheel(-1);
				}
				else if (deltaY < 0) {
					ptrPGE->olc_UpdateMouseWheel(1);
				}
			}
		}
#endif
		static void ThreadFunct() {
#if defined(__APPLE__)
			static bool hasEnabledCocoa = false;
			if (!hasEnabledCocoa) {
				// Objective-C Wizardry
				Class NSApplicationClass = objc_getClass("NSApplication");

				// NSApp = [NSApplication sharedApplication]
				SEL sharedApplicationSel = sel_registerName("sharedApplication");
				id NSApp = ((id(*)(Class, SEL))objc_msgSend)(NSApplicationClass, sharedApplicationSel);
				// window = [NSApp mainWindow]
				SEL mainWindowSel = sel_registerName("mainWindow");
				id window = ((id(*)(id, SEL))objc_msgSend)(NSApp, mainWindowSel);

				// [window setStyleMask: NSWindowStyleMaskClosable | ~NSWindowStyleMaskResizable]
				SEL setStyleMaskSel = sel_registerName("setStyleMask:");
				((void (*)(id, SEL, NSUInteger))objc_msgSend)(window, setStyleMaskSel, 7);

				hasEnabledCocoa = true;
			}
#endif
			if (!*bActiveRef) {
				ExitMainLoop();
				return;
			}
			glutPostRedisplay();
		}

		static void DrawFunct() {
			ptrPGE->olc_CoreUpdate();
		}

		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) override
		{
#if defined(__APPLE__)
			Class GLUTViewClass = objc_getClass("GLUTView");

			SEL scrollWheelSel = sel_registerName("scrollWheel:");
			bool resultAddMethod = class_addMethod(GLUTViewClass, scrollWheelSel, (IMP)scrollWheelUpdate, "v@:@");
			assert(resultAddMethod);
#endif

			renderer->PrepareDevice();

			if (bFullScreen)
			{
				vWindowSize.x = glutGet(GLUT_SCREEN_WIDTH);
				vWindowSize.y = glutGet(GLUT_SCREEN_HEIGHT);
				glutFullScreen();
			}
			else
			{
				if (vWindowSize.x > glutGet(GLUT_SCREEN_WIDTH) || vWindowSize.y > glutGet(GLUT_SCREEN_HEIGHT))
				{
					perror("ERROR: The specified window dimensions do not fit on your screen\n");
					return olc::FAIL;
				}
				glutReshapeWindow(vWindowSize.x, vWindowSize.y - 1);
			}

			// Create Keyboard Mapping
			mapKeys[0x00] = Key::NONE;
			mapKeys['A'] = Key::A; mapKeys['B'] = Key::B; mapKeys['C'] = Key::C; mapKeys['D'] = Key::D; mapKeys['E'] = Key::E;
			mapKeys['F'] = Key::F; mapKeys['G'] = Key::G; mapKeys['H'] = Key::H; mapKeys['I'] = Key::I; mapKeys['J'] = Key::J;
			mapKeys['K'] = Key::K; mapKeys['L'] = Key::L; mapKeys['M'] = Key::M; mapKeys['N'] = Key::N; mapKeys['O'] = Key::O;
			mapKeys['P'] = Key::P; mapKeys['Q'] = Key::Q; mapKeys['R'] = Key::R; mapKeys['S'] = Key::S; mapKeys['T'] = Key::T;
			mapKeys['U'] = Key::U; mapKeys['V'] = Key::V; mapKeys['W'] = Key::W; mapKeys['X'] = Key::X; mapKeys['Y'] = Key::Y;
			mapKeys['Z'] = Key::Z;

			mapKeys[GLUT_KEY_F1] = Key::F1; mapKeys[GLUT_KEY_F2] = Key::F2; mapKeys[GLUT_KEY_F3] = Key::F3; mapKeys[GLUT_KEY_F4] = Key::F4;
			mapKeys[GLUT_KEY_F5] = Key::F5; mapKeys[GLUT_KEY_F6] = Key::F6; mapKeys[GLUT_KEY_F7] = Key::F7; mapKeys[GLUT_KEY_F8] = Key::F8;
			mapKeys[GLUT_KEY_F9] = Key::F9; mapKeys[GLUT_KEY_F10] = Key::F10; mapKeys[GLUT_KEY_F11] = Key::F11; mapKeys[GLUT_KEY_F12] = Key::F12;

			mapKeys[GLUT_KEY_DOWN] = Key::DOWN; mapKeys[GLUT_KEY_LEFT] = Key::LEFT; mapKeys[GLUT_KEY_RIGHT] = Key::RIGHT; mapKeys[GLUT_KEY_UP] = Key::UP;
			mapKeys[13] = Key::ENTER;

			mapKeys[127] = Key::BACK; mapKeys[27] = Key::ESCAPE;
			mapKeys[9] = Key::TAB;  mapKeys[GLUT_KEY_HOME] = Key::HOME;
			mapKeys[GLUT_KEY_END] = Key::END; mapKeys[GLUT_KEY_PAGE_UP] = Key::PGUP; mapKeys[GLUT_KEY_PAGE_DOWN] = Key::PGDN;    mapKeys[GLUT_KEY_INSERT] = Key::INS;
			mapKeys[32] = Key::SPACE; mapKeys[46] = Key::PERIOD;

			mapKeys[48] = Key::K0; mapKeys[49] = Key::K1; mapKeys[50] = Key::K2; mapKeys[51] = Key::K3; mapKeys[52] = Key::K4;
			mapKeys[53] = Key::K5; mapKeys[54] = Key::K6; mapKeys[55] = Key::K7; mapKeys[56] = Key::K8; mapKeys[57] = Key::K9;

			// NOTE: MISSING KEYS :O

			glutKeyboardFunc([](unsigned char key, int x, int y) -> void {
				switch (glutGetModifiers()) {
				case 0: //This is when there are no modifiers
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				case GLUT_ACTIVE_SHIFT:
					ptrPGE->olc_UpdateKeyState(Key::SHIFT, true);
					break;
				case GLUT_ACTIVE_CTRL:
					if ('a' <= key && key <= 'z') key -= 32;
					ptrPGE->olc_UpdateKeyState(Key::CTRL, true);
					break;
				case GLUT_ACTIVE_ALT:
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				}

				if (mapKeys[key])
					ptrPGE->olc_UpdateKeyState(mapKeys[key], true);
				});

			glutKeyboardUpFunc([](unsigned char key, int x, int y) -> void {
				switch (glutGetModifiers()) {
				case 0: //This is when there are no modifiers
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				case GLUT_ACTIVE_SHIFT:
					ptrPGE->olc_UpdateKeyState(Key::SHIFT, false);
					break;
				case GLUT_ACTIVE_CTRL:
					if ('a' <= key && key <= 'z') key -= 32;
					ptrPGE->olc_UpdateKeyState(Key::CTRL, false);
					break;
				case GLUT_ACTIVE_ALT:
					if ('a' <= key && key <= 'z') key -= 32;
					//No ALT in PGE
					break;
				}

				if (mapKeys[key])
					ptrPGE->olc_UpdateKeyState(mapKeys[key], false);
				});

			//Special keys
			glutSpecialFunc([](int key, int x, int y) -> void {
				if (mapKeys[key])
					ptrPGE->olc_UpdateKeyState(mapKeys[key], true);
				});

			glutSpecialUpFunc([](int key, int x, int y) -> void {
				if (mapKeys[key])
					ptrPGE->olc_UpdateKeyState(mapKeys[key], false);
				});

			glutMouseFunc([](int button, int state, int x, int y) -> void {
				switch (button) {
				case GLUT_LEFT_BUTTON:
					if (state == GLUT_UP) ptrPGE->olc_UpdateMouseState(0, false);
					else if (state == GLUT_DOWN) ptrPGE->olc_UpdateMouseState(0, true);
					break;
				case GLUT_MIDDLE_BUTTON:
					if (state == GLUT_UP) ptrPGE->olc_UpdateMouseState(2, false);
					else if (state == GLUT_DOWN) ptrPGE->olc_UpdateMouseState(2, true);
					break;
				case GLUT_RIGHT_BUTTON:
					if (state == GLUT_UP) ptrPGE->olc_UpdateMouseState(1, false);
					else if (state == GLUT_DOWN) ptrPGE->olc_UpdateMouseState(1, true);
					break;
				}
				});

			auto mouseMoveCall = [](int x, int y) -> void {
				ptrPGE->olc_UpdateMouse(x, y);
			};

			glutMotionFunc(mouseMoveCall);
			glutPassiveMotionFunc(mouseMoveCall);

			glutEntryFunc([](int state) -> void {
				if (state == GLUT_ENTERED) ptrPGE->olc_UpdateKeyFocus(true);
				else if (state == GLUT_LEFT) ptrPGE->olc_UpdateKeyFocus(false);
				});

			glutDisplayFunc(DrawFunct);
			glutIdleFunc(ThreadFunct);

			return olc::OK;
		}

		virtual olc::rcode SetWindowTitle(const std::string& s) override
		{
			glutSetWindowTitle(s.c_str());
			return olc::OK;
		}

		virtual olc::rcode StartSystemEventLoop() override {
			glutMainLoop();
			return olc::OK;
		}

		virtual olc::rcode HandleSystemEvent() override
		{
			return olc::OK;
		}
	};

	std::atomic<bool>* Platform_GLUT::bActiveRef{ nullptr };

	//Custom Start
	olc::rcode PixelGameEngine::Start()
	{
		if (platform->ApplicationStartUp() != olc::OK) return olc::FAIL;

		// Construct the window
		if (platform->CreateWindowPane({ 30,30 }, vWindowSize, bFullScreen) != olc::OK) return olc::FAIL;
		olc_UpdateWindowSize(vWindowSize.x, vWindowSize.y);

		if (platform->ThreadStartUp() == olc::FAIL)  return olc::FAIL;
		olc_PrepareEngine();
		if (!OnUserCreate()) return olc::FAIL;
		Platform_GLUT::bActiveRef = &bAtomActive;
		glutWMCloseFunc(Platform_GLUT::ExitMainLoop);
		bAtomActive = true;
		platform->StartSystemEventLoop();

		//This code will not even be run but why not
		if (platform->ApplicationCleanUp() != olc::OK) return olc::FAIL;

		return olc::OK;
	}
}

#endif
// O------------------------------------------------------------------------------O
// | END PLATFORM: GLUT                                                           |
// O------------------------------------------------------------------------------O
#pragma endregion 





#pragma region platform_emscripten
// O------------------------------------------------------------------------------O
// | START PLATFORM: Emscripten - Totally Game Changing...                        |
// O------------------------------------------------------------------------------O

//
//	Firstly a big mega thank you to members of the OLC Community for sorting this 
//	out. Making a browser compatible version has been a priority for quite some
//	time, but I lacked the expertise to do it. This awesome feature is possible
//	because a group of former strangers got together and formed friendships over
//	their shared passion for code. If anything demonstrates how powerful helping
//	each other can be, it's this. - Javidx9

//	Emscripten Platform: MaGetzUb, Moros1138, Slavka, Dandistine, Gorbit99, Bispoo
//				   also: Ishidex, Gusgo99, SlicEnDicE, Alexio


#if defined(OLC_PLATFORM_EMSCRIPTEN)

#include <emscripten/html5.h>
#include <emscripten/key_codes.h>

extern "C" 
{
	EMSCRIPTEN_KEEPALIVE inline int olc_OnPageUnload()
	{ olc::platform->ApplicationCleanUp(); return 0; }
}

namespace olc
{
	class Platform_Emscripten : public olc::Platform
	{
	public:

		virtual olc::rcode SetWindowSize(const olc::vi2d& vPos, const olc::vi2d& vSize) override
		{ return olc::rcode::OK; }

		virtual olc::rcode ShowWindowFrame(const bool bShowFrame = true) override
		{ return olc::rcode::OK; }

		virtual olc::rcode ApplicationStartUp() override 
		{ return olc::rcode::OK; }

		virtual olc::rcode ApplicationCleanUp() override 
		{ ThreadCleanUp(); return olc::rcode::OK; }

		virtual olc::rcode ThreadStartUp() override
		{ return olc::rcode::OK; }

		virtual olc::rcode ThreadCleanUp() override
		{ renderer->DestroyDevice(); return olc::OK; }

		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) override
		{
			if (renderer->CreateDevice({}, bFullScreen, bEnableVSYNC) == olc::rcode::OK)
			{
				renderer->UpdateViewport(vViewPos, vViewSize);
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}

		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) override
		{
			emscripten_set_canvas_element_size("#canvas", vWindowSize.x, vWindowSize.y);

			mapKeys[DOM_PK_UNKNOWN] = Key::NONE;
			mapKeys[DOM_PK_A] = Key::A; mapKeys[DOM_PK_B] = Key::B; mapKeys[DOM_PK_C] = Key::C; mapKeys[DOM_PK_D] = Key::D;
			mapKeys[DOM_PK_E] = Key::E; mapKeys[DOM_PK_F] = Key::F; mapKeys[DOM_PK_G] = Key::G; mapKeys[DOM_PK_H] = Key::H;
			mapKeys[DOM_PK_I] = Key::I; mapKeys[DOM_PK_J] = Key::J; mapKeys[DOM_PK_K] = Key::K; mapKeys[DOM_PK_L] = Key::L;
			mapKeys[DOM_PK_M] = Key::M; mapKeys[DOM_PK_N] = Key::N; mapKeys[DOM_PK_O] = Key::O; mapKeys[DOM_PK_P] = Key::P;
			mapKeys[DOM_PK_Q] = Key::Q; mapKeys[DOM_PK_R] = Key::R; mapKeys[DOM_PK_S] = Key::S; mapKeys[DOM_PK_T] = Key::T;
			mapKeys[DOM_PK_U] = Key::U; mapKeys[DOM_PK_V] = Key::V; mapKeys[DOM_PK_W] = Key::W; mapKeys[DOM_PK_X] = Key::X;
			mapKeys[DOM_PK_Y] = Key::Y; mapKeys[DOM_PK_Z] = Key::Z;
			mapKeys[DOM_PK_0] = Key::K0; mapKeys[DOM_PK_1] = Key::K1; mapKeys[DOM_PK_2] = Key::K2;
			mapKeys[DOM_PK_3] = Key::K3; mapKeys[DOM_PK_4] = Key::K4; mapKeys[DOM_PK_5] = Key::K5;
			mapKeys[DOM_PK_6] = Key::K6; mapKeys[DOM_PK_7] = Key::K7; mapKeys[DOM_PK_8] = Key::K8;
			mapKeys[DOM_PK_9] = Key::K9;
			mapKeys[DOM_PK_F1] = Key::F1; mapKeys[DOM_PK_F2] = Key::F2; mapKeys[DOM_PK_F3] = Key::F3; mapKeys[DOM_PK_F4] = Key::F4;
			mapKeys[DOM_PK_F5] = Key::F5; mapKeys[DOM_PK_F6] = Key::F6; mapKeys[DOM_PK_F7] = Key::F7; mapKeys[DOM_PK_F8] = Key::F8;
			mapKeys[DOM_PK_F9] = Key::F9; mapKeys[DOM_PK_F10] = Key::F10; mapKeys[DOM_PK_F11] = Key::F11; mapKeys[DOM_PK_F12] = Key::F12;
			mapKeys[DOM_PK_ARROW_UP] = Key::UP; mapKeys[DOM_PK_ARROW_DOWN] = Key::DOWN;
			mapKeys[DOM_PK_ARROW_LEFT] = Key::LEFT; mapKeys[DOM_PK_ARROW_RIGHT] = Key::RIGHT;
			mapKeys[DOM_PK_SPACE] = Key::SPACE; mapKeys[DOM_PK_TAB] = Key::TAB;
			mapKeys[DOM_PK_SHIFT_LEFT] = Key::SHIFT; mapKeys[DOM_PK_SHIFT_RIGHT] = Key::SHIFT;
			mapKeys[DOM_PK_CONTROL_LEFT] = Key::CTRL; mapKeys[DOM_PK_CONTROL_RIGHT] = Key::CTRL;
			mapKeys[DOM_PK_INSERT] = Key::INS; mapKeys[DOM_PK_DELETE] = Key::DEL; mapKeys[DOM_PK_HOME] = Key::HOME;
			mapKeys[DOM_PK_END] = Key::END; mapKeys[DOM_PK_PAGE_UP] = Key::PGUP; mapKeys[DOM_PK_PAGE_DOWN] = Key::PGDN;
			mapKeys[DOM_PK_BACKSPACE] = Key::BACK; mapKeys[DOM_PK_ESCAPE] = Key::ESCAPE;
			mapKeys[DOM_PK_ENTER] = Key::ENTER; mapKeys[DOM_PK_NUMPAD_EQUAL] = Key::EQUALS;
			mapKeys[DOM_PK_NUMPAD_ENTER] = Key::ENTER; mapKeys[DOM_PK_PAUSE] = Key::PAUSE;
			mapKeys[DOM_PK_SCROLL_LOCK] = Key::SCROLL;
			mapKeys[DOM_PK_NUMPAD_0] = Key::NP0; mapKeys[DOM_PK_NUMPAD_1] = Key::NP1; mapKeys[DOM_PK_NUMPAD_2] = Key::NP2;
			mapKeys[DOM_PK_NUMPAD_3] = Key::NP3; mapKeys[DOM_PK_NUMPAD_4] = Key::NP4; mapKeys[DOM_PK_NUMPAD_5] = Key::NP5;
			mapKeys[DOM_PK_NUMPAD_6] = Key::NP6; mapKeys[DOM_PK_NUMPAD_7] = Key::NP7; mapKeys[DOM_PK_NUMPAD_8] = Key::NP8;
			mapKeys[DOM_PK_NUMPAD_9] = Key::NP9;
			mapKeys[DOM_PK_NUMPAD_MULTIPLY] = Key::NP_MUL; mapKeys[DOM_PK_NUMPAD_DIVIDE] = Key::NP_DIV;
			mapKeys[DOM_PK_NUMPAD_ADD] = Key::NP_ADD; mapKeys[DOM_PK_NUMPAD_SUBTRACT] = Key::NP_SUB;
			mapKeys[DOM_PK_NUMPAD_DECIMAL] = Key::NP_DECIMAL;
			mapKeys[DOM_PK_PERIOD] = Key::PERIOD; mapKeys[DOM_PK_EQUAL] = Key::EQUALS;
			mapKeys[DOM_PK_COMMA] = Key::COMMA; mapKeys[DOM_PK_MINUS] = Key::MINUS;
			mapKeys[DOM_PK_CAPS_LOCK] = Key::CAPS_LOCK;
			mapKeys[DOM_PK_SEMICOLON] = Key::OEM_1;	mapKeys[DOM_PK_SLASH] = Key::OEM_2; mapKeys[DOM_PK_BACKQUOTE] = Key::OEM_3;
			mapKeys[DOM_PK_BRACKET_LEFT] = Key::OEM_4; mapKeys[DOM_PK_BACKSLASH] = Key::OEM_5; mapKeys[DOM_PK_BRACKET_RIGHT] = Key::OEM_6;
			mapKeys[DOM_PK_QUOTE] = Key::OEM_7; mapKeys[DOM_PK_BACKSLASH] = Key::OEM_8;

			// Keyboard Callbacks
			emscripten_set_keydown_callback("#canvas", 0, 1, keyboard_callback);
			emscripten_set_keyup_callback("#canvas", 0, 1, keyboard_callback);

			// Mouse Callbacks
			emscripten_set_wheel_callback("#canvas", 0, 1, wheel_callback);
			emscripten_set_mousedown_callback("#canvas", 0, 1, mouse_callback);
			emscripten_set_mouseup_callback("#canvas", 0, 1, mouse_callback);
			emscripten_set_mousemove_callback("#canvas", 0, 1, mouse_callback);

			// Touch Callbacks
			emscripten_set_touchstart_callback("#canvas", 0, 1, touch_callback);
			emscripten_set_touchmove_callback("#canvas", 0, 1, touch_callback);
			emscripten_set_touchend_callback("#canvas", 0, 1, touch_callback);

			// Canvas Focus Callbacks
			emscripten_set_blur_callback("#canvas", 0, 1, focus_callback);
			emscripten_set_focus_callback("#canvas", 0, 1, focus_callback);
			
#pragma warning disable format
			EM_ASM( window.onunload = Module._olc_OnPageUnload; );

			// IMPORTANT! - Sorry About This...
			//
			//	In order to handle certain browser based events, such as resizing and
			//	going to full screen, we have to effectively inject code into the container
			//	running the PGE. Yes, I vomited about 11 times too when the others were
			//	convincing me this is the future. Well, this isnt the future, and if it
			//	were to be, I want no part of what must be a miserable distopian free
			//	for all of anarchic code injection to get rudimentary events like "Resize()".
			//
			//	Wake up people! Of course theres a spoon. There has to be to keep feeding
			//	the giant web baby.
			

			EM_ASM({

			// olc_ApsectRatio
			// 
			// Used by olc_ResizeHandler to calculate the viewport from the
			// dimensions of the canvas container's element.
			Module.olc_AspectRatio = $0 / $1;

			// HACK ALERT!
			// 
			// Here we assume any html shell that uses 3 or more instance of the class "emscripten"
			// is using one of the default or minimal emscripten page layouts
			Module.olc_AssumeDefaultShells = (document.querySelectorAll('.emscripten').length >= 3) ? true : false;

			// olc_ResizeHandler
			// 
			// Used by olc_Init, and is called when a resize observer and fullscreenchange event is triggered.
			var olc_ResizeHandler = function()
			{
				// are we in fullscreen mode?
				let isFullscreen = (document.fullscreenElement != null);

				// get the width of the containing element
				let width  = (isFullscreen) ? window.innerWidth  : Module.canvas.parentNode.clientWidth;
				let height = (isFullscreen) ? window.innerHeight : Module.canvas.parentNode.clientHeight;

				// calculate the expected viewport size
				let viewWidth  = width;
				let viewHeight = width / Module.olc_AspectRatio;

				// if we're taller than the containing element, recalculate based on height
				if(viewHeight > height)
				{
					viewWidth  = height * Module.olc_AspectRatio;
					viewHeight = height;
				}
    
				// ensure resulting viewport is in integer space
				viewWidth  = parseInt(viewWidth);
				viewHeight = parseInt(viewHeight);

				setTimeout(function()
				{
					// if default shells, apply default styles
					if(Module.olc_AssumeDefaultShells)
						Module.canvas.parentNode.setAttribute('style', 'width: 100%; height: 70vh; margin-left: auto; margin-right: auto;');
				
					// apply viewport dimensions to teh canvas
					Module.canvas.setAttribute('width', viewWidth);
					Module.canvas.setAttribute('height', viewHeight);
					Module.canvas.setAttribute('style', `width: ${viewWidth}px; height: ${viewHeight}px;`);
					
					// update the PGE window size
					Module._olc_PGE_UpdateWindowSize(viewWidth, viewHeight);
					
					// force focus on our PGE canvas
					Module.canvas.focus();
				}, 200);
			};

			
			// olc_Init
			// 
			// set up resize observer and fullscreenchange event handler
			var olc_Init = function()
			{
				if(Module.olc_AspectRatio === undefined)
				{
					setTimeout(function() { Module.olc_Init(); }, 50);
					return;
				}
					
				let resizeObserver = new ResizeObserver(function(entries)
				{
					Module.olc_ResizeHandler();
				}).observe(Module.canvas.parentNode);

				let mutationObserver = new MutationObserver(function(mutationsList, observer)
				{
					setTimeout(function() { Module.olc_ResizeHandler(); },  200);
				}).observe(Module.canvas.parentNode, { attributes: false, childList: true, subtree: false });

				window.addEventListener('fullscreenchange', function(e)
				{
					setTimeout(function() { Module.olc_ResizeHandler();},  200);
				});
			};
			
			// set up hooks
			Module.olc_ResizeHandler = (Module.olc_ResizeHandler != undefined) ? Module.olc_ResizeHandler : olc_ResizeHandler;
			Module.olc_Init          = (Module.olc_Init          != undefined) ? Module.olc_Init : olc_Init;

			// run everything!
			Module.olc_Init();

			}, vWindowSize.x, vWindowSize.y); // Fullscreen and Resize Observers
#pragma warning restore format
			return olc::rcode::OK;
		}

		// Interface PGE's UpdateWindowSize, for use in Javascript
		void UpdateWindowSize(int width, int height)
		{
			ptrPGE->olc_UpdateWindowSize(width, height);
		}

		//TY Gorbit
		static EM_BOOL focus_callback(int eventType, const EmscriptenFocusEvent* focusEvent, void* userData) 
		{
			if (eventType == EMSCRIPTEN_EVENT_BLUR)
			{
				ptrPGE->olc_UpdateKeyFocus(false);
				ptrPGE->olc_UpdateMouseFocus(false);
			}
			else if (eventType == EMSCRIPTEN_EVENT_FOCUS)
			{
				ptrPGE->olc_UpdateKeyFocus(true);
				ptrPGE->olc_UpdateMouseFocus(true);
			}

			return 0;
		}

		//TY Moros
		static EM_BOOL keyboard_callback(int eventType, const EmscriptenKeyboardEvent* e, void* userData)
		{
			if (eventType == EMSCRIPTEN_EVENT_KEYDOWN)
				ptrPGE->olc_UpdateKeyState(mapKeys[emscripten_compute_dom_pk_code(e->code)], true);

			// THANK GOD!! for this compute function. And thanks Dandistine for pointing it out!
			if (eventType == EMSCRIPTEN_EVENT_KEYUP)
				ptrPGE->olc_UpdateKeyState(mapKeys[emscripten_compute_dom_pk_code(e->code)], false);

			//Consume keyboard events so that keys like F1 and F5 don't do weird things
			return EM_TRUE;
		}

		//TY Moros
		static EM_BOOL wheel_callback(int eventType, const EmscriptenWheelEvent* e, void* userData)
		{
			if (eventType == EMSCRIPTEN_EVENT_WHEEL)
				ptrPGE->olc_UpdateMouseWheel(-1 * e->deltaY);

			return EM_TRUE;
		}

		//TY Bispoo
		static EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent* e, void* userData)
		{
			// Move
			if (eventType == EMSCRIPTEN_EVENT_TOUCHMOVE)
			{
				ptrPGE->olc_UpdateMouse(e->touches->targetX, e->touches->targetY);
			}

			// Start
			if (eventType == EMSCRIPTEN_EVENT_TOUCHSTART)
			{
				ptrPGE->olc_UpdateMouse(e->touches->targetX, e->touches->targetY);
				ptrPGE->olc_UpdateMouseState(0, true);
			}

			// End
			if (eventType == EMSCRIPTEN_EVENT_TOUCHEND)
			{
				ptrPGE->olc_UpdateMouseState(0, false);
			}

			return EM_TRUE;
		}

		//TY Moros
		static EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData)
		{
			//Mouse Movement
			if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
				ptrPGE->olc_UpdateMouse(e->targetX, e->targetY);


			//Mouse button press
			if (e->button == 0) // left click
			{
				if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptrPGE->olc_UpdateMouseState(0, true);
				else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
					ptrPGE->olc_UpdateMouseState(0, false);
			}

			if (e->button == 2) // right click
			{
				if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptrPGE->olc_UpdateMouseState(1, true);
				else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
					ptrPGE->olc_UpdateMouseState(1, false);

			}

			if (e->button == 1) // middle click
			{
				if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptrPGE->olc_UpdateMouseState(2, true);
				else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
					ptrPGE->olc_UpdateMouseState(2, false);

				//at the moment only middle mouse needs to consume events.
				return EM_TRUE;
			}

			return EM_FALSE;
		}


		virtual olc::rcode SetWindowTitle(const std::string& s) override
		{ emscripten_set_window_title(s.c_str()); return olc::OK; }

		virtual olc::rcode StartSystemEventLoop() override 
		{ return olc::OK; }

		virtual olc::rcode HandleSystemEvent() override
		{ return olc::OK; }

		static void MainLoop()
		{
			olc::Platform::ptrPGE->olc_CoreUpdate();
			if (!ptrPGE->olc_IsRunning())
			{
				if (ptrPGE->OnUserDestroy())
				{
					emscripten_cancel_main_loop();
					platform->ApplicationCleanUp();
				}
				else
				{
					ptrPGE->olc_Reanimate();
				}
			}
		}
	};

	//Emscripten needs a special Start function
	//Much of this is usually done in EngineThread, but that isn't used here
	olc::rcode PixelGameEngine::Start()
	{
		if (platform->ApplicationStartUp() != olc::OK) return olc::FAIL;

		// Construct the window
		if (platform->CreateWindowPane({ 30,30 }, vWindowSize, bFullScreen) != olc::OK) return olc::FAIL;
		olc_UpdateWindowSize(vWindowSize.x, vWindowSize.y);

		// Some implementations may form an event loop here
		if (platform->ThreadStartUp() == olc::FAIL)	return olc::FAIL;

		// Do engine context specific initialisation
		olc_PrepareEngine();

		// Consider the "thread" started
		bAtomActive = true;

		// Create user resources as part of this thread
		for (auto& ext : vExtensions) ext->OnBeforeUserCreate();
		if (!OnUserCreate()) bAtomActive = false;
		for (auto& ext : vExtensions) ext->OnAfterUserCreate();

		platform->StartSystemEventLoop();

		//This causes a heap memory corruption in Emscripten for some reason
		//Platform_Emscripten::bActiveRef = &bAtomActive;
		emscripten_set_main_loop(&Platform_Emscripten::MainLoop, 0, 1);

		// Wait for thread to be exited
		if (platform->ApplicationCleanUp() != olc::OK) return olc::FAIL;
		return olc::OK;
	}
}

extern "C" 
{
	EMSCRIPTEN_KEEPALIVE inline void olc_PGE_UpdateWindowSize(int width, int height)
	{
		emscripten_set_canvas_element_size("#canvas", width, height);
		// Thanks slavka
		((olc::Platform_Emscripten*)olc::platform.get())->UpdateWindowSize(width, height);
	}
}

#endif
// O------------------------------------------------------------------------------O
// | END PLATFORM: Emscripten                                                     |
// O------------------------------------------------------------------------------O
#pragma endregion


#endif // Headless

// O------------------------------------------------------------------------------O
// | olcPixelGameEngine Auto-Configuration                                        |
// O------------------------------------------------------------------------------O
#pragma region pge_config
namespace olc
{
	void PixelGameEngine::olc_ConfigureSystem()
	{

//#if !defined(OLC_PGE_HEADLESS)

		olc::Sprite::loader = nullptr;

#if defined(OLC_IMAGE_GDI)
		olc::Sprite::loader = std::make_unique<olc::ImageLoader_GDIPlus>();
#endif

#if defined(OLC_IMAGE_LIBPNG)
		olc::Sprite::loader = std::make_unique<olc::ImageLoader_LibPNG>();
#endif

#if defined(OLC_IMAGE_STB)
		olc::Sprite::loader = std::make_unique<olc::ImageLoader_STB>();
#endif

#if defined(OLC_IMAGE_CUSTOM_EX)
		olc::Sprite::loader = std::make_unique<OLC_IMAGE_CUSTOM_EX>();
#endif


#if defined(OLC_PLATFORM_HEADLESS)
		platform = std::make_unique<olc::Platform_Headless>();
#endif

#if defined(OLC_PLATFORM_WINAPI)
		platform = std::make_unique<olc::Platform_Windows>();
#endif

#if defined(OLC_PLATFORM_X11)
		platform = std::make_unique<olc::Platform_Linux>();
#endif

#if defined(OLC_PLATFORM_GLUT)
		platform = std::make_unique<olc::Platform_GLUT>();
#endif

#if defined(OLC_PLATFORM_EMSCRIPTEN)
		platform = std::make_unique<olc::Platform_Emscripten>();
#endif

#if defined(OLC_PLATFORM_CUSTOM_EX)
		platform = std::make_unique<OLC_PLATFORM_CUSTOM_EX>();
#endif

#if defined(OLC_GFX_HEADLESS)
		renderer = std::make_unique<olc::Renderer_Headless>();
#endif

#if defined(OLC_GFX_OPENGL10)
		renderer = std::make_unique<olc::Renderer_OGL10>();
#endif

#if defined(OLC_GFX_OPENGL33)
		renderer = std::make_unique<olc::Renderer_OGL33>();
#endif

#if defined(OLC_GFX_OPENGLES2)
		renderer = std::make_unique<olc::Renderer_OGLES2>();
#endif

#if defined(OLC_GFX_DIRECTX10)
		renderer = std::make_unique<olc::Renderer_DX10>();
#endif

#if defined(OLC_GFX_DIRECTX11)
		renderer = std::make_unique<olc::Renderer_DX11>();
#endif

#if defined(OLC_GFX_CUSTOM_EX)
		renderer = std::make_unique<OLC_RENDERER_CUSTOM_EX>();
#endif

		// Associate components with PGE instance
		platform->ptrPGE = this;
		renderer->ptrPGE = this;
//#else
//		olc::Sprite::loader = nullptr;
//		platform = nullptr;
//		renderer = nullptr;
//#endif
	}
}

#pragma endregion

#endif // End OLC_PGE_APPLICATION

// O------------------------------------------------------------------------------O
// | END OF OLC_PGE_APPLICATION                                                   |
// O------------------------------------------------------------------------------O

