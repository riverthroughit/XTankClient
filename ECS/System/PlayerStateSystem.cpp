#include "PlayerStateSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/World.h"
#include "Config.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/Component/HealthComponent.h"
#include <ECS/Component/HitComponent.h>

void PlayerStateSystem::Tick(float dt)
{

	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();


	for (Entity player : mEntities) {
		
		auto& playerComp = mWorld->GetComponent<PlayerComponent>(player);
		Entity tank = playerComp.charId;

		//子弹击中
		if (IsBulletHit(player)) {
			auto& bulletSpawnComp = mWorld->GetComponent<BulletSpawnComponent>(tank);
			bulletSpawnComp.canFire = true;
			mWorld->RemoveComponent<HitComponent>(player);
		}

		//击中其他坦克
		if (int hitNum = GetHitTankNum(player)) {
			++playerComp.score;	
		}

		//正在复活
		if (IsReviving(player)) {
			if (--playerComp.respawnTime <= 0) {
				
				mWorld->AddComponent<CommandComponent>(player, CommandComponent());
				//生成新坦克
				auto tankArgs = std::make_shared<ENTITY_SPAWN_ARGS::Tank>();
				tankArgs->ownerId = player;
				tankArgs->pos = PLAYERS_SPAWN_POS_FIXED[playerComp.localId];
				tankArgs->direc = PLAYERS_SPAWN_DIREC_FIXED[playerComp.localId];
				entitySpawnComp.entitySpawnMap[ENTITY::TANK].push_back({ ENTITY::TANK, tankArgs });
			}
		}

		//被击中
		if (IsHitByOther(player)) {
			--playerComp.hp;
			playerComp.respawnTime = RESPAWN_TIME;
			playerComp.charId = NULL_ENTITY;
			mWorld->RemoveComponent<CommandComponent>(player);
		}

		//死亡
		if (IsDead(player)) {
			mWorld->AddComponent<DestroyComponent>(player, DestroyComponent());
		}
		
	}
}

bool PlayerStateSystem::IsBulletHit(Entity player)
{
	return mWorld->HasComponent<HitComponent>(player);
}

int PlayerStateSystem::GetHitTankNum(Entity player)
{
	int num{};

	if (IsBulletHit(player)) {
		auto& hitComp = mWorld->GetComponent<HitComponent>(player);

		for (Entity beHitEntity : hitComp.beHitEntities) {
			if (mWorld->HasComponent<BulletSpawnComponent>(beHitEntity)) {
				++num;
			}
		}
	}

	return num;
}

bool PlayerStateSystem::IsHitByOther(Entity player)
{
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(player);
	//auto& chaHealthComp = mWorld->GetComponent<HealthComponent>(playerComp.charId);
	return mWorld->HasComponent<DestroyComponent>(playerComp.charId);
}

bool PlayerStateSystem::IsDead(Entity player)
{
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(player);
	return playerComp.hp <= 0;
}

bool PlayerStateSystem::IsReviving(Entity player)
{
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(player);
	return playerComp.respawnTime > 0;
}

