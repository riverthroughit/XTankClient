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

	//先复制一下 直接遍历mEntities删除会迭代器失效
	std::vector<Entity> entityToDestroy(mEntities.begin(), mEntities.end());
	for (Entity entity : entityToDestroy) {
		
		//若有碰撞组件 需将其从空间划分中移除
		if (mWorld->HasComponent<CollisionComponent>(entity)) {
			auto& posComp = mWorld->GetComponent<PosComponent>(entity);
			auto& collComp = mWorld->GetComponent<CollisionComponent>(entity);

			RemoveEntityFromGrid(entity, posComp, collComp, gridComp);
		}
		
		
		mWorld->DestroyEntity(entity);
	}
}
