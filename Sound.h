#pragma once
#ifndef __SOUND__
#define __SOUND__
#include "Game.h"
#include "dxaudio.h"
#include <map>
using namespace std;

#define SOUND_ID_BULLET_FIRE	1
#define SOUND_ID_BULLET_EXPLOSION	2
#define SOUND_ID_IS_ATTACKED	3
#define SOUND_ID_PLAYER_JUMPING	4
#define SOUND_ID_EATING_ITEM	5
#define SOUND_ID_PLAYER_EXPLOSION	6
#define SOUND_ID_SKULL_THROW	7
#define SOUND_ID_BOOMB_EXPLOSION	8
#define SOUND_ID_STARTING_BOSS	9
#define SOUND_ID_AREA_2	10
#define SOUND_ID_BOSS	11
#define SOUND_ID_OPENING	12
#define SOUND_ID_BOSS_EXPLOSION	13
#define SOUND_ID_VEHICLE	14
#define SOUND_ID_END	15
class Sound
{
public:
	~Sound();
	static Sound* getInstance();
	void loadSound(HWND hWnd);
	void Play(int soundid);
	void PlayNew(int soundid);					// nếu sound đang phát thì chạy đè lên
	void Stop(int soundid);
	void StopAll();
	void PlayLoop(int soundid);
	bool IsPlaying(int soundid);
	void setVolume(int vol);
	void setMute(bool mute);
	bool getMute();
private:
	Sound();
	static Sound* _instance;
	map<int, CSound*> _listSound;
	float _volume;
	bool _mute;
};

#endif 