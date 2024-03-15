#include "ObstacleSystem.h"
#include "ECS/Component/ObstacleComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/World.h"
#include "ECS/ECSUtil.h"
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
	//判断碰撞是否结束	
	if (mWorld->HasComponent<SpeedComponent>(entity)) {

		CollisionComponent& collisionComp = mWorld->GetComponent<CollisionComponent>(entity);
		PosComponent& posComp = mWorld->GetComponent<PosComponent>(entity);
		SpeedComponent& speedComp = mWorld->GetComponent<SpeedComponent>(entity);
		PosComponent newPosComp{ posComp.pos + speedComp.direc * speedComp.speed };
		bool isEnd = true;

		for (Entity& hitEntity : collisionComp.hitEntities) {
			
			//如果两物体距离在变远 则可以分开
			//否则继续碰撞 将速度置0

			CollisionComponent& collisionComp1 = mWorld->GetComponent<CollisionComponent>(hitEntity);
			PosComponent& posComp1 = mWorld->GetComponent<PosComponent>(hitEntity);
			
			FixedPoint preDist = (posComp.pos - posComp1.pos).squareLenth();
			
			PosComponent newPosComp1;

			if (mWorld->HasComponent<SpeedComponent>(hitEntity)) {
				SpeedComponent& speedComp1 = mWorld->GetComponent<SpeedComponent>(hitEntity);
				newPosComp1.pos = posComp1.pos + speedComp1.direc * speedComp1.speed;
			}
			else {
				newPosComp1 = posComp1;
			}

			FixedPoint curDist = (newPosComp.pos - newPosComp1.pos).squareLenth();
			
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
