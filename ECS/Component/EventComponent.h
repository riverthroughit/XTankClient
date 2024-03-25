#pragma once

#include "ECS/Types.h"
#include <unordered_map>
#include <vector>
#include <any>
#include "TypeConfig.h"

struct EventComponent {

	//�����¼�����typeId ֵ���¼�����
	std::unordered_map<const char*, std::any> eventToArgsMap;

};