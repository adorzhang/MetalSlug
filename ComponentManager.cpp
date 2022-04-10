#include "ComponentManager.h"

void CComponentManager::AddComponent(CComponent* component)
{
	components.push_back(component);
}
