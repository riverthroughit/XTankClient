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

	//读写锁 更新索引时需互斥
	std::shared_mutex wrMutex;

	//三缓冲
	std::array<BufferMap, 3> renderBuffer;

	//次新帧 最新帧 正在写入帧 对应的索引
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