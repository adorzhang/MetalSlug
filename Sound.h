#pragma once
#include <dsound.h>
#include <unordered_map>

#include "System.h"
#include "Utils.h"

class CSound : public CSystem
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	std::unordered_map<std::string, IDirectSoundBuffer8*> m_soundBufferList;

	bool InitializeDirectSound(HWND hWnd);
	bool LoadWaveFile(char* filename, std::string soundname);
public:
	CSound(HWND hWnd);
	CSound(const CSound&);
	~CSound();

	bool Initialize(HWND);
	bool PlayWaveFile(std::string soundname);
};

