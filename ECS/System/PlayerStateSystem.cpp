#include "PlayerStateSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/World.h"

void PlayerStateSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		auto& playerComp = mWorld->GetComponent<PlayerComponent>(entity);
		
		//����Ƿ��и������
		if (playerComp.hp <= 0) {
			mWorld->AddComponent(entity, DestroyComponent());
			continue;
		}
		
		//�ж�̹���Ƿ񱻻���
		if (mWorld->HasComponent<DestroyComponent>(playerComp.charId)) {
			//�������� ��ֱ�ӽ����ƶ��������λ ���Ƴ�����������

			mWorld->RemoveComponent<DestroyComponent>(playerComp.charId);
		}
	}
}
