#pragma once

#include "ECS/System.h"

class SceneChangeSystem :public System<SceneChangeSystem> {

public:
	virtual void Tick(float dt) override;

	//生成场景内所有方块
	virtual void Init() override;
};