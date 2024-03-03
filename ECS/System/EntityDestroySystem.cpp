#include "EntityDestroySystem.h"
#include <vector>
#include "ECS/World.h"

void EntityDestroySystem::Tick(float dt)
{
	//�ȸ���һ�� ֱ�ӱ���mEntitiesɾ���������ʧЧ
	std::vector<Entity> entityToDestroy(mEntities.begin(), mEntities.end());
	for (Entity entity : entityToDestroy) {
		mWorld->DestroyEntity(entity);
	}
}
