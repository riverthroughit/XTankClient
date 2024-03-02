#pragma once
#include <unordered_map>
#include <Util/Macro.h>

struct KeyboardComponent {
	SINGLETON(KeyboardComponent);

	std::unordered_map<int, bool> keysDown;
};