#include "GunHub.h"

CGunHub::CGunHub()
{

}

void CGunHub::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CGunHub::Render()
{


	int p = 0;
	int ani = -1;
	if (player != NULL)
	{
		p = player->GetPower();
		ani = GUNHUB_ANI_LEVEL;
	}
	animation_set->at(ani)->isHubObject = true;
	//animation_set->at(GUNHUB_ANI_TEXT)->isHubObject = true;
	animation_set->at(ani)->Render(x, y);
	//animation_set->at(GUNHUB_ANI_TEXT)->Render(x, y + 40);
}
void CGunHub::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

}