#pragma once
#include "ECS/System.h"
#include <vector>

class PosComponent;
class CollisionComponent;
class UniformGridComponent;

class CollisionSystem :public System<CollisionSystem>{

public:
	virtual void Tick(float dt)override;
	virtual void Init()override;

private:

	//根据entity位置更新空间划分数据结构
	void UpdateGrids();

	//清除当前碰撞数据
	void ClearHitEntities();

	//更新碰撞情况
	void UpdateCollision();

	//更新一个格子内部的碰撞情况
	void UpdateCollisionInGrid(const std::vector<Entity>& entities);
	//void UpdateCollisionInGrid2(const std::set<Entity>& entities);

};