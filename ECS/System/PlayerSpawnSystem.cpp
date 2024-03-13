#include "PlayerSpawnSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/World.h"
#include "Config.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/SocketComponent.h"

void PlayerSpawnSystem::Init()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	for (int id = 0; id < socketComp.playerNum; ++id) {
		CreatePlayerEntity(id);
	}
}

void PlayerSpawnSystem::Tick(float dt)
{

}

void PlayerSpawnSystem::CreatePlayerEntity(int id)
{

	Entity localPlayer = mWorld->CreateEntity();
	PlayerComponent playerComp;
	playerComp.playerId = id;
	playerComp.hp = PLAYER_HP;
	playerComp.score = 0;
	playerComp.respawnTime = 0;

	CommandComponent commandComp;
	mWorld->AddComponent(localPlayer, playerComp);
	mWorld->AddComponent(localPlayer, commandComp);

	//tank
	auto tankArgs = std::make_shared<ENTITY_SPAWN_ARGS::Tank>();
	tankArgs->ownerId = localPlayer;
	tankArgs->pos = PLAYERS_SPAWN_POS_FIXED[id];
	tankArgs->direc = PLAYERS_SPAWN_DIREC_FIXED[id];

	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();
	entitySpawnComp.entitySpawnMap[ENTITY::TANK].push_back({ ENTITY::TANK, tankArgs });

}
