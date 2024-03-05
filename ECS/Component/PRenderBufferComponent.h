#pragma once
#include <Util/Macro.h>
#include <array>
#include <unordered_map>
#include "TypeConfig.h"
#include "Math/Vec2.h"
#include <shared_mutex>
#include <mutex>
#include "ECS/Types.h"
#include <utility>

struct PRenderData {
	Entity entityId;
	PRENDER_SHAPE::Type shape;
	Vec2f pos;
	Vec2f direc;

};


class PRenderBufferComponent {

	SINGLETON(PRenderBufferComponent);

	//key:entity ID
	using BufferMap = std::unordered_map<Entity,PRenderData>;

	//读写锁 更新索引时需互斥
	std::shared_mutex wrMutex;

	//三缓冲
	std::array<BufferMap, 3> renderBuffer;
	//当前帧索引
	unsigned int frameId{};

	//次新帧 最新帧 正在写入帧 对应的索引
	int preIndex{ 1 };
	int curIndex{ 2 };
	int writeIndex{ 0 };

public:

	std::pair<unsigned int, std::pair<BufferMap, BufferMap>> GetFrameIdAndReadBuffers() {
		std::shared_lock sharedLock(wrMutex);
		return { frameId,{ renderBuffer[preIndex],renderBuffer[curIndex] } };
	}

};