#pragma once
#include "Item.h"

class CPower : public CItem
{
private:
	const Vector2 DEFAULT_SIZE = Vector2(16.0f, 16.0f);
	void InitSprites();
public:
	CPower();
	void GetEffect(CPlayable* picker);
	void Render();
};

