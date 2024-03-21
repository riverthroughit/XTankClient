#include "TickUtil.h"

TickUtil::TickUtil(float tkTime)
{
	tickTime = tkTime;
	Reset();
}

void TickUtil::Reset()
{
	frameTime = tickTime;
	frameId = -1;
	percent = 0;
	isStart = false;
	dt = 0;
}

void TickUtil::SetTickTime(float val)
{

	tickTime = val;

}
void TickUtil::Tick()
{

	if (!isStart) {
		isStart = true;
		needTick = true;
		preTime = clock::now();
		frameId = 0;
		dt = 0;
		return;
	}

	needTick = false;
	curTime = clock::now();
	duration dtn = std::chrono::duration_cast<duration>(curTime - preTime);
	preTime = curTime;
	frameTime += dtn.count();
	dt = frameTime;

	if (frameTime > tickTime) {
		needTick = true;
		int dframe = frameTime / tickTime;
		assert(dframe == 1 && "too slow");
		frameTime -= dframe * tickTime;
		frameId += dframe;
	}

	percent = frameTime / tickTime;
}
bool TickUtil::NeedTick()
{
	return needTick;
}

float TickUtil::TimeToNextFrame()
{
	clock::time_point time = clock::now();
	duration dtn = std::chrono::duration_cast<duration>(time - preTime);
	return tickTime - (frameTime + dtn.count());
}

void TickUtil::SetDelay(float val)
{
	frameTime -= val;
}

int TickUtil::GetFrameId()
{
	return frameId;
}

float TickUtil::GetPercent()
{
	return percent;
}

float TickUtil::GetDt()
{
	return dt;
}

float TickUtil::GetTickTime()
{
	return tickTime;
}
