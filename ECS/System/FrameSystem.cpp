#include "FrameSystem.h"

#include "Config.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/World.h"

void FrameSystem::Tick(float dt)
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	
	frameComp.frameTime += dt;

	//ÐÂµÄÒ»Ö¡
	if (frameComp.frameTime > LOCKSTEP_TICK) {
		frameComp.frameTime -= LOCKSTEP_TICK;
		++frameComp.frameId;
		frameComp.isNeedTick = true;
	}
	else {
		frameComp.isNeedTick = false;
	}

	frameComp.percent = frameComp.frameTime / LOCKSTEP_TICK;
}

void FrameSystem::TickInRollback()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	++frameComp.frameId;
	frameComp.frameTime = 0;
	frameComp.isNeedTick = true;
	frameComp.percent = 0;
}
