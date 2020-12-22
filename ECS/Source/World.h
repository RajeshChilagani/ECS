#pragma once
#include "Entity.h"
#include "../Components.h"
void TesTRValue(int&& a)
{
	std::cout << a;
}

template<typename ... Args>
void TestFunc(Args&&... MArgs)
{
	int a[] = { 0,0,(TesTRValue(std::forward<Args>(MArgs)),0)... };
}
static void ECSPoolsExample(eae6320::ECS::Registry& R)
{
	//E1
	eae6320::ECS::Entity E1 = eae6320::ECS::CreateEntity(&R);
	E1.AddComponent<TagComponent>(TagComponent("E1B"));
	E1.AddComponent<TransformComponent>(TransformComponent(1));
	//E2
	eae6320::ECS::Entity E2 = eae6320::ECS::CreateEntity(&R);
	E2.AddComponent<TransformComponent>(TransformComponent(3));
	E2.AddComponent<TagComponent>(TagComponent("E2"));

	auto& all = R.GetInstancesMap<TagComponent>();

	auto p = all.begin();
	auto REfp = (*p);
	if (E2.HasComponent<TagComponent>())
	{
		for (auto it = R.GetComponentStorageIterator<TagComponent>(); it.Get() != it.End(); ++it)
		{
			std::cout << it->second.Name << std::endl;;
		}
	}
	R.RemoveAllComponents(E2.GetUEntityID());
	std::cout << "removing all components of E2" << std::endl;
	for (auto it = R.GetComponentStorageIterator<TagComponent>(); it.Get() != it.End(); ++it)
	{
		std::cout << it->second.Name;
	}
}
