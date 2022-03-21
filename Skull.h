#pragma once
#include "EnemyObject.h"
#define SKULL_MOVE_SPEED 0.1f;
#define SKULL_GRAVITY 0.002f;

#define SKULL_BBOX_WIDTH 17
#define SKULL_BBOX_HEIGHT 20

#define SKULL_STATE_MOVE_UP 100
#define SKULL_STATE_MOVE_RIGHT 101
#define SKULL_STATE_MOVE_LEFT 102
#define SKULL_STATE_MOVE_RIGHT_ATTACK 103
#define SKULL_STATE_MOVE_LEFT_ATTACK 104


#define SKULL_STATE_DIE 200

#define SKULL_STATE_IDLE 300


#define SKULL_ANI_MOVE_LEFT 0
#define SKULL_ANI_MOVE_RIGHT 1
#define SKULL_ANI_MOVE_LEFT_ATTACK 2
#define SKULL_ANI_MOVE_RIGHT_ATTACK 3

class CSkull :public CEnemyObject
{
	public :
		DWORD time_moving;
	DWORD time_change;
	bool isRepeat;
	vector<CGameObject*> list_weapon;
public:
	CSkull(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void SetIsAttack(bool _flag) { isAttacked = _flag; }
};
#pragma once
