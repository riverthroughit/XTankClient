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

	//����entityλ�ø��¿ռ仮�����ݽṹ
	void UpdateGrids();

	//�����ǰ��ײ����
	void ClearHitEntities();

	//������ײ���
	void UpdateCollision();

	//����һ�������ڲ�����ײ���
	void UpdateCollisionInGrid(const std::vector<Entity>& entities);
	//void UpdateCollisionInGrid2(const std::set<Entity>& entities);

};