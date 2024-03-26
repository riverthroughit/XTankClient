#include "CollisionSystem.h"
#include "ECS/Component/UniformGridComponent.h"
#include "ECS/World.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "Event/EventDefine.h"
#include "Util/TickUtil.h"
#include <set>
#include "ECS/Util/ECSUtil.h"
#include "ECS/Util/ECSSceneUtil.h"


void CollisionSystem::Tick(float dt)
{

	
	ClearHitEntities();

	UpdateCollision();
}

void CollisionSystem::Init()
{
	UpdateGrids();
}

void CollisionSystem::UpdateGrids()
{
	auto& gridComp = mWorld->GetSingletonComponent<UniformGridComponent>();
	gridComp.mGrids.fill(std::array<std::set<Entity>, SCENE_SIDE_NUM>());

	for (Entity entity : mEntities) {
		auto& posComp = mWorld->GetComponent<PosComponent>(entity);
		auto& collComp = mWorld->GetComponent<CollisionComponent>(entity);
		UpdateGridOfEntity(entity, posComp, collComp, gridComp);
	}
}

void CollisionSystem::ClearHitEntities()
{
	for (const Entity& entity : mEntities) {
		auto& c = mWorld->GetComponent<CollisionComponent>(entity);
		c.hitEntities.clear();
	}
}

void CollisionSystem::UpdateCollision()
{
	auto& gridComp = mWorld->GetSingletonComponent<UniformGridComponent>();
	//����ÿ��grid�е�entity �ж����Ƿ�����Χentity�ཻ
	for (int row = 0; row < SCENE_SIDE_NUM; ++row) {
		for (int col = 0; col < SCENE_SIDE_NUM; ++col) {
			std::set<Entity>& curGrid = gridComp.mGrids[row][col];
			UpdateCollisionInGrid(std::vector<Entity>(curGrid.begin(), curGrid.end()));
		}
	}
}

void CollisionSystem::UpdateCollisionInGrid(const std::vector<Entity>& entities)
{
	for (size_t i = 0; i + 1 < entities.size();++i) {

		PosComponent& p1 = mWorld->GetComponent<PosComponent>(entities[i]);
		CollisionComponent& c1 = mWorld->GetComponent<CollisionComponent>(entities[i]);

		for (size_t t = i + 1; t < entities.size(); ++t) {

			PosComponent& p2 = mWorld->GetComponent<PosComponent>(entities[t]);
			CollisionComponent& c2 = mWorld->GetComponent<CollisionComponent>(entities[t]);

			if (IsCollision(p1, c1, p2, c2)) {

				c1.hitEntities.push_back(entities[t]);
				c2.hitEntities.push_back(entities[i]);
			}
		}
	}
}



