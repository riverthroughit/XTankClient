#pragma once
#include "Math/Vec2.h"
#include "ECS/Types.h"
#include "Config.h"
#include <array>

namespace ENTITY {
	enum Type {
		//PlayerComponent CommandComponent
		PLAYER,

		//AttachComp CollisionComp ObstacleComp PosComp PRenderComp BlockComp SpeedComp BulletSpawnComp
		TANK,

		//AttachComp CollisionComp  PosComp PRenderComp SpeedComp
		BULLET,

		//CollisionComp ObstacleComp PosComp PRenderComp BlockComp
		BLOCK,
	};
}

namespace BUTTON {
	enum Type {
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FIRE,
		IDLE,
		CUT_IN,	//中途加入
		MAX,
	};
}

//房间内所有玩家一帧的操作
struct PlayersCommand {

	int frameId{};

	std::array<BUTTON::Type, PLAYER_NUM> commandArray{};

};

inline bool operator == (const PlayersCommand& a, const PlayersCommand& b) {
	return a.frameId == b.frameId && a.commandArray == b.commandArray;
}

inline bool operator != (const PlayersCommand& a, const PlayersCommand& b) {
	return !(a == b);
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
		TANK,
		BLOCK,
		BULLET,
	};
}

namespace OBSTACLE {
	enum Type {
		OVERLAP,
		BLOCK,
	};
}

namespace BLOCK {
	enum Type {
		HARD,
		FRAGILE,
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
		Vec2Fixed direc{};
		BLOCK::Type blockType;
	};
}
