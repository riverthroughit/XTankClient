#include "CommandSystem.h"
#include "ECS/Event.h"
#include "ECS/World.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/Component/InputComponent.h"

void CommandSystem::Tick(float dt)
{

	SocketComponent& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	for (const Entity& entity : mEntities) {
		PlayerComponent& playerComp = mWorld->GetComponent<PlayerComponent>(entity);
		CommandComponent& cmdComp = mWorld->GetComponent<CommandComponent>(entity);

		if (playerComp.playerId == socketComp.localPlayerId) {
			//本地玩家从InputComponent中取得命令
			InputComponent& inputComp = mWorld->GetSingletonComponent<InputComponent>();
			cmdComp.cmd = inputComp.curBtn;
		}
		else {

		}
	}
}
