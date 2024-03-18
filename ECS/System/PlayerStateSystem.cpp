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


		switch (playerComp.status)
		{

		case PLAYER_STATUS::LIVE:

			break;

		case PLAYER_STATUS::DEAD:

			break;

		case PLAYER_STATUS::HIT_OTHER:
			
			++playerComp.score;
			playerComp.status = PLAYER_STATUS::LIVE;

			break;

		case PLAYER_STATUS::BE_HIT:

			//复活期间或死亡后不可发送指令
			mWorld->RemoveComponent<CommandComponent>(entity);

			if (--playerComp.hp <= 0) {

				playerComp.status = PLAYER_STATUS::DEAD;
			}
			else {
				playerComp.respawnTime = RESPAWN_TIME;
				playerComp.status = PLAYER_STATUS::REVIVING;
			}

			break;

		case PLAYER_STATUS::REVIVING:

			if (--playerComp.respawnTime <= 0) {

				mWorld->AddComponent<CommandComponent>(entity, CommandComponent());

				//生成新坦克
				auto tankArgs = std::make_shared<ENTITY_SPAWN_ARGS::Tank>();
				tankArgs->ownerId = playerComp.playerId;
				tankArgs->pos = PLAYERS_SPAWN_POS_FIXED[playerComp.playerId];
				tankArgs->direc = PLAYERS_SPAWN_DIREC_FIXED[playerComp.playerId];
				entitySpawnComp.entitySpawnMap[ENTITY::TANK].push_back({ ENTITY::TANK, tankArgs });

				playerComp.status = PLAYER_STATUS::LIVE;
			}
			break;

		default:
			break;
		}
		
	}
}
