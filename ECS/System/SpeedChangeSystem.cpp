#include "SpeedChangeSystem.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/World.h"

void SpeedChangeSystem::Tick(float dt)
{

	for (const Entity& entity : mEntities) {

		CommandComponent& cmdComp = mWorld->GetComponent<CommandComponent>(entity);
		
		//�Ƿ����ٶ��������
		if (!cmdComp.HasSpeedChangeCmd()) {
			continue;
		}

		//��ȡ����Ƶ�ʵ��(��̹��)���ٶ�������޸�
		PlayerComponent& playerComp = mWorld->GetComponent<PlayerComponent>(entity);

		//���жϸ���ҵ�character�Ƿ����ٶ����
		if (!mWorld->HasComponent<SpeedComponent>(playerComp.pawnId)) {
			continue;
		}
		SpeedComponent& speedComp = mWorld->GetComponent<SpeedComponent>(playerComp.pawnId);
		//��̨����ķ���ҲӦ���޸�
		auto& bulletSpawnComp = mWorld->GetComponent<BulletSpawnComponent>(playerComp.pawnId);
		auto& posComp = mWorld->GetComponent<PosComponent>(playerComp.pawnId);

		switch (cmdComp.cmd)
		{
		case BUTTON::UP:
			speedComp.accDirec = Vec2Fixed(FixedPoint(0), FixedPoint(-1));
			posComp.preDirec = posComp.direc;
			posComp.direc = speedComp.accDirec;
			bulletSpawnComp.spawnDirec = speedComp.accDirec;
			speedComp.direc.y = std::max(FixedPoint(-1), speedComp.direc.y - speedComp.accSpeed);
			break;

		case BUTTON::DOWN:
			speedComp.accDirec = Vec2Fixed(FixedPoint(0), FixedPoint(1));
			posComp.preDirec = posComp.direc;
			posComp.direc = speedComp.accDirec;
			bulletSpawnComp.spawnDirec = speedComp.accDirec;
			speedComp.direc.y = std::min(FixedPoint(1), speedComp.direc.y + speedComp.accSpeed);
			break;

		case BUTTON::LEFT:
			speedComp.accDirec = Vec2Fixed(FixedPoint(-1), FixedPoint(0));
			posComp.preDirec = posComp.direc;
			posComp.direc = speedComp.accDirec;
			bulletSpawnComp.spawnDirec = speedComp.accDirec;
			speedComp.direc.x = std::max(FixedPoint(-1), speedComp.direc.x - speedComp.accSpeed);
			break;

		case BUTTON::RIGHT:
			speedComp.accDirec = Vec2Fixed(FixedPoint(1), FixedPoint(0));
			posComp.preDirec = posComp.direc;
			posComp.direc = speedComp.accDirec;
			bulletSpawnComp.spawnDirec = speedComp.accDirec;
			speedComp.direc.x = std::min(FixedPoint(1), speedComp.direc.x + speedComp.accSpeed);
			break;

		case BUTTON::NONE:
			//ע��˴����ı���̨����
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
