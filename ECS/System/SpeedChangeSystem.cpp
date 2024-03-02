#include "SpeedChangeSystem.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/PlayerComponent.h"
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
		SpeedComponent& tankSpeedComp = mWorld->GetComponent<SpeedComponent>(playerComp.charId);

		switch (cmdComp.cmd)
		{
		case BUTTON::UP:
			tankSpeedComp.direc = Vec2Fixed(FixedPoint(0), FixedPoint(-1));

			break;
		case BUTTON::DOWN:
			tankSpeedComp.direc = Vec2Fixed(FixedPoint(0), FixedPoint(1));

			break;
		case BUTTON::LEFT:
			tankSpeedComp.direc = Vec2Fixed(FixedPoint(-1), FixedPoint(0));

			break;
		case BUTTON::RIGHT:
			tankSpeedComp.direc = Vec2Fixed(FixedPoint(1), FixedPoint(0));

			break;
		default:
			break;
		}
	}
}
