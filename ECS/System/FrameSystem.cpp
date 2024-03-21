#include "FrameSystem.h"

#include "Config.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/World.h"



void FrameSystem::Tick(float)
{
	
}

void FrameSystem::Init()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	frameComp.clientTick.SetTickTime(LOCKSTEP_TICK);

}


void FrameSystem::AddServerFrameId()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	
	++frameComp.serverFrameId;

}

void FrameSystem::TickClientFrame()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	frameComp.clientTick.Tick();
}

bool FrameSystem::NeedClientTick()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	return frameComp.clientTick.NeedTick();
}

float FrameSystem::GetClientTickDt()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	return frameComp.clientTick.GetDt();
}

float FrameSystem::GetTimeToNextClientFrame()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	return frameComp.clientTick.TimeToNextFrame();
}

