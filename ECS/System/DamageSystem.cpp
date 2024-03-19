#include "DamageSystem.h"
#include "ECS/World.h"
#include "ECS/Component/HealthComponent.h"
#include "ECS/Component/BeHitComponent.h"
#include "ECS/Component/DestroyComponent.h"

void DamageSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		auto& healthComp = mWorld->GetComponent<HealthComponent>(entity);
		auto& beHitComp = mWorld->GetComponent<BeHitComponent>(entity);
		
		if (healthComp.type != HEALTH::SUPER) {
			healthComp.hp -= beHitComp.damage;
			if (healthComp.hp <= 0) {
				mWorld->AddComponent<DestroyComponent>(entity, DestroyComponent());
			}
		}
	}

	//移除被击中组件
	RemoveEntitiesComponent<BeHitComponent>();

}
