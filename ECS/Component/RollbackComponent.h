#pragma once

#include "ECS/XTankWorld.h"
#include <memory>
#include <utility>
#include <map>
#include <array>
#include "TypeConfig.h"

struct RollbackComponent {

	//����ȷ�ϵ�׼ȷ����Ϸ����
	std::shared_ptr<XTankWorld> latestConfirmedWorld{};
	//����ȷ�ϵ������Ӧ֡��
	unsigned int latestConfirmedFrameId{};

	//��ӦframeIdΪlastConfirmedFrameIdʱ Ԥ�������
	PlayersCommand prePredictedCmd{};

	//��ǰ�Ƿ��з����������
	bool hasPreciseCmd{};

	//��ǰ�����ķ���������
	PlayersCommand curPreciseCmd{};

	//��ǰ֡Ԥ�������
	PlayersCommand curPredictedCmd{};

	//�����û������� {frameId,cmd}
	std::map<int,BUTTON::Type> localPlayerCmdMap;

};