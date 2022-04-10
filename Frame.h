#pragma once
#include "Sprites.h"

class CFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CFrame(LPSPRITE sprite, DWORD time) { this->sprite = sprite; this->time = time; }
	LPSPRITE GetSprite() { return sprite; }
	DWORD GetTime() { return time; }
};

typedef CFrame* LPFRAME;