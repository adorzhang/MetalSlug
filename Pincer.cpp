
#include "Pincer.h"
#include "Weapon.h"

CPincer::CPincer(float start_x, float start_y,bool is_left) :CEnemyObject()
{
	SetState(PINCER_STATE_IDLE);
	this->is_left = is_left;
	this->start_x = start_x;
	this->start_y = start_y;
	isEnable = true;
	isDisplay = true;
	x = this->start_x;
	y = this->start_y;
}

void CPincer::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - PINCER_BBOX_HEIGHT;
			right = x + PINCER_BBOX_WIDTH;
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

void CPincer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	time_moving += dt;
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(PINCER_STATE_DIE);
	}
	if (player)
	{
		float player_l, player_t, player_r, player_b;
		player->GetBoundingBox(player_l, player_t, player_r, player_b);
		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		if (CGame::GetInstance()->CheckCollision(player_l, player_t, player_r, player_b, l, t, r, b))
		{
			player->SetIsAttacked(true);
			player->SetPosition(player_l - 1, player_t - 1);
		}
	}
}

void CPincer::Render()
{
	if (isEnable)
	{
		int ani = -1;
		if (is_left)
			ani = PINCER_ANI_LEFT;
		else
			ani = PINCER_ANI_RIGHT;
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			//RenderBoundingBox();
		}
	}
}

void CPincer::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PINCER_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case PINCER_STATE_MOVE_LEFT:
		vx = -PINCER_MOVE_SPEED;
		vy = 0;
		break;
	case PINCER_STATE_MOVE_RIGHT:
		vx = PINCER_MOVE_SPEED;
		vy = 0;
		break;
	case PINCER_STATE_MOVE_UP:
		vy = PINCER_MOVE_SPEED;
		vx = 0;
		break;
	case PINCER_STATE_MOVE_DOWN:
		vy = -PINCER_MOVE_SPEED;
		vx = 0;
		break;
	case PINCER_STATE_DIE:
		isDisplay = false;
		isEnable = false;
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