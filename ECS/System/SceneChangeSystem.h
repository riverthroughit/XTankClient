#pragma once

#include "ECS/System.h"

class SceneChangeSystem :public System<SceneChangeSystem> {

public:

	//���ɳ��������з���
	virtual void Init() override;
};