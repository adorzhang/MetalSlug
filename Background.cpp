#include "Background.h"
#include "Game.h"
CBackground::CBackground(float x, float y)
{

	this->x = x;
	this->y = y;

	isDisplay = true;
	isEnable = true;

	SetState(BACKGROUND_ANI_NORMAL);
}

void CBackground::Update(DWORD dt)
{
	if (isEnable)
	{
		
	}
}
void CBackground::Render()
{
	int ani = -1;
	if (isEnable)
	{
		
		animation_set->at(0)->Render(x, y);
		RenderBoundingBox();
	}

}
void CBackground::SetState(int state)
{
	CGameObject::SetState(state);

}
void CBackground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isEnable)
	{
		
	}

}