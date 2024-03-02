#include "InputSystem.h"
#include "ECS/Component/InputComponent.h"
#include "ECS/World.h"
#include "TypeConfig.h"
#include <Windows.h>
#include <algorithm>
#include "ECS/Component/KeyboardComponent.h"

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
	KeyboardComponent& keyboardComp = mWorld->GetSingletonComponent<KeyboardComponent>();

	for (int key : inputComp.buttonToKey) {
		if (keyboardComp.keysDown.find(key) != keyboardComp.keysDown.end()) {
			inputComp.keysDown[key] = true;
		}
		else {
			inputComp.keysDown[key] = false;
		}
	}
}

void InputSystem::SetDirecButton()
{
	InputComponent& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	auto& direcButtons = inputComp.direcButtons;

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

	keySetFunc(BUTTON::UP);
	keySetFunc(BUTTON::DOWN);
	keySetFunc(BUTTON::LEFT);
	keySetFunc(BUTTON::RIGHT);

}

void InputSystem::SetPressedButton()
{
	InputComponent& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	
	//开火优先级最高
	if (inputComp.IsButtonDown(BUTTON::FIRE)) {
		inputComp.curBtn = BUTTON::FIRE;
	}
	else if(!inputComp.direcButtons.empty()){
		inputComp.curBtn = inputComp.direcButtons.back();
	}
}

