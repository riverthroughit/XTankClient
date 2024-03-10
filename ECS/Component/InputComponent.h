#pragma once
#include <array>
#include <vector>
#include <unordered_set>
#include "TypeConfig.h"

struct InputComponent{

	//单例组件

	//键盘按键是否按下
	std::unordered_set<int> keysDown;

	//指令到键盘的映射
	std::array<int, BUTTON::MAX> buttonToKey{ 
		-1,0x01000013 ,0x01000015 ,0x01000012,0x01000014,0x20 }; //上下左右空格

	//按顺序存储已按下的方向键 每次仅输入最近按下的键
	std::vector<BUTTON::Type> direcButtons;

	//当前按下的键
	BUTTON::Type curBtn = BUTTON::NONE;

	bool IsButtonDown(BUTTON::Type btn) {
		return keysDown.count(buttonToKey[btn]);
	}

	bool HasDirecButtonDown() {
		return keysDown.count(buttonToKey[BUTTON::UP])
			|| keysDown.count(buttonToKey[BUTTON::DOWN])
			|| keysDown.count(buttonToKey[BUTTON::LEFT])
			|| keysDown.count(buttonToKey[BUTTON::RIGHT]);
	}

};