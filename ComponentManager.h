#pragma once
#include <vector>
#include <unordered_map>
#include "Component.h"

class CComponentManager
{
private:
	std::vector<CComponent*> components;
public:
	void AddComponent(CComponent* component);
	template <typename T> T* GetComponent();
};

template<typename T>
inline T* CComponentManager::GetComponent()
{
	for (auto component : components)
	{
		T* result = dynamic_cast<T*>(component);
		if (result) return result;
	}
	return nullptr;
}