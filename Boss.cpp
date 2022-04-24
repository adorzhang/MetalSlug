
#include "Boss.h"
#include "Weapon.h"
#include "Sound.h"

CBoss::CBoss(float x,float y,int _item) :CEnemyObject()
{
	SetState(BOSS_STATE_IDLE);
	this->blood = 40;
	item = _item;
	time_moving = 0;
	time_moving_coupling = 0;
	isEnable = true;
	isDisplay = true;
	start_x = x;
	start_y = y;
	CreateCouplingElements();
}

void CBoss::CreateCouplingElements()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set;
	for (int i = 0; i < 4; i++)
	{
		CCoupling* object_right = new CCoupling(start_x+BOSS_BBOX_WIDTH,start_y-BOSS_BBOX_HEIGHT/2-i*COUPLING_BBOX_HEIGHT,false);
		ani_set = animation_sets->Get(3);
		object_right->SetAnimationSet(ani_set);
		object_right->SetIndex(i);
		object_right->SetPlayerObject(player);
		right_coupling_elements.push_back(object_right);
	}
	for (int i = 0; i < 4; i++)
	{
		CCoupling* object_left = new CCoupling(start_x, start_y - BOSS_BBOX_HEIGHT / 2 - i * COUPLING_BBOX_HEIGHT,true);
		ani_set = animation_sets->Get(3);
		object_left->SetAnimationSet(ani_set);
		object_left->SetIndex(i);
		object_left->SetPlayerObject(player);
		left_coupling_elements.push_back(object_left);
	}
	CPincer* object_right = new CPincer(start_x+BOSS_BBOX_WIDTH, start_y - BOSS_BBOX_HEIGHT / 2 - 4 * COUPLING_BBOX_HEIGHT,false);
	ani_set = animation_sets->Get(3);
	object_right->SetAnimationSet(ani_set);
	object_right->SetPlayerObject(player);
	right_pincer = object_right;

	CPincer* object_left = new CPincer(start_x, start_y - BOSS_BBOX_HEIGHT / 2 - 4 * COUPLING_BBOX_HEIGHT,true);
	ani_set = animation_sets->Get(3);
	object_left->SetAnimationSet(ani_set);
	object_left->SetPlayerObject(player);
	left_pincer = object_left;
}
void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isEnable)
	{
		left = x;
		if (state != STATE_ITEM)
		{
			top = y - BOSS_BBOX_HEIGHT;
			right = x + BOSS_BBOX_WIDTH;
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

void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable) {
		return;
	}
	if (state != BOSS_STATE_IDLE)
	{
		time_moving += dt;
		time_moving_y += dt;
		if (time_moving >= 1000)
		{
			time_moving = 0;
			SetState(BOSS_STATE_ATTACK);
		}
		if (time_moving_y >= 1000)
		{
			time_moving_y = 0;
			SetState(BOSS_STATE_MOVE_CHANGE_DIRECTION_Y);
		}
		if (x <= 0 || x >= CGame::GetInstance()->GetScreenWidth() - BOSS_BBOX_WIDTH - 1)
		{
			SetState(BOSS_STATE_MOVE_CHANGE_DIRECTION_X);
		}
		else if (y <= 0 || y >= CGame::GetInstance()->GetScreenHeight() - BOSS_BBOX_HEIGHT - 1)
		{
			//SetState(BOSS_STATE_MOVE_CHANGE_DIRECTION_Y);
		}
	}
	// Calculate dx, dy 
	CGameObject::Update(dt);

	if (this->blood < 0)
	{
		if (item > 0)
			SetState(STATE_ITEM);
		else
		{
			SetState(BOSS_STATE_EXPLOSION);
			return;
		}
	}
	
	x += dx;
	y += dy;
	for (int i = 0; i < list_weapon.size(); i++)
	{
		list_weapon[i]->Update(dt, coObjects);
	}
	if (state != BOSS_STATE_IDLE)
	{
		time_moving_coupling += dt;
		time_moving_coupling_left += dt;
		for (int i = 0; i < right_coupling_elements.size(); i++)
		{
			right_coupling_elements[i]->SetPosition(x + BOSS_BBOX_WIDTH - 10, y - BOSS_BBOX_HEIGHT / 2 - i * COUPLING_BBOX_HEIGHT);
			right_coupling_elements[i]->Update(dt, coObjects);
		}
		for (int i = 0; i < left_coupling_elements.size(); i++)
		{
			left_coupling_elements[i]->SetPosition(x, y - BOSS_BBOX_HEIGHT / 2 -i * COUPLING_BBOX_HEIGHT);
			left_coupling_elements[i]->Update(dt, coObjects);
		}
	}
	//Xử lý Pincer
	float _x, _y;
	float _delta_x, _delta_y;
	right_coupling_elements[3]->GetPosition(_x, _y);
	dynamic_cast<CPincer*>(right_pincer)->SetPlayerObject(player);
	right_pincer->SetPosition(_x, _y - COUPLING_BBOX_HEIGHT);
	right_pincer->Update(dt, coObjects);

	left_coupling_elements[3]->GetPosition(_x, _y);
	dynamic_cast<CPincer*>(left_pincer)->SetPlayerObject(player);
	left_pincer->SetPosition(_x, _y - COUPLING_BBOX_HEIGHT);
	left_pincer->Update(dt, coObjects);
}

void CBoss::Render()
{
	if (isEnable)
	{

		int ani = -1;
		switch (state)
		{
		case BOSS_STATE_ATTACK:
		case BOSS_STATE_MOVE_CHANGE_DIRECTION_X:
		case BOSS_STATE_MOVE_CHANGE_DIRECTION_Y:
		case BOSS_STATE_MOVE:
			ani = BOSS_ANI_MOVE_LEFT;
			break;
		case BOSS_STATE_IDLE:
			ani = BOSS_ANI_START;
			break;
		case BOSS_STATE_EXPLOSION:
			ani= BOSS_ANI_EXPLOSION;
			break;
		case STATE_ITEM:
			ani = item;
			animation_item_set->at(ani - 1)->Render(x, y);
			break;
		}
		if (isDisplay)
		{
			if(state==BOSS_STATE_IDLE)
			{
				animation_set->at(ani)->Render(x, y);
				//RenderBoundingBox();
				if(animation_set->at(ani)->isFinish)
				{
					SetState(BOSS_STATE_MOVE);
				}
				Sound::getInstance()->Stop(SOUND_ID_AREA_2);
				Sound::getInstance()->Play(SOUND_ID_STARTING_BOSS);
			}
			else if (state == BOSS_STATE_EXPLOSION)
			{
				Sound::getInstance()->Stop(SOUND_ID_BOSS);
				Sound::getInstance()->Play(SOUND_ID_BOSS_EXPLOSION);
				animation_set->at(ani)->Render(x+BOSS_BBOX_WIDTH/2, y-BOSS_BBOX_HEIGHT/2);
				if (animation_set->at(ani)->isFinish)
				{
					SetState(BOSS_STATE_DIE);
					Sound::getInstance()->Stop(SOUND_ID_BOSS_EXPLOSION);
				}
			}
			else
			{
				Sound::getInstance()->Stop(SOUND_ID_STARTING_BOSS);
				Sound::getInstance()->Play(SOUND_ID_BOSS);
				animation_set->at(ani)->Render(x, y);
				//RenderBoundingBox();
				for (int i = 0; i < left_coupling_elements.size(); i++)
				{
					if (left_coupling_elements[i])
					{
						left_coupling_elements[i]->Render();
					}
				}
				for (int i = 0; i < right_coupling_elements.size(); i++)
				{
					if (right_coupling_elements[i])
					{
						right_coupling_elements[i]->Render();
					}
				}
				if (right_pincer)
					right_pincer->Render();
				if (left_pincer)
					left_pincer->Render();
				for (int i = 0; i < list_weapon.size(); i++)
				{
					list_weapon[i]->Render();
				}
			}
		}
	}
	
}

void CBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case BOSS_STATE_MOVE:
		vx = BOSS_MOVE_SPEED;
		vy = -0.02;
		break;
	case BOSS_STATE_MOVE_CHANGE_DIRECTION_X:
		vx = -vx;
		break;
	case BOSS_STATE_MOVE_CHANGE_DIRECTION_Y:
		vy = -vy;
		break;
	case BOSS_STATE_ATTACK:
	{
		if (isDisplay)
		{
			CWeapon* weapon = new CWeapon(WEAPON_TYPE_BOSS);// Khởi tạo weapon
			weapon->SetDirection(nx);
			weapon->SetPosition(x + BOSS_BBOX_WIDTH / 2, y);
			weapon->SetState(WEAPON_BOSS_STATE_FLY);
			if (player)
				weapon->SetPlayerObject(player);
			list_weapon.push_back(weapon);
		}

	}
	break;
	case BOSS_STATE_EXPLOSION:
		vx = vy = 0;
		break;
	case BOSS_STATE_DIE:
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