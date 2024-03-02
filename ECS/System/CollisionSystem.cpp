#include "CollisionSystem.h"
#include "ECS/Component/UniformGridComponent.h"
#include "ECS/World.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/ECSUtil.h"
#include "Event/EventDefine.h"

void CollisionSystem::Tick(float dt)
{
	
	ClearHitEntities();

	JudgeCollisionInGrids();

}

void CollisionSystem::ClearHitEntities()
{
	for (const Entity& entity : mEntities) {
		CollisionComponent& c = mWorld->GetComponent<CollisionComponent>(entity);
		c.hitEntities.clear();
	}
}


void CollisionSystem::JudgeCollisionInGrids()
{
	UniformGridComponent& gridComp = mWorld->GetSingletonComponent<UniformGridComponent>();
	//遍历每个grid中的entity 判断其是否与周围entity相交
	for (int row = 0; row < SCENE_SIDE_NUM; ++row) {
		for (int col = 0; col < SCENE_SIDE_NUM; ++col) {
			std::set<Entity>& curGrid = gridComp.mGrids[row][col];
			for (auto ite = curGrid.begin(); ite != curGrid.end(); ++ite) {
				JudgeCollisionByEntity(*ite, row, col, gridComp);
			}
		}
	}
}

void CollisionSystem::JudgeCollisionByEntity(const Entity& entity, int row, int col, UniformGridComponent& gridComp)
{
	//auto& collisionEventQueue = mWorld->GetEventQueue<CollisionEventQueue>();

	for (int irow = row - 1; irow < 2; ++irow) {
		if (irow < 0 || irow >= SCENE_SIDE_NUM)continue;
		for (int icol = col - 1; icol < 2; ++icol) {
			if (icol < 0 || icol >= SCENE_SIDE_NUM)continue;

			for (const Entity& otherEntity : gridComp.mGrids[irow][icol]) {
				if (entity == otherEntity)continue;

				PosComponent& p1 = mWorld->GetComponent<PosComponent>(entity);
				PosComponent& p2 = mWorld->GetComponent<PosComponent>(otherEntity);
				CollisionComponent& c1 = mWorld->GetComponent<CollisionComponent>(entity);
				CollisionComponent& c2 = mWorld->GetComponent<CollisionComponent>(otherEntity);

				if (isCollision(p1, c1, p2, c2)) {
					//collisionEventQueue.PushEventArgs(entity, otherEntity);
					c1.hitEntities.push_back(otherEntity);
				}
			}
		}
	}
}



