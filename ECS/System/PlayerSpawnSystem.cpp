#include "PlayerSpawnSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/World.h"
#include "Config.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/Component/DestroyComponent.h"

void PlayerSpawnSystem::Init()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	for (int id = 0; id < socketComp.playerNum; ++id) {
		CreatePlayerEntity(id);
	}
}

void PlayerSpawnSystem::Tick(float dt)
{
	auto& rollComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& playersCmds = rollComp.preciseCmd.commandArray;

	//判断是否有中途加入的玩家
	for (int i = 0; i < playersCmds.size(); ++i) {
		switch (playersCmds[i])
		{
		case BUTTON::CUT_IN:
			CreatePlayerEntity(i);
			break;

		case BUTTON::EXIT:
			RemovePlayerEntity(i);
			break;

		default:
			break;
		}
	}
}

void PlayerSpawnSystem::CreatePlayerEntity(int localId)
{

	Entity localPlayer = mWorld->CreateEntity();
	PlayerComponent playerComp;
	playerComp.playerId = localId;
	playerComp.status = PLAYER_STATUS::LIVE;
	playerComp.hp = PLAYER_HP;
	playerComp.score = 0;
	playerComp.respawnTime = 0;

	CommandComponent commandComp;
	mWorld->AddComponent(localPlayer, playerComp);
	mWorld->AddComponent(localPlayer, commandComp);

	//tank
	auto tankArgs = std::make_shared<ENTITY_SPAWN_ARGS::Tank>();
	tankArgs->ownerId = localPlayer;
	tankArgs->pos = PLAYERS_SPAWN_POS_FIXED[localId];
	tankArgs->direc = PLAYERS_SPAWN_DIREC_FIXED[localId];

	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();
	entitySpawnComp.entitySpawnMap[ENTITY::TANK].push_back({ ENTITY::TANK, tankArgs });

}

void PlayerSpawnSystem::RemovePlayerEntity(int localId)
{

	for (Entity playerEntity : mEntities) {
		
		auto& playerComp = mWorld->GetComponent<PlayerComponent>(playerEntity);
		
		if (playerComp.playerId == localId) {
			//玩家控制的实体可以直接销毁
			if (playerComp.status == PLAYER_STATUS::LIVE) {

				mWorld->AddComponent<DestroyComponent>(playerComp.charId, DestroyComponent());
			}

			//延迟销毁玩家本身
			mWorld->AddComponent<DestroyComponent>(playerEntity, DestroyComponent{ PLAYER_DESTROY_LATENCY });

			break;
		}
	}

}
