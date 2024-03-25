#pragma once

#include "ECS/Types.h"
#include <unordered_map>
#include <unordered_set>
#include "TypeConfig.h"

struct ObservedComponent {
	
	//键：事件类型typeId 值：观察者id
	std::unordered_map<const char*, std::unordered_set<Entity>> eventToObserverMap;

};