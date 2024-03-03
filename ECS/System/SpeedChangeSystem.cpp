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
		
		//�Ƿ����ٶ��������
		if (cmdComp.cmd == BUTTON::NONE || cmdComp.cmd == BUTTON::FIRE) {
			continue;
		}

		//��ȡ����Ƶ�ʵ��(��̹��)���ٶ�������޸�
		PlayerComponent& playerComp = mWorld->GetComponent<PlayerComponent>(entity);
		SpeedComponent& speedComp = mWorld->GetComponent<SpeedComponent>(playerComp.charId);
		//��̨����ķ���ҲӦ���޸�
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
			//ע��˴���̨����Ϊ֮ǰ���ٶȷ���
			bulletSpawnComp.spawnDirec = speedComp.direc;
			speedComp.direc = Vec2Fixed(FixedPoint(0), FixedPoint(0));
			break;
		default:
			break;
		}
	}
}
