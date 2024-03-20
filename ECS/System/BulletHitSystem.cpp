#include "BulletHitSystem.h"
#include "ECS/World.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/Component/AttachComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/BulletComponent.h"
#include "ECS/Component/HitComponent.h"
#include "ECS/Component/BeHitComponent.h"
#include "ECS/Component/SideComponent.h"
#include "Config.h"
#include <ECS\Component\SpeedComponent.h>

void BulletHitSystem::Tick(float dt)
{
	for (Entity bullet : mEntities) {
		
		auto& bulletColliComp = mWorld->GetComponent<CollisionComponent>(bullet);
		auto& bulletSideComp = mWorld->GetComponent<SideComponent>(bullet);
		HitComponent hitComp;
		
		for (Entity beHitEntity : bulletColliComp.hitEntities) {
			auto& hitSideComp = mWorld->GetComponent<SideComponent>(beHitEntity);

			//不同阵营才可攻击
			if (bulletSideComp.sideId != hitSideComp.sideId) {
				
				//防止重复添加
				if (!mWorld->HasComponent<BeHitComponent>(beHitEntity)) {
					mWorld->AddComponent<BeHitComponent>(beHitEntity, BeHitComponent{ BULLET_DAMAGE });
					hitComp.beHitEntities.push_back(beHitEntity);
				}
			}

		}

		//若击中了物体 则为自身owner添加hitComponent
		if (!hitComp.beHitEntities.empty()) {

			auto& attachComp = mWorld->GetComponent<AttachComponent>(bullet);
			if (attachComp.ownerId != NULL_ENTITY) {
				mWorld->AddComponent<HitComponent>(attachComp.ownerId, hitComp);
			}

			//自身销毁
			mWorld->AddComponent<BeHitComponent>(bullet, BeHitComponent{ BULLET_DAMAGE });
		}
	}
}

