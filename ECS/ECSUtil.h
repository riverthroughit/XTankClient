#pragma once

#include "ECS/Types.h"

struct PosComponent;
struct CollisionComponent;
struct UniformGridComponent;
struct SocketComponent;
struct RollbackComponent;


//更新对应entity所处的grid
void UpdateGridOfEntity(Entity entity,const PosComponent& posComp, 
	const CollisionComponent& collComp,UniformGridComponent& gridComp);

//将entity从对应grid中删除
void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);

//当前帧是否有服务器下发操作
bool HasNewCmdMsg(SocketComponent& socketComp);

//是否达到预测上限
bool IsReachPredictLimit(RollbackComponent& rollbackComp);