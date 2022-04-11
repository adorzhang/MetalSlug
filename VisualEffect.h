#pragma once
#include "GameObject.h"
class CVisualEffect : public CGameObject
{
protected:
	const std::string DEFAULT_ANI_NAME = "default-ani-name";
public:
	CVisualEffect();
	void Update(DWORD dt);
	void Render();
};

