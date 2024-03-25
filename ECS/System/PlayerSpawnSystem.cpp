#include "PlayerSpawnSystem.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/World.h"
#include "Config.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/EventComponent.h"

void PlayerSpawnSystem::Init()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	for (int id = 0; id < socketComp.existPlayers.size(); ++id) {
		if (socketComp.existPlayers[id]) {
			CreatePlayerEntity(id);
		}
	}
}

void PlayerSpawnSystem::Tick(float dt)
{
	auto& rollComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& playersCmds = rollComp.preciseCmd.commandArray;

	//�ж��Ƿ�����;��������
	for (int i = 0; i < playersCmds.size(); ++i) {
		switch (playersCmds[i])
		{
		case BUTTON::CUT_IN:
			CreatePlayerEntity(i);
			break;

		case BUTTON::EXIT:
			RemovePlayerEntity(i);
			break;

		default:
			break;
		}
	}
}

void PlayerSpawnSystem::CreatePlayerEntity(int localId)
{

	Entity localPlayer = mWorld->CreateEntity();
	PlayerComponent playerComp;
	playerComp.localId = localId;
	playerComp.status = PLAYER_STATUS::LIVE;
	playerComp.hp = PLAYER_HP;
	playerComp.score = 0;
	playerComp.respawnTime = 0;

	CommandComponent commandComp;
	EventComponent eventComp;
	mWorld->AddComponent(localPlayer, playerComp);
	mWorld->AddComponent(localPlayer, commandComp);
	mWorld->AddComponent(localPlayer, eventComp);

	//tank
	auto tankArgs = std::make_shared<ENTITY_SPAWN_ARGS::Tank>();
	tankArgs->ownerId = localPlayer;
	tankArgs->pos = PLAYERS_SPAWN_POS_FIXED[localId];
	tankArgs->direc = PLAYERS_SPAWN_DIREC_FIXED[localId];

	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();
	entitySpawnComp.entitySpawnMap[ENTITY::TANK].push_back({ ENTITY::TANK, tankArgs });

}

void PlayerSpawnSystem::RemovePlayerEntity(int localId)
{

	for (Entity playerEntity : mEntities) {
		
		auto& playerComp = mWorld->GetComponent<PlayerComponent>(playerEntity);
		
		if (playerComp.localId == localId) {
			//��ҿ��Ƶ�ʵ�����ֱ������
			if (playerComp.pawnId != NULL_ENTITY) {

				mWorld->AddComponent<DestroyComponent>(playerComp.pawnId, DestroyComponent());
			}

			//�ӳ�������ұ���
			mWorld->AddComponent<DestroyComponent>(playerEntity, DestroyComponent{});

			break;
		}
	}

}
