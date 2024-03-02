#include "SocketSystem.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/World.h"

void SocketSystem::Tick(float dt)
{
	SocketComponent& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	socketComp.localPlayerId = 0;
	
	ReceiveMsg();
}

void SocketSystem::ReceiveMsg()
{

}
