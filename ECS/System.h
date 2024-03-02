#pragma once

#include "Types.h"
#include <set>

class World;

class System
{

protected:
	World* mWorld = nullptr;

public:
	std::set<Entity> mEntities;

	virtual void Tick(float dt) {};
	virtual void Init() {};

	void SetWorld(World* world) { mWorld = world; };
	World* GetWorld() { return mWorld; };
};

