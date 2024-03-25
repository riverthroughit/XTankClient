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



//更新对应entity所处的grid
void UpdateGridOfEntity(Entity entity,const PosComponent& posComp, 
	const CollisionComponent& collComp,UniformGridComponent& gridComp);

//将entity从对应grid中删除
void RemoveEntityFromGrid(Entity entity, const PosComponent& posComp,
	const CollisionComponent& collComp, UniformGridComponent& gridComp);

//指令缓冲是否为空
bool IsCmdBufferEmpty(SocketComponent& socketComp);

//当前是否有需要执行的操作
bool HasCurCmd(SocketComponent& socketComp);

//取得当前socketComponent中的命令
PlayersCommand GetSocketCurCmd(SocketComponent& socketComp);

//是否达到预测上限
bool IsReachPredictLimit(RollbackComponent& rollbackComp);

//添加某事件的观察者
template<typename T>
void AddObserver(ObservedComponent& observedComp,Entity observerId) {

	const char* eventType = typeid(T).name();
	observedComp.eventToObserverMap[eventType].insert(observerId);
}

//获取观察者
template<typename T>
std::unordered_set<Entity>& GetObservers(ObservedComponent& observedComp) {

	const char* eventType = typeid(T).name();
	return observedComp.eventToObserverMap[eventType];
}

//删除观察者
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

//是否触发事件
template<typename T>
bool HasEvent(EventComponent& eventComp) {

	const char* eventType = typeid(T).name();
	return eventComp.eventToArgsMap.count(eventType);
}

//返回事件参数 若当前无该事件 则会默认构造一个参数并返回
template<typename T>
T& GetEventArgs(EventComponent& eventComp) {

	const char* eventType = typeid(T).name();
	const auto& [ite, isExit] = eventComp.eventToArgsMap.insert({ eventType ,std::make_any<T>() });
	return *(ite->second._Cast<T>());
}

//清除该事件的参数
template<typename T>
void RemoveEvent(EventComponent& eventComp) {
	
	const char* eventType = typeid(T).name();
	eventComp.eventToArgsMap.erase(eventType);
}

