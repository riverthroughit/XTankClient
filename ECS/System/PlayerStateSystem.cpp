#include "PlayerStateSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/World.h"
#include "Config.h"
#include "ECS/Component/EntitySpawnComponent.h"

void PlayerStateSystem::Tick(float dt)
{

	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();


	for (Entity entity : mEntities) {
		auto& playerComp = mWorld->GetComponent<PlayerComponent>(entity);
		
		//玩家是否还有复活机会
		if (playerComp.hp <= 0) {
			mWorld->RemoveComponent<CommandComponent>(entity);
			continue;
		}

		//复活cd
		if (playerComp.respawnTime > 0) {
			if (0 == --playerComp.respawnTime) {
				//生成新坦克
				auto tankArgs = std::make_shared<ENTITY_SPAWN_ARGS::Tank>();
				tankArgs->ownerId = playerComp.playerId;
				tankArgs->pos = PLAYERS_SPAWN_POS_FIXED[playerComp.playerId];
				tankArgs->direc = PLAYERS_SPAWN_DIREC_FIXED[playerComp.playerId];
				entitySpawnComp.entitySpawnMap[ENTITY::TANK].push_back({ ENTITY::TANK, tankArgs });
			}
		}
		
	}
}
