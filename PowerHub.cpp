#include "PowerHub.h"

CPowerHub::CPowerHub()
{
	
}

void CPowerHub::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
}

void CPowerHub::Render()
{
	

	int p = 0;
	int ani = -1;
	if (player != NULL)
	{
		p = player->GetPower();
		switch (p)
		{
		case 1:
			ani = 1;
			break;
		case 2:
			ani = 4;
			break;
		case 3:
			ani = 5;
			break;
		case 4:
			ani = 6;
			break;
		case 5:
			ani = 7;
			break;
		case 6:
			ani = 8;
			break;
		case 7:
			ani = 9;
			break;
		case 8:
			ani = 10;
			break;
		case 0:
			ani = 11;
			break;
		default:
			ani = 11;
			break;
		}
	}
	animation_set->at(ani)->isHubObject = true;
	animation_set->at(POWERHUB_ANI_TEXT)->isHubObject = true;
	animation_set->at(ani)->Render(x, y);
	animation_set->at(POWERHUB_ANI_TEXT)->Render(x, y+40);
}
void CPowerHub::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
}