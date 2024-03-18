#include "EntityDestroySystem.h"
#include <vector>
#include "ECS/World.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/UniformGridComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/ECSUtil.h"

void EntityDestroySystem::Tick(float dt)
{

	auto& gridComp = mWorld->GetSingletonComponent<UniformGridComponent>();

	//直接遍历mEntities删除会迭代器失效
	std::vector<Entity> entityToDestroy;

	for (Entity entity : mEntities) {
		
		auto& detroyComp = mWorld->GetComponent<DestroyComponent>(entity);

		if (--detroyComp.latency <= 0) {

			entityToDestroy.push_back(entity);

			//若有碰撞组件 需将其从空间划分中移除
			if (mWorld->HasComponent<CollisionComponent>(entity)) {
				auto& posComp = mWorld->GetComponent<PosComponent>(entity);
				auto& collComp = mWorld->GetComponent<CollisionComponent>(entity);

				RemoveEntityFromGrid(entity, posComp, collComp, gridComp);
			}
		}
		
	}

	for (Entity entity : entityToDestroy) {
		mWorld->DestroyEntity(entity);
	}

}
