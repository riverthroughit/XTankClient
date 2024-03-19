#pragma once

#include "Types.h"
#include <set>
#include <memory>
#include <vector>

class World;

class SystemAbstract
{

protected:
	World* mWorld = nullptr;

public:

	virtual ~SystemAbstract() = default;

	std::set<Entity> mEntities;

	virtual void Tick(float dt) {};
	virtual void Init() {};

	void SetWorld(World* world) { mWorld = world; };
	World* GetWorld() { return mWorld; };

	template<typename T>
	void RemoveEntitiesComponent() {
		std::vector<Entity> entities(mEntities.begin(), mEntities.end());
		for (Entity entity : entities) {
			mWorld->RemoveComponent<T>(entity);
		}
	}

	virtual std::unique_ptr<SystemAbstract> GetACopy() const = 0;
};

template<typename T>
class System :public SystemAbstract {

public:

	std::unique_ptr<SystemAbstract> GetACopy() const override {
		return std::make_unique<T>(static_cast<const T&>(*this));
	}

protected:

	System() = default;
	System(const System&) = default;
	System(System&&) = default;

};

