
#include "Coupling.h"
#include "Weapon.h"

CCoupling::CCoupling(float start_x, float start_y,bool is_left) :CEnemyObject()
{
	SetState(COUPLING_STATE_IDLE);
	this->is_left = is_left;
	this->start_x = start_x;
	this->start_y = start_y;
	isEnable = true;
	isDisplay = true;
	x = this->start_x;
	y = this->start_y;
}

void CCoupling::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - COUPLING_BBOX_HEIGHT;
			right = x + COUPLING_BBOX_WIDTH;
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

void CCoupling::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	time_moving += dt;
	if (is_left)
	{
		switch (current_state)
		{
		case 0:
			SetState(COUPLING_STATE_MOVE_UP);
			if (time_moving >= 2500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 1:
			SetState(COUPLING_STATE_MOVE_LEFT);
			if (time_moving >= 500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 2:
			SetState(COUPLING_STATE_MOVE_DOWN);
			if (time_moving >= 2500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 3:
			SetState(COUPLING_STATE_MOVE_RIGHT);
			if (time_moving >= 500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 4:
			SetState(COUPLING_STATE_MOVE_UP);
			if (time_moving >= 2500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 5:
			SetState(COUPLING_STATE_MOVE_DOWN);
			if (time_moving >= 2500)
			{
				time_moving = 0;
				current_state = 0;
			}
			break;
		}
	}
	else
	{
		switch (current_state)
		{
		case 0:
			SetState(COUPLING_STATE_MOVE_RIGHT);
			if (time_moving >= 500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 1:
			SetState(COUPLING_STATE_MOVE_UP);
			if (time_moving >= 2500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 2:
			SetState(COUPLING_STATE_MOVE_DOWN);
			if (time_moving >= 2500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 3:
			SetState(COUPLING_STATE_MOVE_LEFT);
			if (time_moving >= 500)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 4:
			SetState(COUPLING_STATE_MOVE_UP);
			if (time_moving >= 3000)
			{
				time_moving = 0;
				current_state += 1;
			}
			break;
		case 5:
			SetState(COUPLING_STATE_MOVE_DOWN);
			if (time_moving >= 3000)
			{
				time_moving = 0;
				current_state = 0;
			}
			break;
		}
	}
	//if (x <= 0 || x >= CGame::GetInstance()->GetScreenWidth() - COUPLING_BBOX_WIDTH - 1)
		//SetState(COUPLING_STATE_MOVE_CHANGE_DIRECTION_X);
	CGameObject::Update(dt);
	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
			SetState(COUPLING_STATE_DIE);
	}
	delta_x+= dx;
	delta_y+= dy;
	x += delta_x;
	y += delta_y;

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

void CCoupling::Render()
{
	if (isEnable)
	{

		int ani=-1;
		if(is_left)
			ani= COUPLING_ANI_LEFT;
		else
			ani = COUPLING_ANI_RIGHT;
		if (isDisplay)
		{
			animation_set->at(ani)->Render(x, y);
			//RenderBoundingBox();
		}
	}
}

void CCoupling::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COUPLING_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case COUPLING_STATE_MOVE_LEFT:
		vx = -index * COUPLING_MOVE_SPEED;
		vy = 0;
		break;
	case COUPLING_STATE_MOVE_RIGHT:
		vx = index*COUPLING_MOVE_SPEED;
		vy = 0;
		break;
	case COUPLING_STATE_MOVE_UP:
		vy = index* COUPLING_MOVE_SPEED;
		vx = 0;
		break;
	case COUPLING_STATE_MOVE_DOWN:
		vy = -index* COUPLING_MOVE_SPEED;
		vx = 0;
		break;
	case COUPLING_STATE_DIE:
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