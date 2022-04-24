

#include "Sound.h"

Sound* Sound::_instance;

Sound* Sound::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Sound();
	}
	return _instance;
}

void Sound::loadSound(HWND hWnd)
{
	// Khởi tạo CSoundManager.
	DirectSound_Init(hWnd);

	CSound* sound = nullptr;

	// DYING
	sound = LoadSound("Sound//bullet_fire.wav");
	_listSound[SOUND_ID_BULLET_FIRE] = sound;

	sound = LoadSound("Sound//bullet_explosion.wav");
	_listSound[SOUND_ID_BULLET_EXPLOSION] = sound;

	sound = LoadSound("Sound//is_attacked.wav");
	_listSound[SOUND_ID_IS_ATTACKED] = sound;

	sound = LoadSound("Sound//jump.wav");
	_listSound[SOUND_ID_PLAYER_JUMPING] = sound;

	sound = LoadSound("Sound//eating_item.wav");
	_listSound[SOUND_ID_EATING_ITEM] = sound;

	sound = LoadSound("Sound//player_explosion.wav");
	_listSound[SOUND_ID_PLAYER_EXPLOSION] = sound;

	sound = LoadSound("Sound//skull_throw.wav");
	_listSound[SOUND_ID_SKULL_THROW] = sound;

	sound = LoadSound("Sound//boomb_explosion.wav");
	_listSound[SOUND_ID_BOOMB_EXPLOSION] = sound;

	sound = LoadSound("Sound//starting_boss.wav");
	_listSound[SOUND_ID_STARTING_BOSS] = sound;

	sound = LoadSound("Sound//area2.wav");
	_listSound[SOUND_ID_AREA_2] = sound;

	sound = LoadSound("Sound//boss.wav");
	_listSound[SOUND_ID_BOSS] = sound;

	sound = LoadSound("Sound//opening.wav");
	_listSound[SOUND_ID_OPENING] = sound;

	sound = LoadSound("Sound//boss_explosion.wav");
	_listSound[SOUND_ID_BOSS_EXPLOSION] = sound;

	sound = LoadSound("Sound//vehicle.wav");
	_listSound[SOUND_ID_VEHICLE] = sound;

	sound = LoadSound("Sound//end_scence.wav");
	_listSound[SOUND_ID_END] = sound;
}

bool Sound::IsPlaying(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr)
	{
		return sound->IsSoundPlaying();
	}
	return false;
}

void Sound::setVolume(int vol)
{
	_volume = vol;
}

void Sound::setMute(bool mute)
{
	_mute = mute;
}

bool Sound::getMute()
{
	return _mute;
}

void Sound::Play(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr && _mute == false)
	{
		PlaySound(sound, _volume);
	}
}
void Sound::PlayNew(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr && _mute == false)
	{
		auto newSound = new CSound(*sound);
		PlaySound(newSound);
	}
}
void Sound::PlayLoop(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr)
	{
		sound->Play(0, DSBPLAY_LOOPING);
	}
}
void Sound::Stop(int soundid)
{
	auto sound = _listSound[soundid];
	if (sound != nullptr && _mute == false)
	{
		StopSound(sound);
	}
}

void Sound::StopAll()
{
	for (auto sound : _listSound)
	{
		if (sound.second->IsSoundPlaying())
		{
			this->Stop(sound.first);
		}
	}
}

Sound::Sound()
{
	_mute = false;
	_volume = 3;
}

Sound::~Sound()
{
}