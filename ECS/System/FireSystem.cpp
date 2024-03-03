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
		auto& cmdComp = mWorld->GetComponent<CommandComponent>(entity);
		if (cmdComp.cmd == BUTTON::FIRE) {

			auto& playerComp = mWorld->GetComponent<PlayerComponent>(entity);

			//判断当前能否发射
			auto& bulletSpawnComp = mWorld->GetComponent<BulletSpawnComponent>(playerComp.charId);
			if (!bulletSpawnComp.canFire) {
				continue;
			}
			bulletSpawnComp.canFire = false;

			//获得该player的tank的位置和速度等
			auto& posComp = mWorld->GetComponent<PosComponent>(playerComp.charId);
			auto& speedComp = mWorld->GetComponent<SpeedComponent>(playerComp.charId);

			auto args = std::make_shared<ENTITY_SPAWN_ARGS::Bullet>();
			args->ownerId = entity;
			args->pos = posComp.pos;
			args->speedDirec = bulletSpawnComp.spawnDirec;

			entitySpawnComp.entitySpawnMap[ENTITY::BULLET].push_back({ ENTITY::BULLET,args });
		}
	}
}
