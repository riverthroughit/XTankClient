#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"

class InputSystem :public System {
	
public:

	virtual void Tick(float dt) override;

private:
	//设置按键映射
	void SetButtonToKey(BUTTON::Type btn, int key);

	//重置键盘输入
	void ResetInputKey();

	//接收键盘输入
	void ReceiveInputKey();

	//设置方向键次序
	void SetDirecButton();

	//确定当前帧按下的按键
	void SetPressedButton();

};