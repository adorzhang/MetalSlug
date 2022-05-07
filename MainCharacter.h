#pragma once
#include "GameObject.h"
#include "BarrelObject.h"
#include "Legs.h"

#include "Weapon.h"

#include "Barrel.h"

#define MAIN_CHARACTER_RUN_SPEED							0.15f 
//0.1f
#define MAIN_CHARACTER_JUMP_SPEED_Y							0.6f
#define HUMAN_SMALL_JUMP_SPEED_Y							0.6f
#define MAIN_CHARACTER_JUMP_DEFLECT_SPEED					0.2f
#define MAIN_CHARACTER_GRAVITY								0.002f
#define MAIN_CHARACTER_DIE_DEFLECT_SPEED					0.5f

#define MAIN_CHARACTER_STATE_IDLE							0
#define MAIN_CHARACTER_STATE_RUN_RIGHT						100


#define MAIN_CHARACTER_STATE_RUN_LEFT						200
#define MAIN_CHARACTER_STATE_JUMP							300
#define MAIN_CHARACTER_STATE_DIE							401
#define MAIN_CHARACTER_STATE_EXPLOSION						400
#define MAIN_CHARACTER_STATE_UP_BARREL						500
#define MAIN_CHARACTER_STATE_STRAIGHT_BARREL				501
#define MAIN_CHARACTER_STATE_DOWN_BARREL					502
#define MAIN_CHARACTER_STATE_NONE_COLLISION					600
#define MAIN_CHARACTER_STATE_HUMAN							800
#define MAIN_CHARACTER_STATE_SWIM							801
#define MAIN_CHARACTER_STATE_JUMP_STRAIGHT					802

#define MAIN_CHARACTER_STATE_BARREL_FIRE					700
#define MAIN_CHARACTER_STATE_BARREL_FIRE_UP					701
#define MAIN_CHARACTER_STATE_FIRE_ROCKET					702

#define MAIN_CHARACTER_ANI_IDLE_RIGHT						0
#define MAIN_CHARACTER_ANI_IDLE_LEFT						0
#define MAIN_CHARACTER_ANI_EXPLOSION						0
#define MAIN_CHARACTER_ANI_JUMP								0
#define MAIN_CHARACTER_ANI_LIE_RIGHT						1
#define MAIN_CHARACTER_ANI_LIE_LEFT							1
#define MAIN_CHARACTER_ANI_SWIM_RIGHT						2
#define MAIN_CHARACTER_ANI_SWIM_LEFT						2
#define MAIN_CHARACTER_ANI_STARE_RIGHT						3
#define MAIN_CHARACTER_ANI_STARE_LEFT						3
#define MAIN_CHARACTER_ANI_SHOOT_UP_RIGHT					4
#define MAIN_CHARACTER_ANI_SHOOT_UP_LEFT					4
#define MAIN_CHARACTER_ANI_SHOOT_DOWN_RIGHT					5
#define MAIN_CHARACTER_ANI_SHOOT_DOWN_LEFT					5
#define MAIN_CHARACTER_ANI_KNIFE_RIGHT						6
#define MAIN_CHARACTER_ANI_KNIFE_LEFT						6

#define MAIN_CHARACTER_ANI_RUN_RIGHT						0
#define MAIN_CHARACTER_ANI_RUN_LEFT							0


#define MAIN_CHARACTER_ANI_DIE								8


#define	MAIN_CHARACTER_LEVEL_SMALL							1
#define	MAIN_CHARACTER_LEVEL_BIG							2


#define MAIN_CHARACTER_BBOX_WIDTH							26
#define MAIN_CHARACTER_BBOX_HEIGHT							16
#define MAIN_CHARACTER_STATE_BARREL_UP_BBOX_WIDTH			26
#define MAIN_CHARACTER_STATE_BARREL_UP_BBOX_HEIGHT			34
#define MAIN_CHARACTER_STATE_EXPLOSION_BBOX_WIDTH			48
#define MAIN_CHARACTER_STATE_EXPLOSION_BBOX_HEIGHT			40
#define MAIN_CHARACTER_CHANGE_BBOX_HEIGHT					18
#define MAIN_CHARACTER_UNTOUCHABLE_TIME						500

class CMainCharacter : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of MAIN_CHARACTER at scene
	float start_y;

	//Bo sung them object
	vector<LPGAMEOBJECT> componentObjects;
	vector<CWeapon*> list_weapon;

	int blood;
	int alive;
	int heart;
	int score;
	int power;
	
	bool isStateStraightBarrel;
	bool doIncreaseYValue;
	bool isStartFire;
	bool isBeingUpBarrel;
	bool isCabinOpened;

	bool isEnable;
	bool isDisplay;
public:

	//Bo sung property
	bool isOnGround;
	bool isHuman;
	//bool CanChangeState;
	bool IsStartingBossScence;
	CMainCharacter(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void AddComponentObject(CGameObject* object);
	vector<LPGAMEOBJECT> GetComponentObjects();
	void ClearObjects() { componentObjects.clear(); list_weapon.clear(); }

	float GetStartX() { return start_x; }
	float GetStartY() { return start_y; }

	void CollisionItem(int item);
	int GetPower() { return power; }
	void SetPower(int _power) { power = _power; }
	int GetAlive() { return alive; }
	void SetAlive(int _alive) { alive = _alive; }
};