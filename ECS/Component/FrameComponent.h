#pragma once

#include "Config.h"
#include "Util/TickUtil.h"

struct FrameComponent {

	//�������

	//���ڹ���ͻ����߼�֡id
	TickUtil clientTick{ LOCKSTEP_TICK };

	//���ڹ���������߼�֡id
	int serverFrameId{ -1 };


};