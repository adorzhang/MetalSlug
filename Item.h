#pragma once
#include "GameObject.h"

#define ITEM_ANI_H_ROCKET	1
#define ITEM_ANI_H_GREEN	2
#define ITEM_ANI_FLASH	3
#define ITEM_ANI_P_GREEN	4
#define ITEM_ANI_KEY 5

#define	ITEM_BBOX_WIDTH	16
#define	ITEM_BBOX_HEIGHT	16

#define ITEM_STATE_IDLE 300
#define ITEM_STATE_DIE 200
class CItem : public CGameObject
{
	int width;
	int height;
	bool isDisplay;
	bool isEnable;
	int type;
public:
	CItem(int type);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	int GetType() { return this->type; }
};