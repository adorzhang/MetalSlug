
#include "Floater.h"
#include "Weapon.h"

CFloater::CFloater(int _item) :CEnemyObject()
{
	SetState(FLOATER_STATE_MOVE);
	this->blood = 1;
	item = _item;
	time_moving = 0;
	isEnable = true;
	isDisplay = true;
	
}

void CFloater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - FLOATER_BBOX_HEIGHT;
			right = x + FLOATER_BBOX_WIDTH;
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

void CFloater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	

	time_moving += dt;
	if (isRepeat)
	{
		if (time_moving >= TIME_CHANGE_DIRECTION)
		{
			SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y);
			isBeingDown = false;
			time_moving = 0;
			isRepeat = false;
		}
	}
	else
	{
		if (!isBeingDown)
		{
			if (time_moving >= TIME_CHANGE_DIRECTION)
			{
				SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_X);
				isBeingDown = true;
				time_moving = 0;
			}
		}
		else
		{
			if (isBeingUp)
			{
				if (time_moving >= TIME_CHANGE_DIRECTION)
				{
					SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_X);
					isBeingUp = false;
					time_moving = 0;
					isRepeat = true;
				}
			}
			else
			{
				if (time_moving >= TIME_CHANGE_DIRECTION)
				{
					SetState(FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y);
					isBeingUp = true;
					time_moving = 0;
					SetState(FLOATER_STATE_ATTACK);
				}
			}

		}
	}
	
	// Calculate dx, dy 
	CGameObject::Update(dt);
	
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(FLOATER_STATE_DIE);
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

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = -vx;
		if (ny != 0) vy = -vy;

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Update(dt, coObjects);
	}
	
}

void CFloater::Render()
{
	if (isEnable)
	{

		int ani = -1;
		switch (state)
		{
		case FLOATER_STATE_ATTACK:
		case FLOATER_STATE_MOVE_CHANGE_DIRECTION_X:
		case FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y:
		case FLOATER_STATE_IDLE:
		case FLOATER_STATE_MOVE:
			if (nx > 0)
				ani = FLOATER_ANI_MOVE_RIGHT;
			else
				ani = FLOATER_ANI_MOVE_LEFT;
			break;
		case STATE_ITEM:
			ani = item;
			animation_item_set->at(ani - 1)->Render(x, y);
			break;
		}
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			RenderBoundingBox();
		}
		
	}
	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Render();
	}

}

void CFloater::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOATER_STATE_IDLE:
		vx = 0;
		break;
	case FLOATER_STATE_MOVE:
		if (nx > 0)
		{
			vx = FLOATER_MOVE_SPEED;
		}
		else
		{
			vx = -FLOATER_MOVE_SPEED;
		}
		vy = -FLOATER_MOVE_SPEED;
		break;
	case FLOATER_STATE_MOVE_CHANGE_DIRECTION_X:
		vx = -vx;
		break;
	case FLOATER_STATE_MOVE_CHANGE_DIRECTION_Y:
		vy = -vy;
		break;
	case FLOATER_STATE_ATTACK:
		{
			if (isDisplay)
			{
				CWeapon* weapon = new CWeapon(WEAPON_TYPE_ENEMY_FLOATER);// Khởi tạo weapon
				weapon->SetDirection(nx);
				weapon->SetPosition(x+FLOATER_BBOX_WIDTH/2, y);
				weapon->SetState(WEAPON_FLOATER_STATE_FLY);
				if(player)
					weapon->SetPlayerObject(player);
				list_weapon.push_back(weapon);
			}
			
		}
		break;
	case FLOATER_STATE_DIE:
		isDisplay = false;
		isEnable = false;
		break;
	case STATE_ITEM:
		vx=0;
		vy=0;
		isDisplay = false;
		break;
	default:
		break;
	}
}