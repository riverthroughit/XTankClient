#pragma once

#include "ECS/Types.h"

struct PosComponent;
struct CollisionComponent;
struct UniformGridComponent;


//���¶�Ӧentity������grid
void UpdateGridOfEntity(Entity entity,const PosComponent& posComp, 
	const CollisionComponent& collComp,UniformGridComponent& gridComp);

//��entity�Ӷ�Ӧgrid��ɾ��
void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);