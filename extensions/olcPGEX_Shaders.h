/*
	olcPGEX_Shaders.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                    Shaders   v1.00                          |
	+-------------------------------------------------------------+

	NOTE: UNDER ACTIVE DEVELOPMENT - THERE ARE BUGS/GLITCHES - VERY UNFINISHED
	SIMPLY PUT - DO NOT USE UNLESS YOU KNOW WHAT YOU ARE DOING :P

	What is this?
	~~~~~~~~~~~~~
	This is an "unfinished" extension that allows you to render to decals
	via a genuine GPU pixel shader. It has all sorts of caveats. Do not use.

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2025 OneLoneCoder.com

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
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020, 2021, 2022, 2023, 2024, 2025

	Revisions:
	1.00:	Initial Release
	
*/

#pragma once
#ifndef OLC_PGEX_SHADER_H
#define OLC_PGEX_SHADER_H

#if !defined(OLC_GFX_OPENGL33)
#error The olc::Shader Extension requires the OpenGL 3.3 Renderer to be specified!
#endif

#if !defined(OLC_USING_PGEX_SHADER)
#define OLC_USING_PGEX_SHADER
#endif


#define OLC_GFX_OPENGL33
#include "olcPixelGameEngine.h"

namespace olc
{
	struct EffectConfig
	{
		const std::string sVertexSource;
		const std::string sPixelSource;
		const size_t nInputs;
		const size_t nOutputs;
		const std::vector<std::tuple<std::string, std::string, std::string>> vAttributes;
	};


	namespace fx
	{

#if defined(__arm__) || defined(OLC_PLATFORM_EMSCRIPTEN)
#define SHADER_HEADER "#version 300 es         \n" \
                              "precision mediump float;\n"
#else
#define SHADER_HEADER "#version 330 core       \n"
#endif


#define DEFAULT_VS "void main()                                             \n" \
					   "{                                                       \n" \
					   "	float p = 1.0 / inPos.z;                            \n" \
					   "	gl_Position = p * vec4(inPos.x, inPos.y, 0.0, 1.0); \n" \
					   "	xUV1 = p * inUV1;                                   \n" \
					   "	xCol = inCol;                                       \n" \
					   "}                                                       \n"

#define DEFAULT_PS "void main()                                            \n" \
					   "{                                                      \n" \
					   "	pix_out = texture(tex1, xUV1) * xCol;              \n" \
					   "}                                                      \n"

		static EffectConfig FX_NORMAL =
		{
			DEFAULT_VS,
			DEFAULT_PS,
			1,
			1
		};

		static EffectConfig FX_GREYSCALE =
		{
			DEFAULT_VS,
			"void main()                                                \n"
			"{                                                          \n"
			"	vec4 o = texture(tex1, xUV1) * xCol;                    \n"
			"   float lum = o.r * 0.2126 + o.g * 0.7152 + o.b * 0.0722; \n"
			"	pix_out = vec4(lum, lum, lum, o.a);                     \n"
			"}                                                          \n",
			1,
			1
		};

		static EffectConfig FX_BOXBLUR =
		{
			DEFAULT_VS,

			"void main()                                                      \n"
			"{                                                                \n"
			"	vec2 texelSize = 1.0 / vec2(textureSize(tex1, 0));	          \n"
			"	vec4 result;										          \n"
			"	for (int x = -box_width; x < +box_width; ++x)				  \n"
			"	{															  \n"
			"		for (int y = -box_width; y < +box_width; ++y)			  \n"
			"		{														  \n"
			"			vec2 offset = vec2(float(x), float(y)) * texelSize;	  \n"
			"			result += texture(tex1, xUV1 + offset);               \n"
			"		}														  \n"
			"	}															  \n"
			"	pix_out = (result / (4.0 * float(box_width * box_width))) * xCol;		  \n"
			"}                                                                \n",
			1,
			1,

			{
				{"box_width", "int", "4"},
			}
		};

		static EffectConfig FX_THRESHOLD =
		{
			DEFAULT_VS,

			"void main()                                                        \n"
			"{                                                                  \n"
			"	vec4 o = texture(tex1, xUV1) * xCol;                            \n"
			"   float lum = o.r * 0.2126 + o.g * 0.7152 + o.b * 0.0722;         \n"
			"	pix_out = (lum > threshold) ? vec4(1, 1, 1, 1) : vec4(0,0,0,1);	\n"
			"}                                                                  \n",
			1,
			1,

			{
				{"threshold", "float", "0.5"},
			}
		};


		static EffectConfig FX_SCANLINE =
		{
			DEFAULT_VS,

			"void main()                                                      \n"
			"{                                                                \n"
			"	float scanline = sin(xUV1.y * frequency + phase) * intensity; \n"
			"	pix_out = (texture(tex1, xUV1) + vec4(scanline, scanline, scanline, 0.0)) * xCol;		  \n"
			"}                                                                \n",
			1,
			1,
			{
				{"frequency", "float", "400.0"},
				{"intensity", "float","0.4"},
				{"phase", "float", "0.0"}
			}
		};


		static EffectConfig FX_SOBEL =
		{
			DEFAULT_VS,

			"void make_kernel(inout vec4 n[9], sampler2D tex, vec2 coord)							\n"
			"{																						\n"
			"	vec2 texel = 1.0 / vec2(textureSize(tex, 0));										\n"
			"																						\n"
			"																						\n"
			"	n[0] = texture2D(tex, coord + vec2(-texel.x, -texel.y));							\n"
			"	n[1] = texture2D(tex, coord + vec2(0.0, -texel.y));									\n"
			"	n[2] = texture2D(tex, coord + vec2(texel.x, -texel.y));								\n"
			"	n[3] = texture2D(tex, coord + vec2(-texel.x, 0.0));									\n"
			"	n[4] = texture2D(tex, coord);														\n"
			"	n[5] = texture2D(tex, coord + vec2(texel.x, 0.0));									\n"
			"	n[6] = texture2D(tex, coord + vec2(-texel.x, texel.y));								\n"
			"	n[7] = texture2D(tex, coord + vec2(0.0, texel.y));									\n"
			"	n[8] = texture2D(tex, coord + vec2(texel.x, texel.y));								\n"
			"}																						\n"
			"																						\n"
			"void main(void)																		\n"
			"{																						\n"
			"	vec4 n[9];																			\n"
			"	make_kernel(n, tex1, xUV1);															\n"
			"																						\n"
			"	vec4 sobel_edge_h = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);		\n"
			"	vec4 sobel_edge_v = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);		\n"
			"	vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));	\n"
			"																						\n"
			"	pix_out = vec4(1.0 - sobel.rgb, 1.0);												\n"
			"}																			     		\n",
			1,
			1,
		};



	}


	class Effect
	{
	public:
		Effect() = default;

	public:
		bool IsOK() const;
		const std::string& GetStatus() const;
		const uint32_t GetTargetSlots() const;
		const uint32_t GetInputSlots() const;

	protected:
		friend class Shade;
		void AppendStatus(const std::string& sMsg);
		void SetResourceIDs(const uint32_t id, const uint32_t vsid, const uint32_t psid);
		void SetSlots(const uint32_t nInput, const uint32_t nTarget);

	protected:
		std::string m_sStatus;
		uint32_t m_nPSID = 0;
		uint32_t m_nVSID = 0;
		uint32_t m_nID = 0;
		uint32_t m_nInputSlots = 0;
		uint32_t m_nTargetSlots = 0;
	};

	class Shade : public olc::PGEX
	{
		friend class olc::Effect;

	public:
		Shade();

	public:
		olc::Effect MakeEffect(const olc::EffectConfig& premade);

	public:
		int32_t SetSourceDecal(olc::Decal* pDecal, const uint32_t nSlot = 0, const olc::vf2d& vSourcePos = { 0.0f, 0.0f }, const olc::vf2d& vSourceSize = { 1.0f, 1.0f });
		int32_t SetTargetDecal(olc::Decal* pDecal, const uint32_t nSlot = 0);
		int32_t Start(olc::Effect* pEffect);
		int32_t End();

	public: // Simplified Interface
		void Clear(const olc::Pixel& p = olc::BLANK);
		void DrawDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE);
		void DrawPartialDecal(const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE);
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& colours);



	public: // Advanced Interface
		int32_t DrawQuad(const olc::vf2d& vPos, const olc::vf2d& vSize);

	private:

		struct sResourceSlot
		{
			bool bInUse = false;
			int nTargetResID = 0;
			olc::vf2d vPos = { 0.0f, 0.0f };
			olc::vf2d vSize = { 1.0f, 1.0f };
			olc::vf2d vInvSize = { 1.0f, 1.0f };
		};

		std::array<sResourceSlot, 8> slotTarget;
		std::array<sResourceSlot, 8> slotSource;


		struct sOmniVertex
		{
			float pos[3];
			olc::Pixel col;
			olc::vf2d tex[8];
		};

		uint32_t m_nVB = 0;
		uint32_t m_nVA = 0;

		sOmniVertex pVertexMem[olc::OLC_MAX_VERTS];

		uint32_t m_nFBO = 0;
		uint32_t m_nEffectID = 0;

		int32_t RenderDecal();

		void Render(const olc::DecalInstance& decal);

		void RenderPolygon(
			const std::vector<olc::Decal*>& decals,	// Source Textures
			const std::vector<olc::vf2d>& pos,		// Vertex positions
			const std::vector<olc::vf2d>& uv,		// Texture Coords
			const std::vector<olc::Pixel>& colours, // Vertex Colours
			const olc::DecalStructure& structure);	// Primitive

		void ClearAllSlots();

		olc::Effect ConstructShader(const std::string& sVS, const std::string& sPS);

	private: // Local OpenGL Functions
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
		locGetUniformLocation_t* glGetUniformLocation = nullptr;
		locUniform1f_t* locUniform1f = nullptr;
		locUniform1i_t* locUniform1i = nullptr;
		locUniform2fv_t* locUniform2fv = nullptr;
		locGenFrameBuffers_t* locGenFrameBuffers = nullptr;
		locBindFrameBuffer_t* locBindFrameBuffer = nullptr;
		locCheckFrameBufferStatus_t* locCheckFrameBufferStatus = nullptr;
		locDeleteFrameBuffers_t* locDeleteFrameBuffers = nullptr;
		locFrameBufferTexture2D_t* locFrameBufferTexture2D = nullptr;
		locActiveTexture_t* locActiveTexture = nullptr;
		locDrawBuffers_t* locDrawBuffers = nullptr;
		locBlendFuncSeparate_t* locBlendFuncSeparate = nullptr;

		olc::Renderable dummyTex;

	protected:
		void OnBeforeUserCreate() override;

	};
}


#ifdef OLC_PGEX_SHADERS
#undef OLC_PGEX_SHADERS

namespace olc
{
	void Effect::SetSlots(const uint32_t nInput, const uint32_t nTarget)
	{
		m_nTargetSlots = nTarget;
		m_nInputSlots = nInput;
	}

	const uint32_t Effect::GetTargetSlots() const
	{
		return m_nTargetSlots;
	}

	const uint32_t Effect::GetInputSlots() const
	{
		return m_nInputSlots;
	}

	bool Effect::IsOK() const
	{
		return m_sStatus.empty();
	}

	const std::string& Effect::GetStatus() const
	{
		return m_sStatus;
	}

	void Effect::AppendStatus(const std::string& sMsg)
	{
		m_sStatus.append(sMsg);
	}

	void Effect::SetResourceIDs(const uint32_t id, const uint32_t vsid, const uint32_t psid)
	{
		m_nID = id;
		m_nVSID = vsid;
		m_nPSID = psid;
	}


	Shade::Shade() : PGEX(true)
	{

	}


	olc::Effect Shade::ConstructShader(const std::string& sVS, const std::string& sPS)
	{
		olc::Effect effect;

		char sError[512];
		GLsizei nLogSize = 0;

		// Vertex Shader
		GLuint vs_id = locCreateShader(0x8B31);
		const GLchar* vs_str = sVS.c_str();
		locShaderSource(vs_id, 1, &vs_str, NULL);
		locCompileShader(vs_id);

		// Check for errors
		locGetShaderInfoLog(vs_id, 512, &nLogSize, sError);
		if (nLogSize > 0)
		{
			effect.AppendStatus(std::string(sError, nLogSize));
		}

		// Pixel Shader
		GLuint ps_id = locCreateShader(0x8B30);
		const GLchar* ps_str = sPS.c_str();
		locShaderSource(ps_id, 1, &ps_str, NULL);
		locCompileShader(ps_id);

		// Check for errors
		locGetShaderInfoLog(ps_id, 512, &nLogSize, sError);
		if (nLogSize > 0)
		{
			effect.AppendStatus(std::string(sError, nLogSize));
		}

		// Combined Shader
		GLuint sh_id = locCreateProgram();
		locAttachShader(sh_id, ps_id);
		locAttachShader(sh_id, vs_id);
		locLinkProgram(sh_id);

		// Check for errors
		locGetShaderInfoLog(sh_id, 512, &nLogSize, sError);
		if (nLogSize > 0)
		{
			effect.AppendStatus(std::string(sError, nLogSize));
		}

		// Store resources in shader object
		effect.SetResourceIDs(sh_id, vs_id, ps_id);
		return effect;
	}


	olc::Effect Shade::MakeEffect(const olc::EffectConfig& premade)
	{
		// Construct Vertex Shader
		std::string sVertexShader =
			SHADER_HEADER
			"layout(location = 0) in vec3 inPos; \n"
			"layout(location = 1) in vec4 inCol; \n";

		for (size_t i = 0; i < premade.nInputs; i++)
		{
			sVertexShader.append("layout(location = " + std::to_string(2 + i) + ") in vec2 inUV" + std::to_string(1 + i) + "; \n");
		}

		for (size_t i = 0; i < premade.nInputs; i++)
		{
			sVertexShader.append("out vec2 xUV" + std::to_string(1 + i) + "; \n");
		}

		sVertexShader.append("out vec4 xCol; \n");
		sVertexShader.append(premade.sVertexSource);


		// Construct Pixel Shader
		std::string sPixelShader =
			SHADER_HEADER
			"in vec4 xCol; \n";

		for (size_t i = 0; i < premade.nInputs; i++)
		{
			sPixelShader.append("in vec2 xUV" + std::to_string(1 + i) + "; \n");
		}

		for (size_t i = 0; i < premade.nInputs; i++)
		{
			sPixelShader.append("uniform sampler2D tex" + std::to_string(1 + i) + ";\n");
		}

		for (const auto& attribute : premade.vAttributes)
		{
			sPixelShader.append("uniform " + std::get<1>(attribute) + " " + std::get<0>(attribute) + " = " + std::get<2>(attribute) + ";\n");
		}

		sPixelShader.append("out vec4 pix_out;\n");
		sPixelShader.append(premade.sPixelSource);

		olc::Effect effect = ConstructShader(sVertexShader, sPixelShader);
		effect.SetSlots(uint32_t(premade.nInputs), uint32_t(premade.nOutputs));
		return effect;
	}


	void Shade::OnBeforeUserCreate()
	{
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


		locGenFrameBuffers = OGL_LOAD(locGenFrameBuffers_t, glGenFramebuffers);
		locBindFrameBuffer = OGL_LOAD(locBindFrameBuffer_t, glBindFramebuffer);
		locCheckFrameBufferStatus = OGL_LOAD(locCheckFrameBufferStatus_t, glCheckFramebufferStatus);
		locDeleteFrameBuffers = OGL_LOAD(locDeleteFrameBuffers_t, glDeleteFramebuffers);
		locFrameBufferTexture2D = OGL_LOAD(locFrameBufferTexture2D_t, glFramebufferTexture2D);
		locActiveTexture = OGL_LOAD(locActiveTexture_t, glActiveTexture);

		locBlendFuncSeparate = OGL_LOAD(locBlendFuncSeparate_t, glBlendFuncSeparate);

#if !defined(OLC_PLATFORM_EMSCRIPTEN)
		locBindVertexArray = OGL_LOAD(locBindVertexArray_t, glBindVertexArray);
		locGenVertexArrays = OGL_LOAD(locGenVertexArrays_t, glGenVertexArrays);
		locDrawBuffers = OGL_LOAD(locDrawBuffers_t, glDrawBuffers);
#else
		locBindVertexArray = glBindVertexArrayOES;
		locGenVertexArrays = glGenVertexArraysOES;
		locDrawBuffers = glDrawBuffersNV;
#endif



		// Create a Frame Buffer Object that represents the
		// offscreen rendering target gubbins	
		locGenFrameBuffers(1, (GLuint*)&m_nFBO);

		// Create a Vertex object to represent polygons
		locGenBuffers(1, &m_nVB);
		locGenVertexArrays(1, &m_nVA);
		locBindVertexArray(m_nVB);

		locBindBuffer(0x8892, m_nVB);
		sOmniVertex verts[OLC_MAX_VERTS];
		locBufferData(0x8892, sizeof(sOmniVertex) * OLC_MAX_VERTS, verts, 0x88E0);

		locVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(0 * sizeof(float)));
		locVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(sOmniVertex), (void*)(3 * sizeof(float)));
		locVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(4 * sizeof(float)));
		locVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(6 * sizeof(float)));
		locVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(8 * sizeof(float)));
		locVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(10 * sizeof(float)));
		locVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(12 * sizeof(float)));
		locVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(14 * sizeof(float)));
		locVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(16 * sizeof(float)));
		locVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(18 * sizeof(float)));

		locEnableVertexAttribArray(0); // Pos
		locEnableVertexAttribArray(1); // Colour
		locEnableVertexAttribArray(2); // Tex 0
		locEnableVertexAttribArray(3); // Tex 1
		locEnableVertexAttribArray(4); // Tex 2
		locEnableVertexAttribArray(5); // Tex 3
		locEnableVertexAttribArray(6); // Tex 4
		locEnableVertexAttribArray(7); // Tex 5
		locEnableVertexAttribArray(8); // Tex 6
		locEnableVertexAttribArray(9); // Tex 7

		locBindBuffer(0x8892, 0);
		locBindVertexArray(0);

		dummyTex.Create(1, 1);
		dummyTex.Sprite()->GetData()[0] = olc::WHITE;
		dummyTex.Decal()->Update();
	}

	void Shade::ClearAllSlots()
	{
		slotTarget[0] = { false, 0, {0.0f, 0.0f}, {1.0f, 1.0f} };
	}

	int32_t Shade::SetSourceDecal(olc::Decal* pDecal, const uint32_t nSlot, const olc::vf2d& vSourcePos, const olc::vf2d& vSourceSize)
	{
		if (pDecal == nullptr)
		{
			slotSource[nSlot] = { false, dummyTex.Decal()->id, {0.0f, 0.0f}, {1.0f, 1.0f} };
		}
		else
		{
			slotSource[nSlot].bInUse = true;
			slotSource[nSlot].nTargetResID = pDecal->id;
			slotSource[nSlot].vPos = vSourcePos;
			slotSource[nSlot].vSize = vSourceSize;
		}

		locActiveTexture(0x84C0); // TODO
		glBindTexture(GL_TEXTURE_2D, slotSource[nSlot].nTargetResID);
		return 0;
	}

	int32_t Shade::SetTargetDecal(olc::Decal* pDecal, const uint32_t nSlot)
	{
		slotTarget[nSlot].bInUse = true;
		slotTarget[nSlot].nTargetResID = pDecal->id;
		slotTarget[nSlot].vSize = olc::vi2d{ pDecal->sprite->width, pDecal->sprite->height };
		slotTarget[nSlot].vInvSize = 1.0f / slotTarget[nSlot].vSize;
		return 0;
	}

	int32_t Shade::Start(olc::Effect* pEffect)
	{
		// Reconstruct frame buffer
		locBindFrameBuffer(36160U, m_nFBO);

		std::array<GLenum, 8> attachments =
		{ { 36064U, 36065U, 36066U, 36067U, 36068U, 36069U, 36070U, 36071U } };

		// Remove existing buffer links
		for (const auto& a : attachments)
		{
			locFrameBufferTexture2D(36160U, a, GL_TEXTURE_2D, 0, 0);
		}

		// Count Target slots in use from beginning
		int nTargetSlots = 0;
		while (slotTarget[nTargetSlots].nTargetResID > 0)
			nTargetSlots++;

		// Allocate target buffers
		locDrawBuffers(nTargetSlots, attachments.data());

		// Bind buffers to texture resources
		for (uint32_t i = 0; i < pEffect->GetTargetSlots(); i++)
		{
			locFrameBufferTexture2D(36160U, attachments[i], GL_TEXTURE_2D, slotTarget[i].nTargetResID, 0);
		}

		// Apply Effect
		locUseProgram(pEffect->m_nID);

		// Set vertex stream source
		locBindVertexArray(m_nVA);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		//glEnable(GL_BLEND);
		//locBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


#if defined(OLC_PLATFORM_EMSCRIPTEN)
		locVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), 0); locEnableVertexAttribArray(0);
		locVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(sOmniVertex), (void*)(3 * sizeof(float))); locEnableVertexAttribArray(1);
		locVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(sOmniVertex), (void*)(5 * sizeof(float)));	locEnableVertexAttribArray(2);
#endif

		glViewport(0, 0, int32_t(slotTarget[0].vSize.x), int32_t(slotTarget[0].vSize.y));

		return 0;
	}

	int32_t Shade::End()
	{
		// Unbind the frame buffer
		locBindBuffer(0x8892, 0);
		locBindVertexArray(0);
		locUseProgram(0);
		locBindFrameBuffer(36160U, 0);
		return 0;
	}


	int32_t Shade::DrawQuad(const olc::vf2d& vPos, const olc::vf2d& vSize)
	{
		locBindBuffer(0x8892, m_nVB);

		pVertexMem[0].pos[0] = vPos.x;
		pVertexMem[0].pos[1] = vPos.y;
		pVertexMem[0].pos[2] = 1.0f;
		pVertexMem[0].tex[0] = { 0.0f, 0.0f };
		pVertexMem[0].col = olc::WHITE;

		pVertexMem[1].pos[0] = vPos.x;
		pVertexMem[1].pos[1] = vPos.y + vSize.y;
		pVertexMem[1].pos[2] = 1.0f;
		pVertexMem[1].tex[0] = { 0.0f, 1.0f };
		pVertexMem[1].col = olc::WHITE;

		pVertexMem[2].pos[0] = vPos.x + vSize.x;
		pVertexMem[2].pos[1] = vPos.y + vSize.y;
		pVertexMem[2].pos[2] = 1.0f;
		pVertexMem[2].tex[0] = { 1.0f, 1.0f };
		pVertexMem[2].col = olc::WHITE;

		pVertexMem[3].pos[0] = vPos.x + vSize.x;
		pVertexMem[3].pos[1] = vPos.y;
		pVertexMem[3].pos[2] = 1.0f;
		pVertexMem[3].tex[0] = { 1.0f, 0.0f };
		pVertexMem[3].col = olc::WHITE;


		locBufferData(0x8892, sizeof(sOmniVertex) * 4, pVertexMem, 0x88E0);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		return 0;
	}


	void Shade::Clear(const olc::Pixel& p)
	{
		glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}



	void Shade::DrawDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& scale, const olc::Pixel& tint)
	{
		olc::vf2d vScreenSpacePos =
		{
			(std::floor(pos.x) * slotTarget[0].vInvSize.x) * 2.0f - 1.0f,
			((std::floor(pos.y) * slotTarget[0].vInvSize.y) * 2.0f - 1.0f),
		};

		olc::vf2d vScreenSpaceDim =
		{
			vScreenSpacePos.x + (2.0f * (float(decal->sprite->width) * slotTarget[0].vInvSize.x)) * scale.x,
			vScreenSpacePos.y + (2.0f * (float(decal->sprite->height) * slotTarget[0].vInvSize.y)) * scale.y
		};

		SetSourceDecal(decal, 0);

		DecalInstance di;
		di.decal = decal;
		di.points = 4;
		di.tint = { tint, tint, tint, tint };
		di.pos = { { vScreenSpacePos.x, vScreenSpacePos.y }, { vScreenSpacePos.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpacePos.y } };
		di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		di.w = { 1, 1, 1, 1 };
		Render(di);
	}

	void Shade::DrawPartialDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale, const olc::Pixel& tint)
	{
		olc::vf2d vScreenSpacePos =
		{
			(std::floor(pos.x) * slotTarget[0].vInvSize.x) * 2.0f - 1.0f,
			((std::floor(pos.y) * slotTarget[0].vInvSize.y) * 2.0f - 1.0f)
		};

		olc::vf2d vScreenSpaceDim =
		{
			vScreenSpacePos.x + (2.0f * source_size.x * slotTarget[0].vInvSize.x) * scale.x,
			vScreenSpacePos.y + (2.0f * source_size.y * slotTarget[0].vInvSize.y) * scale.y
		};

		SetSourceDecal(decal, 0);

		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.pos = { { vScreenSpacePos.x, vScreenSpacePos.y }, { vScreenSpacePos.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpacePos.y } };
		olc::vf2d uvtl = source_pos * decal->vUVScale;
		olc::vf2d uvbr = uvtl + (source_size * decal->vUVScale);
		di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
		di.w = { 1,1,1,1 };
		Render(di);
	}

	void Shade::DrawPartialDecal(const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint)
	{
		olc::vf2d vScreenSpacePos =
		{
			(std::floor(pos.x) * slotTarget[0].vInvSize.x) * 2.0f - 1.0f,
			((std::floor(pos.y) * slotTarget[0].vInvSize.y) * 2.0f - 1.0f)
		};

		olc::vf2d vScreenSpaceDim =
		{
			vScreenSpacePos.x + (2.0f * source_size.x * slotTarget[0].vInvSize.x),
			vScreenSpacePos.y + (2.0f * source_size.y * slotTarget[0].vInvSize.y)
		};

		SetSourceDecal(decal, 0);

		DecalInstance di;
		di.points = 4;
		di.decal = decal;
		di.tint = { tint, tint, tint, tint };
		di.pos = { { vScreenSpacePos.x, vScreenSpacePos.y }, { vScreenSpacePos.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpacePos.y } };
		olc::vf2d uvtl = (source_pos)*decal->vUVScale;
		olc::vf2d uvbr = uvtl + ((source_size)*decal->vUVScale);
		di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
		di.w = { 1,1,1,1 };
		Render(di);
	}

	void Shade::DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& colours)
	{
		// Assumed points are normalised, transform points into target space
		std::vector<olc::vf2d> vTransformedPos(pos.size());
		std::transform(pos.begin(), pos.end(), vTransformedPos.begin(),
			[this](const olc::vf2d& pin)
			{
				return (pin);  ///* slotTarget[0].vInvSize*/) * 2.0f - olc::vf2d{ 1.0f, 1.0f };
			});

		RenderPolygon({ decal }, vTransformedPos, uv, colours, DecalStructure::FAN);
	}

	void Shade::RenderPolygon(
		const std::vector<olc::Decal*>& decals,	// Source Textures
		const std::vector<olc::vf2d>& pos,		// Vertex positions
		const std::vector<olc::vf2d>& uv,		// Texture Coords
		const std::vector<olc::Pixel>& colours, // Vertex Colours
		const olc::DecalStructure& structure)	// Primitive
	{
		// Setup multi-texture sources
		for (size_t i = 0; i < decals.size(); i++)
			SetSourceDecal(decals[i], uint32_t(i));

		// Populate vertex buffer (note for now, texture coords are shared)
		for (uint32_t i = 0; i < pos.size(); i++)
		{
			pVertexMem[i].pos[0] = pos[i].x;
			pVertexMem[i].pos[1] = pos[i].y;
			pVertexMem[i].pos[2] = 1.0f;
			pVertexMem[i].tex[0] = uv[i];
			pVertexMem[i].col = colours[i];
		}

		// Bind vertex buffer
		locBindBuffer(0x8892, m_nVB);

		// Send to GPU
		locBufferData(0x8892, sizeof(sOmniVertex) * pos.size(), pVertexMem, 0x88E0);

		// Draw!
		switch (structure)
		{
		case DecalStructure::FAN:
			glDrawArrays(GL_TRIANGLE_FAN, 0, GLsizei(pos.size()));
			break;
		case DecalStructure::LIST:
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(pos.size()));
			break;
		case DecalStructure::STRIP:
			glDrawArrays(GL_TRIANGLE_STRIP, 0, GLsizei(pos.size()));
			break;
		}
	}


	void Shade::Render(const olc::DecalInstance& decal)
	{
		locBindBuffer(0x8892, m_nVB);

		for (uint32_t i = 0; i < decal.points; i++)
		{
			pVertexMem[i].pos[0] = decal.pos[i].x;
			pVertexMem[i].pos[1] = decal.pos[i].y;
			pVertexMem[i].pos[2] = decal.w[i];
			pVertexMem[i].tex[0] = { decal.uv[i].x, decal.uv[i].y };
			pVertexMem[i].col = decal.tint[i];
		}


		locBufferData(0x8892, sizeof(sOmniVertex) * decal.points, pVertexMem, 0x88E0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, decal.points);
	}
}

#endif // OLC_PGEX_SHADER
#endif // OLC_PGEX_SHADER_H