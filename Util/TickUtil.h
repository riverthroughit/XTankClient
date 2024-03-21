#pragma once

#include <chrono>
#include <cassert>

class TickUtil {

	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float, std::milli>;

	//周期
	float tickTime{};
	//当前帧id
	int frameId{ -1 };
	//当前帧内的时间
	float frameTime{};
	//当前帧进度
	float percent{};
	//间隔时间
	float dt{};
	//是否经过一帧 需要更新
	bool needTick{};

	clock::time_point preTime{};
	clock::time_point curTime{};

	bool isStart{};

public:
	TickUtil(float tkTime);

	void Reset();

	void SetTickTime(float val);


	void Tick();

	bool NeedTick();

	float TimeToNextFrame();

	//设置一次延迟
	void SetDelay(float val);

	int GetFrameId();

	float GetPercent();

	float GetDt();

	float GetTickTime();

};