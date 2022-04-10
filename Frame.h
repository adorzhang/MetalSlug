#pragma once
#include "Sprites.h"

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, DWORD time) { this->sprite = sprite; this->time = time; }
	LPSPRITE GetSprite() { return sprite; }
	DWORD GetTime() { return time; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;