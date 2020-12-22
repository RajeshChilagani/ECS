#include "Entity.h"
#include "../Components.h"
#include "EC/EC.h"
#include "EC/ECComponents.h"
#include <time.h>
#include <iostream>
int main()
{
	eae6320::ECS::Registry R;
	Manager& M = Manager::Get();
	auto Start = std::chrono::steady_clock::now();
	for (int i = 0; i < 100000; ++i)
	{
		auto E = eae6320::ECS::CreateEntity(&R);
		E.EmplaceComponent<TagComponent>("DefaultString");
		E.EmplaceComponent<TransformComponent>(1);
		E.EmplaceComponent<ColliderComponent>(Vector{ 1,2,3 }, Vector{4,5,6});
	}
	for (auto it = R.GetComponentStorageIterator<TagComponent>(); it.Get() != it.End(); ++it)
	{
		it->second.Name = "sdsd";
	}
	auto End = std::chrono::steady_clock::now();
	std::chrono::duration<double> elpased_seconds = End - Start;
	std::cout<<"ECS:"<< elpased_seconds.count() << std::endl;

	// ECS Old
	Start = std::chrono::steady_clock::now();
	for (int i = 0; i < 100000; ++i)
	{
		auto& E = M.AddEntity();
		E->AddComponent<TagCom>(new TagCom());
		E->AddComponent<TranCom>(new TranCom());
		E->AddComponent<TranCom2>(new TranCom2());
		E->AddComponent<TranCom3>(new TranCom3());
		E->AddComponent<TranCom4>(new TranCom4());
		E->AddComponent<TranCom5>(new TranCom5());
		E->AddComponent<TranCom6>(new TranCom6());
		E->AddComponent<TranCom7>(new TranCom7());
		E->AddComponent<TranCom8>(new TranCom8());
		E->AddComponent<TranCom9>(new TranCom9());
	}
	{
		M.updateCom<TagCom>();
	}
	End = std::chrono::steady_clock::now();
	elpased_seconds = End - Start;
	std::cout <<"EC:"<< elpased_seconds.count() << std::endl;
}
