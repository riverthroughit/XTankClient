#pragma once

#include "TypeConfig.h"
#include <array>
#include <vector>
#include <deque>

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

	//��ǰ���������
	std::deque<PlayersCommand> playersCmdsBuffer;

	//��ǰ�յ��ķ�����֡id
	int curServerFrameId{-1};

	//��������Ƿ�Ϊ ��;���� �����ڵ�һ��׷֡
	bool isCutInChasing{};

};