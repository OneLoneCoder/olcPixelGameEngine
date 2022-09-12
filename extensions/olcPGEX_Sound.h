/*
	olcPGEX_Sound.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                       Sound - v0.4                          |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	sound generation and wave playing routines.

	Special Thanks:
	~~~~~~~~~~~~~~~	
	Slavka - For entire non-windows system back end!
	Gorbit99 - Testing, Bug Fixes
	Cyberdroid - Testing, Bug Fixes
	Dragoneye - Testing
	Puol - Testing

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2019 OneLoneCoder.com

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

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/


#ifndef OLC_PGEX_SOUND_H
#define OLC_PGEX_SOUND_H

#include <istream>
#include <cstring>
#include <climits>
#include <condition_variable>
#include <algorithm>
#undef min
#undef max

// Choose a default sound backend
#if !defined(USE_ALSA) && !defined(USE_OPENAL) && !defined(USE_WINDOWS)
#ifdef __linux__
#define USE_ALSA
#endif

#ifdef __EMSCRIPTEN__
#define USE_OPENAL
#endif

#ifdef _WIN32
#define USE_WINDOWS
#endif

#endif

#ifdef USE_ALSA
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#endif

#ifdef USE_OPENAL
#include <AL/al.h>
#include <AL/alc.h>
#include <queue>
#endif

#pragma pack(push, 1)
typedef struct {
	uint16_t wFormatTag;
	uint16_t nChannels;
	uint32_t nSamplesPerSec;
	uint32_t nAvgBytesPerSec;
	uint16_t nBlockAlign;
	uint16_t wBitsPerSample;
	uint16_t cbSize;
} OLC_WAVEFORMATEX;
#pragma pack(pop)

namespace olc
{
	// Container class for Advanced 2D Drawing functions
	class SOUND : public olc::PGEX
	{
		// A representation of an affine transform, used to rotate, scale, offset & shear space
	public:
		class AudioSample
		{
		public:
			AudioSample();
			AudioSample(std::string sWavFile, olc::ResourcePack *pack = nullptr);
			olc::rcode LoadFromFile(std::string sWavFile, olc::ResourcePack *pack = nullptr);

		public:
			OLC_WAVEFORMATEX wavHeader;
			float *fSample = nullptr;
			long nSamples = 0;
			int nChannels = 0;
			bool bSampleValid = false;
		};

		struct sCurrentlyPlayingSample
		{
			int nAudioSampleID = 0;
			long nSamplePosition = 0;
			bool bFinished = false;
			bool bLoop = false;
			bool bFlagForStop = false;
		};

		static std::list<sCurrentlyPlayingSample> listActiveSamples;

	public:
		static bool InitialiseAudio(unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512);
		static bool DestroyAudio();
		static void SetUserSynthFunction(std::function<float(int, float, float)> func);
		static void SetUserFilterFunction(std::function<float(int, float, float)> func);

	public:
		static int LoadAudioSample(std::string sWavFile, olc::ResourcePack *pack = nullptr);
		static void PlaySample(int id, bool bLoop = false);
		static void StopSample(int id);
		static void StopAll();
		static float GetMixerOutput(int nChannel, float fGlobalTime, float fTimeStep);


	private:
#ifdef USE_WINDOWS // Windows specific sound management
		static void CALLBACK waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2);
		static unsigned int m_nSampleRate;
		static unsigned int m_nChannels;
		static unsigned int m_nBlockCount;
		static unsigned int m_nBlockSamples;
		static unsigned int m_nBlockCurrent;
		static short* m_pBlockMemory;
		static WAVEHDR *m_pWaveHeaders;
		static HWAVEOUT m_hwDevice;
		static std::atomic<unsigned int> m_nBlockFree;
		static std::condition_variable m_cvBlockNotZero;
		static std::mutex m_muxBlockNotZero;
#endif

#ifdef USE_ALSA
		static snd_pcm_t *m_pPCM;
		static unsigned int m_nSampleRate;
		static unsigned int m_nChannels;
		static unsigned int m_nBlockSamples;
		static short* m_pBlockMemory;
#endif

#ifdef USE_OPENAL
		static std::queue<ALuint> m_qAvailableBuffers;
		static ALuint *m_pBuffers;
		static ALuint m_nSource;
		static ALCdevice *m_pDevice;
		static ALCcontext *m_pContext;
		static unsigned int m_nSampleRate;
		static unsigned int m_nChannels;
		static unsigned int m_nBlockCount;
		static unsigned int m_nBlockSamples;
		static short* m_pBlockMemory;
#endif

		static void AudioThread();
		static std::thread m_AudioThread;
		static std::atomic<bool> m_bAudioThreadActive;
		static std::atomic<float> m_fGlobalTime;
		static std::function<float(int, float, float)> funcUserSynth;
		static std::function<float(int, float, float)> funcUserFilter;
	};
}


// Implementation, platform-independent

#ifdef OLC_PGEX_SOUND
#undef OLC_PGEX_SOUND

namespace olc
{
	SOUND::AudioSample::AudioSample()
	{	}

	SOUND::AudioSample::AudioSample(std::string sWavFile, olc::ResourcePack *pack)
	{
		LoadFromFile(sWavFile, pack);
	}

	olc::rcode SOUND::AudioSample::LoadFromFile(std::string sWavFile, olc::ResourcePack *pack)
	{
		auto ReadWave = [&](std::istream &is)
		{
			char dump[4];
			is.read(dump, sizeof(char) * 4); // Read "RIFF"
			if (strncmp(dump, "RIFF", 4) != 0) return olc::FAIL;
			is.read(dump, sizeof(char) * 4); // Not Interested
			is.read(dump, sizeof(char) * 4); // Read "WAVE"
			if (strncmp(dump, "WAVE", 4) != 0) return olc::FAIL;

			// Read Wave description chunk
			is.read(dump, sizeof(char) * 4); // Read "fmt "
			unsigned int nHeaderSize = 0;
			is.read((char*)&nHeaderSize, sizeof(unsigned int)); // Not Interested
			is.read((char*)&wavHeader, nHeaderSize);// sizeof(WAVEFORMATEX)); // Read Wave Format Structure chunk
														// Note the -2, because the structure has 2 bytes to indicate its own size
														// which are not in the wav file

			// Just check if wave format is compatible with olcPGE
			if (wavHeader.wBitsPerSample != 16 || wavHeader.nSamplesPerSec != 44100)
				return olc::FAIL;

			// Search for audio data chunk
			uint32_t nChunksize = 0;
			is.read(dump, sizeof(char) * 4); // Read chunk header
			is.read((char*)&nChunksize, sizeof(uint32_t)); // Read chunk size
			while (strncmp(dump, "data", 4) != 0)
			{
				// Not audio data, so just skip it
				//std::fseek(f, nChunksize, SEEK_CUR);
				is.seekg(nChunksize, std::istream::cur);
				is.read(dump, sizeof(char) * 4);
				is.read((char*)&nChunksize, sizeof(uint32_t));
			}

			// Finally got to data, so read it all in and convert to float samples
			nSamples = nChunksize / (wavHeader.nChannels * (wavHeader.wBitsPerSample >> 3));
			nChannels = wavHeader.nChannels;

			// Create floating point buffer to hold audio sample
			fSample = new float[nSamples * nChannels];
			float *pSample = fSample;

			// Read in audio data and normalise
			for (long i = 0; i < nSamples; i++)
			{
				for (int c = 0; c < nChannels; c++)
				{
					short s = 0;
					if (!is.eof())
					{
						is.read((char*)&s, sizeof(short));

						*pSample = (float)s / (float)(SHRT_MAX);
						pSample++;
					}
				}
			}

			// All done, flag sound as valid
			bSampleValid = true;
			return olc::OK;
		};

		if (pack != nullptr)
		{			
			olc::ResourceBuffer rb = pack->GetFileBuffer(sWavFile);
			std::istream is(&rb);			
			return ReadWave(is);
		}
		else
		{
			// Read from file
			std::ifstream ifs(sWavFile, std::ifstream::binary);
			if (ifs.is_open())
			{
				return ReadWave(ifs);
			}
			else
				return olc::FAIL;
		}
	}

	// This vector holds all loaded sound samples in memory
	std::vector<olc::SOUND::AudioSample> vecAudioSamples;

	// This structure represents a sound that is currently playing. It only
	// holds the sound ID and where this instance of it is up to for its
	// current playback

	void SOUND::SetUserSynthFunction(std::function<float(int, float, float)> func)
	{
		funcUserSynth = func;
	}

	void SOUND::SetUserFilterFunction(std::function<float(int, float, float)> func)
	{
		funcUserFilter = func;
	}

	// Load a 16-bit WAVE file @ 44100Hz ONLY into memory. A sample ID
	// number is returned if successful, otherwise -1
	int SOUND::LoadAudioSample(std::string sWavFile, olc::ResourcePack *pack)
	{

		olc::SOUND::AudioSample a(sWavFile, pack);
		if (a.bSampleValid)
		{
			vecAudioSamples.push_back(a);
			return (unsigned int)vecAudioSamples.size();
		}
		else
			return -1;
	}

	// Add sample 'id' to the mixers sounds to play list
	void SOUND::PlaySample(int id, bool bLoop)
	{
		olc::SOUND::sCurrentlyPlayingSample a;
		a.nAudioSampleID = id;
		a.nSamplePosition = 0;
		a.bFinished = false;
		a.bFlagForStop = false;
		a.bLoop = bLoop;
		SOUND::listActiveSamples.push_back(a);
	}

	void SOUND::StopSample(int id)
	{
		// Find first occurence of sample id
		auto s = std::find_if(listActiveSamples.begin(), listActiveSamples.end(), [&](const olc::SOUND::sCurrentlyPlayingSample &s) { return s.nAudioSampleID == id; });
		if (s != listActiveSamples.end())
			s->bFlagForStop = true;
	}

	void SOUND::StopAll()
	{
		for (auto &s : listActiveSamples)
		{
			s.bFlagForStop = true;
		}
	}

	float SOUND::GetMixerOutput(int nChannel, float fGlobalTime, float fTimeStep)
	{
		// Accumulate sample for this channel
		float fMixerSample = 0.0f;

		for (auto &s : listActiveSamples)
		{
			if (m_bAudioThreadActive)
			{
				if (s.bFlagForStop)
				{
					s.bLoop = false;
					s.bFinished = true;
				}
				else
				{
					// Calculate sample position
					s.nSamplePosition += roundf((float)vecAudioSamples[s.nAudioSampleID - 1].wavHeader.nSamplesPerSec * fTimeStep);

					// If sample position is valid add to the mix
					if (s.nSamplePosition < vecAudioSamples[s.nAudioSampleID - 1].nSamples)
						fMixerSample += vecAudioSamples[s.nAudioSampleID - 1].fSample[(s.nSamplePosition * vecAudioSamples[s.nAudioSampleID - 1].nChannels) + nChannel];
					else
					{
						if (s.bLoop)
						{
							s.nSamplePosition = 0;
						}
						else
							s.bFinished = true; // Else sound has completed
					}
				}
			}
			else
				return 0.0f;
		}

		// If sounds have completed then remove them
		listActiveSamples.remove_if([](const sCurrentlyPlayingSample &s) {return s.bFinished; });

		// The users application might be generating sound, so grab that if it exists
		if (funcUserSynth != nullptr)
			fMixerSample += funcUserSynth(nChannel, fGlobalTime, fTimeStep);

		// Return the sample via an optional user override to filter the sound
		if (funcUserFilter != nullptr)
			return funcUserFilter(nChannel, fGlobalTime, fMixerSample);
		else
			return fMixerSample;
	}

	std::thread SOUND::m_AudioThread;
	std::atomic<bool> SOUND::m_bAudioThreadActive{ false };
	std::atomic<float> SOUND::m_fGlobalTime{ 0.0f };
	std::list<SOUND::sCurrentlyPlayingSample> SOUND::listActiveSamples;
	std::function<float(int, float, float)> SOUND::funcUserSynth = nullptr;
	std::function<float(int, float, float)> SOUND::funcUserFilter = nullptr;
}

// Implementation, Windows-specific
#ifdef USE_WINDOWS
#pragma comment(lib, "winmm.lib")

namespace olc
{
	bool SOUND::InitialiseAudio(unsigned int nSampleRate, unsigned int nChannels, unsigned int nBlocks, unsigned int nBlockSamples)
	{
		// Initialise Sound Engine
		m_bAudioThreadActive = false;
		m_nSampleRate = nSampleRate;
		m_nChannels = nChannels;
		m_nBlockCount = nBlocks;
		m_nBlockSamples = nBlockSamples;
		m_nBlockFree = m_nBlockCount;
		m_nBlockCurrent = 0;
		m_pBlockMemory = nullptr;
		m_pWaveHeaders = nullptr;

		// Device is available
		WAVEFORMATEX waveFormat;
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = m_nSampleRate;
		waveFormat.wBitsPerSample = sizeof(short) * 8;
		waveFormat.nChannels = m_nChannels;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		listActiveSamples.clear();

		// Open Device if valid
		if (waveOutOpen(&m_hwDevice, WAVE_MAPPER, &waveFormat, (DWORD_PTR)SOUND::waveOutProc, (DWORD_PTR)0, CALLBACK_FUNCTION) != S_OK)
			return DestroyAudio();

		// Allocate Wave|Block Memory
		m_pBlockMemory = new short[m_nBlockCount * m_nBlockSamples];
		if (m_pBlockMemory == nullptr)
			return DestroyAudio();
		ZeroMemory(m_pBlockMemory, sizeof(short) * m_nBlockCount * m_nBlockSamples);

		m_pWaveHeaders = new WAVEHDR[m_nBlockCount];
		if (m_pWaveHeaders == nullptr)
			return DestroyAudio();
		ZeroMemory(m_pWaveHeaders, sizeof(WAVEHDR) * m_nBlockCount);

		// Link headers to block memory
		for (unsigned int n = 0; n < m_nBlockCount; n++)
		{
			m_pWaveHeaders[n].dwBufferLength = m_nBlockSamples * sizeof(short);
			m_pWaveHeaders[n].lpData = (LPSTR)(m_pBlockMemory + (n * m_nBlockSamples));
		}

		m_bAudioThreadActive = true;
		m_AudioThread = std::thread(&SOUND::AudioThread);

		// Start the ball rolling with the sound delivery thread
		std::unique_lock<std::mutex> lm(m_muxBlockNotZero);
		m_cvBlockNotZero.notify_one();
		return true;
	}

	// Stop and clean up audio system
	bool SOUND::DestroyAudio()
	{
		m_bAudioThreadActive = false;
		if(m_AudioThread.joinable())
			m_AudioThread.join();
		return false;
	}

	// Handler for soundcard request for more data
	void CALLBACK SOUND::waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2)
	{
		if (uMsg != WOM_DONE) return;
		m_nBlockFree++;
		std::unique_lock<std::mutex> lm(m_muxBlockNotZero);
		m_cvBlockNotZero.notify_one();
	}

	// Audio thread. This loop responds to requests from the soundcard to fill 'blocks'
	// with audio data. If no requests are available it goes dormant until the sound
	// card is ready for more data. The block is fille by the "user" in some manner
	// and then issued to the soundcard.
	void SOUND::AudioThread()
	{
		m_fGlobalTime = 0.0f;
		static float fTimeStep = 1.0f / (float)m_nSampleRate;

		// Goofy hack to get maximum integer for a type at run-time
		short nMaxSample = (short)pow(2, (sizeof(short) * 8) - 1) - 1;
		float fMaxSample = (float)nMaxSample;
		short nPreviousSample = 0;

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();

		while (m_bAudioThreadActive)
		{
			// Wait for block to become available
			if (m_nBlockFree == 0)
			{
				std::unique_lock<std::mutex> lm(m_muxBlockNotZero);
				while (m_nBlockFree == 0) // sometimes, Windows signals incorrectly
					m_cvBlockNotZero.wait(lm);
			}

			// Block is here, so use it
			m_nBlockFree--;

			// Prepare block for processing
			if (m_pWaveHeaders[m_nBlockCurrent].dwFlags & WHDR_PREPARED)
				waveOutUnprepareHeader(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR));

			short nNewSample = 0;
			int nCurrentBlock = m_nBlockCurrent * m_nBlockSamples;

			auto clip = [](float fSample, float fMax)
			{
				if (fSample >= 0.0)
					return fmin(fSample, fMax);
				else
					return fmax(fSample, -fMax);
			};

			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;

			// Our time per frame coefficient
			float fElapsedTime = elapsedTime.count();

			for (unsigned int n = 0; n < m_nBlockSamples; n += m_nChannels)
			{
				// User Process
				for (unsigned int c = 0; c < m_nChannels; c++)
				{
					nNewSample = (short)(clip(GetMixerOutput(c, m_fGlobalTime + fTimeStep * (float)n, fTimeStep), 1.0) * fMaxSample);
					m_pBlockMemory[nCurrentBlock + n + c] = nNewSample;
					nPreviousSample = nNewSample;
				}				
			}

			m_fGlobalTime = m_fGlobalTime + fTimeStep * (float)m_nBlockSamples;

			// Send block to sound device
			waveOutPrepareHeader(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR));
			waveOutWrite(m_hwDevice, &m_pWaveHeaders[m_nBlockCurrent], sizeof(WAVEHDR));
			m_nBlockCurrent++;
			m_nBlockCurrent %= m_nBlockCount;
		}
	}

	unsigned int SOUND::m_nSampleRate = 0;
	unsigned int SOUND::m_nChannels = 0;
	unsigned int SOUND::m_nBlockCount = 0;
	unsigned int SOUND::m_nBlockSamples = 0;
	unsigned int SOUND::m_nBlockCurrent = 0;
	short* SOUND::m_pBlockMemory = nullptr;
	WAVEHDR *SOUND::m_pWaveHeaders = nullptr;
	HWAVEOUT SOUND::m_hwDevice;
	std::atomic<unsigned int> SOUND::m_nBlockFree = 0;
	std::condition_variable SOUND::m_cvBlockNotZero;
	std::mutex SOUND::m_muxBlockNotZero;
}

#elif defined(USE_ALSA)

namespace olc
{
	bool SOUND::InitialiseAudio(unsigned int nSampleRate, unsigned int nChannels, unsigned int nBlocks, unsigned int nBlockSamples)
	{
		// Initialise Sound Engine
		m_bAudioThreadActive = false;
		m_nSampleRate = nSampleRate;
		m_nChannels = nChannels;
		m_nBlockSamples = nBlockSamples;
		m_pBlockMemory = nullptr;

		// Open PCM stream
		int rc = snd_pcm_open(&m_pPCM, "default", SND_PCM_STREAM_PLAYBACK, 0);
		if (rc < 0)
			return DestroyAudio();


		// Prepare the parameter structure and set default parameters
		snd_pcm_hw_params_t *params;
		snd_pcm_hw_params_alloca(&params);
		snd_pcm_hw_params_any(m_pPCM, params);

		// Set other parameters
		snd_pcm_hw_params_set_access(m_pPCM, params, SND_PCM_ACCESS_RW_INTERLEAVED);
		snd_pcm_hw_params_set_format(m_pPCM, params, SND_PCM_FORMAT_S16_LE);
		snd_pcm_hw_params_set_rate(m_pPCM, params, m_nSampleRate, 0);
		snd_pcm_hw_params_set_channels(m_pPCM, params, m_nChannels);
		snd_pcm_hw_params_set_period_size(m_pPCM, params, m_nBlockSamples, 0);
		snd_pcm_hw_params_set_periods(m_pPCM, params, nBlocks, 0);

		// Save these parameters
		rc = snd_pcm_hw_params(m_pPCM, params);
		if (rc < 0)
			return DestroyAudio();

		listActiveSamples.clear();

		// Allocate Wave|Block Memory
		m_pBlockMemory = new short[m_nBlockSamples];
		if (m_pBlockMemory == nullptr)
			return DestroyAudio();
		std::fill(m_pBlockMemory, m_pBlockMemory + m_nBlockSamples, 0);

		// Unsure if really needed, helped prevent underrun on my setup
		snd_pcm_start(m_pPCM);
		for (unsigned int i = 0; i < nBlocks; i++)
			rc = snd_pcm_writei(m_pPCM, m_pBlockMemory, 512);

		snd_pcm_start(m_pPCM);
		m_bAudioThreadActive = true;
		m_AudioThread = std::thread(&SOUND::AudioThread);

		return true;
	}

	// Stop and clean up audio system
	bool SOUND::DestroyAudio()
	{
		m_bAudioThreadActive = false;
		if(m_AudioThread.joinable())
			m_AudioThread.join();
		snd_pcm_drain(m_pPCM);
		snd_pcm_close(m_pPCM);
		return false;
	}


	// Audio thread. This loop responds to requests from the soundcard to fill 'blocks'
	// with audio data. If no requests are available it goes dormant until the sound
	// card is ready for more data. The block is fille by the "user" in some manner
	// and then issued to the soundcard.
	void SOUND::AudioThread()
	{
		m_fGlobalTime = 0.0f;
		static float fTimeStep = 1.0f / (float)m_nSampleRate;

		// Goofy hack to get maximum integer for a type at run-time
		short nMaxSample = (short)pow(2, (sizeof(short) * 8) - 1) - 1;
		float fMaxSample = (float)nMaxSample;
		short nPreviousSample = 0;

		while (m_bAudioThreadActive)
		{
			short nNewSample = 0;

			auto clip = [](float fSample, float fMax)
			{
				if (fSample >= 0.0)
					return fmin(fSample, fMax);
				else
					return fmax(fSample, -fMax);
			};

			for (unsigned int n = 0; n < m_nBlockSamples; n += m_nChannels)
			{
				// User Process
				for (unsigned int c = 0; c < m_nChannels; c++)
				{
					nNewSample = (short)(GetMixerOutput(c, m_fGlobalTime + fTimeStep * (float)n, fTimeStep), 1.0) * fMaxSample;
					m_pBlockMemory[n + c] = nNewSample;
					nPreviousSample = nNewSample;
				}		
			}

			m_fGlobalTime = m_fGlobalTime + fTimeStep * (float)m_nBlockSamples;

			// Send block to sound device
			snd_pcm_uframes_t nLeft = m_nBlockSamples;
			short *pBlockPos = m_pBlockMemory;
			while (nLeft > 0)
			{
				int rc = snd_pcm_writei(m_pPCM, pBlockPos, nLeft);
				if (rc > 0)
				{
					pBlockPos += rc * m_nChannels;
					nLeft -= rc;
				}
				if (rc == -EAGAIN) continue;
				if (rc == -EPIPE) // an underrun occured, prepare the device for more data
					snd_pcm_prepare(m_pPCM);
			}
		}
	}

	snd_pcm_t* SOUND::m_pPCM = nullptr;
	unsigned int SOUND::m_nSampleRate = 0;
	unsigned int SOUND::m_nChannels = 0;
	unsigned int SOUND::m_nBlockSamples = 0;
	short* SOUND::m_pBlockMemory = nullptr;
}

#elif defined(USE_OPENAL)

namespace olc
{
	bool SOUND::InitialiseAudio(unsigned int nSampleRate, unsigned int nChannels, unsigned int nBlocks, unsigned int nBlockSamples)
	{
		// Initialise Sound Engine
		m_bAudioThreadActive = false;
		m_nSampleRate = nSampleRate;
		m_nChannels = nChannels;
		m_nBlockCount = nBlocks;
		m_nBlockSamples = nBlockSamples;
		m_pBlockMemory = nullptr;

		// Open the device and create the context
		m_pDevice = alcOpenDevice(NULL);
		if (m_pDevice)
		{
			m_pContext = alcCreateContext(m_pDevice, NULL);
			alcMakeContextCurrent(m_pContext);
		}
		else
			return DestroyAudio();

		// Allocate memory for sound data
		alGetError();
		m_pBuffers = new ALuint[m_nBlockCount];
		alGenBuffers(m_nBlockCount, m_pBuffers);
		alGenSources(1, &m_nSource);

		for (unsigned int i = 0; i < m_nBlockCount; i++)
			m_qAvailableBuffers.push(m_pBuffers[i]);

		listActiveSamples.clear();

		// Allocate Wave|Block Memory
		m_pBlockMemory = new short[m_nBlockSamples];
		if (m_pBlockMemory == nullptr)
			return DestroyAudio();
		std::fill(m_pBlockMemory, m_pBlockMemory + m_nBlockSamples, 0);

		m_bAudioThreadActive = true;
		m_AudioThread = std::thread(&SOUND::AudioThread);
		return true;
	}

	// Stop and clean up audio system
	bool SOUND::DestroyAudio()
	{
		m_bAudioThreadActive = false;
		if(m_AudioThread.joinable())
			m_AudioThread.join();

		alDeleteBuffers(m_nBlockCount, m_pBuffers);
		delete[] m_pBuffers;
		alDeleteSources(1, &m_nSource);

		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_pContext);
		alcCloseDevice(m_pDevice);
		return false;
	}


	// Audio thread. This loop responds to requests from the soundcard to fill 'blocks'
	// with audio data. If no requests are available it goes dormant until the sound
	// card is ready for more data. The block is fille by the "user" in some manner
	// and then issued to the soundcard.
	void SOUND::AudioThread()
	{
		m_fGlobalTime = 0.0f;
		static float fTimeStep = 1.0f / (float)m_nSampleRate;

		// Goofy hack to get maximum integer for a type at run-time
		short nMaxSample = (short)pow(2, (sizeof(short) * 8) - 1) - 1;
		float fMaxSample = (float)nMaxSample;
		short nPreviousSample = 0;

		std::vector<ALuint> vProcessed;

		while (m_bAudioThreadActive)
		{
			ALint nState, nProcessed;
			alGetSourcei(m_nSource, AL_SOURCE_STATE, &nState);
			alGetSourcei(m_nSource, AL_BUFFERS_PROCESSED, &nProcessed);

			// Add processed buffers to our queue
			vProcessed.resize(nProcessed);
			alSourceUnqueueBuffers(m_nSource, nProcessed, vProcessed.data());
			for (ALint nBuf : vProcessed) m_qAvailableBuffers.push(nBuf);

			// Wait until there is a free buffer (ewww)
			if (m_qAvailableBuffers.empty()) continue;

			short nNewSample = 0;

			auto clip = [](float fSample, float fMax)
			{
				if (fSample >= 0.0)
					return fmin(fSample, fMax);
				else
					return fmax(fSample, -fMax);
			};

			for (unsigned int n = 0; n < m_nBlockSamples; n += m_nChannels)
			{
				// User Process
				for (unsigned int c = 0; c < m_nChannels; c++)
				{
					nNewSample = (short)(clip(GetMixerOutput(c, m_fGlobalTime, fTimeStep), 1.0) * fMaxSample);
					m_pBlockMemory[n + c] = nNewSample;
					nPreviousSample = nNewSample;
				}

				m_fGlobalTime = m_fGlobalTime + fTimeStep;
			}

			// Fill OpenAL data buffer
			alBufferData(
				m_qAvailableBuffers.front(),
				m_nChannels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
				m_pBlockMemory,
				2 * m_nBlockSamples,
				m_nSampleRate
			);
			// Add it to the OpenAL queue
			alSourceQueueBuffers(m_nSource, 1, &m_qAvailableBuffers.front());
			// Remove it from ours
			m_qAvailableBuffers.pop();

			// If it's not playing for some reason, change that
			if (nState != AL_PLAYING)
				alSourcePlay(m_nSource);
		}
	}

	std::queue<ALuint> SOUND::m_qAvailableBuffers;
	ALuint *SOUND::m_pBuffers = nullptr;
	ALuint SOUND::m_nSource = 0;
	ALCdevice *SOUND::m_pDevice = nullptr;
	ALCcontext *SOUND::m_pContext = nullptr;
	unsigned int SOUND::m_nSampleRate = 0;
	unsigned int SOUND::m_nChannels = 0;
	unsigned int SOUND::m_nBlockCount = 0;
	unsigned int SOUND::m_nBlockSamples = 0;
	short* SOUND::m_pBlockMemory = nullptr;
}

#else // Some other platform

namespace olc
{
	bool SOUND::InitialiseAudio(unsigned int nSampleRate, unsigned int nChannels, unsigned int nBlocks, unsigned int nBlockSamples)
	{
		return true;
	}

	// Stop and clean up audio system
	bool SOUND::DestroyAudio()
	{
		return false;
	}


	// Audio thread. This loop responds to requests from the soundcard to fill 'blocks'
	// with audio data. If no requests are available it goes dormant until the sound
	// card is ready for more data. The block is fille by the "user" in some manner
	// and then issued to the soundcard.
	void SOUND::AudioThread()
	{	}
}

#endif
#endif
#endif // OLC_PGEX_SOUND