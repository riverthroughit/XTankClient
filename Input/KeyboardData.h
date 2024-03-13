#pragma once
#include <unordered_set>
#include "Util/Macro.h"

class KeyboardData {
	
	SINGLETON(KeyboardData);

private:
	std::unordered_set<int> keysDown;

public:

	bool IsKeyDown(int key) {
		return keysDown.count(key);
	}

	void SetKeyDown(int key, bool isDown) {
		if (isDown) {
			keysDown.insert(key);
		}
		else {
			keysDown.erase(key);
		}
	}

};