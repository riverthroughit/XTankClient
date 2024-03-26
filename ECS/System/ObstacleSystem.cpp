#include "ObstacleSystem.h"
#include "ECS/Component/ObstacleComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/World.h"
#include "ECS/Util/ECSUtil.h"
#include "TypeConfig.h"

void ObstacleSystem::Tick(float dt)
{
	for (const Entity& entity : mEntities) {
		
		ObstacleComponent& obstacleComp = mWorld->GetComponent<ObstacleComponent>(entity);

		if (OBSTACLE::BLOCK == obstacleComp.obstacleType) {
			BlockTypeSpeedTick(entity);
		}
		else if (OBSTACLE::OVERLAP == obstacleComp.obstacleType) {

		}
	}
}

void ObstacleSystem::BlockTypeSpeedTick(Entity entity)
{
	//�ж���ײ�Ƿ����	
	if (mWorld->HasComponent<SpeedComponent>(entity)) {

		CollisionComponent& collisionComp = mWorld->GetComponent<CollisionComponent>(entity);
		PosComponent& posComp = mWorld->GetComponent<PosComponent>(entity);
		SpeedComponent& speedComp = mWorld->GetComponent<SpeedComponent>(entity);
		PosComponent newPosComp{ posComp.pos + speedComp.direc * speedComp.speed };
		bool isEnd = true;

		for (Entity& hitEntity : collisionComp.hitEntities) {
			
			//�������������ڱ�Զ ����Էֿ�
			//���������ײ ���ٶ���0

			//û���ϰ�������������ΪOVERLAP�򲻲����赲
			if (!mWorld->HasComponent<ObstacleComponent>(hitEntity)) {
				continue;
			}
			auto& hitObstacleComp = mWorld->GetComponent<ObstacleComponent>(hitEntity);
			if (hitObstacleComp.obstacleType == OBSTACLE::OVERLAP) {
				continue;
			}

			PosComponent& hitPosComp = mWorld->GetComponent<PosComponent>(hitEntity);
			
			PosComponent hitNewPosComp;

			if (mWorld->HasComponent<SpeedComponent>(hitEntity)) {
				SpeedComponent& speedComp1 = mWorld->GetComponent<SpeedComponent>(hitEntity);
				hitNewPosComp.pos = hitPosComp.pos + speedComp1.direc * speedComp1.speed;
			}
			else {
				hitNewPosComp = hitPosComp;
			}

			FixedPoint curDist = (newPosComp.pos - hitNewPosComp.pos).squareLenth();	
			FixedPoint preDist = (posComp.pos - hitPosComp.pos).squareLenth();
			//������ ����Ҫ�����ٶ�Ϊ0
			if (curDist < preDist) {
				isEnd = false;
				break;
			}
		}

		if (!isEnd) {
			speedComp.direc = { FixedPoint(0),FixedPoint(0) };
		}
	}
}
