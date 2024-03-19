#include "AttachSystem.h"
#include "ECS/Types.h"
#include "ECS/World.h"
#include "ECS/Component/AttachComponent.h"
#include "ECS/Component/DestroyComponent.h"

void AttachSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		
		//���丽�ӵ�entity�������� ��ownerId�ÿ�
		auto& attachComp = mWorld->GetComponent<AttachComponent>(entity);
		if (mWorld->HasComponent<DestroyComponent>(attachComp.ownerId)) {
			attachComp.ownerId = NULL_ENTITY;
		}
	}
}
