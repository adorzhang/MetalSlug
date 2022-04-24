#include "Lava.h"
#include "Game.h"
#include "MainCharacter.h"
CLava::CLava(float l, float t, float r, float b)
{

	x = l;

	y = t;

	width = r - l + 1;

	height = -(b - t + 1);

	isDisplay = true;
	isEnable = true;
	isCollision = false;
	player = NULL;
}

void CLava::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isEnable)
	{
		float l1, t1, r1, b1, l2, t2, r2, b2;
		if (player)
		{
			player->GetBoundingBox(l1, t1, r1, b1);
			l2 = x;
			t2 = y - height;
			r2 = x + width;
			b2 = y;
			float player_x;
			float player_y;
			player->GetPosition(player_x, player_y);
			if (CGame::GetInstance()->CheckCollision(l1, t1, r1, b1, l2, t2, r2, b2) == true)
			{
				player->SetIsAttacked(true);
			}
		}
		
	}
}
void CLava::Render()
{
	int ani = -1;
	if (isEnable)
	{
		RenderBoundingBox();
	}

}
void CLava::SetState(int state)
{
	CGameObject::SetState(state);
}
void CLava::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isCollision)
	{
		l = x;
		t = y - height;
		r = x + width;
		b = y;
	}

}