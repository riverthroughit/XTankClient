#include "PlayerStateSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/World.h"

void PlayerStateSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		auto& playerComp = mWorld->GetComponent<PlayerComponent>(entity);
		
		//玩家是否还有复活机会
		if (playerComp.hp <= 0) {
			mWorld->AddComponent(entity, DestroyComponent());
			continue;
		}
		
		//判断坦克是否被击毁
		if (mWorld->HasComponent<DestroyComponent>(playerComp.charId)) {
			//若被击毁 则直接将其移动到随机空位 并移除标记销毁组件

			mWorld->RemoveComponent<DestroyComponent>(playerComp.charId);
		}
	}
}
