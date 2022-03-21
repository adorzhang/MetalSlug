#pragma once
#include "./GameObject.h"
#include "./MainCharacter.h"

#define HUMAN_ANI_IDLE		0
#define HUMAN_ANI_UP			2
#define HUMAN_ANI_DOWN			1

#define	HUMAN_LEVEL_SMALL	1
#define	HUMAN_LEVEL_BIG		2

#define HUMAN_BIG_BBOX_WIDTH  24
#define HUMAN_BIG_BBOX_HEIGHT 32

#define HUMAN_SMALL_BBOX_WIDTH  8
#define HUMAN_SMALL_BBOX_HEIGHT 16

#define HUMAN_SMALL_CRAWL_BBOX_WIDTH  16
#define HUMAN_SMALL_CRAWL_BBOX_HEIGHT 10

#define HUMAN_WALKING_SPEED		0.1f 

#define HUMAN_JUMP_SPEED_Y		0.5f
#define HUMAN_JUMP_DEFLECT_SPEED 0.2f
#define HUMAN_GRAVITY			0.002f


#define HUMAN_STATE_IDLE			0
#define HUMAN_STATE_WALKING_RIGHT	100
#define HUMAN_STATE_WALKING_LEFT	200
#define HUMAN_STATE_JUMP			300
#define HUMAN_STATE_DIE				400

#define HUMAN_STATE_CLIMB				600

#define HUMAN_ANI_BIG_IDLE		0
#define HUMAN_ANI_SMALL_IDLE		0

#define HUMAN_ANI_BIG_WALKING			3
#define HUMAN_ANI_BIG_WALKING_UP			4
#define HUMAN_ANI_BIG_WALKING_DOWN			5

#define HUMAN_ANI_SMALL_WALKING		1
#define HUMAN_ANI_SMALL_CRAWLING		2
#define HUMAN_ANI_SMALL_CLIMBING		6
#define HUMAN_ANI_SMALL_DYING		7

#define HUMAN_ANI_DIE				8

class CHuman : public CGameObject
{
	int level;

	float start_x;			// initial position of Human at scene
	float start_y;
	bool is_on_ground;
	bool is_set_level;

	bool isGoingUp;
	bool isGoingDown;
	DWORD untouchable_start;
	bool isBeingHuman;
	bool isStateCrawl;
	bool isStateClimb;
	bool isStateExplosion;
	bool isFininshAnimationDying;
	CGameObject* player;
	bool isEnable;
	bool canChangeStateClimb;
	bool isStartBeingHuman;
	vector<CWeapon*> list_weapon;
public:
	bool CanChangeBeingPLayer;
	bool IsStartingBossScence;
	CHuman(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; is_set_level = true; }
	int GetLevel() { return level; }
	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void SetGoingUp(bool flag) { isGoingUp = flag; }
	void SetGoingDown(bool flag) { isGoingDown = flag; }
	bool GetGoingUp() { return isGoingUp; }
	bool GetGoingDown() { return isGoingDown; }
	void SetIsBeingHuman(bool _flag) { isBeingHuman = _flag; }
	void SetPlayerObject(CGameObject* _player) { player = _player; }
	int GetIsStateCrawl() { return isStateCrawl; }
	int GetIsStateExplosion() { return isStateExplosion; }
	int GetIsFinishAnimationDying() { return isFininshAnimationDying; }
	void SetIsStartBeingHuman(bool _flag) { isStartBeingHuman = _flag; }
};
