#pragma once

#include "ECS/Types.h"

struct PosComponent;
struct CollisionComponent;
struct UniformGridComponent;


//更新对应entity所处的grid
void UpdateGridOfEntity(Entity entity,const PosComponent& posComp, 
	const CollisionComponent& collComp,UniformGridComponent& gridComp);

//将entity从对应grid中删除
void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);