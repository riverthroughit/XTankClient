#pragma once

#include "ECS/System.h"

class PRenderData;

class PRenderBufferSystem :public System {

public:
	virtual void Tick(float dt)override;

private:
	//写入渲染队列
	void WriteToPRenderBuffer(const PRenderData& data);

	//交换、刷新渲染队列
	void SwapPRenderBuffer();

	//添加信息至渲染队列
	void TickPRenderBuffer();
};