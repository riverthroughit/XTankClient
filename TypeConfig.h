#pragma once
#include "Math/Vec2.h"
#include "ECS/Types.h"

namespace ENTITY {
	enum Type {
		//PlayerComponent CommandComponent
		PLAYER,
		//AttachComp CollisionComp ObstacleComp PosComp PRenderComp SpeedComp BulletSpawnComp
		TANK,
		//AttachComp CollisionComp ObstacleComp PosComp PRenderComp SpeedComp
		BULLET,
		//CollisionComp ObstacleComp PosComp PRenderComp
		BLOCK,
	};
}

namespace ENTITY_SPAWN_ARGS {

	struct Player {
		unsigned int id{};
	};

	struct Tank {
		Entity ownerId{};
		Vec2Fixed pos{};
		Vec2Fixed direc{};
	};

	struct Bullet {
		Entity ownerId{};
		Vec2Fixed pos{};
		Vec2Fixed speedDirec{};
	};

	struct Block {
		Vec2Fixed pos{};
	};
}


namespace BUTTON {
	enum Type {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FIRE,
		IDLE,
		NONE,
		MAX,
	};
}

namespace LOGIC_SHAPE {
	enum Type
	{
		CIRCLE,
		NONE,
	};
}

namespace PRENDER_SHAPE {
	enum Type {
		CIRCLE,
	};
}

namespace OBSTACLE {
	enum Type {
		OVERLAP,
		BLOCK,
	};
}

