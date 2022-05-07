#include "Foreground.h"
#include "Game.h"
CForeground::CForeground(float x, float y)
{

	this->x = x;
	this->y = y;

	isDisplay = true;
	isEnable = true;

	SetState(BACKGROUND_ANI_NORMAL);
}

void CForeground::Update(DWORD dt)
{
	if (isEnable)
	{

	}
}
void CForeground::Render()
{
	int ani = -1;
	if (isEnable)
	{
		
		animation_set->at(0)->Render(x*-0.001, y);
		animation_set->at(1)->Render(x+ 1611-235, y-7);
		animation_set->at(2)->Render(x + 1611 - 250, y-135);
		RenderBoundingBox();
	}

}
void CForeground::SetState(int state)
{
	CGameObject::SetState(state);

}
void CForeground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isEnable)
	{
		
	}

}