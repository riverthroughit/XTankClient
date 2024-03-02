#include "PRenderBufferSystem.h"

#include "ECS/World.h"
#include "ECS/Component/PRenderComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/PRenderBufferComponent.h"
#include "Util/Util.h"

void PRenderBufferSystem::Tick(float dt)
{
	TickPRenderBuffer();
}

void PRenderBufferSystem::WriteToPRenderBuffer(const PRenderData& data)
{
	auto& bufferComp = mWorld->GetSingletonComponent<PRenderBufferComponent>();
	std::shared_lock sharedLock(bufferComp.wrMutex);
	bufferComp.renderBuffer[bufferComp.writeIndex].insert({ data.entityId,data });
}

void PRenderBufferSystem::SwapPRenderBuffer()
{
	auto& bufferComp = mWorld->GetSingletonComponent<PRenderBufferComponent>();

	std::unique_lock sharedLock(bufferComp.wrMutex);

	bufferComp.preIndex = (bufferComp.preIndex + 1) % 3;
	bufferComp.curIndex = (bufferComp.curIndex + 1) % 3;
	bufferComp.writeIndex = (bufferComp.writeIndex + 1) % 3;

	bufferComp.renderBuffer[bufferComp.writeIndex].clear();
}


void PRenderBufferSystem::TickPRenderBuffer()
{
	PRenderBufferComponent& pRenderBufferComp = mWorld->GetSingletonComponent<PRenderBufferComponent>();

	for (const Entity& entity : mEntities) {
		PosComponent& posComp = mWorld->GetComponent<PosComponent>(entity);
		PRenderComponent& pRenderComp = mWorld->GetComponent<PRenderComponent>(entity);
		SpeedComponent& speedComp = mWorld->GetComponent<SpeedComponent>(entity);
	
		//转换为浮点数
		Vec2f pos = { (float)(posComp.pos.x),(float)(posComp.pos.x) };
		Vec2f direc = { (float)(speedComp.direc.x),(float)(speedComp.direc.x) };

		WriteToPRenderBuffer(PRenderData{ entity, pRenderComp.shape,pos,direc});
	}

	SwapPRenderBuffer();
}
