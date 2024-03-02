#pragma once
#include "Config.h"
#include <array>
#include "ECS/Types.h"
#include "Util/Macro.h"
#include <set>

struct UniformGridComponent {

	SINGLETON(UniformGridComponent);

	std::array<std::array<std::set<Entity>, SCENE_SIDE_NUM>, SCENE_SIDE_NUM> mGrids;

};