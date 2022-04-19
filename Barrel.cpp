#include "Barrel.h"
#include "RossiHorizontalState.h"
#include "RossiUpward45State.h"
#include "RossiUpwardState.h"

void CBarrel::InitSprites()
{
	auto sprite_manager = CGame::GetInstance()->GetSystem<CSprites>();
	AddSprite(SPR_CABIN_00, sprite_manager->Get("spr-rossi-cabin"));
	AddSprite(SPR_CABIN_UPWARD, sprite_manager->Get("spr-rossi-barrel-45"));
	AddSprite(SPR_CABIN_TURN, sprite_manager->Get("spr--rossi-barrel-turn"));
	AddSprite(SPR_CABIN_OPEN, sprite_manager->Get("spr--rossi-barrel-open"));
}

CBarrel::CBarrel(CMarcoRossi* rossi)
{
	parent = rossi;
	InitSprites();
	nx = parent->GetDirection();
	isSwitching = false;
}

CBarrel::~CBarrel()
{
}

void CBarrel::Switching()
{
	isSwitching = true;
	lastTimeSwitching = GetTickCount();
}

void CBarrel::Update(DWORD dt)
{
	if (isSwitching == true)
	{
		DWORD now = GetTickCount();
		if (now - lastTimeSwitching > switchingDuration)
		{
			lastTimeSwitching = now;
			isSwitching = false;
		}
	}
}

void CBarrel::Render()
{
	int colorIndex = parent->GetColorIndex();
	auto directionState = parent->GetDirectionState();
	if (dynamic_cast<CRossiHorizontalState*>(directionState))
	{
		/*if (isSwitching == true)
		{
			sprites.at(SPR_CABIN_OPEN)->Draw(transform.position + parent->GetPosition() + Vector2(0.0f, 4.0f), -nx, layer_index, 0, damagedColor[colorIndex]);
		}
		else
		{
			if (dynamic_cast<CRossiHorizontalState*>(directionState)->IsTurning() == false)
			{
				sprites.at(SPR_CABIN_00)->Draw(transform.position + parent->GetPosition(), -nx, layer_index, 0, damagedColor[colorIndex]);
			}
			else
			{
				sprites.at(SPR_CABIN_TURN)->Draw(transform.position + parent->GetPosition(), -nx, layer_index, 0, damagedColor[colorIndex]);
			}
		}*/
	}
	else
	{
		nx = parent->GetDirection();
		//sprites.at(SPR_CABIN_UPWARD)->Draw(transform.position + parent->GetPosition(), -nx, layer_index, 0, damagedColor[colorIndex]);
	}
}