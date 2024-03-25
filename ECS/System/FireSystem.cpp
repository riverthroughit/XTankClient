#include "FireSystem.h"
#include "ECS/World.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "TypeConfig.h"
#include <memory>

void FireSystem::Tick(float dt)
{
	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();
	for (Entity entity : mEntities) {
		
		auto& playerComp = mWorld->GetComponent<PlayerComponent>(entity);

		//刷新子弹冷却时间
		auto& bulletSpawnComp = mWorld->GetComponent<BulletSpawnComponent>(playerComp.pawnId);
		if (bulletSpawnComp.reFireTime > 0) {
			--bulletSpawnComp.reFireTime;
			continue;
		}

		auto& cmdComp = mWorld->GetComponent<CommandComponent>(entity);
		
		if (cmdComp.cmd == BUTTON::FIRE) {

			bulletSpawnComp.reFireTime = BULLET_CD;

			//获得该player的tank的位置和速度等
			auto& posComp = mWorld->GetComponent<PosComponent>(playerComp.pawnId);
			auto& speedComp = mWorld->GetComponent<SpeedComponent>(playerComp.pawnId);

			auto args = std::make_shared<ENTITY_SPAWN_ARGS::Bullet>();
			args->ownerId = entity;
			args->pos = posComp.pos;
			args->speedDirec = bulletSpawnComp.spawnDirec;

			entitySpawnComp.entitySpawnMap[ENTITY::BULLET].push_back({ ENTITY::BULLET,args });
		}
	}
}
