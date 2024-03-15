#pragma once

#include "ECS/System.h"


class PRenderBufferSystem :public System<PRenderBufferSystem> {

public:
	virtual void Tick(float dt)override;

private:

	//更新渲染信息至渲染队列
	void UpdatePRenderBuffer(float dt);
};