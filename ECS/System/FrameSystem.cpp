#include "FrameSystem.h"

#include "Config.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/World.h"

void FrameSystem::Tick(float dt)
{
	FrameComponent& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	
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
