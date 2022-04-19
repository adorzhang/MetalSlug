#pragma once
#include <vector>
#include <unordered_map>
#include "System.h"

class CSystemManager
{
private:
	std::vector<CSystem*> systems;
public:
	void AddSystem(CSystem* system);
	template <typename T> T* GetSystem();
};

template<typename T>
inline T* CSystemManager::GetSystem()
{
	for (auto system : systems)
	{
		T* result = dynamic_cast<T*>(system);
		if (result) return result;
	}
	return nullptr;
}