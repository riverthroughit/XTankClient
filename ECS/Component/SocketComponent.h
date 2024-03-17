#pragma once

#include "TypeConfig.h"
#include <array>
#include <vector>

struct SocketComponent{

	//�������

	//�����û��ķ�����id
	int localPlayerId{-1};

	//�������
	int playerNum{};

	//�Ƿ�������������
	bool hasNewCmdMsg{};

	//����������������
	PlayersCommand curPlayersCmd{};

	//��;����������·�������
	std::vector<PlayersCommand> chasingCmds;
	//��ǰ׷����֡id
	int chasingFrameId{};

};