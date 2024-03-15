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
		if (!cmdComp.HasSpeedChangeCmd()) {
			continue;
		}

		//获取其控制的实体(即坦克)的速度组件并修改
		PlayerComponent& playerComp = mWorld->GetComponent<PlayerComponent>(entity);

		//先判断该玩家的character是否有速度组件
		if (!mWorld->HasComponent<SpeedComponent>(playerComp.charId)) {
			continue;
		}
		SpeedComponent& speedComp = mWorld->GetComponent<SpeedComponent>(playerComp.charId);
		//炮台组件的方向也应该修改
		auto& bulletSpawnComp = mWorld->GetComponent<BulletSpawnComponent>(playerComp.charId);

		switch (cmdComp.cmd)
		{
		case BUTTON::UP:
			bulletSpawnComp.spawnDirec = speedComp.direc;
			speedComp.accDirec = Vec2Fixed(FixedPoint(0), FixedPoint(-1));
			speedComp.direc.y = std::max(FixedPoint(-1), speedComp.direc.y - speedComp.accSpeed);
			break;

		case BUTTON::DOWN:
			bulletSpawnComp.spawnDirec = speedComp.direc;
			speedComp.accDirec = Vec2Fixed(FixedPoint(0), FixedPoint(1));
			speedComp.direc.y = std::min(FixedPoint(1), speedComp.direc.y + speedComp.accSpeed);
			break;

		case BUTTON::LEFT:
			bulletSpawnComp.spawnDirec = speedComp.direc;
			speedComp.accDirec = Vec2Fixed(FixedPoint(-1), FixedPoint(0));
			speedComp.direc.x = std::max(FixedPoint(-1), speedComp.direc.x - speedComp.accSpeed);
			break;

		case BUTTON::RIGHT:
			bulletSpawnComp.spawnDirec = speedComp.direc;
			speedComp.accDirec = Vec2Fixed(FixedPoint(1), FixedPoint(0));
			speedComp.direc.x = std::min(FixedPoint(1), speedComp.direc.x + speedComp.accSpeed);
			break;

		case BUTTON::NONE:
			//注意此处不改变炮台方向
			speedComp.accDirec = Vec2Fixed(FixedPoint(0), FixedPoint(0));
			if (speedComp.direc.x > FixedPoint(0)) {
				speedComp.direc.x = std::max(FixedPoint(0), speedComp.direc.x - speedComp.accSpeed);
			}
			else {
				speedComp.direc.x = std::min(FixedPoint(0), speedComp.direc.x + speedComp.accSpeed);
			}
			
			if (speedComp.direc.y > FixedPoint(0)) {
				speedComp.direc.y = std::max(FixedPoint(0), speedComp.direc.y - speedComp.accSpeed);
			}
			else {
				speedComp.direc.y = std::min(FixedPoint(0), speedComp.direc.y + speedComp.accSpeed);
			}
			break;

		default:
			break;
		}


	}
}
