#pragma once

#include "ECS/Types.h"

struct PlayerComponent {
	//玩家id
	unsigned int playerId{};
	//操控的entity
	Entity charId{};
	//生命值
	int hp{};
	//分数
	unsigned int score{};
	//(操控的entity的)剩余复活时间
	int respawnTime{};
};