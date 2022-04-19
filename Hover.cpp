#include "Hover.h"

void CHover::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetSystem<CSprites>();
	AddSprite("hover-item", sprites->Get("spr-hover-item"));
}

CHover::CHover()
{
	InitSprites();

	dropRate = 50;
	colliders.at(0)->SetBoxSize(DEFAULT_SIZE);
}

void CHover::GetEffect(CPlayable* picker)
{
	DebugOut(L"Player picked a hover item up!!!\n");
}

void CHover::Render()
{
	sprites.at("hover-item")->Draw(transform.position, 1, layer_index, 0, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}
