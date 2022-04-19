#include "Power.h"

void CPower::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetSystem<CSprites>();
	AddSprite("power-item", sprites->Get("spr-power-item"));
}

CPower::CPower()
{
	InitSprites();
	colliders.at(0)->SetBoxSize(DEFAULT_SIZE);
}

void CPower::GetEffect(CPlayable* picker)
{
	picker->AffectPowerAttribute(1);
}

void CPower::Render()
{
	sprites.at("power-item")->Draw(transform.position, 1, layer_index, 0, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}
