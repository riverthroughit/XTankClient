#include "MoveSystem.h"
#include "ECS/World.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/UniformGridComponent.h"

void MoveSystem::Tick(float dt)
{

	UniformGridComponent& gridComp = mWorld->GetSingletonComponent<UniformGridComponent>();

	for (const Entity& entity : mEntities) {
		PosComponent& posComp = mWorld->GetComponent<PosComponent>(entity);
		SpeedComponent& speedComp = mWorld->GetComponent<SpeedComponent>(entity);

		//更新速度
		posComp.prePos = posComp.pos;
		posComp.pos += speedComp.direc * speedComp.speed;

	}
}
