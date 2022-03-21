
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Human.h"
#include "Game.h"

#include "Portal.h"
#include "Brick.h"
#include "Stair.h"

#include "Worm.h"
#include "Spider.h"
#include "EnemyObject1.h"
#include "Spike.h"
#include "Eyeball.h"
#include "Cannon.h"
#include "Floater.h"
#include "Dome.h"
#include "Jumper.h"
#include "Teleporter.h"
#include "Orb.h"
#include "Skull.h"
#include "Boss.h"
#include "Sound.h"
#include "Item.h"
#include "SpecialPortal.h"
#include "Egg.h"
#include "Lava.h"
#include "Arrow.h"
#include "Mine.h"
#define JUMPER_ROUNDING_DISTANCE_X 50
#define JUMPER_ROUNDING_DISTANCE_Y 20
#define ORB_ROUNDING_DISTANCE_X 120
#define ORB_ROUNDING_DISTANCE_Y 110
#define SKULL_ROUNDING_DISTANCE_X 2
CHuman::CHuman(float x, float y) : CGameObject()
{
	level = HUMAN_LEVEL_SMALL;
	SetState(HUMAN_STATE_IDLE);
	isEnable = true;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CHuman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable)
		return;
	if (player != NULL)
	{
		CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
		if (!player_object->Is_Human)
			return;
		float x_player, y_player;
		player_object->GetPosition(x_player, y_player);
		if (player_object->GetPower() < 0)
		{
			if(level==HUMAN_LEVEL_SMALL)
				SetState(MAIN_CHARACTER_STATE_EXPLOSION);
			else
			{
				SetState(MAIN_CHARACTER_STATE_DIE);
				player_object->SetState(MAIN_CHARACTER_STATE_DIE);
			}
		}
		
		if (player_object->Is_Human && level == HUMAN_LEVEL_SMALL)
		{
			if (x_player <= x && x <= (x_player + MAIN_CHARACTER_BBOX_WIDTH) && y_player <= y && y <= (y_player + MAIN_CHARACTER_BBOX_HEIGHT))
				CanChangeBeingPLayer = true;
			else
				CanChangeBeingPLayer = false;
		}
	}
	CGame* game = CGame::GetInstance();
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Xử lý di chuyển của các đối tượng enemy theo đối tượng nhân vật chính
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CFloater*>(coObjects->at(i))) {
			CFloater* floater = dynamic_cast<CFloater*>(coObjects->at(i));
			float x_enemy, y_enemy;
			floater->GetPosition(x_enemy, y_enemy);
			if (x > x_enemy)
				floater->SetDirection(1);
			else
				floater->SetDirection(-1);
			floater->SetPlayerObject(this);
		}
		else if (dynamic_cast<CDome*>(coObjects->at(i))) {
			CDome* dome = dynamic_cast<CDome*>(coObjects->at(i));
			float x_enemy, y_enemy;
			dome->GetPosition(x_enemy, y_enemy);
			if (x > x_enemy)
				dome->SetDirection(1);
			else
				dome->SetDirection(-1);
		}
		else if (dynamic_cast<CWorm*>(coObjects->at(i))) {
			CWorm* worm = dynamic_cast<CWorm*>(coObjects->at(i));

			float x_worm, y_worm;
			worm->GetPosition(x_worm, y_worm);
			if (x > x_worm)
				worm->SetDirection(1);
			else
				worm->SetDirection(-1);
		}
		else if (dynamic_cast<CEyeball*>(coObjects->at(i))) {
			CEyeball* eyeball = dynamic_cast<CEyeball*>(coObjects->at(i));

			float x_eyeball, y_eyeball;
			eyeball->GetPosition(x_eyeball, y_eyeball);
			if (x > x_eyeball)
				eyeball->SetDirection(1);
			else
				eyeball->SetDirection(-1);
			if (y > y_eyeball)
				eyeball->SetDirectionY(1);//Up
			else
				eyeball->SetDirectionY(-1);//Down
			eyeball->SetPlayerObject(this);
		}
		else if (dynamic_cast<CTeleporter*>(coObjects->at(i))) {
			CTeleporter* teleporter = dynamic_cast<CTeleporter*>(coObjects->at(i));

			float x_teleporter, y_teleporter;
			teleporter->GetPosition(x_teleporter, y_teleporter);
			if (x > x_teleporter)
				teleporter->SetDirection(1);
			else
				teleporter->SetDirection(-1);
			if (y > y_teleporter)
				teleporter->SetDirectionY(1);//Up
			else
				teleporter->SetDirectionY(-1);//Down
			teleporter->SetPlayerObject(this);
		}
		else if (dynamic_cast<CCannon*>(coObjects->at(i))) {
			CCannon* cannon = dynamic_cast<CCannon*>(coObjects->at(i));
			cannon->SetPlayerObject(this);
		}
		else if (dynamic_cast<CJumper*>(coObjects->at(i))) {
			CJumper* jumper = dynamic_cast<CJumper*>(coObjects->at(i));

			float x_jumper, y_jumper;
			jumper->GetPosition(x_jumper, y_jumper);
			if (x > x_jumper)
				jumper->SetDirection(1);
			else
				jumper->SetDirection(-1);
			if (jumper->GetState() != STATE_ITEM)
			{
				if (abs(x - x_jumper) < JUMPER_ROUNDING_DISTANCE_X && abs(y - y_jumper) < JUMPER_ROUNDING_DISTANCE_Y)
				{
					jumper->SetState(JUMPER_STATE_JUMP);
					jumper->SetIsJumping(true);
				}

				else
					jumper->SetIsJumping(false);
			}

		}
		else if (dynamic_cast<COrb*>(coObjects->at(i))) {
			COrb* orb = dynamic_cast<COrb*>(coObjects->at(i));
			int type = orb->GetType();
			if (type != 1)
			{
				float x_orb, y_orb;
				orb->GetPosition(x_orb, y_orb);
				if (x > x_orb)
					orb->SetDirection(1);
				else
					orb->SetDirection(-1);
				if (orb->GetState() != STATE_ITEM)
				{
					if (abs(x - x_orb) < ORB_ROUNDING_DISTANCE_X && abs(y - y_orb) < ORB_ROUNDING_DISTANCE_Y)
						orb->SetState(ORB_STATE_ATTACK);
					else orb->SetState(ORB_STATE_IDLE);
				}
			}
		}
		else if (dynamic_cast<CSkull*>(coObjects->at(i))) {
			CSkull* skull = dynamic_cast<CSkull*>(coObjects->at(i));
			skull->SetPlayerObject(this);
			float x_skull, y_skull;
			skull->GetPosition(x_skull, y_skull);
			if (x > x_skull)
				skull->SetDirection(1);
			else
				skull->SetDirection(-1);
			if (skull->GetState() != STATE_ITEM)
			{
				if (!skull->isAttacked) {
					if (abs(x - x_skull) > SKULL_ROUNDING_DISTANCE_X) {
						if (x < x_skull)
						{
							skull->SetState(SKULL_STATE_MOVE_LEFT);
						}
						else if (x > x_skull)
						{
							skull->SetState(SKULL_STATE_MOVE_RIGHT);
						}
					}
					else
					{
						skull->time_moving += dt;
						//skull->SetState(SKULL_STATE_MOVE_UP);
						skull->SetState(SKULL_STATE_MOVE_RIGHT_ATTACK);
						skull->SetIsAttack(true);

					}

				}
				else
				{
					if (skull->time_moving < dt) {
						skull->SetState(SKULL_STATE_MOVE_UP);
					}
				}

			}
		}
		else if (dynamic_cast<CBoss*>(coObjects->at(i))) {
			CBoss* boss = dynamic_cast<CBoss*>(coObjects->at(i));
			boss->SetPlayerObject(this);
		}
		else if (dynamic_cast<CLava*>((coObjects->at(i)))) // if e->obj is CLava
		{
			CLava* lava = dynamic_cast<CLava*>(coObjects->at(i));
			lava->SetPlayerObject(this);
		}
		else if (dynamic_cast<CStair*>((coObjects->at(i))))
		{
			float l_stair, t_stair, r_stair, b_stair;
			coObjects->at(i)->GetBoundingBox(l_stair, t_stair, r_stair, b_stair);
			float stair_width = abs(l_stair - r_stair);
			float stair_height = abs(t_stair - b_stair);
			float l, t, r, b;
			GetBoundingBox(l, t, r, b);
			if (l_stair <= x && x <= r_stair && abs(b - t_stair)<=HUMAN_SMALL_BBOX_HEIGHT)
				canChangeStateClimb = true;
			else
				canChangeStateClimb = false;
			float l1, t1, r1, b1, l2, t2, r2, b2;
			GetBoundingBox(l1, t1, r1, b1);
			dynamic_cast<CStair*>(coObjects->at(i))->GetBoundingBox(l2, t2, r2, b2);
			if (isStateClimb)
			{
				if (!CGame::GetInstance()->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2))
					isStateClimb = false;
			}
		}
		else if (dynamic_cast<CItem*>((coObjects->at(i))))
		{
			CItem* item = dynamic_cast<CItem*>((coObjects->at(i)));
			if (item->GetState() != ITEM_STATE_DIE)
			{
				if (CGame::GetInstance()->GetCurrentScenceID()==24)
				{
					float x_item, y_item;
					item->GetPosition(x_item, y_item);
					if (x_item >= x)
						SetState(MAIN_CHARACTER_STATE_RUN_RIGHT);
					else
						SetState(MAIN_CHARACTER_STATE_RUN_LEFT);
					if (y_item >= y)
						SetState(MAIN_CHARACTER_STATE_UP_BARREL);
					else
						SetState(MAIN_CHARACTER_STATE_DOWN_BARREL);
				}
			}
		}
	}
	// Simple fall down
	if (level == HUMAN_LEVEL_SMALL && !isStateClimb)
	{
		vy -= HUMAN_GRAVITY * dt;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != HUMAN_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (isAttacked)
	{
		if (untouchable == 0)
		{
			StartUntouchable();
			isAttacked = false;
			if (player != NULL)
			{
				CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
				int power = player_object->GetPower();
				power--;
				player_object->SetPower(power);
				Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
			}
		}
	}
	// reset untouchable timer if untouchable time has passed
	if (untouchable == 1)
	{
		if (GetTickCount() - untouchable_start > MAIN_CHARACTER_UNTOUCHABLE_TIME)
		{
			untouchable = 0;
			untouchable_start = 0;
		}
	}
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

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny > 0)
				{
					is_on_ground = true;
				}
			}
			else if (dynamic_cast<CStair*>(e->obj))
			{
				x += dx;
			}
			else if (dynamic_cast<CEgg*>(e->obj))
			{
				CEgg* egg = dynamic_cast<CEgg*>(e->obj);
				if (egg->GetState() == STATE_ITEM)
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
						int power = player_object->GetPower();
						if (power < 8)
						{
							power++;
							player_object->SetPower(power);
						}
					}
					egg->SetState(EGG_STATE_DESTROYED);
				}

			}
			else if (dynamic_cast<CSpike*>(e->obj))
			{
				x += dx;
				y += dy;
				float l1, t1, r1, b1, l2, t2, r2, b2;
				GetBoundingBox(l1, t1, r1, b1);
				b1 = b1 - ((b1 - t1) / 2);
				dynamic_cast<CSpike*>(e->obj)->GetBoundingBox(l2, t2, r2, b2);

				if (game->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
				}
			}
			else if (dynamic_cast<CLava*>(e->obj)) // if e->obj is CLava
			{
				if (untouchable == 0)
				{
					if (player != NULL)
					{
						CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
						int power = player_object->GetPower();
						power--;
						player_object->SetPower(power);
					}
					StartUntouchable();
					Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
				}
			}
			else if (dynamic_cast<CArrow*>(e->obj))
			{
				if (untouchable == 0)
				{
					if (player != NULL)
					{
						CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
						int power = player_object->GetPower();
						power--;
						player_object->SetPower(power);
					}
					StartUntouchable();
					Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
				}
			}
			// Nếu là portal object thì thực hiện chuyển cảnh
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				if (level == HUMAN_LEVEL_BIG)
				{
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					//Nếu portal là đối tượng chuyển next scene
					if (p->GetType() == 1)
					{
						CGame::GetInstance()->SetIsNextMap(true);
						CGame::GetInstance()->SetIsPreMap(false);
						CGame::GetInstance()->SetIsUpMap(false);
						CGame::GetInstance()->SetIsDownMap(false);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
					//Nếu portal là đối tượng chuyển up scene
					else if (p->GetType() == 3)
					{
						CGame::GetInstance()->SetIsNextMap(false);
						CGame::GetInstance()->SetIsPreMap(false);
						CGame::GetInstance()->SetIsUpMap(true);
						CGame::GetInstance()->SetIsDownMap(false);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
					//Nếu portal là đối tượng chuyển down scene
					else if (p->GetType() == 4)
					{
						CGame::GetInstance()->SetIsNextMap(false);
						CGame::GetInstance()->SetIsPreMap(false);
						CGame::GetInstance()->SetIsUpMap(false);
						CGame::GetInstance()->SetIsDownMap(true);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
					//Nếu portal là đối tượng chuyển previous scene
					else
					{
						CGame::GetInstance()->SetIsNextMap(false);
						CGame::GetInstance()->SetIsPreMap(true);
						CGame::GetInstance()->SetIsUpMap(false);
						CGame::GetInstance()->SetIsDownMap(false);
						CGame::GetInstance()->SetSceneId(p->GetSceneId());
						CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					}
				}
			}
			else if (dynamic_cast<CSpecialPortal*>(e->obj))
			{
				if (level == HUMAN_LEVEL_SMALL && !isStateClimb)
				{
					CSpecialPortal* p = dynamic_cast<CSpecialPortal*>(e->obj);
					//Nếu portal là đối tượng chuyển overworld
					CGame::GetInstance()->SetIsNextMap(false);
					CGame::GetInstance()->SetIsPreMap(true);//Tránh hiệu ứng di chuyển màn hình
					CGame::GetInstance()->SetIsUpMap(false);
					CGame::GetInstance()->SetIsDownMap(false);
					CGame::GetInstance()->SetSceneId(p->GetSceneId());
					CGame::GetInstance()->SetNextPortalId(p->GetNextPortalId());
					CGame::GetInstance()->SetParamEnteringOverWorld(true);
				}
			}
			//Outdoor enemies
			else if (dynamic_cast<CWorm*>(e->obj))
			{
				CWorm* worm = dynamic_cast<CWorm*>(e->obj);
				float vxWorm, vyWorm;
				worm->GetSpeed(vxWorm, vyWorm);
				if (worm->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyWorm * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
						int power = player_object->GetPower();
						if (power < 8)
						{
							power++;
							player_object->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					worm->SetState(WORM_STATE_DIE);
				}

			}
			else if (dynamic_cast<CFloater*>(e->obj))
			{
				CFloater* floater = dynamic_cast<CFloater*>(e->obj);
				float vxFloater, vyFloater;
				floater->GetSpeed(vxFloater, vyFloater);
				if (floater->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyFloater * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					floater->SetState(FLOATER_STATE_DIE);
				}


			}
			else if (dynamic_cast<CDome*>(e->obj))
			{
				CDome* dome = dynamic_cast<CDome*>(e->obj);
				float vxDome, vyDome;
				dome->GetSpeed(vxDome, vyDome);
				if (dome->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyDome * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					dome->SetState(DOME_STATE_DIE);
				}


			}
			else if (dynamic_cast<CJumper*>(e->obj))
			{
				CJumper* jumper = dynamic_cast<CJumper*>(e->obj);
				float vxJumper, vyJumper;
				jumper->GetSpeed(vxJumper, vyJumper);
				if (jumper->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyJumper * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					jumper->SetState(DOME_STATE_DIE);
				}


			}
			else if (dynamic_cast<COrb*>(e->obj))
			{
				COrb* orb = dynamic_cast<COrb*>(e->obj);
				if (orb->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					float vxOrb, vyOrb;
					orb->GetSpeed(vxOrb, vyOrb);
					if (e->ny != 0)
					{
						y -= 2 * vyOrb * dt;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					orb->SetState(ORB_STATE_DIE);
				}
			}
			else if (dynamic_cast<CSkull*>(e->obj))
			{
				CSkull* skull = dynamic_cast<CSkull*>(e->obj);
				float vxSkull, vySkull;
				skull->GetSpeed(vxSkull, vySkull);
				if (skull->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					skull->SetState(SKULL_STATE_DIE);
				}
			}
			else if (dynamic_cast<CMine*>(e->obj))
			{
				CMine* mine = dynamic_cast<CMine*>(e->obj);
				if (mine->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					mine->SetState(SKULL_STATE_DIE);
				}
			}
			//Indoor enemies
			else if (dynamic_cast<CCannon*>(e->obj))
			{
				CCannon* cannon = dynamic_cast<CCannon*>(e->obj);
				float vxCannon, vyCannon;
				cannon->GetSpeed(vxCannon, vyCannon);
				if (cannon->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					cannon->SetState(CANNON_STATE_DIE);
				}
			}
			else if (dynamic_cast<CEyeball*>(e->obj))
			{
				CEyeball* eyeball = dynamic_cast<CEyeball*>(e->obj);
				float vxEyeball, vyEyeball;
				eyeball->GetSpeed(vxEyeball, vyEyeball);
				if (eyeball->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					eyeball->SetState(EYEBALL_STATE_DIE);

				}
			}
			else if (dynamic_cast<CTeleporter*>(e->obj))
			{
				CTeleporter* teleporter = dynamic_cast<CTeleporter*>(e->obj);
				float vxTeleporter, vyTeleporter;
				teleporter->GetSpeed(vxTeleporter, vyTeleporter);
				if (teleporter->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					teleporter->SetState(TELEPORTER_STATE_DIE);
				}
			}
			//Boss
			else if (dynamic_cast<CBoss*>(e->obj))
			{
				CBoss* boss = dynamic_cast<CBoss*>(e->obj);
				float vxBoss, vyBoss;
				boss->GetSpeed(vxBoss, vyBoss);
				if (boss->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							power--;
							player_object->SetPower(power);
						}
						StartUntouchable();
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= dy;
					}
					else
						x -= dx;
				}
				else
				{
					if (player != NULL)
					{
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
						if (power < 8)
						{
							power++;
							dynamic_cast<CMainCharacter*>(player)->SetPower(power);
						}
					}
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					boss->SetState(BOSS_STATE_DIE);
				}
			}
			//Item
			else if (dynamic_cast<CItem*>(e->obj))
			{
				Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
				CItem* item = dynamic_cast<CItem*>(e->obj);
				if (item->GetType() == 4) {

					int power = dynamic_cast<CMainCharacter*>(player)->GetPower();
					if (power < 8)
					{
						power++;
						dynamic_cast<CMainCharacter*>(player)->SetPower(power);
					}
				}
				{
					if (CGame::GetInstance()->GetCurrentScenceID() == 24)
					{
						if (player != NULL)
						{
							CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
							int power = player_object->GetPower();
							int alives = player_object->GetAlive();
							CGame::GetInstance()->SwitchScene(8, alives, power);
						}
					}
				}
				item->SetState(ITEM_STATE_DIE);
			}
		}
	}
	//Update list of weapon objects
	if (list_weapon.size() > 0)
	{
		for (int i = 0; i < list_weapon.size(); i++)
			list_weapon[i]->Update(dt, coObjects);
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
int ani = HUMAN_ANI_BIG_WALKING;
int ani_small = HUMAN_ANI_SMALL_WALKING;
int flip = true;
void CHuman::Render()
{
	if (player)
	{
		CMainCharacter* player_object = dynamic_cast<CMainCharacter*>(player);
		if (!player_object->Is_Human)
			return;
	}
	if (!isEnable)
	{
		return;
	}
	{
		if (level == HUMAN_LEVEL_BIG)
		{
			int alpha = 255;
			if (untouchable)
				alpha = 128;
			switch (state)
			{
			case MAIN_CHARACTER_STATE_RUN_RIGHT:
				flip = true;
				ani = HUMAN_ANI_BIG_WALKING;
				break;
			case MAIN_CHARACTER_STATE_RUN_LEFT:
				flip = false;
				ani = HUMAN_ANI_BIG_WALKING;
				break;
			case MAIN_CHARACTER_STATE_UP_BARREL:
				ani = HUMAN_ANI_BIG_WALKING_UP;
				break;
			case MAIN_CHARACTER_STATE_DOWN_BARREL:
				ani = HUMAN_ANI_BIG_WALKING_DOWN;
				break;
			}
			if (vx == 0 && vy == 0) // Nhân vật đứng yên
			{
				//if (ani == animation_set->size())
					//return;
				animation_set->at(ani)->isPause = true; //Dừng animation 
				animation_set->at(ani)->Render(x, y, flip, alpha); // Vẽ frame đang bị tạm dừng
			}
			else // Nhân vật di chuyển
			{
				animation_set->at(ani)->isPause = false; // Tiếp tục animation đã dừng trước đó
				animation_set->at(ani)->Render(x, y, flip, alpha);
			}
			animation_set->at(ani)->Render(x, y, flip, alpha);
			RenderBoundingBox();
		}
		else if (level == HUMAN_LEVEL_SMALL)
		{
			if (vx == 0)
			{
				if (nx > 0)
				{
					if (isStateCrawl)
					{
						ani_small = HUMAN_ANI_SMALL_CRAWLING;
						flip = false;
					}
					else
					{
						flip = true;
						ani_small = HUMAN_ANI_SMALL_IDLE;
					}

				}
				else
				{
					if (isStateCrawl)
					{
						flip = true;
						ani_small = HUMAN_ANI_SMALL_CRAWLING;
					}
					else if (isStateClimb)
					{
						ani_small = HUMAN_ANI_SMALL_CLIMBING;
					}
					else
					{
						flip = false;
						ani_small = HUMAN_ANI_SMALL_IDLE;
					}

				}
				animation_set->at(ani_small)->isPause = true; //Dừng animation
			}
			else if (vx > 0)
			{
				if (isStateCrawl)
				{
					flip = false;
					ani_small = HUMAN_ANI_SMALL_CRAWLING;
				}
				else
				{
					flip = true;
					ani_small = HUMAN_ANI_SMALL_WALKING;
				}
				animation_set->at(ani_small)->isPause = false; // Tiếp tục animation đã dừng trước đó

			}
			else if (vx < 0)
			{
				if (isStateCrawl)
				{
					flip = true;
					ani_small = HUMAN_ANI_SMALL_CRAWLING;
				}
				else
				{
					flip = false;
					ani_small = HUMAN_ANI_SMALL_WALKING;
				}
				animation_set->at(ani_small)->isPause = false; // Tiếp tục animation đã dừng trước đó	
			}
			if (isStateClimb)
			{
				if (vy != 0)
				{
					ani_small = HUMAN_ANI_SMALL_CLIMBING;
					animation_set->at(ani_small)->isPause = false;
				}
				else
				{
					ani_small = HUMAN_ANI_SMALL_CLIMBING;
					animation_set->at(ani_small)->isPause = true;
				}
			}
			if (isStateExplosion)
			{
				ani_small = HUMAN_ANI_SMALL_DYING;
				Sound::getInstance()->Play(SOUND_ID_PLAYER_EXPLOSION);
				if (animation_set->at(ani_small)->isFinish)
				{
					SetState(MAIN_CHARACTER_STATE_DIE);
					if(player)
						dynamic_cast<CMainCharacter*>(player)->SetState(MAIN_CHARACTER_STATE_DIE);
				}
			}
			int alpha = 255;
			if (untouchable)
				alpha = 128;
			animation_set->at(ani_small)->Render(x, y, flip, alpha);
		}
	}
	if (list_weapon.size() > 0)
	{
		for (int i = 0; i < list_weapon.size(); i++)
			list_weapon[i]->Render();
	}
}

void CHuman::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		//if (!isStateClimb)
	{
		vx = HUMAN_WALKING_SPEED;
		nx = 1;
		isGoingUp = false;
		isGoingDown = false;
	}
	break;
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		//if (!isStateClimb)
	{
		vx = -HUMAN_WALKING_SPEED;
		nx = -1;
		isGoingUp = false;
		isGoingDown = false;
	}
	break;
	case MAIN_CHARACTER_STATE_JUMP:
		// TODO: need to check if HUMAN is *current* on a platform before allowing to jump again
		if (level == HUMAN_LEVEL_SMALL)
		{
			if (is_on_ground && !isStateCrawl)
			{
				vy = HUMAN_SMALL_JUMP_SPEED_Y;
				is_on_ground = false;
				Sound::getInstance()->PlayNew(SOUND_ID_PLAYER_JUMPING);
			}
		}
		isGoingUp = false;
		isGoingDown = false;
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		vx = 0;
		if (level == HUMAN_LEVEL_BIG)
			vy = 0;
		else if (level == HUMAN_LEVEL_SMALL)
		{
			if (isStateClimb)
				vy = 0;
		}
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
		if (level == HUMAN_LEVEL_BIG)
		{
			isGoingUp = true;
			isGoingDown = false;
			vy = HUMAN_WALKING_SPEED;
		}
		else if (level == HUMAN_LEVEL_SMALL)
		{
			if (isStateCrawl)
			{
				vy = 0.14f;//Tránh rớt va chạm khi chuyển state từ CRAWL->IDLE
				isStateCrawl = false;
			}
			else if (isStateClimb)
			{
				vy = 0.04f;
			}
			else
			{
				if (canChangeStateClimb)
					SetState(HUMAN_STATE_CLIMB);
			}
		}
		break;
	case MAIN_CHARACTER_STATE_DOWN_BARREL:
		{
			if (level == HUMAN_LEVEL_BIG)
			{
				isGoingUp = false;
				isGoingDown = true;
				vy = -HUMAN_WALKING_SPEED;
			}
			else if (level == HUMAN_LEVEL_SMALL)
			{
				if (!isStateClimb)
					isStateCrawl = true;
				if (isStateClimb)
				{
					vy = -0.04f;
				}
			}
		}
		break;
	case MAIN_CHARACTER_STATE_DIE:
		isEnable = false;
		vx = vy = 0;
		break;
	case MAIN_CHARACTER_STATE_EXPLOSION:
		vx = vy = 0;
		isStateExplosion = true;
		break;
	case HUMAN_STATE_CLIMB:
		isStateClimb = true;
		vy = 0;
		vx = 0;
		break;
	case MAIN_CHARACTER_STATE_BARREL_FIRE:
		{
			CWeapon* weapon = new CWeapon(WEAPON_TYPE_BIG_HUMAN);// Khởi tạo weapon theo x,y của human
			float x_human = this->x;
			float y_human = this->y;
			if (level == HUMAN_LEVEL_SMALL)
			{
				if (GetIsStateCrawl())
					weapon->SetPosition(x_human, y_human);
				else
					weapon->SetPosition(x_human, y_human - HUMAN_SMALL_BBOX_HEIGHT / 2);
				weapon->SetDirection(nx);
				weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY);
			}
			else
			{
				switch (ani)
				{
				case HUMAN_ANI_BIG_WALKING_UP:
					weapon->SetPosition(x_human + HUMAN_BIG_BBOX_WIDTH / 2, y_human);
					weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY_UP);
					break;
				case HUMAN_ANI_BIG_WALKING_DOWN:
					weapon->SetPosition(x_human + HUMAN_BIG_BBOX_WIDTH / 2, y_human);
					weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY_DOWN);
					break;
				case HUMAN_ANI_BIG_WALKING:
					if (flip)
					{
						weapon->SetPosition(x_human, y_human - HUMAN_BIG_BBOX_HEIGHT / 2);
						weapon->SetDirection(1);
						weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY);
					}
					else
					{
						weapon->SetPosition(x_human, y_human - HUMAN_BIG_BBOX_HEIGHT / 2);
						weapon->SetDirection(-1);
						weapon->SetState(WEAPON_BIG_HUMAN_STATE_FLY);
					}
					break;
				}
			}
			list_weapon.push_back(weapon);
		}
		break;
	}
}

void CHuman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	if (level == HUMAN_LEVEL_BIG)
	{
		top = y - HUMAN_BIG_BBOX_HEIGHT;
		right = x + HUMAN_BIG_BBOX_WIDTH;
		bottom = y;
	}
	else
	{
		if (isStateCrawl)
		{
			top = y - HUMAN_SMALL_CRAWL_BBOX_HEIGHT;
			right = x + HUMAN_SMALL_CRAWL_BBOX_WIDTH;
			bottom = y;
		}
		else
		{
			top = y - HUMAN_SMALL_BBOX_HEIGHT;
			right = x + HUMAN_SMALL_BBOX_WIDTH;
			bottom = y;
		}
	}
}

/*
	Reset HUMAN status to the beginning state of a scene
*/
void CHuman::Reset()
{
	SetState(MAIN_CHARACTER_STATE_IDLE);
	if(level==HUMAN_LEVEL_BIG)
		SetLevel(HUMAN_LEVEL_BIG);
	else
		SetLevel(HUMAN_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

