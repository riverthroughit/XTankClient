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
#include "ECS/System/DamageSystem.h"
#include "ECS/System/AttachSystem.h"


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
#include "ECS/Component/RandComponent.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/Component/RollbackPosComponent.h"
#include "ECS/Component/HitComponent.h"
#include "ECS/Component/BeHitComponent.h"
#include "ECS/Component/HealthComponent.h"
#include "ECS/Component/SideComponent.h"
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
	mDamageSystem = GetSystem<DamageSystem>();
	mAttachSystem = GetSystem<AttachSystem>();
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
	RegisterSingletonComponent<RandComponent>();
	RegisterSingletonComponent<EntitySpawnComponent>();
	RegisterSingletonComponent<RollbackComponent>();
	RegisterComponent<RollbackPosComponent>();
	RegisterComponent<HitComponent>();
	RegisterComponent<BeHitComponent>();
	RegisterComponent<HealthComponent>();
	RegisterComponent<SideComponent>();

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
	ComponentType rollbackPosCompType = GetComponentType<RollbackPosComponent>();
	ComponentType hitCompType = GetComponentType<HitComponent>();
	ComponentType beHitCompType = GetComponentType<BeHitComponent>();
	ComponentType healthCompType = GetComponentType<HealthComponent>();
	ComponentType sideCompType = GetComponentType<SideComponent>();

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
	signature.reset();
	signature.set(playerCompType);
	SetSystemSignature<PlayerSpawnSystem>(signature);

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
	SetSystemSignature<SceneChangeSystem>(signature);

	mRollbackSystem = RegisterSystem<RollbackSystem>();
	signature.reset();
	signature.set(posCompType);
	signature.set(rollbackPosCompType);
	SetSystemSignature<RollbackSystem>(signature);

	mAttachSystem = RegisterSystem<AttachSystem>();
	signature.reset();
	signature.set(attachCompType);
	SetSystemSignature<AttachSystem>(signature);

	mDamageSystem = RegisterSystem<DamageSystem>();
	signature.reset();
	signature.set(healthCompType);
	signature.set(beHitCompType);
	SetSystemSignature<DamageSystem>(signature);
}

void XTankWorld::SystemInit()
{

	mFrameSystem->Init();
	//获取初始信息
	mSocketSystem->Init();
	//初始化场景
	mSceneChangeSystem->Init();
	//生成玩家
	mPlayerSpawnSystem->Init();

	//生成entity
	mEntitySpawnSystem->Init();

	//初始化碰撞信息
	mCollisionSystem->Init();

	//回滚初始化 拷贝当前世界状态
	mRollbackSystem->Init();

	//等待服务器通知
	mSocketSystem->WaitStart();
}

void XTankWorld::SystemTickInLogic(float dt)
{

	mFrameSystem->AddServerFrameId();
	mPlayerSpawnSystem->Tick(dt);

	mCommandSystem->Tick(dt);
	mCollisionSystem->Tick(dt);
	mSpeedChangeSystem->Tick(dt);
	mObstacleSystem->Tick(dt);
	mMoveSystem->Tick(dt);
	mFireSystem->Tick(dt);
	mBulletHitSystem->Tick(dt);
	mDamageSystem->Tick(dt);
	mPlayerStateSystem->Tick(dt);
	mAttachSystem->Tick(dt);
	mEntityDestroySystem->Tick(dt);
	mEntitySpawnSystem->Tick(dt);

}

void XTankWorld::SystemTickInDuplicate(float dt, const std::vector<PlayersCommand>& playersCmds)
{

	//设置命令
	auto& rollbackComp = GetSingletonComponent<RollbackComponent>();
	for (const PlayersCommand& cmd : playersCmds) {
		
		rollbackComp.preciseCmd = cmd;

		mFrameSystem->AddServerFrameId();
		mCommandSystem->Tick(dt);
		mCollisionSystem->Tick(dt);
		mSpeedChangeSystem->Tick(dt);
		mObstacleSystem->Tick(dt);
		mMoveSystem->Tick(dt);
		mFireSystem->Tick(dt);
		mBulletHitSystem->Tick(dt);
		mDamageSystem->Tick(dt);
		mPlayerStateSystem->Tick(dt);
		mAttachSystem->Tick(dt);
		mEntityDestroySystem->Tick(dt);
		mEntitySpawnSystem->Tick(dt);

	}
}

void XTankWorld::DuplicateRenderTick(float dt)
{
	//用预测的世界渲染
	XTankWorld* dupWorld = mRollbackSystem->GetDuplicateWorld();
	dupWorld->mPRenderBufferSystem->Tick(dt);
}

void XTankWorld::GameTick()
{

	auto& clientTick = GetSingletonComponent<FrameComponent>().clientTick;

	clientTick.Tick();
	float dt = clientTick.GetDt();

	//逻辑
	if (clientTick.NeedTick()) {

		mInputSystem->Tick(dt);
		mSocketSystem->Tick(dt);

		float tickTime = mSocketSystem->GetTickTimeBasedOnChasing();
		TickUtil serverTick(tickTime);

		float remainTime = clientTick.TimeToNextFrame();

		//有服务器新命令且没超过tick时间
		while (!mSocketSystem->IsCmdBufferEmpty() && (remainTime > (clientTick.GetTickTime() / 2))) {

			serverTick.Tick();
			dt += serverTick.GetDt();

			if (serverTick.NeedTick()) {

				mSocketSystem->UpdateCurPlayersCmd();
				//更新权威操作
				mRollbackSystem->UpdatePreciseCmd();

				SystemTickInLogic(dt);

				//尝试回滚
				mRollbackSystem->TryRollbackPredictWorld(dt);
			}

			DuplicateRenderTick(dt);

		}

		//预测命令
		mRollbackSystem->PredictNextCmd();
		//预测世界
		mRollbackSystem->RunAheadPredictWorld(dt);

	}

	DuplicateRenderTick(dt);
}

void XTankWorld::ChaseUpToServer()
{
	if (mSocketSystem->IsCutInChasing()) {
		
		auto& clientTick = GetSingletonComponent<FrameComponent>().clientTick;
		
		clientTick.SetTickTime(MIN_CHASING_TICK);
		clientTick.Reset();

		//延迟一会儿
		int latencyTimes = 2;

		while (mSocketSystem->IsCutInChasing()) {

			clientTick.Tick();
			float dt = clientTick.GetDt();

			//逻辑
			if (clientTick.NeedTick()) {

				mSocketSystem->ReceiveCmd();

				if (--latencyTimes < 0) {

					mSocketSystem->UpdateCurPlayersCmd();
					//更新权威操作
					mRollbackSystem->UpdatePreciseCmd();

					SystemTickInLogic(dt);

					//尝试回滚
					mRollbackSystem->TryRollbackPredictWorld(dt);

				}

				//预测命令
				mRollbackSystem->PredictNextCmd();
				//预测世界
				mRollbackSystem->RunAheadPredictWorld(dt);

				mSocketSystem->TrySendChaseUpNtf();
			}

			DuplicateRenderTick(dt);
			
		}

		clientTick.SetTickTime(LOCKSTEP_TICK);
	}
}

void XTankWorld::Start(const bool& isEnd)
{

	SystemInit();

	ChaseUpToServer();

	while (!isEnd) {

		GameTick();
	}
}
