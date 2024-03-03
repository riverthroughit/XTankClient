#include "XTankWorld.h"
#include "ECS/Component/AttachComponent.h"
#include "ECS/Component/CollisionComponent.h"
#include "ECS/Component/CommandComponent.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/Component/InputComponent.h"
#include "ECS/Component/KeyboardComponent.h"
#include "ECS/Component/ObstacleComponent.h"
#include "ECS/Component/PlayerComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/PRenderBufferComponent.h"
#include "ECS/Component/PRenderComponent.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/SpeedComponent.h"
#include "ECS/Component/UniformGridComponent.h"
#include "ECS/Component/BulletSpawnComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/BulletComponent.h"
#include <chrono>
#include "Config.h"
#include <thread>

void XTankWorld::Init()
{

	World::Init();

	RegisterComponent<AttachComponent>();
	RegisterComponent<CollisionComponent>();
	RegisterComponent<CommandComponent>();
	RegisterSingleComponent<FrameComponent>();
	RegisterSingleComponent<InputComponent>();
	RegisterSingleComponent<KeyboardComponent>();
	RegisterComponent<ObstacleComponent>();
	RegisterComponent<PlayerComponent>();
	RegisterComponent<PosComponent>();
	RegisterSingleComponent<PRenderBufferComponent>();
	RegisterComponent<PRenderComponent>();
	RegisterSingleComponent<SocketComponent>();
	RegisterComponent<SpeedComponent>();
	RegisterSingleComponent<UniformGridComponent>();
	RegisterComponent<BulletSpawnComponent>();
	RegisterComponent<DestroyComponent>();
	RegisterComponent<BulletComponent>();

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

	Signature signature;

	mCollisionSystem = RegisterSystem<CollisionSystem>();
	signature.reset();
	signature.set(collisionCompType);
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
}

void XTankWorld::Start()
{

	//渲染线程
	//std::thread()

	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float, std::milli>;
	using std::chrono::duration_cast;

	clock::time_point begTime = clock::now();

	while (1) {

		//计算间隔
		clock::time_point endTime = clock::now();
		duration dtn = duration_cast<duration>(endTime - begTime);
		begTime = endTime;
		float dt = dtn.count();

		//帧数系统Tick
		mFrameSystem->Tick(dt);
		FrameComponent& frameComp = GetSingletonComponent<FrameComponent>();
		//回滚系统Tick

		//若需要回滚或已经过一个逻辑帧
		if (frameComp.isNeedTick) {
			mInputSystem->Tick(dt);
			mSocketSystem->Tick(dt);
			mPlayerSpawnSystem->Tick(dt);
			mCommandSystem->Tick(dt);
			mCollisionSystem->Tick(dt);
			mSpeedChangeSystem->Tick(dt);
			mObstacleSystem->Tick(dt);
			mMoveSystem->Tick(dt);
			mFireSystem->Tick(dt);
			mBulletHitSystem->Tick(dt);
			mPlayerStateSystem->Tick(dt);
			mEntityDestroySystem->Tick(dt);
			mEntitySpawnSystem->Tick(dt);
			mPRenderBufferSystem->Tick(dt);
		}
	}
}
