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
			
			//如果两物体在移动后仍碰撞 则将速度置0

			CollisionComponent& collisionComp1 = mWorld->GetComponent<CollisionComponent>(hitEntity);
			PosComponent& posComp1 = mWorld->GetComponent<PosComponent>(hitEntity);
			PosComponent newPosComp1;

			if (mWorld->HasComponent<SpeedComponent>(hitEntity)) {
				SpeedComponent& speedComp1 = mWorld->GetComponent<SpeedComponent>(hitEntity);
				newPosComp1.pos = posComp1.pos + speedComp1.direc * speedComp1.speed;
			}
			else {
				newPosComp1 = posComp1;
			}

			if (isCollision(newPosComp, collisionComp, newPosComp1, collisionComp1)) {
				isEnd = false;
				break;
			}
		}

		if (!isEnd) {
			speedComp.direc = { FixedPoint(0),FixedPoint(0) };
			speedComp.speed = FixedPoint(0);
		}
	}
}
