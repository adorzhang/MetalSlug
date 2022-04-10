#include "AnimationManager.h"
#include "Utils.h"

void CAnimationManager::Add(std::string id, LPANIMATION anim)
{
	animations.insert(make_pair(id, anim));
	/*DebugOut(L"[INFO] animation added: %s\n", ToWSTR(id).c_str());*/
}

LPANIMATION CAnimationManager::Get(std::string id)
{
	return animations.at(id);
}

LPANIMATION CAnimationManager::Clone(std::string id)
{
	auto clone = new CAnimation(*Get(id));
	return clone;
}

void CAnimationManager::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION s = x.second;
		delete s;
	}

	animations.clear();
}