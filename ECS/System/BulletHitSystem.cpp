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

			//����
			mWorld->AddComponent(entity, DestroyComponent());
			mWorld->AddComponent(hitEntity, DestroyComponent());

			//���ݱ����е��������ӵ���̹�˻����ϰ���ִ�в�ͬ�߼�
			//̹��
			if (mWorld->HasComponent<BulletSpawnComponent>(hitEntity)) {
				hitTank(entity, hitEntity);
			}
		}
	}
}

void BulletHitSystem::hitTank(Entity bulletId, Entity tankId)
{
	//����������ҷ���
	auto& attachComp = mWorld->GetComponent<AttachComponent>(bulletId);
	auto& playerComp = mWorld->GetComponent<PlayerComponent>(attachComp.ownerId);
	++playerComp.score;
	//���ٶԷ��������ֵ
	attachComp = mWorld->GetComponent<AttachComponent>(tankId);
	playerComp = mWorld->GetComponent<PlayerComponent>(attachComp.ownerId);
	--playerComp.hp;
}
