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

	static int a, b;
	auto& inputComp = GetSingletonComponent<InputComponent>();
	auto& frameComp = GetSingletonComponent<FrameComponent>();
	auto& socketComp = GetSingletonComponent<SocketComponent>();
	auto& rollbackComp = GetSingletonComponent<RollbackComponent>();


	mFrameSystem->Tick(dt);
	mInputSystem->Tick(dt);
	mSocketSystem->Tick(dt);

	if (inputComp.curBtn == BUTTON::RIGHT) {
		a = frameComp.clientTick.GetFrameId();
	}
	if (socketComp.curPlayersCmd.commandArray[socketComp.localPlayerId] == BUTTON::RIGHT) {
		b = frameComp.clientTick.GetFrameId();
	}

	mRollbackSystem->Tick(dt);







	//有服务器下发命令时才更新主状态
	if (mSocketSystem->HasNewCmdMsg()) {
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

void XTankWorld::Start(const bool& isEnd)
{

	SystemInit();

	TickUtil tickUtil(LOCKSTEP_TICK);

	while (!isEnd) {

		tickUtil.Tick();
		float dt = tickUtil.GetDt();

		//逻辑
		if (tickUtil.NeedTick()) {
			
			//真实世界更新
			SystemTickInLogic(dt);

			//预测世界更新
			mRollbackSystem->TickPredictWorld(dt);

			//若收到不止一条来自服务器的指令 则根据数量更改tick速度 直至追上
			float curTickTime = mSocketSystem->GetTickTimeBasedOnChasing();

			tickUtil.SetTickTime(curTickTime);
		}

		//用预测的世界渲染
		XTankWorld* dupWorld = mRollbackSystem->GetDuplicateWorld();
		dupWorld->mPRenderBufferSystem->Tick(dt);

	}
}
