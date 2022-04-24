#include "Arrow.h"
#include "Game.h"
CArrow::CArrow(float l, float t, float r, float b)
{

	x = l;

	y = t;

	width = r - l + 1;

	height = -(b - t + 1);

	isDisplay = true;
	isEnable = true;


}

void CArrow::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{

	}
}
void CArrow::Render()
{
	int ani = -1;
	if (isEnable)
	{
		RenderBoundingBox();
	}

}
void CArrow::SetState(int state)
{
	CGameObject::SetState(state);
}
void CArrow::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isEnable)
	{
		l = x;
		t = y - height;
		r = x + width;
		b = y;
	}

}