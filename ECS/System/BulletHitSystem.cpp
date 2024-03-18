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
#include <ECS\Component\SpeedComponent.h>

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

	if (!mWorld->HasComponent<DestroyComponent>(bulletId)) {
		mWorld->AddComponent(bulletId, DestroyComponent());
	}
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

	auto& myAttachComp = mWorld->GetComponent<AttachComponent>(bulletId);
	auto& myPlayerComp = mWorld->GetComponent<PlayerComponent>(myAttachComp.ownerId);

	//���������̹����ֱ�Ӻ���
	if (myPlayerComp.charId == tankId) {
		return;
	}

	//�����������״̬
	myPlayerComp.status = PLAYER_STATUS::HIT_OTHER;


	//���öԷ�״̬
	auto& hisAttachComp = mWorld->GetComponent<AttachComponent>(tankId);
	auto& hisPlayerComp = mWorld->GetComponent<PlayerComponent>(hisAttachComp.ownerId);
	hisPlayerComp.status = PLAYER_STATUS::BE_HIT;

	HitBlock(bulletId, tankId);
}

void BulletHitSystem::HitBullet(Entity bulletId1, Entity bulletId2)
{
	//����
	BulletDestroy(bulletId1);
	BulletDestroy(bulletId2);
}
