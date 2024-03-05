#include "BulletHitSystem.h"
#include "ECS/World.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/Component/AttachComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/BlockComponent.h"
#include "ECS/Component/BulletComponent.h"
#include "Config.h"

void BulletHitSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		auto& collisionComp = mWorld->GetComponent<CollisionComponent>(entity);
		for (Entity hitEntity : collisionComp.hitEntities) {

			//根据被击中的物体是子弹、坦克还是障碍物执行不同逻辑
			//坦克
			if (mWorld->HasComponent<BulletSpawnComponent>(hitEntity)) {
				HitTank(entity, hitEntity);
			}
			else if (mWorld->HasComponent<BlockComponent>(hitEntity)) {
				HitBlock(entity, hitEntity);
			}
			else if (mWorld->HasComponent<BulletComponent>(hitEntity)) {
				HitBullet(entity, hitEntity);
			}
		}
	}
}

void BulletHitSystem::BulletDestroy(Entity bulletId)
{
	auto& attachComp = mWorld->GetComponent<AttachComponent>(bulletId);
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(attachComp.ownerId);
	auto& bulletSpawnComp = mWorld->GetComponent<BulletSpawnComponent>(playerComp.charId);
	bulletSpawnComp.canFire = true;

	mWorld->AddComponent(bulletId, DestroyComponent());
}

void BulletHitSystem::HitBlock(Entity bulletId, Entity blockId)
{

	//销毁
	BulletDestroy(bulletId);
	auto& blockComp = mWorld->GetComponent<BlockComponent>(blockId);

	if (blockComp.blockType == BLOCK::FRAGILE) {
		blockComp.isHit = true;
	}

}

void BulletHitSystem::HitTank(Entity bulletId, Entity tankId)
{

	auto& attachComp = mWorld->GetComponent<AttachComponent>(bulletId);
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(attachComp.ownerId);
	//若是自身的坦克则直接忽略
	if (playerComp.charId == tankId) {
		return;
	}

	//增加自身玩家分数
	++playerComp.score;
	//减少对方玩家生命值
	attachComp = mWorld->GetComponent<AttachComponent>(tankId);
	playerComp = mWorld->GetComponent<PlayerComponent>(attachComp.ownerId);
	--playerComp.hp;
	//设置对方复活时间
	playerComp.respawnTime = RESPAWN_TIME;

	HitBlock(bulletId, tankId);
}

void BulletHitSystem::HitBullet(Entity bulletId1, Entity bulletId2)
{
	//销毁
	BulletDestroy(bulletId1);
	BulletDestroy(bulletId2);
}
