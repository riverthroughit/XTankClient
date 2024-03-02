#pragma once
#include "Util/Macro.h"
#include <array>
#include <vector>
#include <unordered_map>
#include "TypeConfig.h"

struct InputComponent{

	SINGLETON(InputComponent);

	//���̰����Ƿ���
	std::unordered_map<int,bool> keysDown;

	//ָ����̵�ӳ��
	std::array<int, 5> buttonToKey{ 
		0x01000013 ,0x01000015 ,0x01000012,0x01000014,0x20 }; //�������ҿո�

	//��˳��洢�Ѱ��µķ���� ÿ�ν�����������µļ�
	std::vector<BUTTON::Type> direcButtons;

	//��ǰ���µļ�
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