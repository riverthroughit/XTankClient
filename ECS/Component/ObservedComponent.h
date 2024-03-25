#pragma once

#include "ECS/Types.h"
#include <unordered_map>
#include <unordered_set>
#include "TypeConfig.h"

struct ObservedComponent {
	
	//�����¼�����typeId ֵ���۲���id
	std::unordered_map<const char*, std::unordered_set<Entity>> eventToObserverMap;

};