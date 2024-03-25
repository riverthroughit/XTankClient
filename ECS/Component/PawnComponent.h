#pragma once

#include "ECS/Types.h"

//被玩家直接控制的实体
struct PawnComponent {

	//玩家id
	Entity controllerId{ NULL_ENTITY };

};