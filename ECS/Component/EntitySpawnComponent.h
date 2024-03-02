#pragma once

#include <memory>
#include "TypeConfig.h"
#include <deque>
#include <unordered_map>
#include "Util/Macro.h"


struct EntitySpawnArgs {
	ENTITY::Type type;
	std::shared_ptr<void> args;
};


struct EntitySpawnComponent {

	SINGLETON(EntitySpawnComponent);


	std::unordered_map<ENTITY::Type, std::deque<EntitySpawnArgs>> entitySpawnMap;
};