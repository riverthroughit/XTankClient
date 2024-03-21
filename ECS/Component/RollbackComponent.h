#pragma once
#include <memory>
#include <map>
#include <deque>
#include "TypeConfig.h"

class XTankWorld;

struct RollbackComponent {

	//Ԥ�������
	XTankWorld* duplicateWorld{};

	//�Ƿ���Ҫ����Ԥ������磨������Ԥ�����ʱ��
	bool NeedRollback{};

	//Ԥ����������
	std::deque<PlayersCommand> predCmdDeq{};

	//���Ԥ�����
	int maxPredictTimes{};

	//��ǰԤ���֡id
	int curPredictFrameId{-1};

	//��ǰ�����ķ���������
	PlayersCommand preciseCmd{};

};