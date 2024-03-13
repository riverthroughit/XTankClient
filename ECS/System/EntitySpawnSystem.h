#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"
#include <memory>

class EntitySpawnSystem :public System<EntitySpawnSystem> {

public:
	virtual void Tick(float dt) override;

private:
	void BlockSpawn(std::shared_ptr<ENTITY_SPAWN_ARGS::Block> args);

	void TankSpawn(std::shared_ptr<ENTITY_SPAWN_ARGS::Tank> args);
	
	void BulletSpawn(std::shared_ptr<ENTITY_SPAWN_ARGS::Bullet> args);
};