#include "PRenderBufferSystem.h"

#include "ECS/World.h"
#include "ECS/Component/PRenderComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/FrameComponent.h"
#include "Util/Util.h"
#include "Renderer/PRenderBuffer.h"

void PRenderBufferSystem::Tick(float dt)
{
	UpdatePRenderBuffer();
}


void PRenderBufferSystem::UpdatePRenderBuffer()
{

	auto& pRenderBuffer = PRenderBuffer::Instance();

	for (const Entity& entity : mEntities) {
		PosComponent& posComp = mWorld->GetComponent<PosComponent>(entity);
		PRenderComponent& pRenderComp = mWorld->GetComponent<PRenderComponent>(entity);
	
		//转换为浮点数
		Vec2f pos = { (float)(posComp.pos.x),(float)(posComp.pos.y) };
		Vec2f direc = { (float)(posComp.direc.x),(float)(posComp.direc.y) };

		pRenderBuffer.WriteToBuffer(PRenderData{ entity, pRenderComp.shape,pos,direc });
	}

	pRenderBuffer.SwapPRenderBuffer();

	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	pRenderBuffer.SetFrameId(frameComp.frameId);
}
