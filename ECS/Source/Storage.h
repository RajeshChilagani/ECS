#pragma once
#include "Includes.h"

#include <unordered_map>

namespace eae6320
{
	namespace ECS
	{
		template<typename T>
		class ComponentStorageIterator;

		class Storage
		{
		public:
			virtual size_t GetStorageObjectID() = 0;
			virtual ~Storage() = default;
		};

		template<typename Component>
		class ComponentStorage : public Storage
		{
			friend ComponentStorageIterator<Component>;
		public:
			ComponentStorage()
				:StorageComponentId((size_t)typeid(Component).hash_code())
			{}

			~ComponentStorage() {}

			bool AddInstance(EntityID Id, const Component& ComponentInstance)
			{
				//@TODO Design on what should be done when an EntityIDId already exists For now just replace
				if (InstancesMap.find(Id) != InstancesMap.end())
				{
					InstancesMap[Id] = ComponentInstance;
					return true;
				}
				InstancesMap.insert(std::pair<EntityID, Component>(Id, ComponentInstance));
				return true;
			}

			Component* GetInstance(EntityID Id)
			{
				if (InstancesMap.find(Id) != InstancesMap.end())
				{
					return &InstancesMap[Id];
				}
				return nullptr;
			}

			void RemoveInstance(EntityID Id)
			{
				if (InstancesMap.find(Id) != InstancesMap.end())
				{
					InstancesMap.erase(Id);
				}
			}

			std::unordered_map<EntityID, Component>& GetAllInstances()
			{
				return InstancesMap;
			}

			bool ContainsEntity(EntityID Id)
			{
				return InstancesMap.find(Id) != InstancesMap.end() ? true : false;
			}

			size_t GetStorageObjectID() override
			{
				return StorageComponentId;
			}

		private:
			size_t StorageComponentId;
			std::unordered_map<EntityID, Component> InstancesMap;
		};

		template<typename Component>
		class ComponentStorageIterator
		{
			using ComponentMapPtr = typename std::unordered_map<EntityID, Component>::iterator;
		public:
			ComponentStorageIterator(ComponentStorage<Component>* i_DataPtr)
				:m_ComponentPtr(i_DataPtr)
			{
				if (i_DataPtr)
				{
					m_BasePtr = i_DataPtr->InstancesMap.begin();
				}
				else
				{
					m_BasePtr = EmptyMap.begin();
				}
			}

			ComponentMapPtr operator++()
			{
				return ++m_BasePtr;
			}

			ComponentMapPtr operator++(int)
			{
				return m_BasePtr++;
			}

			ComponentMapPtr operator--()
			{
				return --m_BasePtr;
			}

			ComponentMapPtr operator--(int)
			{
				return m_BasePtr--;
			}

			ComponentMapPtr operator->()
			{
				return Get();
			}

			ComponentMapPtr Get()
			{
				return m_BasePtr;
			}

			ComponentMapPtr End()
			{
				if (m_ComponentPtr)
				{
					return m_ComponentPtr->InstancesMap.end();
				}
				return EmptyMap.end();
			}

		private:
			ComponentStorage<Component>* m_ComponentPtr;
			ComponentMapPtr m_BasePtr;
			std::unordered_map<EntityID, Component> EmptyMap;
		};
	}
}