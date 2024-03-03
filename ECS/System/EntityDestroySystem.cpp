#include "EntityDestroySystem.h"
#include <vector>
#include "ECS/World.h"

void EntityDestroySystem::Tick(float dt)
{
	//先复制一下 直接遍历mEntities删除会迭代器失效
	std::vector<Entity> entityToDestroy(mEntities.begin(), mEntities.end());
	for (Entity entity : entityToDestroy) {
		mWorld->DestroyEntity(entity);
	}
}
