#include "ECS/ECSUtil.h"

#include "ECS/Component/PosComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/UniformGridComponent.h"

void UpdateGridOfEntity(Entity entity,const PosComponent& posComp, 
	const CollisionComponent& collComp, UniformGridComponent& gridComp)
{

	Vec2Fixed pos = posComp.pos;
	Vec2Fixed prePos = posComp.prePos;

	LOGIC_SHAPE::Type shape = collComp.shape;

	if (shape == LOGIC_SHAPE::CIRCLE) {
		//根据上下左右四个点改变对应格子
		FixedPoint r = collComp.shapeData.r;

		const static Vec2Fixed direc[4] = {
			{FixedPoint(1),FixedPoint(0)},
			{FixedPoint(-1),FixedPoint(0)},
			{FixedPoint(0),FixedPoint(1)},
			{FixedPoint(0),FixedPoint(-1)},
		};

		for (int i = 0; i < 4; ++i) {
			int x = static_cast<int>((prePos.x + direc[i].x * r) / CUBE_SIDE_LENTH_FIXED);
			int y = static_cast<int>((prePos.y + direc[i].y * r) / CUBE_SIDE_LENTH_FIXED);
			if (x < 0 || y < 0 || x >= SCENE_SIDE_NUM || y >= SCENE_SIDE_NUM) {
				continue;
			}

			gridComp.mGrids[y][x].erase(entity);
		}

		for (int i = 0; i < 4; ++i) {
			int x = static_cast<int>((pos.x + direc[i].x * r) / CUBE_SIDE_LENTH_FIXED);
			int y = static_cast<int>((pos.y + direc[i].y * r) / CUBE_SIDE_LENTH_FIXED);
			if (x < 0 || y < 0 || x >= SCENE_SIDE_NUM || y >= SCENE_SIDE_NUM) {
				continue;
			}

			gridComp.mGrids[y][x].insert(entity);
		}
	}
}

void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp, 
	const CollisionComponent& collComp, UniformGridComponent& gridComp)
{
	Vec2Fixed pos = posComp.pos;

	LOGIC_SHAPE::Type shape = collComp.shape;

	if (shape == LOGIC_SHAPE::CIRCLE) {
		//根据上下左右四个点改变对应格子
		FixedPoint r = collComp.shapeData.r;

		const static Vec2Fixed direc[4] = {
			{FixedPoint(1),FixedPoint(0)},
			{FixedPoint(-1),FixedPoint(0)},
			{FixedPoint(0),FixedPoint(1)},
			{FixedPoint(0),FixedPoint(-1)},
		};

		for (int i = 0; i < 4; ++i) {
			int x = static_cast<int>((pos.x + direc[i].x * r) / CUBE_SIDE_LENTH_FIXED);
			int y = static_cast<int>((pos.y + direc[i].y * r) / CUBE_SIDE_LENTH_FIXED);
			if (x < 0 || y < 0 || x >= SCENE_SIDE_NUM || y >= SCENE_SIDE_NUM) {
				continue;
			}

			gridComp.mGrids[y][x].erase(entity);
		}
	}
}
