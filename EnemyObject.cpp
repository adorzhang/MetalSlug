#include "EnemyObject.h"

CEnemyObject::CEnemyObject():CGameObject()
{

}

void CEnemyObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}