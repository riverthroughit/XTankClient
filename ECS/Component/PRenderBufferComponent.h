#pragma once
#include <Util/Macro.h>
#include <array>
#include <unordered_map>
#include "TypeConfig.h"
#include "Math/Vec2.h"
#include <shared_mutex>
#include <mutex>

struct PRenderData {
	unsigned int entityId;
	PRENDER_SHAPE::Type shape;
	Vec2f pos;
	Vec2f direc;

};


class PRenderBufferComponent {

	SINGLETON(PRenderBufferComponent);

	using BufferMap = std::unordered_map<unsigned int,PRenderData>;

	//��д�� ��������ʱ�軥��
	std::shared_mutex wrMutex;

	//������
	std::array<BufferMap, 3> renderBuffer;

	//����֡ ����֡ ����д��֡ ��Ӧ������
	int preIndex = 1;
	int curIndex = 2;
	int writeIndex = 0;

public:

	BufferMap GetCurBuffer() {
		std::shared_lock sharedLock(wrMutex);
		return renderBuffer[curIndex];
	}

	BufferMap GetPreBuffer() {
		std::shared_lock sharedLock(wrMutex);
		return renderBuffer[preIndex];
	}

};