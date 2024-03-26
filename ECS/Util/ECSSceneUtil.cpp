#include "ECSSceneUtil.h"

#include "ECS/Component/PosComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/UniformGridComponent.h"
#include "Math/GeometricUtil.h"
#include "ECS/World.h"


bool IsCollision(PosComponent& p1, CollisionComponent& c1, PosComponent& p2, CollisionComponent& c2)
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

void UpdateGridOfEntity(Entity entity, const PosComponent& posComp,
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

		//从上一个格子中移除
		for (int i = 0; i < 4; ++i) {

			int x = GetIndexByLogicPosNum(prePos.x + direc[i].x * r);
			int y = GetIndexByLogicPosNum(prePos.y + direc[i].y * r);

			if (x < 0 || y < 0 || x >= SCENE_SIDE_NUM || y >= SCENE_SIDE_NUM) {
				continue;
			}

			gridComp.mGrids[y][x].erase(entity);
		}

		//添加到当前格子
		for (int i = 0; i < 4; ++i) {

			int x = GetIndexByLogicPosNum(pos.x + direc[i].x * r);
			int y = GetIndexByLogicPosNum(pos.y + direc[i].y * r);

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

			int x = GetIndexByLogicPosNum(pos.x + direc[i].x * r);
			int y = GetIndexByLogicPosNum(pos.y + direc[i].y * r);

			if (x < 0 || y < 0 || x >= SCENE_SIDE_NUM || y >= SCENE_SIDE_NUM) {
				continue;
			}

			gridComp.mGrids[y][x].erase(entity);
		}
	}
}

bool IsIntersectionByRay(PosComponent& p, CollisionComponent& c, Vec2Fixed startPos, Vec2Fixed direc)
{
	bool res = false;

	if (c.shape == LOGIC_SHAPE::CIRCLE) {
		//计算圆心到直线的距离
		FixedPoint dist = DistFromPointToRay(p.pos, startPos, direc);
		if (dist < c.shapeData.r) {
			res = true;
		}
	}

	return res;
}


std::vector<Entity> Raycast(Vec2Fixed startPos, DIREC::Type direcType, 
	const UniformGridComponent& gridComp, World* world)
{

	//得出射线方向
	Vec2Fixed direc;
	int rowOffest{}, colOffest{};
	switch (direcType)
	{
	case DIREC::UP:
		direc = Vec2Fixed(FixedPoint(0), FixedPoint(-1));
		colOffest = 0, rowOffest = -1;
		break;
	case DIREC::DOWN:
		direc = Vec2Fixed(FixedPoint(0), FixedPoint(1));
		colOffest = 0, rowOffest = 1;
		break;
	case DIREC::LEFT:
		direc = Vec2Fixed(FixedPoint(-1), FixedPoint(0));
		colOffest = -1, rowOffest = 0;
		break;
	case DIREC::RIGHT:
		direc = Vec2Fixed(FixedPoint(1), FixedPoint(0));
		colOffest = 1, rowOffest = 0;
		break;
	default:
		break;
	}

	//起点在均匀格子中的坐标
	int row = static_cast<int>(startPos.y);
	int col = static_cast<int>(startPos.x);

	std::vector<Entity> hitEntities;

	//逐个格子判断射线是否与其中的entity相交
	while (row > -1 && row < SCENE_SIDE_NUM && col > -1 && col < SCENE_SIDE_NUM) {

		auto& entities = gridComp.mGrids[row][col];

		for (Entity entity : entities) {

			auto& collComp = world->GetComponent<CollisionComponent>(entity);
			auto& posComp = world->GetComponent<PosComponent>(entity);

			if (IsIntersectionByRay(posComp, collComp, startPos, direc)) {
				hitEntities.push_back(entity);
			}
		}

		row += rowOffest;
		col += colOffest;
	}

}