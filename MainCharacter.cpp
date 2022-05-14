#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "MainCharacter.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"
#include "Stair.h"
#include "Lava.h"

#include "EnemyObject1.h"
#include "Chowmein.h"
#include "Spider.h"
#include "Eyeball.h"
#include "Floater.h"
#include "Dome.h"
#include "Cannon.h"
#include "Locust.h"
#include "Jumper.h"

#include "Insect.h"
#include "Skull.h"


#include "Teleporter.h"
#include "Arrow.h"
#include "Mine.h"

#define JUMPER_ROUNDING_DISTANCE_X 50
#define JUMPER_ROUNDING_DISTANCE_Y 20


#define SKULL_ROUNDING_DISTANCE_X 2
#define LOCUST_ROUNDING_DISTANCE_X 120
#define LOCUST_ROUNDING_DISTANCE_Y 110
#include "Sound.h"

CMainCharacter::CMainCharacter(float x, float y) : CGameObject()
{
	isEnable = true;
	SetState(MAIN_CHARACTER_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMainCharacter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable)
		return;
	if (power < 0)
	{
		Sound::getInstance()->Play(SOUND_ID_PLAYER_EXPLOSION);
		SetState(MAIN_CHARACTER_STATE_EXPLOSION);
	}
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (x <= 0 && vx < 0)
		dx = 0;
	// reset untouchable timer if untouchable time has passed

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
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
		else if (dynamic_cast<CChowmein*>(coObjects->at(i))) {
			CChowmein* chowmein = dynamic_cast<CChowmein*>(coObjects->at(i));

			float x_chowmein, y_chowmein;
			chowmein->GetPosition(x_chowmein, y_chowmein);
			if (x > x_chowmein)
				chowmein->SetDirection(1);
			else
				chowmein->SetDirection(-1);
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
		}
		else if (dynamic_cast<CLocust*>(coObjects->at(i))) {
			CLocust* locust = dynamic_cast<CLocust*>(coObjects->at(i));
			int type = locust->GetType();
			if (type != 1)
			{
				float x_locust, y_locust;
				locust->GetPosition(x_locust, y_locust);
				if (x > x_locust)
					locust->SetDirection(1);
				else
					locust->SetDirection(-1);
				if (locust->GetState() != STATE_ITEM)
				{
					if (abs(x - x_locust) < LOCUST_ROUNDING_DISTANCE_X && abs(y - y_locust) < LOCUST_ROUNDING_DISTANCE_Y)
						locust->SetState(LOCUST_STATE_ATTACK);
					else locust->SetState(LOCUST_STATE_IDLE);
				}
			}
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
					if (skull->time_moving < 100) {
						skull->SetState(SKULL_STATE_MOVE_UP);
						skull->time_moving = 0;
					}
				}

			}
		}
		else if (dynamic_cast<CLava*>((coObjects->at(i)))) // if e->obj is CLava
		{
			CLava* lava = dynamic_cast<CLava*>(coObjects->at(i));
			lava->SetPlayerObject(this);
		}

	}
	// Simple fall down
	if (state != MAIN_CHARACTER_STATE_NONE_COLLISION)
		vy -= MAIN_CHARACTER_GRAVITY * dt;

	coEvents.clear();
	// turn off collision when die 
	if (state != MAIN_CHARACTER_STATE_NONE_COLLISION && MAIN_CHARACTER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (isAttacked)
	{
		if (untouchable == 0)
		{
			StartUntouchable();
			isAttacked = false;
			power--;
			Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

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
			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is CBrick 
			{
				//CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->ny > 0)
				{
					DebugOut(L"[INFO] Main character cbrick 292 \n");
					isOnGround = true;
				}
				else {
					DebugOut(L"[INFO] Main character cbrick 296 \n");
					isOnGround = false;
				}
					
			}
			else if (dynamic_cast<CLava*>(e->obj)) // if e->obj is CLava
			{
				if (untouchable == 0)
				{
					StartUntouchable();
					power--;
					Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
				}
			}
			else if (dynamic_cast<CStair*>(e->obj))
			{
				x += dx;
			}
			else if (dynamic_cast<CArrow*>(e->obj))
			{
				if (untouchable == 0)
				{
					StartUntouchable();
					power--;
					Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
				}
			}
			// Nếu là portal object thì thực hiện chuyển cảnh
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				isOnGround = false;
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
			//Outdoor enemies
			else if (dynamic_cast<CChowmein*>(e->obj))
			{
				isOnGround = false;
				CChowmein* chowmein = dynamic_cast<CChowmein*>(e->obj);
				float vxChowmein, vyChowmein;
				chowmein->GetSpeed(vxChowmein, vyChowmein);
				if (chowmein->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
						//if(chowmein->GetState() == CHOWMEIN_STATE_WALKING)
						chowmein->SetState(CHOWMEIN_STATE_ATTACK);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyChowmein * dt;
					}
					else
						x += dx;
				}
				else
				{
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
					if (e->ny != 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					chowmein->SetState(CHOWMEIN_STATE_DIE);
				}

			}
			else if (dynamic_cast<CFloater*>(e->obj))
			{
				isOnGround = false;
				CFloater* floater = dynamic_cast<CFloater*>(e->obj);
				float vxFloater, vyFloater;
				floater->GetSpeed(vxFloater, vyFloater);
				if (floater->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
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
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
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
				isOnGround = false;
				CDome* dome = dynamic_cast<CDome*>(e->obj);
				float vxDome, vyDome;
				dome->GetSpeed(vxDome, vyDome);
				if (dome->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
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
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
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
				isOnGround = false;
				CJumper* jumper = dynamic_cast<CJumper*>(e->obj);
				float vxJumper, vyJumper;
				jumper->GetSpeed(vxJumper, vyJumper);
				if (jumper->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
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
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					jumper->SetState(DOME_STATE_DIE);
				}


			}
			else if (dynamic_cast<CInsect*>(e->obj))
			{
				isOnGround = false;
				CInsect* insect = dynamic_cast<CInsect*>(e->obj);
				float vxInsect, vyInsect;
				insect->GetSpeed(vxInsect, vyInsect);
				if (insect->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y -= 2 * vyInsect * dt;
					}
					else
						x += dx;
				}
				else
				{
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					insect->SetState(INSECT_STATE_DIE);
				}
			}
			else if (dynamic_cast<CSkull*>(e->obj))
			{
				isOnGround = false;
				CSkull* skull = dynamic_cast<CSkull*>(e->obj);
				float vxSkull, vySkull;
				skull->GetSpeed(vxSkull, vySkull);
				if (skull->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
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
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					skull->SetState(SKULL_STATE_DIE);
				}
			}
			else if (dynamic_cast<CLocust*>(e->obj))
			{
				isOnGround = false;
				CLocust* locust = dynamic_cast<CLocust*>(e->obj);
				if (locust->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					float vxLocust, vyLocust;
					locust->GetSpeed(vxLocust, vyLocust);
					if (e->ny == 1)
					{
						y += dy;
					}
					else if (e->ny == -1)
					{
						y -= 4 * vyLocust * dt;
					}
					else
						x += dx;
				}
				else
				{
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
					if (e->ny < 0)
					{
						y -= 2 * vy * dt;
					}
					else
						x += dx;
					locust->SetState(LOCUST_STATE_DIE);
				}
			}
			else if (dynamic_cast<CMine*>(e->obj))
			{
				isOnGround = false;
				CMine* mine = dynamic_cast<CMine*>(e->obj);
				if (mine->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
				}
				else
				{
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
					mine->SetState(MINE_STATE_DIE);
				}
			}
			//Indoor enemies
			else if (dynamic_cast<CCannon*>(e->obj))
			{
				isOnGround = false;
				CCannon* cannon = dynamic_cast<CCannon*>(e->obj);
				float vxCannon, vyCannon;
				cannon->GetSpeed(vxCannon, vyCannon);
				if (cannon->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += vyCannon * dt;
					}
					else
						x += dx;
				}
				else
				{
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
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
				isOnGround = false;
				CEyeball* eyeball = dynamic_cast<CEyeball*>(e->obj);
				float vxEyeball, vyEyeball;
				eyeball->GetSpeed(vxEyeball, vyEyeball);
				if (eyeball->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += vyEyeball * dt;
					}
					else
						x += dx;
				}
				else
				{
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
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
				isOnGround = false;
				CTeleporter* teleporter = dynamic_cast<CTeleporter*>(e->obj);
				float vxTeleporter, vyTeleporter;
				teleporter->GetSpeed(vxTeleporter, vyTeleporter);
				if (teleporter->GetState() != STATE_ITEM)
				{
					if (untouchable == 0)
					{
						StartUntouchable();
						power--;
						Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
					}
					if (e->ny != 0)
					{
						y += vyTeleporter * dt;
					}
					else
						x += dx;
				}
				else
				{
					Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
					if (power < 8)
						power++;
					if (e->ny != 0)
					{
						y += dy;
					}
					else
						x += dx;
					teleporter->SetState(TELEPORTER_STATE_DIE);
				}
			}
		}
	}

	//Update list of weapon objects
	if (list_weapon.size() > 0)
	{
		for (int i = 0; i < list_weapon.size(); i++)
			list_weapon[i]->Update(dt, coObjects);
	}

	//Chạy hàm cập nhật của các đối tượng thành phần
	for (int i = 0; i < componentObjects.size(); i++)
	{
		componentObjects[i]->SetPlayerPosition(x, y);
		componentObjects[i]->Update(dt, coObjects);
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMainCharacter::Render()
{
	int alpha = 255;
	CGame* game = CGame::GetInstance();
	if (isEnable)
	{
		if (state == MAIN_CHARACTER_STATE_EXPLOSION)
		{
			animation_set->at(MAIN_CHARACTER_ANI_EXPLOSION)->Render(x, y + 25, alpha, false, 1, 0);
			if (animation_set->at(MAIN_CHARACTER_ANI_EXPLOSION)->isFinish)
				SetState(MAIN_CHARACTER_STATE_DIE);
		}
		else
		{
			animation_set->at(0)->Render(x, y, alpha, false, 1, 0);
			// Vẽ các đối tượng weapon của nhân vật chính
			if (list_weapon.size() > 0)
			{
				for (int i = 0; i < list_weapon.size(); i++)
					list_weapon[i]->Render();
			}
			//Vẽ các object thành phần của player object
			for (int i = 0; i < componentObjects.size(); i++)
			{
				componentObjects[i]->SetUntouchable(untouchable);
				componentObjects[i]->Render();

			}
			//RenderBoundingBox();
		}

	}
}

void CMainCharacter::SetState(int state)
{
	
	CGameObject::SetState(state);
	
	switch (state)
	{
	case MAIN_CHARACTER_STATE_RUN_RIGHT:
		vx = MAIN_CHARACTER_RUN_SPEED;
		nx = 1;
		break;
		
	case MAIN_CHARACTER_STATE_RUN_LEFT:
		vx = -MAIN_CHARACTER_RUN_SPEED;
		nx = -1;
		break;
	case MAIN_CHARACTER_STATE_JUMP:
		// TODO: need to check if MAIN_CHARACTER is *current* on a platform before allowing to jump again
		if (isOnGround)
		{
			vy = MAIN_CHARACTER_JUMP_SPEED_Y;
			isOnGround = false;
			Sound::getInstance()->PlayNew(SOUND_ID_PLAYER_JUMPING);
		}
		break;
	case MAIN_CHARACTER_STATE_IDLE:
		vx = 0;
		break;
	case MAIN_CHARACTER_STATE_DIE:
		vx = vy = 0;
		isEnable = false;
		break;
	case MAIN_CHARACTER_STATE_EXPLOSION:
		vx = vy = 0;
		break;
	case MAIN_CHARACTER_STATE_UP_BARREL:
		break;
	case MAIN_CHARACTER_STATE_STRAIGHT_BARREL:
		break;
	case MAIN_CHARACTER_STATE_DOWN_BARREL:
		break;
	case MAIN_CHARACTER_STATE_BARREL_FIRE:
		vy = 0;
		break;
	case MAIN_CHARACTER_STATE_NONE_COLLISION:
		DebugOut(L"[INFO] Main character none collision \n");
		break;
	default:
		break;

	}
	//Cập nhật state, hướng di chuyển, tốc độ cho các đối tượng thành phần theo nhân vật chính
	for (int i = 0; i < componentObjects.size(); i++)
	{
		componentObjects[i]->SetState(state);
		componentObjects[i]->SetDirection(nx);
		componentObjects[i]->SetSpeed(vx, vy);

		if (state == MAIN_CHARACTER_STATE_BARREL_FIRE)//Nhân vật bắn
		{
			isStartFire = true;
			if (dynamic_cast<CBarrel*>(componentObjects[i]))
			{
				float xBarrel, yBarrel;
				//Lấy vị trí x, y của đối tượng nòng sóng
				dynamic_cast<CBarrel*>(componentObjects[i])->GetPosition(xBarrel, yBarrel);
				//Nếu nòng sóng đang giơ lên
				if (dynamic_cast<CBarrel*>(componentObjects[i])->GetIsBodyUp() == true)
				{
					CWeapon* weapon;
					if (nx > 0)
						weapon = new CWeapon(x + MAIN_CHARACTER_BBOX_WIDTH / 2 + 10, y + 10, nx, state, true);// Khởi tạo weapon theo x,y của barrel
					else
						weapon = new CWeapon(x + MAIN_CHARACTER_BBOX_WIDTH / 2 - 50, y + 10, nx, state, true);// Khởi tạo weapon theo x,y của barrel

					list_weapon.push_back(weapon);
				}
				else
				{
					CWeapon* weapon = new CWeapon(x + 20, y - 5, nx, state, false);// Khởi tạo weapon theo x,y của barrel
					list_weapon.push_back(weapon);
				}

			}
		}
		else if (state == MAIN_CHARACTER_STATE_FIRE_ROCKET)
		{
			CWeapon* weapon = new CWeapon(WEAPON_TYPE_PLAYER_ROCKET);
			weapon->SetPosition(x, y);
			weapon->SetState(WEAPON_PLAYER_ROCKET_STATE_FLY_UP);
			weapon->SetPlayerObject(this);
			list_weapon.push_back(weapon);
			Sound::getInstance()->Play(SOUND_ID_BULLET_FIRE);
		}
		/*else if (state == MAIN_CHARACTER_STATE_UP_BARREL)
		{
			isBeingUpBarrel = true;
		}*/
		else
		{
			isStartFire = false;
			//isBeingUpBarrel = false;
		}

	}

}

void CMainCharacter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == MAIN_CHARACTER_STATE_EXPLOSION)
	{
		left = x;
		top = y - MAIN_CHARACTER_STATE_EXPLOSION_BBOX_HEIGHT;
		right = x + MAIN_CHARACTER_STATE_EXPLOSION_BBOX_WIDTH;
		bottom = y;
	}
	else
	{
		left = x;
		top = y - MAIN_CHARACTER_BBOX_HEIGHT;
		right = x + MAIN_CHARACTER_BBOX_WIDTH;
		bottom = y;
	}


}

/*
	Reset MAIN_CHARACTER status to the beginning state of a scene
*/
void CMainCharacter::Reset()
{

	SetState(MAIN_CHARACTER_STATE_IDLE);
	SetPosition(start_x, start_y);
}

void CMainCharacter::AddComponentObject(CGameObject* object)
{
	componentObjects.push_back(object);
}

vector<LPGAMEOBJECT> CMainCharacter::GetComponentObjects()
{
	return componentObjects;
}

void CMainCharacter::CollisionItem(int item)
{
	/*
		0: whip item
		1: knife item
		2: axe item
		3: bomerang item
		4: small heart
		5: big heart
		6: fire
	*/
	if (item == 0)
	{

	}
	else if (item == 1)
	{

	}
	else if (item == 2)
	{

	}
	else if (item == 3)
	{

	}
	else if (item == 4)
	{

	}
	else if (item == 5)
	{

	}
	//else

}
