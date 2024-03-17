#include "XTankWorld.h"

#include "ECS/System/CollisionSystem.h"
#include "ECS/System/CommandSystem.h"
#include "ECS/System/FireSystem.h"
#include "ECS/System/FrameSystem.h"
#include "ECS/System/InputSystem.h"
#include "ECS/System/MoveSystem.h"
#include "ECS/System/ObstacleSystem.h"
#include "ECS/System/PRenderBufferSystem.h"
#include "ECS/System/SocketSystem.h"
#include "ECS/System/SpeedChangeSystem.h"
#include "ECS/System/PlayerSpawnSystem.h"
#include "ECS/System/EntitySpawnSystem.h"
#include "ECS/System/EntityDestroySystem.h"
#include "ECS/System/BulletHitSystem.h"
#include "ECS/System/PlayerStateSystem.h"
#include "ECS/System/SceneChangeSystem.h"
#include "ECS/System/RollbackSystem.h"


#include "ECS/Component/AttachComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/Component/InputComponent.h"
#include "ECS/Component/ObstacleComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/PRenderComponent.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/UniformGridComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/BulletComponent.h"
#include "ECS/Component/BlockComponent.h"
#include "ECS/Component/RandComponent.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/Component/RollbackPosComponent.h"
#include "Config.h"
#include <thread>
#include "QtWidget/WidgetManager.h"
#include "Util/TickUtil.h"


void XTankWorld::ResetSystemPtr()
{
	mCollisionSystem = GetSystem<CollisionSystem>();
	mCommandSystem = GetSystem<CommandSystem>();
	mFireSystem = GetSystem<FireSystem>();
	mFrameSystem = GetSystem<FrameSystem>();
	mInputSystem = GetSystem<InputSystem>();
	mMoveSystem = GetSystem<MoveSystem>();
	mObstacleSystem = GetSystem<ObstacleSystem>();
	mPRenderBufferSystem = GetSystem<PRenderBufferSystem>();
	mSocketSystem = GetSystem<SocketSystem>();
	mSpeedChangeSystem = GetSystem<SpeedChangeSystem>();
	mPlayerSpawnSystem = GetSystem<PlayerSpawnSystem>();
	mEntitySpawnSystem = GetSystem<EntitySpawnSystem>();
	mEntityDestroySystem = GetSystem<EntityDestroySystem>();
	mBulletHitSystem = GetSystem<BulletHitSystem>();
	mPlayerStateSystem = GetSystem<PlayerStateSystem>();
	mSceneChangeSystem = GetSystem<SceneChangeSystem>();
	mRollbackSystem = GetSystem<RollbackSystem>();
}

void XTankWorld::Init()
{

	World::Init();

	RegisterComponent<AttachComponent>();
	RegisterComponent<CollisionComponent>();
	RegisterComponent<CommandComponent>();
	RegisterSingletonComponent<FrameComponent>();
	RegisterSingletonComponent<InputComponent>();
	RegisterComponent<ObstacleComponent>();
	RegisterComponent<PlayerComponent>();
	RegisterComponent<PosComponent>();
	RegisterComponent<PRenderComponent>();
	RegisterSingletonComponent<SocketComponent>();
	RegisterComponent<SpeedComponent>();
	RegisterSingletonComponent<UniformGridComponent>();
	RegisterComponent<BulletSpawnComponent>();
	RegisterComponent<DestroyComponent>();
	RegisterComponent<BulletComponent>();
	RegisterComponent<BlockComponent>();
	RegisterSingletonComponent<RandComponent>();
	RegisterSingletonComponent<EntitySpawnComponent>();
	RegisterSingletonComponent<RollbackComponent>();
	RegisterComponent<RollbackPosComponent>();

	ComponentType attachCompType = GetComponentType<AttachComponent>();
	ComponentType collisionCompType = GetComponentType<CollisionComponent>();
	ComponentType commandCompType = GetComponentType<CommandComponent>();
	ComponentType obstacleCompType = GetComponentType<ObstacleComponent>();
	ComponentType playerCompType = GetComponentType<PlayerComponent>();
	ComponentType posCompType = GetComponentType<PosComponent>();
	ComponentType pRenderCompType = GetComponentType<PRenderComponent>();
	ComponentType speedCompType = GetComponentType<SpeedComponent>();
	ComponentType bulletSpawnCompType = GetComponentType<BulletSpawnComponent>();
	ComponentType destroyCompType = GetComponentType<DestroyComponent>();
	ComponentType bulletCompType = GetComponentType<BulletComponent>();
	ComponentType blockCompType = GetComponentType<BlockComponent>();
	ComponentType rollbackPosCompType = GetComponentType<RollbackPosComponent>();

	Signature signature;

	mCollisionSystem = RegisterSystem<CollisionSystem>();
	signature.reset();
	signature.set(collisionCompType);
	signature.set(posCompType);
	SetSystemSignature<CollisionSystem>(signature);

	mCommandSystem = RegisterSystem<CommandSystem>();
	signature.reset();
	signature.set(playerCompType);
	signature.set(commandCompType);
	SetSystemSignature<CommandSystem>(signature);

	mFireSystem = RegisterSystem<FireSystem>();
	signature.reset();
	signature.set(playerCompType);
	signature.set(commandCompType);
	SetSystemSignature<FireSystem>(signature);

	mFrameSystem = RegisterSystem<FrameSystem>();

	mInputSystem = RegisterSystem<InputSystem>();

	mMoveSystem = RegisterSystem<MoveSystem>();
	signature.reset();
	signature.set(posCompType);
	signature.set(speedCompType);
	SetSystemSignature<MoveSystem>(signature);

	mObstacleSystem = RegisterSystem<ObstacleSystem>();
	signature.reset();
	signature.set(posCompType);
	signature.set(collisionCompType);
	signature.set(obstacleCompType);
	SetSystemSignature<ObstacleSystem>(signature);

	mPRenderBufferSystem = RegisterSystem<PRenderBufferSystem>();
	signature.reset();
	signature.set(posCompType);
	signature.set(pRenderCompType);
	SetSystemSignature<PRenderBufferSystem>(signature);

	mSocketSystem = RegisterSystem<SocketSystem>();

	mSpeedChangeSystem = RegisterSystem<SpeedChangeSystem>();
	signature.reset();
	signature.set(commandCompType);
	signature.set(playerCompType);
	SetSystemSignature<SpeedChangeSystem>(signature);

	mPlayerSpawnSystem = RegisterSystem<PlayerSpawnSystem>();

	mEntitySpawnSystem = RegisterSystem<EntitySpawnSystem>();

	mEntityDestroySystem = RegisterSystem<EntityDestroySystem>();
	signature.reset();
	signature.set(destroyCompType);
	SetSystemSignature<EntityDestroySystem>(signature);

	mBulletHitSystem = RegisterSystem<BulletHitSystem>();
	signature.reset();
	signature.set(bulletCompType);
	SetSystemSignature<BulletHitSystem>(signature);

	mPlayerStateSystem = RegisterSystem<PlayerStateSystem>();
	signature.reset();
	signature.set(playerCompType);
	SetSystemSignature<PlayerStateSystem>(signature);

	mSceneChangeSystem = RegisterSystem<SceneChangeSystem>();
	signature.reset();
	signature.set(blockCompType);
	SetSystemSignature<SceneChangeSystem>(signature);

	mRollbackSystem = RegisterSystem<RollbackSystem>();
	signature.reset();
	signature.set(posCompType);
	signature.set(rollbackPosCompType);
	SetSystemSignature<RollbackSystem>(signature);
}

void XTankWorld::SystemInit()
{

	mFrameSystem->Init();
	//��ʼ������
	mSceneChangeSystem->Init();
	//��ȡ��ʼ��Ϣ
	mSocketSystem->Init();
	//�������
	mPlayerSpawnSystem->Init();

	//����entity
	mEntitySpawnSystem->Init();

	//��ʼ����ײ��Ϣ
	mCollisionSystem->Init();

	//�ع���ʼ�� ������ǰ����״̬
	mRollbackSystem->Init();

	//�ȴ�������֪ͨ
	mSocketSystem->WaitStart();
}

void XTankWorld::SystemTickInLogic(float dt)
{

	mInputSystem->Tick(dt);
	mSocketSystem->Tick(dt);
	mRollbackSystem->Tick(dt);
	mPlayerSpawnSystem->Tick(dt);

	mCommandSystem->Tick(dt);
	mCollisionSystem->Tick(dt);
	mSpeedChangeSystem->Tick(dt);
	mObstacleSystem->Tick(dt);
	mMoveSystem->Tick(dt);
	mFireSystem->Tick(dt);
	mBulletHitSystem->Tick(dt);
	mSceneChangeSystem->Tick(dt);
	mPlayerStateSystem->Tick(dt);
	mEntityDestroySystem->Tick(dt);
	mEntitySpawnSystem->Tick(dt);


}

void XTankWorld::SystemTickInDuplicate(float dt, const std::vector<PlayersCommand>& playersCmds)
{

	//��������
	auto& rollbackComp = GetSingletonComponent<RollbackComponent>();
	for (const PlayersCommand& cmd : playersCmds) {
		
		rollbackComp.preciseCmd = cmd;

		mCommandSystem->Tick(dt);
		mCollisionSystem->Tick(dt);
		mSpeedChangeSystem->Tick(dt);
		mObstacleSystem->Tick(dt);
		mMoveSystem->Tick(dt);
		mFireSystem->Tick(dt);
		mBulletHitSystem->Tick(dt);
		mSceneChangeSystem->Tick(dt);
		mPlayerStateSystem->Tick(dt);
		mEntityDestroySystem->Tick(dt);
		mEntitySpawnSystem->Tick(dt);

		mFrameSystem->AddFrameId();
	}
}

void XTankWorld::SystemTickInChasing()
{
	//����Ϊ׷֡ʱ��Ƶ��
	mFrameSystem->SetTickTime(CHASING_TICK);

	auto& rollbackComp = GetSingletonComponent<RollbackComponent>();
	auto& socketComp = GetSingletonComponent<SocketComponent>();

	while (!mSocketSystem->IsChasingEnd()) {

		mFrameSystem->Tick();
		float dt = mFrameSystem->GetDt();

		if (mFrameSystem->IsNeedTick()) {
			
			mSocketSystem->TickInChasing(dt);
			
			rollbackComp.preciseCmd = socketComp.curPlayersCmd;

			mCommandSystem->Tick(dt);
			mCollisionSystem->Tick(dt);
			mSpeedChangeSystem->Tick(dt);
			mObstacleSystem->Tick(dt);
			mMoveSystem->Tick(dt);
			mFireSystem->Tick(dt);
			mBulletHitSystem->Tick(dt);
			mSceneChangeSystem->Tick(dt);
			mPlayerStateSystem->Tick(dt);
			mEntityDestroySystem->Tick(dt);
			mEntitySpawnSystem->Tick(dt);
			mPRenderBufferSystem->Tick(dt);
		}

	}


	//socketComp.chasingCmds.clear();

	//֪ͨ������׷֡���
	MsgSendQueue::Instance().SendPlayerChaseUpNtf();

	//���ø���Ƶ�ʲ�������ȷ��֡id
	mFrameSystem->SetTickTime(LOCKSTEP_TICK);
	mFrameSystem->SetFrameId(socketComp.chasingFrameId - 1);

}

void XTankWorld::Start(const bool& isEnd)
{

	SystemInit();

	if (mSocketSystem->NeedChasing()) {
		SystemTickInChasing();
	}

	while (!isEnd) {

		mFrameSystem->Tick();
		float dt = mFrameSystem->GetDt();

		//�߼�
		if (mFrameSystem->IsNeedTick()) {
			
			//��ʵ�������
			SystemTickInLogic(dt);

			//Ԥ���������
			mRollbackSystem->TickPredictWorld(dt);

		}

		//��Ԥ���������Ⱦ
		XTankWorld* dupWorld = mRollbackSystem->GetDuplicateWorld();
		dupWorld->mPRenderBufferSystem->Tick(dt);

	}
}
