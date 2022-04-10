#pragma once
#include "Animate.h"

class CAnimation : public CComponent
{
	std::unordered_map<std::string, LPANIMATION> animations;

public:
	void Add(std::string id, LPANIMATION anim);
	LPANIMATION Get(std::string id);
	LPANIMATION Clone(std::string id);
	void Clear();
};