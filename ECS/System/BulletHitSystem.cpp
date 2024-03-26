#include "BulletHitSystem.h"
#include "ECS/World.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/Component/ObservedComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/BulletComponent.h"
#include "ECS/Component/SideComponent.h"
#include "ECS/Component/EventComponent.h"
#include "Config.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Util/ECSUtil.h"

void BulletHitSystem::Tick(float dt)
{
	for (Entity bullet : mEntities) {
		
		auto& bulletColliComp = mWorld->GetComponent<CollisionComponent>(bullet);
		auto& bulletSideComp = mWorld->GetComponent<SideComponent>(bullet);

		std::vector<Entity> beHitEntities;

		for (Entity beHitEntity : bulletColliComp.hitEntities) {
			auto& hitSideComp = mWorld->GetComponent<SideComponent>(beHitEntity);

			//��ͬ��Ӫ�ſɹ���
			if (bulletSideComp.sideId != hitSideComp.sideId) {
				
				//�����е�����
				auto& beHitEventComp = mWorld->GetComponent<EventComponent>(beHitEntity);
				auto& beHitArgs = GetEventArgs<EVENT::BeHit>(beHitEventComp);
				beHitArgs.damages.push_back(BULLET_DAMAGE);
				
				beHitEntities.push_back(beHitEntity);
			}

		}

		//������������ ��֪ͨ����۲���
		if (!beHitEntities.empty()) {

			//����۲���
			auto& observedComp = mWorld->GetComponent<ObservedComponent>(bullet);
			for (Entity hitObserver : GetObservers<EVENT::Hit>(observedComp)) {
				auto& hitEventComp = mWorld->GetComponent<EventComponent>(hitObserver);
				auto& beHitArgs = GetEventArgs<EVENT::Hit>(hitEventComp);
				beHitArgs.beHitEntities.insert(beHitArgs.beHitEntities.end(), beHitEntities.begin(), beHitEntities.end());
			}

			//��������
			auto& beHitEventComp = mWorld->GetComponent<EventComponent>(bullet);
			auto& beHitArgs = GetEventArgs<EVENT::BeHit>(beHitEventComp);
			beHitArgs.damages.push_back(BULLET_DAMAGE);
		}


	}
}

