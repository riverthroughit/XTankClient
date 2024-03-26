#include "MoveSystem.h"
#include "ECS/World.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/UniformGridComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Util/ECSUtil.h"
#include "ECS/Util/ECSSceneUtil.h"


void MoveSystem::Tick(float dt)
{

	UniformGridComponent& gridComp = mWorld->GetSingletonComponent<UniformGridComponent>();

	for (const Entity& entity : mEntities) {
		auto& posComp = mWorld->GetComponent<PosComponent>(entity);
		auto& speedComp = mWorld->GetComponent<SpeedComponent>(entity);

		//更新速度
		posComp.prePos = posComp.pos;
		posComp.pos += speedComp.direc * speedComp.speed;

		//更新碰撞位置
		if (mWorld->HasComponent<CollisionComponent>(entity)) {
			auto& collComp = mWorld->GetComponent<CollisionComponent>(entity);
			UpdateGridOfEntity(entity, posComp, collComp, gridComp);
		}

	}
}
