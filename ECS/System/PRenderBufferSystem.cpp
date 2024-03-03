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

	for (const Entity& entity : mEntities) {
		PosComponent& posComp = mWorld->GetComponent<PosComponent>(entity);
		PRenderComponent& pRenderComp = mWorld->GetComponent<PRenderComponent>(entity);
	
		//转换为浮点数
		Vec2f pos = { (float)(posComp.pos.x),(float)(posComp.pos.y) };
		Vec2f direc = { (float)(posComp.direc.x),(float)(posComp.direc.y) };

		WriteToPRenderBuffer(PRenderData{ entity, pRenderComp.shape,pos,direc});
	}

	SwapPRenderBuffer();
}
