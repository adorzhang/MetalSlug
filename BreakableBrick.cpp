#include "BreakableBrick.h"

void CBreakableBrick::InitSprites()
{
	auto sprites = CGame::GetInstance()->GetSystem<CSprites>();
	AddSprite("Breakable", sprites->Get("spr-breakable-brick"));
}

CBreakableBrick::CBreakableBrick()
{
	InitSprites();
	layer_index = 2;
}

void CBreakableBrick::Update(DWORD dt)
{
}

void CBreakableBrick::Render()
{
	sprites.at("Breakable")->Draw(transform.position, 1, layer_index);
}
