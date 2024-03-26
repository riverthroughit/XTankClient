#include "ObservedSystem.h"
#include "ECS/Types.h"
#include "ECS/World.h"
#include "ECS/Component/ObservedComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Util/ECSUtil.h"

void ObservedSystem::Tick(float dt)
{
	for (Entity observed : mEntities) {
		
		//����۲���entity�������� �򽫸ù۲���ɾ��
		auto& observedComp = mWorld->GetComponent<ObservedComponent>(observed);

		for (auto& [eventType,observers] : observedComp.eventToObserverMap) {
			
			std::vector<Entity> entitiesToDestroy;
			
			for (Entity observer : observers) {
				if (mWorld->HasComponent<DestroyComponent>(observer)) {
					entitiesToDestroy.push_back(observer);
				}
			}

			for (Entity observer : entitiesToDestroy) {
				RemoveObserver(observedComp, eventType, observer);
			}
		}

	}
}
