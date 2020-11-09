#pragma once
#include "Registry.h"
namespace eae6320
{
	namespace ECS
	{
		class Entity
		{
		public:
			Entity(Registry* i_Registry)
				:UEntityID(GetUEntityID())
				,EntityRegistry(i_Registry)
			{}
			
			template<typename Component>
			bool HasComponent()
			{
				//AddAssert			
				return EntityRegistry->HasComponent<Component>(UEntityID);
			}

			template<typename Component>
			void AddComponent(const Component& ComponentInstance)
			{
				EntityRegistry->Add<Component>(UEntityID, ComponentInstance);
			}

			template<typename Component>
			Component* GetComponent()
			{
				return EntityRegistry->Get<Component>(UEntityID);
			}

			template<typename Component>
			void RemoveComponent()
			{
				EntityRegistry->Remove<Component>(UEntityID);
			}

		private:
			EntityID UEntityID;
			Registry* EntityRegistry;
		};

		Entity CreateEntity(Registry* i_Registry)
		{
			return Entity(i_Registry);
		}
	}
}
