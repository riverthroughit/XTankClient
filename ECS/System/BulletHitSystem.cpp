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

			//���ݱ����е��������ӵ���̹�˻����ϰ���ִ�в�ͬ�߼�
			//̹��
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

	//����
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
	//���������̹����ֱ�Ӻ���
	if (playerComp.charId == tankId) {
		return;
	}

	//����������ҷ���
	++playerComp.score;
	//���ٶԷ��������ֵ
	attachComp = mWorld->GetComponent<AttachComponent>(tankId);
	playerComp = mWorld->GetComponent<PlayerComponent>(attachComp.ownerId);
	--playerComp.hp;
	//���öԷ�����ʱ��
	playerComp.respawnTime = RESPAWN_TIME;

	HitBlock(bulletId, tankId);
}

void BulletHitSystem::HitBullet(Entity bulletId1, Entity bulletId2)
{
	//����
	BulletDestroy(bulletId1);
	BulletDestroy(bulletId2);
}
