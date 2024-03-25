#include "DamageSystem.h"
#include "ECS/World.h"
#include "ECS/Component/HealthComponent.h"
#include "ECS/Component/EventComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/ECSUtil.h"

void DamageSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		auto& healthComp = mWorld->GetComponent<HealthComponent>(entity);
		auto& eventComp = mWorld->GetComponent<EventComponent>(entity);
		
		if (healthComp.type != HEALTH::SUPER) {

			if (!HasEvent<EVENT::BeHit>(eventComp)) {
				continue;
			}

			const auto& beHitArgs = GetEventArgs<EVENT::BeHit>(eventComp);

			for (int damage : beHitArgs.damages) {
				healthComp.hp -= damage;
			}
			
			if (healthComp.hp <= 0) {
				mWorld->AddComponent<DestroyComponent>(entity, DestroyComponent());
			}

			//移除被击中事件
			RemoveEvent<EVENT::BeHit>(eventComp);
		}
	}

}
