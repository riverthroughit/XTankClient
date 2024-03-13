#include "FrameSystem.h"

#include "Config.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/World.h"



void FrameSystem::Tick(float)
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();


	if (!frameComp.isStart) {
		frameComp.isStart = true;
		frameComp.preTime = frameComp.curTime = clock::now();
	}

	frameComp.curTime = clock::now();
	duration dtn = std::chrono::duration_cast<duration>(frameComp.curTime - frameComp.preTime);
	frameComp.dt = dtn.count();
	frameComp.preTime = frameComp.curTime;
	frameComp.frameTime += frameComp.dt;

	if (frameComp.frameTime > frameComp.tickTime) {
		int dframe = frameComp.frameTime / frameComp.tickTime;
		assert(dframe == 1 && "too slow");
		frameComp.frameTime -= dframe * frameComp.tickTime;
		frameComp.frameId += dframe;
		frameComp.isNeedTick = true;
	}
	else {
		frameComp.isNeedTick = false;
	}

	frameComp.percent = frameComp.frameTime / frameComp.tickTime;
}

void FrameSystem::Init()
{
	SetTickTime(LOCKSTEP_TICK);
}

void FrameSystem::TickInRollback()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	++frameComp.frameId;
	frameComp.frameTime = 0;
	frameComp.isNeedTick = true;
	frameComp.percent = 0;
}

void FrameSystem::Reset()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	frameComp.frameTime = frameComp.tickTime;
	frameComp.frameId = -1;
	frameComp.percent = 0;
	frameComp.isNeedTick = false;
	frameComp.isStart = false;
	frameComp.preTime = frameComp.curTime = clock::now();
}

void FrameSystem::SetTickTime(float val)
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	frameComp.tickTime = val;
	Reset();
}

bool FrameSystem::IsNeedTick()
{
	return mWorld->GetSingletonComponent<FrameComponent>().isNeedTick;
}

void FrameSystem::SetDelay(float val)
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	frameComp.frameTime -= val;
}

float FrameSystem::GetDt()
{
	return mWorld->GetSingletonComponent<FrameComponent>().dt;
}
