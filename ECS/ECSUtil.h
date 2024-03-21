#pragma once

#include "ECS/Types.h"

struct PosComponent;
struct CollisionComponent;
struct UniformGridComponent;
struct SocketComponent;
struct RollbackComponent;


//���¶�Ӧentity������grid
void UpdateGridOfEntity(Entity entity,const PosComponent& posComp, 
	const CollisionComponent& collComp,UniformGridComponent& gridComp);

//��entity�Ӷ�Ӧgrid��ɾ��
void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);

//ָ����Ƿ�Ϊ��
bool IsCmdBufferEmpty(SocketComponent& socketComp);

//��ǰ�Ƿ�����Ҫִ�еĲ���
bool HasCurCmd(SocketComponent& socketComp);

//�Ƿ�ﵽԤ������
bool IsReachPredictLimit(RollbackComponent& rollbackComp);
