//#include "entt.hpp"

#include "Entity.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <typeinfo>
#include <time.h>



struct TagComponent
{
	std::string Name;
	TagComponent() = default;
	TagComponent(const char* name)
		:Name(name)
	{}
};

struct TransformComponent
{
	float X, Y;
	TransformComponent() = default;
	TransformComponent(float O)
		:X(O)
		, Y(O)
	{}
};

int main()
{
	eae6320::ECS::Registry R;
	//E1
	eae6320::ECS::Entity base = eae6320::ECS::CreateEntity(&R);
	base.AddComponent<TagComponent>(TagComponent("baseEntiy"));
	base.AddComponent<TransformComponent>(TransformComponent(1));
	//E2
	eae6320::ECS::Entity E2 = eae6320::ECS::CreateEntity(&R);
	E2.AddComponent<TransformComponent>(TransformComponent(3));
	E2.AddComponent<TagComponent>(TagComponent("E2"));

	auto& all = R.GetInstancesMap<TagComponent>();
	
	if (E2.HasComponent<TagComponent>())
	{
		for (auto it = R.GetComponentStorageIterator<TagComponent>(); it.Get() != it.End(); ++it)
		{
			it->second.Name = "test";
		}
	}

	for (auto it = R.GetComponentStorageIterator<TagComponent>(); it.Get() != it.End(); ++it)
	{
		std::cout<<it->second.Name;
	}
}
