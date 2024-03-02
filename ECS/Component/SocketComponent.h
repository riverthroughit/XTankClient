#pragma once

#include "Util/Macro.h"

struct SocketComponent {

	SINGLETON(SocketComponent);

	unsigned int localPlayerId{};

};