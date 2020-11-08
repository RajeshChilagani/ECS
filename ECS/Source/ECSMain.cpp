//#include "entt.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <typeinfo>

using Entity = uint32_t;
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

class Storage
{
public:
	std::unordered_map<Entity, size_t> InstanceIndices;
	virtual size_t GetStorageObjectID() = 0;
	virtual ~Storage()=default;
};

template<typename Component>
class ComponentStorage: public Storage
{
public:
	ComponentStorage()
		:StorageComponentId((size_t)typeid(Component).hash_code())
	{}

	~ComponentStorage() {}
	
	bool AddInstance(Entity E, const Component& Instance)
	{
		//@TODO Design on what should be done when an entityId already exists For now just replace
		if (InstanceIndices.find(E) != InstanceIndices.end())
		{
			Instances[InstanceIndices[E]] = Instance;
			return true;
		}
		Instances.push_back(Instance);
		InstanceIndices.insert(std::pair<Entity, size_t>(E, Instances.size() - 1));
		return true;
	}

	Component* GetInstance(Entity E)
	{
		if (InstanceIndices.find(E) != InstanceIndices.end())
		{
			return &Instances[InstanceIndices[E]];
		}
		return nullptr;
	}

	void RemoveInstance(Entity E)
	{
		if (InstanceIndices.find(E) != InstanceIndices.end())
		{
			auto InstanceIndex = InstanceIndices[E];
			Instances.erase(Instances.begin()+InstanceIndex);
		}
	}

	std::vector<Component>& GetAllInstances()
	{
		return Instances;
	}

	size_t GetStorageObjectID() override
	{
		return StorageComponentId;
	}
	size_t StorageComponentId;
	std::vector<Component> Instances;
};

class Registry
{
public:
	Registry() = default;
	~Registry() = default;

	template<typename Component>
	void Add(Entity entity,const Component& T)
	{
		auto ComponentId = typeid(Component).hash_code();
		if (StoragePools.find(ComponentId) == StoragePools.end())
		{
			InsertPool<Component>();
		}
		auto Pool = static_cast<ComponentStorage<Component>*>(StoragePools.find(ComponentId)->second);
		Pool->AddInstance(entity, T);
	}

	template<typename Component>
	Component* Get(Entity entity)
	{
		auto ComponentId = typeid(Component).hash_code();
		if (StoragePools.find(ComponentId) == StoragePools.end())
		{
			return nullptr;
		}
		ComponentStorage<Component>* Pool = static_cast<ComponentStorage<Component>*>(StoragePools.find(ComponentId)->second);
		return Pool->GetInstance(entity);

	}

	template<typename Component>
	void Remove(Entity entity)
	{
		auto ComponentId = typeid(Component).hash_code();
		if (StoragePools.find(ComponentId) == StoragePools.end())
		{
			
		}
		ComponentStorage<Component>* Pool = static_cast<ComponentStorage<Component>*>(StoragePools.find(ComponentId)->second);
		Pool->RemoveInstance(entity);

	}

	template<typename Component>
	std::vector<Component>& GetAll()
	{
		auto ComponentId = typeid(Component).hash_code();
		if (StoragePools.find(ComponentId) == StoragePools.end())
		{
			static std::vector<Component> V;
			return V;
		}
		ComponentStorage<Component>* Pool = static_cast<ComponentStorage<Component>*>(StoragePools.find(ComponentId)->second);
		return Pool->GetAllInstances();
	}

private:
	template<typename Component>
	void InsertPool()
	{
		auto Pool = new ComponentStorage<Component>();
		StoragePools.insert(std::pair<size_t,Storage*>(Pool->GetStorageObjectID(),Pool));
	}
	std::unordered_map<size_t, Storage*> StoragePools;
};
int main()
{
	Registry R;
	R.Add<TagComponent>(1, TagComponent("First"));
	R.Add<TagComponent>(2, TagComponent("FSecond"));
	auto com = R.Get<TagComponent>(1);
	std::cout << com->Name;
	R.Add<TransformComponent>(1, TransformComponent(1));
	auto& all = R.GetAll<TagComponent>();
	R.Remove<TagComponent>(1);
	for (auto a : all)
	{
		std::cout << a.Name;
	}
	
}
