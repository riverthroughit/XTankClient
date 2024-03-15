#pragma once
#include "ECS/Types.h"
#include "Math/Vec2.h"
#include "TypeConfig.h"
#include <shared_mutex>
#include <array>
#include <vector>
#include "Util/Macro.h"


struct PRenderData {
	PRENDER_SHAPE::Type shape;
	Vec2f pos;
	Vec2f direc;
};



class PRenderBuffer {

	SINGLETON(PRenderBuffer)

private:

	//读写锁 更新索引时需互斥
	std::shared_mutex wrMutex;

	//双缓冲
	std::array<std::vector<PRenderData>, 2> renderBuffer;

	//写缓冲读缓冲 对应的索引
	int writeIndex{ 0 };
	int readIndex{ 1 };

public:

	std::vector<PRenderData> GetReadBuffer();

	void WriteToBuffer(const PRenderData& data);

	void SwapPRenderBuffer();

};