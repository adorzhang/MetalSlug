#include "HealthBar.h"

void CHealthBar::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetComponent<CSprites>();
	AddSprite("Power-0", sprites->Get("spr-healthbar-0"));
	AddSprite("Power-1", sprites->Get("spr-healthbar-1"));
	AddSprite("Power-2", sprites->Get("spr-healthbar-2"));
	AddSprite("Power-3", sprites->Get("spr-healthbar-3"));
	AddSprite("Power-4", sprites->Get("spr-healthbar-4"));
	AddSprite("Power-5", sprites->Get("spr-healthbar-5"));
	AddSprite("Power-6", sprites->Get("spr-healthbar-6"));
	AddSprite("Power-7", sprites->Get("spr-healthbar-7"));
	AddSprite("Power-8", sprites->Get("spr-healthbar-8"));
}

CHealthBar::CHealthBar()
{
	InitSprites();

	auto game = CGame::GetInstance();

	offset = Vector2(16, -176);
	mainCam = game->GetComponent<CCamera>();
	owner = (CPlayable*)((CPlayScene*)game->GetComponent<CSceneManager>()->GetCurrentScene())->GetPlayer();
}

void CHealthBar::Update(DWORD dt)
{
}

void CHealthBar::Render()
{
	if (owner == nullptr) return;

	auto camPos = mainCam->GetPosition();
	int power = owner->GetPower();
	if (power < 0) power = 0;
	std::string sprID = "Power-" + std::to_string(power);

	sprites.at(sprID)->Draw(camPos + offset, 1, 2);
}
