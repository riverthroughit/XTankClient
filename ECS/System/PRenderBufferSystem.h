#pragma once

#include "ECS/System.h"


class PRenderBufferSystem :public System<PRenderBufferSystem> {

public:
	virtual void Tick(float dt)override;

private:

	//�����Ϣ����Ⱦ����
	void UpdatePRenderBuffer();
};