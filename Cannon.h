#pragma once

#include "EnemyObject.h"


using namespace std;
#define CANNON_MOVE_SPEED 0.03f;
#define CANNON_GRAVITY 0.002f;

#define CANNON_BBOX_WIDTH 22
#define CANNON_BBOX_HEIGHT 26

#define CANNON_STATE_FIRE_VERTICAL 100
#define CANNON_STATE_FIRE_HORIZONTAL 101

#define CANNON_STATE_DIE 200

#define CANNON_STATE_IDLE 300

#define CANNON_ANI_SETS_ID 10
#define CANNON_ANI_FIRE_VERTICAL 0
#define CANNON_ANI_FIRE_HORIZONTAL 1


#define WEAPON_TYPE_ENEMY_CANNONS			2
class CCannon :public CEnemyObject
{
	int isFireVertical;
	vector<CGameObject*> list_weapon;
public:
	CCannon(int _item);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void LostBlood(int _blood) { this->blood -= _blood; }
};
