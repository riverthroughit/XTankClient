#include "EntitySpawnSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/World.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/AttachComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/ObstacleComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/PRenderComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "Config.h"

void EntitySpawnSystem::Tick(float dt)
{
	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();
	auto& spawnMap = entitySpawnComp.entitySpawnMap;

	for (auto& [entityType, spawnArgsDeq] : spawnMap) {
		if (entityType == ENTITY::TANK) {
			std::shared_ptr<ENTITY_SPAWN_ARGS::Tank> curArgs;
			for (EntitySpawnArgs& spawnArgs : spawnArgsDeq) {
				curArgs = std::static_pointer_cast<ENTITY_SPAWN_ARGS::Tank>(spawnArgs.args);

				Entity tank = mWorld->CreateEntity();

				//设置玩家组件中的坦克id
				PlayerComponent& playerComp = mWorld->GetComponent<PlayerComponent>(curArgs->ownerId);
				playerComp.charId = tank;

				AttachComponent attachComp{ curArgs->ownerId };
				CollisionComponent collComp{ LOGIC_SHAPE::CIRCLE,{BULLET_RADIUS_FIXED} };
				ObstacleComponent obsComp{ OBSTACLE::BLOCK };
				PosComponent posComp{ curArgs->pos};
				SpeedComponent speedComp{};
				PRenderComponent pRenderComp{ PRENDER_SHAPE::CIRCLE };
				mWorld->AddComponent(tank, attachComp);
				mWorld->AddComponent(tank, collComp);
				mWorld->AddComponent(tank, obsComp);
				mWorld->AddComponent(tank, posComp);
				mWorld->AddComponent(tank, speedComp);
				mWorld->AddComponent(tank, pRenderComp);
			}
		}
		else {

		}
	}

	spawnMap.clear();

}
