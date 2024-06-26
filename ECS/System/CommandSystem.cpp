#include "CommandSystem.h"
#include "ECS/World.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/Component/InputComponent.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/Component/RollbackComponent.h"

void CommandSystem::Tick(float dt)
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	for (const Entity& entity : mEntities) {
		
		PlayerComponent& playerComp = mWorld->GetComponent<PlayerComponent>(entity);
		CommandComponent& cmdComp = mWorld->GetComponent<CommandComponent>(entity);

		cmdComp.frameId = rollbackComp.preciseCmd.frameId;

		//回滚组件中得到的命令
		cmdComp.cmd = rollbackComp.preciseCmd.commandArray[playerComp.localId];

	}
}
