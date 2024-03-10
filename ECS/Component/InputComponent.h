#pragma once
#include <array>
#include <vector>
#include <unordered_set>
#include "TypeConfig.h"

struct InputComponent{

	//�������

	//���̰����Ƿ���
	std::unordered_set<int> keysDown;

	//ָ����̵�ӳ��
	std::array<int, BUTTON::MAX> buttonToKey{ 
		-1,0x01000013 ,0x01000015 ,0x01000012,0x01000014,0x20 }; //�������ҿո�

	//��˳��洢�Ѱ��µķ���� ÿ�ν�����������µļ�
	std::vector<BUTTON::Type> direcButtons;

	//��ǰ���µļ�
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