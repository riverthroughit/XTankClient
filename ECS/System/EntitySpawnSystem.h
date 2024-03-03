#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"
#include <memory>

class EntitySpawnSystem :public System {

public:
	virtual void Tick(float dt) override;

private:
	void TankSpawn(std::shared_ptr<ENTITY_SPAWN_ARGS::Tank> args);
	
	void BulletSpawn(std::shared_ptr<ENTITY_SPAWN_ARGS::Bullet> args);
};