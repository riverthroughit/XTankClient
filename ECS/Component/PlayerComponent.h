#pragma once

#include "ECS/Types.h"
#include "TypeConfig.h"

struct PlayerComponent {
	//玩家房间内id
	unsigned int playerId{};
	//操控的entity
	Entity charId{};
	//玩家状态
	PLAYER_STATUS::Type status{};
	//生命值
	int hp{};
	//分数
	unsigned int score{};
	//(操控的entity的)剩余复活时间
	int respawnTime{};
};