#include "ECSPools/Entity.h"

namespace eae6320
{
	namespace ECS
	{
		EntityID GenerateUEntityID()
		{
			std::chrono::time_point Time = std::chrono::system_clock::now();
			EntityID Id = (EntityID)std::chrono::duration_cast<std::chrono::nanoseconds>(Time.time_since_epoch()).count();
			return Id;
		}

		Entity CreateEntity(Registry* i_Registry)
		{
			return Entity(i_Registry);
		}

		//Entity
		Entity::Entity(Registry* i_Registry)
			:UEntityID(GenerateUEntityID())
			, EntityRegistry(i_Registry)
		{}
	}
}