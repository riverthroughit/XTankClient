#pragma once

#include "ECS/System.h"

class SceneChangeSystem :public System<SceneChangeSystem> {

public:
	virtual void Tick(float dt) override;

	//���ɳ��������з���
	virtual void Init() override;
};