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
#include "Worm.h"
#include "Spider.h"
#include "Eyeball.h"
#include "Floater.h"
#include "Dome.h"
#include "Cannon.h"
#include "Orb.h"
#include "Jumper.h"

#include "Insect.h"
#include "Skull.h"


#include "Teleporter.h"
#include "Arrow.h"
#include "Mine.h"

#define JUMPER_ROUNDING_DISTANCE_X 50
#define JUMPER_ROUNDING_DISTANCE_Y 20


#define SKULL_ROUNDING_DISTANCE_X 2
#define ORB_ROUNDING_DISTANCE_X 120
#define ORB_ROUNDING_DISTANCE_Y 110
#include "Sound.h"
CMainCharacter::CMainCharacter(float x, float y) : CGameObject()
{
	isEnable = true;
	SetState(MAIN_CHARACTER_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	Is_Human = false;
}

void CMainCharacter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isEnable)
		return;
	if (power < 0 && !Is_Human)
	{
		Sound::getInstance()->Play(SOUND_ID_PLAYER_EXPLOSION);
		SetState(MAIN_CHARACTER_STATE_EXPLOSION);
	}
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (x <= 0&&vx<0)
		dx = 0;
	// reset untouchable timer if untouchable time has passed

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	//Chỉ xét va chạm cho Main Character khi không phải ở state Human
	if (!Is_Human)
	{
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
						Is_On_Ground = true;
					}
					else
						Is_On_Ground = false;
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
					Is_On_Ground = false;
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
				else if (dynamic_cast<CWorm*>(e->obj))
				{
					Is_On_Ground = false;
					CWorm* worm = dynamic_cast<CWorm*>(e->obj);
					float vxWorm, vyWorm;
					worm->GetSpeed(vxWorm, vyWorm);
					if (worm->GetState() != STATE_ITEM)
					{
						if (untouchable == 0)
						{
							StartUntouchable();
							power--;
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
						Sound::getInstance()->PlayNew(SOUND_ID_EATING_ITEM);
						if (power < 8)
							power++;
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
					Is_On_Ground = false;
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
							y -=  2*vy * dt;
						}
						else
							x += dx;
						floater->SetState(FLOATER_STATE_DIE);
					}


				}
				else if (dynamic_cast<CDome*>(e->obj))
				{
					Is_On_Ground = false;
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
					Is_On_Ground = false;
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
					Is_On_Ground = false;
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
					Is_On_Ground = false;
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
				else if (dynamic_cast<COrb*>(e->obj))
				{
					Is_On_Ground = false;
					COrb* orb = dynamic_cast<COrb*>(e->obj);
					if (orb->GetState() != STATE_ITEM)
					{
						if (untouchable == 0)
						{
							StartUntouchable();
							power--;
							Sound::getInstance()->PlayNew(SOUND_ID_IS_ATTACKED);
						}
						float vxOrb, vyOrb;
						orb->GetSpeed(vxOrb, vyOrb);
						if (e->ny == 1)
						{
							y += dy;
						}
						else if (e->ny == -1)
						{
							y -= 4 * vyOrb * dt;
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
						orb->SetState(ORB_STATE_DIE);
					}
				}
				else if (dynamic_cast<CMine*>(e->obj))
				{
					Is_On_Ground = false;
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
					Is_On_Ground = false;
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
					Is_On_Ground = false;
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
					Is_On_Ground = false;
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
						if(power<8)
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
	}
	
	//Update list of weapon objects
	if (list_weapon.size() > 0)
	{
		for (int i = 0; i < list_weapon.size(); i++)
			list_weapon[i]->Update(dt, coObjects);
	}
	//Cập nhật vị trí cho các đối tượng thành phần như bánh xe, cabin, human...
	//Chạy hàm cập nhật của các đối tượng thành phần
	for (int i = 0; i < componentObjects.size(); i++)
	{
		{
			{
				componentObjects[i]->SetPlayerPosition(x, y);
				componentObjects[i]->Update(dt, coObjects);
			}

		}
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
			animation_set->at(MAIN_CHARACTER_ANI_EXPLOSION)->Render(x, y + 25, alpha);
			if (animation_set->at(MAIN_CHARACTER_ANI_EXPLOSION)->isFinish)
				SetState(MAIN_CHARACTER_STATE_DIE);
		}		
		else
		{
			{
				animation_set->at(0)->Render(x, y, alpha);
				// Vẽ các đối tượng weapon của nhân vật chính
				{
					if (list_weapon.size() > 0)
					{
						for (int i = 0; i < list_weapon.size(); i++)
							list_weapon[i]->Render();
					}
				}
				//Vẽ các object thành phần của player object
				for (int i = 0; i < componentObjects.size(); i++)
				{
					componentObjects[i]->SetUntouchable(untouchable);
					componentObjects[i]->Render();	
					if (dynamic_cast<CVehicle*>(componentObjects[i]))
					{
						//Chuyển sang trạng thái là Human
						if (dynamic_cast<CVehicle*>(componentObjects[i])->GetIsCabinOpened())
							Is_Human = true;
						else
							Is_Human = false;
					}
				}
				//RenderBoundingBox();
			}
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
		if (Is_On_Ground)
		{
			vy = MAIN_CHARACTER_JUMP_SPEED_Y;
			Is_On_Ground = false;
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
		break;
	case MAIN_CHARACTER_STATE_OPEN_CABIN:
		for (int i = 0; i < componentObjects.size(); i++)
		{
			if (dynamic_cast<CVehicle*>(componentObjects[i]))
				dynamic_cast<CVehicle*>(componentObjects[i])->SetState(MAIN_CHARACTER_STATE_OPEN_CABIN);
		}
		break;
	case MAIN_CHARACTER_STATE_CLOSE_CABIN:
		for (int i = 0; i < componentObjects.size(); i++)
		{
			if (dynamic_cast<CVehicle*>(componentObjects[i]))
				dynamic_cast<CVehicle*>(componentObjects[i])->SetState(MAIN_CHARACTER_STATE_CLOSE_CABIN);
		}
		break;
	case MAIN_CHARACTER_STATE_HUMAN:
		break;
	default:
		break;

	}
	//Cập nhật state, hướng di chuyển, tốc độ cho các đối tượng thành phần theo nhân vật chính
	for (int i = 0; i < componentObjects.size(); i++)
	{
		{
			componentObjects[i]->SetState(state);
			componentObjects[i]->SetDirection(nx);
			componentObjects[i]->SetSpeed(vx, vy);
		}

		if (state == MAIN_CHARACTER_STATE_BARREL_FIRE)//Nhân vật bắn
		{
			{
				isStartFire = true;
				if (dynamic_cast<CVehicle*>(componentObjects[i]))
				{
					float x_vehicle_object, y_vehicle_object;
					//Lấy vị trí x, y của đối tượng nòng sóng
					dynamic_cast<CVehicle*>(componentObjects[i])->GetPosition(x_vehicle_object, y_vehicle_object);
					//Nếu nòng sóng đang giơ lên
					if (dynamic_cast<CVehicle*>(componentObjects[i])->GetIsBarrelUp() == true)
					{
						CWeapon* weapon = new CWeapon(x + MAIN_CHARACTER_BBOX_WIDTH / 2, y + 10, nx, state, true);// Khởi tạo weapon theo x,y của barrel
						list_weapon.push_back(weapon);
					}
					else
					{
						CWeapon* weapon = new CWeapon(x, y, nx, state, false);// Khởi tạo weapon theo x,y của barrel
						list_weapon.push_back(weapon);
					}
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
		else if (state == MAIN_CHARACTER_STATE_UP_BARREL)
		{
			isBeingUpBarrel = true;
		}
		else
		{
			isStartFire = false;
			isBeingUpBarrel = false;
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
	Is_Human = false;
	for (int i = 0; i < componentObjects.size(); i++)
	{
		if (dynamic_cast<CVehicle*>(componentObjects[i]))
			dynamic_cast<CVehicle*>(componentObjects[i])->SetState(MAIN_CHARACTER_STATE_CLOSE_CABIN);
	}
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
