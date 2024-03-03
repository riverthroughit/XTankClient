#include "SpeedChangeSystem.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/World.h"

void SpeedChangeSystem::Tick(float dt)
{
	for (const Entity& entity : mEntities) {

		CommandComponent& cmdComp = mWorld->GetComponent<CommandComponent>(entity);
		
		//是否有速度相关命令
		if (cmdComp.cmd == BUTTON::NONE || cmdComp.cmd == BUTTON::FIRE) {
			continue;
		}

		//获取其控制的实体(即坦克)的速度组件并修改
		PlayerComponent& playerComp = mWorld->GetComponent<PlayerComponent>(entity);
		SpeedComponent& speedComp = mWorld->GetComponent<SpeedComponent>(playerComp.charId);
		//炮台组件的方向也应该修改
		auto& bulletSpawnComp = mWorld->GetComponent<BulletSpawnComponent>(playerComp.charId);

		switch (cmdComp.cmd)
		{
		case BUTTON::UP:
			speedComp.direc = Vec2Fixed(FixedPoint(0), FixedPoint(-1));
			bulletSpawnComp.spawnDirec = speedComp.direc;
			break;
		case BUTTON::DOWN:
			speedComp.direc = Vec2Fixed(FixedPoint(0), FixedPoint(1));
			bulletSpawnComp.spawnDirec = speedComp.direc;
			break;
		case BUTTON::LEFT:
			speedComp.direc = Vec2Fixed(FixedPoint(-1), FixedPoint(0));
			bulletSpawnComp.spawnDirec = speedComp.direc;
			break;
		case BUTTON::RIGHT:
			speedComp.direc = Vec2Fixed(FixedPoint(1), FixedPoint(0));
			bulletSpawnComp.spawnDirec = speedComp.direc;
			break;
		case BUTTON::IDLE:
			//注意此处炮台方向为之前的速度方向
			bulletSpawnComp.spawnDirec = speedComp.direc;
			speedComp.direc = Vec2Fixed(FixedPoint(0), FixedPoint(0));
			break;
		default:
			break;
		}
	}
}
