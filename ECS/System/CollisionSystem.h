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
	//�����ǰ��ײ����
	void ClearHitEntities();

	//���ȸ�����entity���ཻ���
	void JudgeCollisionInGrids();

	//һ��entity����Χ����entity���ཻ���
	void JudgeCollisionByEntity(const Entity& entity, int row, int col, UniformGridComponent& gridComp);

};