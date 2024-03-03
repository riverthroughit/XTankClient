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
#include "ECS/Component/BulletSpawnComponent.h"
#include "Config.h"

void EntitySpawnSystem::Tick(float dt)
{
	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();
	auto& spawnMap = entitySpawnComp.entitySpawnMap;

	for (auto& [entityType, spawnArgsDeq] : spawnMap) {
		if (entityType == ENTITY::TANK) {
			std::shared_ptr<ENTITY_SPAWN_ARGS::Tank> args;
			for (EntitySpawnArgs& spawnArgs : spawnArgsDeq) {
				args = std::static_pointer_cast<ENTITY_SPAWN_ARGS::Tank>(spawnArgs.args);
				TankSpawn(args);
			}
		}
		else if(entityType == ENTITY::BULLET){
			std::shared_ptr<ENTITY_SPAWN_ARGS::Bullet> args;
			for (EntitySpawnArgs& spawnArgs : spawnArgsDeq) {
				args = std::static_pointer_cast<ENTITY_SPAWN_ARGS::Bullet>(spawnArgs.args);
				BulletSpawn(args);
			}
		}
		else {

		}
	}

	spawnMap.clear();

}

void EntitySpawnSystem::TankSpawn(std::shared_ptr<ENTITY_SPAWN_ARGS::Tank> args)
{
	Entity tank = mWorld->CreateEntity();

	//设置玩家组件中的坦克id
	PlayerComponent& playerComp = mWorld->GetComponent<PlayerComponent>(args->ownerId);
	playerComp.charId = tank;

	AttachComponent attachComp{ args->ownerId };
	CollisionComponent collComp{ LOGIC_SHAPE::CIRCLE,{TANK_RADIUS_FIXED} };
	ObstacleComponent obsComp{ OBSTACLE::BLOCK };
	PosComponent posComp{ args->pos , args->direc };
	SpeedComponent speedComp{ Vec2Fixed(),TANK_SPEED_FIXED};
	PRenderComponent pRenderComp{ PRENDER_SHAPE::CIRCLE };
	BulletSpawnComponent bulletSpawnComp{ args->direc,true };
	mWorld->AddComponent(tank, attachComp);
	mWorld->AddComponent(tank, collComp);
	mWorld->AddComponent(tank, obsComp);
	mWorld->AddComponent(tank, posComp);
	mWorld->AddComponent(tank, speedComp);
	mWorld->AddComponent(tank, pRenderComp);
	mWorld->AddComponent(tank, bulletSpawnComp);
}

void EntitySpawnSystem::BulletSpawn(std::shared_ptr<ENTITY_SPAWN_ARGS::Bullet> args)
{
	Entity bullet = mWorld->CreateEntity();

	AttachComponent attachComp{ args->ownerId };
	CollisionComponent collComp{ LOGIC_SHAPE::CIRCLE,{BULLET_RADIUS_FIXED} };
	ObstacleComponent obsComp{ OBSTACLE::BLOCK };
	PosComponent posComp{ args->pos , args->speedDirec };
	SpeedComponent speedComp{ args->speedDirec,BULLET_SPEED_FIXED };
	PRenderComponent pRenderComp{ PRENDER_SHAPE::CIRCLE };
	mWorld->AddComponent(bullet, attachComp);
	mWorld->AddComponent(bullet, collComp);
	mWorld->AddComponent(bullet, obsComp);
	mWorld->AddComponent(bullet, posComp);
	mWorld->AddComponent(bullet, speedComp);
	mWorld->AddComponent(bullet, pRenderComp);
}
