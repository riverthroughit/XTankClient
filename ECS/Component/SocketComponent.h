#pragma once

#include "TypeConfig.h"
#include <array>
#include <vector>
#include <deque>

struct SocketComponent{

	//�������

	//�����û��ķ�����id
	int localPlayerId{-1};

	//�±��Ӧ����Ƿ����
	std::array<bool, 4> existPlayers{};

	//�Ƿ��е�ǰ��Ҫִ�е�����
	bool hasCurCmd{};

	//����������������
	PlayersCommand curPlayersCmd{};

	//��ǰ���������
	std::deque<PlayersCommand> playersCmdsBuffer;

	//��ǰ�յ��ķ�����֡id
	int curServerFrameId{-1};

	//��һ�η��ͱ��ز���ʱ��֡id
	int lastSendFrameId{ -1 };

	//��������Ƿ�Ϊ ��;���� �����ڵ�һ��׷֡
	bool isCutInChasing{};

};