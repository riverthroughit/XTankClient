#pragma once

#include "ECS/Types.h"
#include "TypeConfig.h"
#include <vector>
#include <utility>

struct PosComponent;
struct CollisionComponent;
struct UniformGridComponent;
class World;

//通过逻辑坐标获取UniformGridComponent中格子的索引
inline std::pair<int, int> GetIndexByLogicPos(Vec2Fixed pos) {
	return { static_cast<int>(pos.x / CUBE_SIDE_LENTH_FIXED),static_cast<int>(pos.y / CUBE_SIDE_LENTH_FIXED) };
}

inline int GetIndexByLogicPosNum(FixedPoint num) {
	return static_cast<int>(num / CUBE_SIDE_LENTH_FIXED);
}

//两个entity是否碰撞
bool IsCollision(PosComponent& p1, CollisionComponent& c1, PosComponent& p2, CollisionComponent& c2);

//更新对应entity所处的grid
void UpdateGridOfEntity(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);

//将entity从对应grid中删除
void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);

//判断碰撞体是否与射线相交
bool IsIntersectionByRay(PosComponent& p, CollisionComponent& c, Vec2Fixed startPos, Vec2Fixed direc);

//射线检测
//按击中先后顺序放入vector并返回
std::vector<Entity> Raycast(Vec2Fixed startPos, DIREC::Type direcType,
	const UniformGridComponent& gridComp, World* world);