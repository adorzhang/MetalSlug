#pragma once
#include "EnemyObject.h"
#define TELEPORTER_MOVE_SPEED 0.04f;
#define TELEPORTER_GRAVITY 0.002f;

#define TELEPORTER_BBOX_WIDTH	24
#define TELEPORTER_BBOX_HEIGHT 32

#define TELEPORTER_STATE_MOVE 100

#define TELEPORTER_STATE_DIE 200

#define TELEPORTER_STATE_IDLE 300
#define TELEPORTER_STATE_UNCLOAK 400
#define TELEPORTER_STATE_CLOAK 600
#define TELEPORTER_STATE_ATTACK 500

#define TELEPORTER_ANI_CLOAK 0
#define TELEPORTER_ANI_MOVE 1
#define TELEPORTER_ANI_START 2

#define TIME_START_MOVING	1000
#define TIME_MOVING			1000
#define TIME_START_CLOAK	2000
#define TIME_CLOAK			1000
class CTeleporter :public CEnemyObject
{
	DWORD timeWaitingToMove, timeWaitingCloak;
	DWORD timeMoving, timeCloak;
	bool isStartMoving, isStartCloak;
	bool isMoving, isCloak;
	bool isAttack;

	vector<CGameObject*> list_weapon;
public:
	CTeleporter(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);



};