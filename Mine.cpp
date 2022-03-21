#include "Mine.h"
#include "Brick.h"
#include "Weapon.h"
CMine::CMine(int _item) :CEnemyObject()
{
	SetState(MINE_STATE_IDLE);
	this->blood = 0;
	item = _item;
	isEnable = true;
	isDisplay = true;
}

void CMine::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - MINE_BBOX_HEIGHT;
			right = x + MINE_BBOX_WIDTH;
		}
		else
		{
			if (item == 1)
			{
				top = y - ITEM_P_BBOX_HEIGHT;

				right = x + ITEM_P_BBOX_WIDTH;
			}

		}
		bottom = y;
	}

}

void CMine::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (isDisplay)
		//vy -= 0.0001f * dt;
		if (this->blood < 0)
		{
			if (item > 0)
				SetState(STATE_ITEM);
			else
				SetState(MINE_STATE_EXPLOSION);
		}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (isDisplay)
		CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Update(dt, coObjects);
	}
}

void CMine::Render()
{
	if (isEnable)
	{
		int ani = -1;
		switch (state)
		{
		case MINE_STATE_IDLE:
			ani = MINE_ANI_IDLE;
			break;
		case STATE_ITEM:
			ani = item;
			animation_item_set->at(ani - 1)->Render(x, y);
			break;
		case MINE_STATE_EXPLOSION:
			ani = MINE_ANI_EXPLOSION;
			break;
		}
		if (isDisplay)
		{
			if (state == MINE_STATE_EXPLOSION) {
				if (animation_set->at(ani)->isFinish) {
					SetState(MINE_STATE_ATTACK);
				}
			}
			animation_set->at(ani)->Render(x, y);
			RenderBoundingBox();
		}

	}
	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Render();
	}
}

void CMine::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MINE_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case MINE_STATE_ATTACK:
	{
		CWeapon* weapon = new CWeapon(WEAPON_TYPE_MINE);// Khởi tạo weapon
		weapon->SetDirection(-1);
		weapon->SetPosition(x, y);
		weapon->SetState(WEAPON_MINE_STATE_UP);
		if (player)
			weapon->SetPlayerObject(player);
		list_weapon.push_back(weapon);

		weapon = new CWeapon(WEAPON_TYPE_MINE);// Khởi tạo weapon
		weapon->SetDirection(-1);
		weapon->SetPosition(x + MINE_BBOX_WIDTH / 2, y);
		weapon->SetState(WEAPON_MINE_STATE_UP);
		if (player)
			weapon->SetPlayerObject(player);
		list_weapon.push_back(weapon);

		weapon = new CWeapon(WEAPON_TYPE_MINE);// Khởi tạo weapon
		weapon->SetDirection(1);
		weapon->SetPosition(x + MINE_BBOX_WIDTH / 2, y);
		weapon->SetState(WEAPON_MINE_STATE_UP);
		if (player)
			weapon->SetPlayerObject(player);
		list_weapon.push_back(weapon);

		weapon = new CWeapon(WEAPON_TYPE_MINE);// Khởi tạo weapon
		weapon->SetDirection(1);
		weapon->SetPosition(x + MINE_BBOX_WIDTH, y);
		weapon->SetState(WEAPON_MINE_STATE_UP);
		if (player)
			weapon->SetPlayerObject(player);
		list_weapon.push_back(weapon);
		SetState(MINE_STATE_DIE);
	}
		
		break;
	case MINE_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	case MINE_STATE_EXPLOSION:
		break;
	case STATE_ITEM:
		vx = 0;
		vy = 0;
		isDisplay = false;
		break;
	default:
		break;
	}
}