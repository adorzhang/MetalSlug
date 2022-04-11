#pragma once
#include "Brick.h"
class CBreakableBrick : public CBrick
{
private:
	void InitSprites();
public:
	CBreakableBrick();
	void Update(DWORD dt);
	void Render();
};

