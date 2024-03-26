#pragma once

#include "ECS/Types.h"
#include "TypeConfig.h"
#include <vector>
#include <utility>

struct PosComponent;
struct CollisionComponent;
struct UniformGridComponent;
class World;

//ͨ���߼������ȡUniformGridComponent�и��ӵ�����
inline std::pair<int, int> GetIndexByLogicPos(Vec2Fixed pos) {
	return { static_cast<int>(pos.x / CUBE_SIDE_LENTH_FIXED),static_cast<int>(pos.y / CUBE_SIDE_LENTH_FIXED) };
}

inline int GetIndexByLogicPosNum(FixedPoint num) {
	return static_cast<int>(num / CUBE_SIDE_LENTH_FIXED);
}

//����entity�Ƿ���ײ
bool IsCollision(PosComponent& p1, CollisionComponent& c1, PosComponent& p2, CollisionComponent& c2);

//���¶�Ӧentity������grid
void UpdateGridOfEntity(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);

//��entity�Ӷ�Ӧgrid��ɾ��
void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);

//�ж���ײ���Ƿ��������ཻ
bool IsIntersectionByRay(PosComponent& p, CollisionComponent& c, Vec2Fixed startPos, Vec2Fixed direc);

//���߼��
//�������Ⱥ�˳�����vector������
std::vector<Entity> Raycast(Vec2Fixed startPos, DIREC::Type direcType,
	const UniformGridComponent& gridComp, World* world);