#pragma once

#include "Util/Macro.h"
#include <map>
#include "TypeConfig.h"


struct CommandComponent {
	
	//�������ʱ���߼�֡id
	unsigned int frameId;
	//��������
	BUTTON::Type cmd{ BUTTON::NONE };
};