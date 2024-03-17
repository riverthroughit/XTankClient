#include "CollisionSystem.h"
#include "ECS/Component/UniformGridComponent.h"
#include "ECS/World.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "Event/EventDefine.h"
#include "Util/TickUtil.h"
#include <set>
#include <ECS/ECSUtil.h>


void CollisionSystem::Tick(float dt)
{

	TickUtil tickUtil;
	tickUtil.SetTickTime(1000);
	float dts[3]{0};

	tickUtil.Tick();
	//UpdateGrids();
	tickUtil.Tick();
	dts[0] = tickUtil.GetDt();
	
	tickUtil.Tick();
	ClearHitEntities();
	tickUtil.Tick();
	dts[1] = tickUtil.GetDt();

	tickUtil.Tick();
	UpdateCollision();
	tickUtil.Tick();
	dts[2] = tickUtil.GetDt();
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
	//遍历每个grid中的entity 判断其是否与周围entity相交
	for (int row = 0; row < SCENE_SIDE_NUM; ++row) {
		for (int col = 0; col < SCENE_SIDE_NUM; ++col) {
			std::set<Entity>& curGrid = gridComp.mGrids[row][col];
			UpdateCollisionInGrid(std::vector<Entity>(curGrid.begin(), curGrid.end()));
			//UpdateCollisionInGrid2(curGrid);
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

			if (isCollision(p1, c1, p2, c2)) {

				c1.hitEntities.push_back(entities[t]);
				c2.hitEntities.push_back(entities[i]);
			}
		}
	}
}

//void CollisionSystem::UpdateCollisionInGrid2(const std::set<Entity>& entities)
//{
//
//	auto endIte = entities.empty() ? entities.end() : --entities.end();
//
//	for (auto ite = entities.begin(); ite != endIte && ite != entities.end();++ite) {
//
//		PosComponent& p1 = mWorld->GetComponent<PosComponent>(*ite);
//		CollisionComponent& c1 = mWorld->GetComponent<CollisionComponent>(*ite);
//
//		for (auto ite2 = ite; ite2 != entities.end(); ++ite2) {
//			
//			if (ite2 == ite) {
//				++ite2;
//			}
//			
//			PosComponent& p2 = mWorld->GetComponent<PosComponent>(*ite2);
//			CollisionComponent& c2 = mWorld->GetComponent<CollisionComponent>(*ite2);
//
//			if (isCollision(p1, c1, p2, c2)) {
//
//				c1.hitEntities.push_back(*ite2);
//				c2.hitEntities.push_back(*ite);
//			}
//		}
//	}
//}



bool CollisionSystem::isCollision(PosComponent& p1, CollisionComponent& c1, PosComponent& p2, CollisionComponent& c2)
{
	bool res = false;

	if (c1.shape == LOGIC_SHAPE::CIRCLE && c2.shape == LOGIC_SHAPE::CIRCLE) {
		//两圆相交
		Vec2Fixed pp = p1.pos - p2.pos;

		FixedPoint rr = c1.shapeData.r + c2.shapeData.r;

		if (pp * pp < rr * rr) {
			res = true;
		}
	}

	return res;
}


