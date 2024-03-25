#pragma once

#include "ECS/Types.h"
#include "TypeConfig.h"
#include "ECS/Component/EventComponent.h"
#include "ECS/Component/ObservedComponent.h"

struct PosComponent;
struct CollisionComponent;
struct UniformGridComponent;
struct SocketComponent;
struct RollbackComponent;



//���¶�Ӧentity������grid
void UpdateGridOfEntity(Entity entity,const PosComponent& posComp, 
	const CollisionComponent& collComp,UniformGridComponent& gridComp);

//��entity�Ӷ�Ӧgrid��ɾ��
void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);

//ָ����Ƿ�Ϊ��
bool IsCmdBufferEmpty(SocketComponent& socketComp);

//��ǰ�Ƿ�����Ҫִ�еĲ���
bool HasCurCmd(SocketComponent& socketComp);

//ȡ�õ�ǰsocketComponent�е�����
PlayersCommand GetSocketCurCmd(SocketComponent& socketComp);

//�Ƿ�ﵽԤ������
bool IsReachPredictLimit(RollbackComponent& rollbackComp);

//���ĳ�¼��Ĺ۲���
template<typename T>
void AddObserver(ObservedComponent& observedComp,Entity observerId) {

	const char* eventType = typeid(T).name();
	observedComp.eventToObserverMap[eventType].insert(observerId);
}

//��ȡ�۲���
template<typename T>
std::unordered_set<Entity>& GetObservers(ObservedComponent& observedComp) {

	const char* eventType = typeid(T).name();
	return observedComp.eventToObserverMap[eventType];
}

//ɾ���۲���
template<typename T>
void RemoveObserver(ObservedComponent& observedComp, Entity observerId = NULL_ENTITY) {

	const char* eventType = typeid(T).name();
	if (observerId == NULL_ENTITY) {
		observedComp.eventToObserverMap.erase(eventType);
	}
	else {
		observedComp.eventToObserverMap[eventType].erase(observerId);
	}
}

inline void RemoveObserver(ObservedComponent& observedComp,const char* eventType,Entity observerId = NULL_ENTITY) {

	if (observerId == NULL_ENTITY) {
		observedComp.eventToObserverMap.erase(eventType);
	}
	else {
		observedComp.eventToObserverMap[eventType].erase(observerId);
	}
}

//�Ƿ񴥷��¼�
template<typename T>
bool HasEvent(EventComponent& eventComp) {

	const char* eventType = typeid(T).name();
	return eventComp.eventToArgsMap.count(eventType);
}

//�����¼����� ����ǰ�޸��¼� ���Ĭ�Ϲ���һ������������
template<typename T>
T& GetEventArgs(EventComponent& eventComp) {

	const char* eventType = typeid(T).name();
	const auto& [ite, isExit] = eventComp.eventToArgsMap.insert({ eventType ,std::make_any<T>() });
	return *(ite->second._Cast<T>());
}

//������¼��Ĳ���
template<typename T>
void RemoveEvent(EventComponent& eventComp) {
	
	const char* eventType = typeid(T).name();
	eventComp.eventToArgsMap.erase(eventType);
}

