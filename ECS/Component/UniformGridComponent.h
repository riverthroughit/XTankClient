#pragma once
#include "Config.h"
#include <array>
#include "ECS/Types.h"
#include <set>

struct UniformGridComponent {

	//µ¥Àý×é¼þ

	std::array<std::array<std::set<Entity>, SCENE_SIDE_NUM>, SCENE_SIDE_NUM> mGrids;

};