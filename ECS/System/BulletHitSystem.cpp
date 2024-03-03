#include "BulletHitSystem.h"
#include "ECS/World.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/Component/AttachComponent.h"
#include "ECS/Component/PlayerComponent.h"

void BulletHitSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		auto& collisionComp = mWorld->GetComponent<CollisionComponent>(entity);
		for (Entity hitEntity : collisionComp.hitEntities) {

			//销毁
			mWorld->AddComponent(entity, DestroyComponent());
			mWorld->AddComponent(hitEntity, DestroyComponent());

			//根据被击中的物体是子弹、坦克还是障碍物执行不同逻辑
			//坦克
			if (mWorld->HasComponent<BulletSpawnComponent>(hitEntity)) {
				hitTank(entity, hitEntity);
			}
		}
	}
}

void BulletHitSystem::hitTank(Entity bulletId, Entity tankId)
{
	//增加自身玩家分数
	auto& attachComp = mWorld->GetComponent<AttachComponent>(bulletId);
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(attachComp.ownerId);
	++playerComp.score;
	//减少对方玩家生命值
	attachComp = mWorld->GetComponent<AttachComponent>(tankId);
	playerComp = mWorld->GetComponent<PlayerComponent>(attachComp.ownerId);
	--playerComp.hp;
}
