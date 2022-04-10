#include "Sound.h"

bool CSound::InitializeDirectSound(HWND hWnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool CSound::LoadWaveFile(char* filename, std::string soundname)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	// Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// Check that the wave file was recorded in stereo format.
	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer8* secondaryBuffer = 0;
	IDirectSoundBuffer8** pSecondaryBuffer = &secondaryBuffer;

	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*pSecondaryBuffer);
	if (FAILED(result))
	{
		DebugOut(L"[ERROR] Can not create secondary buffer\n");
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*pSecondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*pSecondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	// Set volume of the buffer to 100%.
	(*pSecondaryBuffer)->SetVolume(DSBVOLUME_MAX);

	m_soundBufferList[soundname] = secondaryBuffer;

	DebugOut(L"[INFO] Loading sound file: %s has been loaded successfully\n", ToWSTR(filename).c_str());

	return true;
}

bool CSound::PlayWaveFile(std::string soundname)
{
	HRESULT result;

	std::unordered_map< std::string, IDirectSoundBuffer8*> ::iterator sound;
	sound = m_soundBufferList.find(soundname);

	sound->second->Stop();
	sound->second->SetCurrentPosition(0);
	sound->second->Play(0, 0, 0);

	return true;
}

CSound::CSound(HWND hWnd)
{
	m_DirectSound = 0;
	m_primaryBuffer = 0;

	bool result = InitializeDirectSound(hWnd);
	if (!result)
	{
		DebugOut(L"[ERROR] Can not initialize Direct Sound\n");
	}
}

CSound::CSound(const CSound&)
{
}

CSound::~CSound()
{
	for (auto sound : m_soundBufferList)
	{
		if (sound.second)
		{
			sound.second->Release();
			sound.second = nullptr;
		}
	}

	if (m_primaryBuffer)
		m_primaryBuffer->Release();

	if (m_DirectSound)
		m_DirectSound->Release();

	m_primaryBuffer = nullptr;
	m_DirectSound = nullptr;
}

bool CSound::Initialize(HWND hWnd)
{
	bool result;

	// Load a wave audio file onto a secondary buffer.
	result = LoadWaveFile((char*)"sounds/BossDie.wav", "BossDie");
	result = LoadWaveFile((char*)"sounds/EnemyDie.wav", "EnemyDie");
	result = LoadWaveFile((char*)"sounds/EnemyOnDamaged.wav", "EnemyOnDamaged");
	result = LoadWaveFile((char*)"sounds/EnterBossRoom.wav", "EnterBossRoom");
	result = LoadWaveFile((char*)"sounds/JasonBullet.wav", "JasonBullet");
	result = LoadWaveFile((char*)"sounds/JasonDie.wav", "JasonDie");
	result = LoadWaveFile((char*)"sounds/JasonJump.wav", "JasonJump");
	result = LoadWaveFile((char*)"sounds/JasonOnDamaged.wav", "JasonOnDamaged");
	result = LoadWaveFile((char*)"sounds/PickItemUp.wav", "PickItemUp");
	result = LoadWaveFile((char*)"sounds/ScrollingMapJump.wav", "ScrollingMapJump");
	result = LoadWaveFile((char*)"sounds/SophiaBullet.wav", "SophiaBullet");
	result = LoadWaveFile((char*)"sounds/SophiaDie.wav", "SophiaDie");
	result = LoadWaveFile((char*)"sounds/BulletExplosion.wav", "BulletExplosion");
	result = LoadWaveFile((char*)"sounds/SwitchCharacter.wav", "SwitchCharacter");

	if (!result)
	{
		return false;
	}

	return true;
}
