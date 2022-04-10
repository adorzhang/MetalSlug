#include "Animation.h"
#include "Utils.h"

void CAnimation::Add(std::string id, LPANIMATION anim)
{
	animations.insert(make_pair(id, anim));
	/*DebugOut(L"[INFO] animation added: %s\n", ToWSTR(id).c_str());*/
}

LPANIMATION CAnimation::Get(std::string id)
{
	return animations.at(id);
}

LPANIMATION CAnimation::Clone(std::string id)
{
	auto clone = new CAnimate(*Get(id));
	return clone;
}

void CAnimation::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION s = x.second;
		delete s;
	}

	animations.clear();
}