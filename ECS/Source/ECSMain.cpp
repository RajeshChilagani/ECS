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

template<typename Obj>
class Test
{
public:
	using MyInt = typename Obj::INT;
	MyInt data;
};
class Obj
{
public:
	using INT = int;

};
int main()
{
	eae6320::ECS::Registry R;
	//E1
	eae6320::ECS::Entity E1 = eae6320::ECS::CreateEntity(&R);
	E1.AddComponent<TagComponent>(TagComponent("E1"));
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
