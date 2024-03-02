#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"

class InputSystem :public System {
	
public:

	virtual void Tick(float dt) override;

private:
	//���ð���ӳ��
	void SetButtonToKey(BUTTON::Type btn, int key);

	//���ü�������
	void ResetInputKey();

	//���ռ�������
	void ReceiveInputKey();

	//���÷��������
	void SetDirecButton();

	//ȷ����ǰ֡���µİ���
	void SetPressedButton();

};