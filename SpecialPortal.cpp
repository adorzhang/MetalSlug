#include "SpecialPortal.h"


CSpecialPortal::CSpecialPortal(float l, float t, float r, float b, int scene_id, int next_portal_id)
{
	this->scene_id = scene_id;
	x = l;
	y = t;
	width = r - l + 1;
	height = -(b - t + 1);
	this->next_portal_id = next_portal_id;
}

void CSpecialPortal::Render()
{
	RenderBoundingBox();
}

void CSpecialPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y - height;
	r = x + width;
	b = y;
}