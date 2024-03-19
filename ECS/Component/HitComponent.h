#pragma once
#include <vector>
#include "ECS/Types.h"

struct HitComponent {

	//被击中的实体的id
	std::vector<Entity> beHitEntities;

};