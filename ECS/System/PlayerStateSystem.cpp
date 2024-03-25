#include "PlayerStateSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/World.h"
#include "Config.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/PawnComponent.h"
#include "ECS/Component/HealthComponent.h"
#include "ECS/Component/EventComponent.h"
#include "ECS/ECSUtil.h"

void PlayerStateSystem::Tick(float dt)
{

	for (Entity player : mEntities) {

		//子弹击中
		BulletHit(player);

		//复活
		Reviving(player);

		//被击中
		HitByOther(player);

		//死亡
		Dead(player);
		
	}
}

void PlayerStateSystem::BulletHit(Entity player)
{
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(player);
	auto& eventComp = mWorld->GetComponent<EventComponent>(player);

	if (HasEvent<EVENT::Hit>(eventComp)) {

		//是否击中对方坦克
		for (Entity beHitEntity : GetEventArgs<EVENT::Hit>(eventComp).beHitEntities) {
			if (mWorld->HasComponent<PawnComponent>(beHitEntity)) {
				++playerComp.score;
			}
		}
	}

	RemoveEvent<EVENT::Hit>(eventComp);

}

void PlayerStateSystem::HitByOther(Entity player)
{
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(player);

	if (mWorld->HasComponent<DestroyComponent>(playerComp.pawnId)) {
		--playerComp.hp;
		playerComp.respawnTime = RESPAWN_TIME;
		playerComp.pawnId = NULL_ENTITY;
		mWorld->RemoveComponent<CommandComponent>(player);
	}
}

void PlayerStateSystem::Dead(Entity player)
{
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(player);
	if (playerComp.hp <= 0) {
		mWorld->AddComponent<DestroyComponent>(player, DestroyComponent());
	}
}

void PlayerStateSystem::Reviving(Entity player)
{
	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(player);
	if (playerComp.respawnTime > 0) {
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
}

