#pragma once

#include "ECS/Types.h"

struct PlayerComponent {
	//Íæ¼Òid
	unsigned int playerId{};
	//²Ù¿ØµÄentity
	Entity charId{};
};