#pragma once
#include "Util/Macro.h"
#include <array>
#include <vector>
#include <unordered_map>
#include "TypeConfig.h"

struct InputComponent{

	SINGLETON(InputComponent);

	//键盘按键是否按下
	std::unordered_map<int,bool> keysDown;

	//指令到键盘的映射
	std::array<int, 5> buttonToKey{ 
		0x01000013 ,0x01000015 ,0x01000012,0x01000014,0x20 }; //上下左右空格

	//按顺序存储已按下的方向键 每次仅输入最近按下的键
	std::vector<BUTTON::Type> direcButtons;

	//当前按下的键
	BUTTON::Type curBtn = BUTTON::NONE;

	bool IsButtonDown(BUTTON::Type btn) {
		return keysDown[buttonToKey[btn]];
	}

	bool HasDirecButtonDown() {
		return keysDown[buttonToKey[BUTTON::UP]]
			|| keysDown[buttonToKey[BUTTON::DOWN]]
			|| keysDown[buttonToKey[BUTTON::LEFT]]
			|| keysDown[buttonToKey[BUTTON::RIGHT]];
	}

};