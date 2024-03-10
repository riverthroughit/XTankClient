#include "PlayerSpawnSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/World.h"
#include "Config.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/EntitySpawnComponent.h"

void PlayerSpawnSystem::Init()
{
	//从socket中取得数据并判断是否需要添加用户
	static bool temp = true;

	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();


	if (temp) {
		unsigned int localPlayerId = 0;

		Entity localPlayer = mWorld->CreateEntity();
		PlayerComponent playerComp;
		playerComp.playerId = localPlayerId;
		playerComp.hp = PLAYER_HP;
		playerComp.score = 0;
		playerComp.respawnTime = 0;

		CommandComponent commandComp;
		mWorld->AddComponent(localPlayer, playerComp);
		mWorld->AddComponent(localPlayer, commandComp);

		//tank
		auto tankArgs = std::make_shared<ENTITY_SPAWN_ARGS::Tank>();
		tankArgs->ownerId = localPlayer;
		tankArgs->pos = PLAYERS_SPAWN_POS_FIXED[localPlayerId];
		tankArgs->direc = PLAYERS_SPAWN_DIREC_FIXED[localPlayerId];
		entitySpawnComp.entitySpawnMap[ENTITY::TANK].push_back({ ENTITY::TANK, tankArgs });

		temp = false;
	}
}

void PlayerSpawnSystem::Tick(float dt)
{

}
