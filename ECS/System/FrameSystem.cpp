#include "FrameSystem.h"

#include "Config.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/World.h"



void FrameSystem::Tick(float)
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	frameComp.clientTick.Tick();
}

void FrameSystem::Init()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	frameComp.serverFrameId = -1;
}


void FrameSystem::AddServerFrameId()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	
	++frameComp.serverFrameId;

}

