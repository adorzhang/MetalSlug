#include "Portal.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id, int type,int next_portal_id)
{
	this->scene_id = scene_id;
	x = l; 
	y = t;
	width = r - l + 1;
	height =-( b - t + 1);
	this->type = type;
	this->next_portal_id = next_portal_id;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y- height;
	r = x + width;
	b = y ;
}