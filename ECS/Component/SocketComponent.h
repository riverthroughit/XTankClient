#pragma once

#include "TypeConfig.h"
#include <array>

struct SocketComponent{

	//�������

	//�����û��ķ�����id
	unsigned int localPlayerId{};

	//�������
	int playerNum{};

	//�Ƿ�������������
	bool hasNewCmdMsg{};

	PlayersCommand curPlayersCmd{};
};