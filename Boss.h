#pragma once
#include "EnemyObject.h"
#include "Game.h"
#include "Pincer.h"
#include "Coupling.h"
#define BOSS_MOVE_SPEED 0.04f;
#define BOSS_GRAVITY 0.002f;

#define BOSS_BBOX_WIDTH 60
#define BOSS_BBOX_HEIGHT 64

#define BOSS_STATE_MOVE 100
#define BOSS_STATE_MOVE_CHANGE_DIRECTION_X 101
#define BOSS_STATE_MOVE_CHANGE_DIRECTION_Y 102
#define BOSS_STATE_ATTACK 103
#define BOSS_STATE_EXPLOSION 104

#define BOSS_STATE_DIE 200

#define BOSS_STATE_IDLE 300


#define BOSS_ANI_MOVE_LEFT 0
#define BOSS_ANI_START 1
#define BOSS_ANI_EXPLOSION 6

#define TIME_CHANGE_DIRECTION 1000
class CBoss :public CEnemyObject
{
	DWORD time_moving;
	DWORD time_moving_y;
	DWORD time_moving_coupling;
	DWORD time_moving_coupling_left;
	bool isBeingDown;
	bool isBeingUp;
	bool isRepeat;
	bool isStartAllCouplingRight;
	bool isStartAllCouplingLeft;
	vector<CGameObject*> list_weapon;
	vector<CGameObject*> left_coupling_elements;
	vector<CGameObject*> right_coupling_elements;
	CGameObject* left_pincer;
	CGameObject* right_pincer;
	float start_x;
	float start_y;
public:
	CBoss(float x,float y,int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void CreateCouplingElements();
};
