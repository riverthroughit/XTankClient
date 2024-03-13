#include "InputSystem.h"
#include "ECS/Component/InputComponent.h"
#include "ECS/World.h"
#include "TypeConfig.h"
#include <Windows.h>
#include <algorithm>
#include "Input/KeyboardData.h"

void InputSystem::Tick(float dt)
{

	ResetInputKey();
	ReceiveInputKey();
	SetDirecButton();
	SetPressedButton();

}

void InputSystem::SetButtonToKey(BUTTON::Type btn, int key)
{
	InputComponent& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	inputComp.buttonToKey[btn] = key;
}

void InputSystem::ResetInputKey()
{
	InputComponent& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	inputComp.keysDown.clear();
	inputComp.curBtn = BUTTON::NONE;
}

void InputSystem::ReceiveInputKey()
{
	InputComponent& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	auto& keyboardData = KeyboardData::Instance();

	for (int key : inputComp.buttonToKey) {
		if (keyboardData.IsKeyDown(key)) {
			inputComp.keysDown.insert(key);
		}
		else {
			inputComp.keysDown.erase(key);
		}
	}
}

void InputSystem::SetDirecButton()
{
	InputComponent& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	auto& direcButtons = inputComp.direcButtons;

	//�Ƴ���һ֡�ľ�ֹ��ť
	if (!direcButtons.empty() && direcButtons.back() == BUTTON::IDLE) {
		direcButtons.pop_back();
	}

	auto keySetFunc = [&](BUTTON::Type btn) {
		bool isDown = inputComp.IsButtonDown(btn);
		auto ite = std::find(direcButtons.begin(), direcButtons.end(), btn);
		
		if (isDown && ite == direcButtons.end()) {
			direcButtons.push_back(btn);
		}
		else if(!isDown && ite != direcButtons.end()){
			direcButtons.erase(ite);
		}
	};

	//���µ�ǰ֡�ķ���ť ������һ֡�ſ����з���ť ����Ӿ�ֹ��ť
	bool isPreEmpty = inputComp.direcButtons.empty();
	keySetFunc(BUTTON::UP);
	keySetFunc(BUTTON::DOWN);
	keySetFunc(BUTTON::LEFT);
	keySetFunc(BUTTON::RIGHT);
	bool isCurEmpty = inputComp.direcButtons.empty();
	if (!isPreEmpty && isCurEmpty) {
		inputComp.direcButtons.push_back(BUTTON::IDLE);
	}

}

void InputSystem::SetPressedButton()
{
	InputComponent& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	
	//�������ȼ����
	if (inputComp.IsButtonDown(BUTTON::FIRE)) {
		inputComp.curBtn = BUTTON::FIRE;
	}
	else if (!inputComp.direcButtons.empty()) {
		inputComp.curBtn = inputComp.direcButtons.back();
	}
	else {
		inputComp.curBtn = BUTTON::NONE;
	}
}

