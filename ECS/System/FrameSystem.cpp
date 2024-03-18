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
	frameComp.preTime = frameComp.curTime;
	frameComp.dt += dtn.count();

	if (frameComp.dt > frameComp.tickTime) {
		int dframe = frameComp.dt / frameComp.tickTime;
		assert(dframe == 1 && "too slow");
		frameComp.dt -= dframe * frameComp.tickTime;
		frameComp.frameId += dframe;
		frameComp.isNeedTick = true;
	}
	else {
		frameComp.isNeedTick = false;
	}

	frameComp.percent = frameComp.dt / frameComp.tickTime;
}

void FrameSystem::Init()
{
	SetTickTime(LOCKSTEP_TICK);
}



void FrameSystem::Reset()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	frameComp.dt = frameComp.tickTime;
	frameComp.frameId = -1;
	frameComp.percent = 0;
	frameComp.isNeedTick = false;
	frameComp.isStart = false;
}

void FrameSystem::SetTickTime(float val)
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	frameComp.tickTime = val;
}

bool FrameSystem::IsNeedTick()
{
	return mWorld->GetSingletonComponent<FrameComponent>().isNeedTick;
}

void FrameSystem::AddFrameId()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	frameComp.dt = frameComp.tickTime;
	frameComp.percent = 0;
	++frameComp.frameId;
	frameComp.isNeedTick = false;
}

void FrameSystem::SetDelay(float val)
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	frameComp.dt -= val;
}

void FrameSystem::SetFrameId(int id)
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	frameComp.frameId = id;
}

float FrameSystem::GetDt()
{
	return mWorld->GetSingletonComponent<FrameComponent>().dt;
}
