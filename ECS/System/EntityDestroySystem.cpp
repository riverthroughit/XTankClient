#include "EntityDestroySystem.h"
#include <vector>
#include "ECS/World.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/UniformGridComponent.h"
#include "ECS/ECSUtil.h"

void EntityDestroySystem::Tick(float dt)
{

	auto& gridComp = mWorld->GetSingletonComponent<UniformGridComponent>();

	//�ȸ���һ�� ֱ�ӱ���mEntitiesɾ���������ʧЧ
	std::vector<Entity> entityToDestroy(mEntities.begin(), mEntities.end());
	for (Entity entity : entityToDestroy) {
		
		//������ײ��� �轫��ӿռ仮�����Ƴ�
		if (mWorld->HasComponent<CollisionComponent>(entity)) {
			auto& posComp = mWorld->GetComponent<PosComponent>(entity);
			auto& collComp = mWorld->GetComponent<CollisionComponent>(entity);

			RemoveEntityFromGrid(entity, posComp, collComp, gridComp);
		}
		
		
		mWorld->DestroyEntity(entity);
	}
}
