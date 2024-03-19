#include "AttachSystem.h"
#include "ECS/Types.h"
#include "ECS/World.h"
#include "ECS/Component/AttachComponent.h"
#include "ECS/Component/DestroyComponent.h"

void AttachSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		
		//若其附加的entity即将销毁 则将ownerId置空
		auto& attachComp = mWorld->GetComponent<AttachComponent>(entity);
		if (mWorld->HasComponent<DestroyComponent>(attachComp.ownerId)) {
			attachComp.ownerId = NULL_ENTITY;
		}
	}
}
