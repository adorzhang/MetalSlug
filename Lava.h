#pragma once
#include "GameObject.h"

#define LAVA_BBOX_WIDTH  1280
#define LAVA_BBOX_HEIGHT 16

#define LAVA_STATE_NORMAL	100
#define LAVA_STATE_DESTROYED	101
#define LAVA_STATE_NONE	102

#define LAVA_ANI_NORMAL	0

#define LAVA_ANI_DESTROYED	1
#define LAVA_TYPE_NORMAL	1
#define LAVA_TYPE_DESTRUCTIBLE_1	2
#define LAVA_TYPE_DESTRUCTIBLE_2	3
class CLava : public CGameObject
{
	int width;
	int height;
	bool isDisplay;
	bool isEnable;
	bool isCollision;
	LPGAMEOBJECT player;
public:
	CLava(float l, float t, float r, float b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void SetState(int state);
	void SetPlayerObject(LPGAMEOBJECT _player) { player = _player; }
};