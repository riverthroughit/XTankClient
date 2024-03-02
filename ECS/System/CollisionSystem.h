#pragma once
#include "ECS/System.h"
#include <set>

class PosComponent;
class CollisionComponent;
class UniformGridComponent;

class CollisionSystem :public System{

public:
	virtual void Tick(float dt)override;

private:
	//清除当前碰撞数据
	void ClearHitEntities();

	//均匀格子中entity的相交情况
	void JudgeCollisionInGrids();

	//一个entity与周围所有entity的相交情况
	void JudgeCollisionByEntity(const Entity& entity, int row, int col, UniformGridComponent& gridComp);

};