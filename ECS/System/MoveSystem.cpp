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

		//更新速度 并更新空间划分结构
		Vec2<FixedPoint>& pos = posComp.pos;
		int x = static_cast<int>(pos.x / CUBE_SIDE_LENTH_FIXED);
		int y = static_cast<int>(pos.y / CUBE_SIDE_LENTH_FIXED);
		gridComp.mGrids[y][x].erase(entity);

		pos += speedComp.direc * speedComp.speed;
		x = static_cast<int>(pos.x / CUBE_SIDE_LENTH_FIXED);
		y = static_cast<int>(pos.y / CUBE_SIDE_LENTH_FIXED);
		gridComp.mGrids[y][x].insert(entity);
	}
}
