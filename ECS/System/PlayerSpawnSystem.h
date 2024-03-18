#pragma once

#include "ECS/System.h"

class PlayerSpawnSystem :public System<PlayerSpawnSystem> {

public:
	virtual void Init() override;
	virtual void Tick(float dt)override;

private:

	void CreatePlayerEntity(int localId);

	void RemovePlayerEntity(int localId);
};