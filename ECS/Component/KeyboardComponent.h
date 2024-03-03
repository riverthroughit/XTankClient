#pragma once
#include <unordered_set>
#include <Util/Macro.h>

struct KeyboardComponent {
	SINGLETON(KeyboardComponent);

	std::unordered_set<int> keysDown;
};