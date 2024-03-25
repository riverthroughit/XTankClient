#pragma once

#include "ECS/Types.h"
#include <unordered_map>
#include <vector>
#include <any>
#include "TypeConfig.h"

struct EventComponent {

	//键：事件类型typeId 值：事件参数
	std::unordered_map<const char*, std::any> eventToArgsMap;

};