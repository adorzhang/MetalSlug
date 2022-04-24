#include "Stair.h"

CStair::CStair(float l, float t, float r, float b)
{
	x = l;

	y = t;

	width = r - l + 1;

	height = -(b - t + 1);
}

void CStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y - height;
	r = x + width;
	b = y;
}

void CStair::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();

}

