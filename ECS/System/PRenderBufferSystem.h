#pragma once

#include "ECS/System.h"


class PRenderBufferSystem :public System<PRenderBufferSystem> {

public:
	virtual void Tick(float dt)override;

private:

	//������Ⱦ��Ϣ����Ⱦ����
	void UpdatePRenderBuffer(float dt);
};