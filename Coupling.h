#pragma once
#include "EnemyObject.h"
#include "Game.h"
#define COUPLING_MOVE_SPEED 0.015f;
#define COUPLING_GRAVITY 0.002f;

#define COUPLING_BBOX_WIDTH 16
#define COUPLING_BBOX_HEIGHT 17

#define COUPLING_STATE_MOVE_LEFT 100
#define COUPLING_STATE_MOVE_RIGHT 101
#define COUPLING_STATE_MOVE_UP 102
#define COUPLING_STATE_MOVE_DOWN 103

#define COUPLING_STATE_DIE 200

#define COUPLING_STATE_IDLE 300


#define COUPLING_ANI_RIGHT 2
#define COUPLING_ANI_LEFT 4

#define TIME_CHANGE_DIRECTION 1000
class CCoupling :public CEnemyObject
{
	DWORD time_moving;
	bool isBeingDown;
	bool isBeingUp;
	bool isRepeat;
	float start_x;
	float start_y;
	bool is_left;
	int current_state = 0;
	float delta_x;
	float delta_y;
	int index;
public:
	CCoupling(float start_x,float start_y,bool is_left);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void GetDelta(float& _delta_x, float& _delta_y) { _delta_x = delta_x; _delta_y = delta_y; }
	void SetIndex(int _index) { this->index = _index; }
};
