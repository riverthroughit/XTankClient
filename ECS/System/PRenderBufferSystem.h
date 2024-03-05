#pragma once

#include "ECS/System.h"

class PRenderData;

class PRenderBufferSystem :public System {

public:
	virtual void Tick(float dt)override;

private:
	//д����Ⱦ����
	void WriteToPRenderBuffer(const PRenderData& data);

	//������ˢ����Ⱦ����
	void SwapPRenderBuffer();

	//���µ�ǰ֡����
	void UpdateFrameId();

	//�����Ϣ����Ⱦ����
	void UpdatePRenderBuffer();
};