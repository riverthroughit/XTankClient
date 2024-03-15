#include "PRenderBufferSystem.h"

#include "ECS/World.h"
#include "ECS/Component/PRenderComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "Util/Util.h"
#include "Renderer/PRenderBuffer.h"

void PRenderBufferSystem::Tick(float dt)
{
	UpdatePRenderBuffer(dt);
}


void PRenderBufferSystem::UpdatePRenderBuffer(float dt)
{

	auto& pRenderBuffer = PRenderBuffer::Instance();
	float percent = dt / LOCKSTEP_TICK;

	for (const Entity& entity : mEntities) {
		PosComponent& posComp = mWorld->GetComponent<PosComponent>(entity);
		PRenderComponent& pRenderComp = mWorld->GetComponent<PRenderComponent>(entity);
	
		//转换为浮点数
		Vec2f pos = { (float)(posComp.pos.x),(float)(posComp.pos.y) };
		Vec2f direc = { (float)(posComp.direc.x),(float)(posComp.direc.y) };

		Vec2f prePos = { (float)(posComp.prePos.x),(float)(posComp.prePos.y) };
		Vec2f preDirec = { (float)(posComp.preDirec.x),(float)(posComp.preDirec.y) };

		//插值
		Vec2f interpPos = linearInterp(prePos, pos, percent);
		Vec2f interpDirec = linearInterp(preDirec, direc, percent);

		pRenderBuffer.WriteToBuffer(PRenderData{ pRenderComp.shape,interpPos,interpDirec });
	}

	pRenderBuffer.SwapPRenderBuffer();

}
