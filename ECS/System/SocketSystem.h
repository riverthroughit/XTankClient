#pragma once

#include "ECS/System.h"

class SocketSystem :public System {

public:
	virtual void Tick(float dt)override;

private:
	void ReceiveMsg();
};