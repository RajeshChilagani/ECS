#pragma once
#include "Includes.h"
#include "Storage.h"

#include <chrono>
#include <unordered_map>

namespace eae6320
{
	namespace ECS
	{
		
		class Registry
		{
		public:
			Registry() = default;
			~Registry() = default;

			void RemoveAllComponents(EntityID Id)
			{
				for (auto it = StoragePools.begin(); it!=StoragePools.end(); ++it)
				{
					it->second->RemoveInstance(Id);
				}
			}

			template<typename Component>
			void Add(EntityID Id, const Component& ComponentInstance)
			{
				auto ComponentId = typeid(Component).hash_code();
				if (StoragePools.find(ComponentId) == StoragePools.end())
				{
					InsertPool<Component>();
				}
				auto Pool = static_cast<ComponentStorage<Component>*>(StoragePools[ComponentId]);
				Pool->AddInstance(Id, ComponentInstance);
			}

			template<typename Component, typename... Args>
			void Emplace(EntityID Id, Args&&... MArgs)
			{
				auto ComponentId = typeid(Component).hash_code();
				if (StoragePools.find(ComponentId) == StoragePools.end())
				{
					InsertPool<Component>();
				}
				auto Pool = static_cast<ComponentStorage<Component>*>(StoragePools[ComponentId]);
				Pool->EmplaceInstance(Id,std::forward<Args>(MArgs)...);
			}

			template<typename Component>
			Component* Get(EntityID Id)
			{
				auto ComponentId = typeid(Component).hash_code();
				if (StoragePools.find(ComponentId) == StoragePools.end())
				{
					return nullptr;
				}
				ComponentStorage<Component>* Pool = static_cast<ComponentStorage<Component>*>(StoragePools[ComponentId]);
				return Pool->GetInstance(Id);

			}

			template<typename Component>
			void Remove(EntityID Id)
			{
				auto ComponentId = typeid(Component).hash_code();
				if (StoragePools.find(ComponentId) != StoragePools.end())
				{
					ComponentStorage<Component>* Pool = static_cast<ComponentStorage<Component>*>(StoragePools[ComponentId]);
					Pool->RemoveInstance(Id);
				}
			}

			template<typename Component>
			bool HasComponent(EntityID Id)
			{
				auto ComponentId = typeid(Component).hash_code();
				if (StoragePools.find(ComponentId) == StoragePools.end())
				{
					return false;
				}
				auto Pool = static_cast<ComponentStorage<Component>*>(StoragePools[ComponentId]);
				return Pool->ContainsEntity(Id);
			}

			template<typename Component>
			ComponentStorageIterator<Component> GetComponentStorageIterator()
			{
				auto ComponentId = typeid(Component).hash_code();
				if (StoragePools.find(ComponentId) != StoragePools.end())
				{
					return ComponentStorageIterator<Component>(static_cast<ComponentStorage<Component>*>(StoragePools[ComponentId]));
				}
				return ComponentStorageIterator<Component>(nullptr);
			}

			template<typename Component>
			std::unordered_map<EntityID, Component>& GetInstancesMap()
			{
				auto ComponentId = typeid(Component).hash_code();
				if (StoragePools.find(ComponentId) == StoragePools.end())
				{
					static std::unordered_map<EntityID, Component> V;
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
				StoragePools.insert(std::pair<size_t, Storage*>(Pool->GetStorageObjectID(), Pool));
			}
			std::unordered_map<size_t, Storage*> StoragePools;
		};
	}
}