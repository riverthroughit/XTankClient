#pragma once
#include "ECS/World.h"
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
#include "TypeConfig.h"

class XTankWorld :public World {

private:
	std::shared_ptr<CollisionSystem> mCollisionSystem;
	std::shared_ptr<CommandSystem> mCommandSystem;
	std::shared_ptr<FireSystem> mFireSystem;
	std::shared_ptr<FrameSystem> mFrameSystem;
	std::shared_ptr<InputSystem> mInputSystem;
	std::shared_ptr<MoveSystem> mMoveSystem;
	std::shared_ptr<ObstacleSystem> mObstacleSystem;
	std::shared_ptr<PRenderBufferSystem> mPRenderBufferSystem;
	std::shared_ptr<SocketSystem> mSocketSystem;
	std::shared_ptr<SpeedChangeSystem> mSpeedChangeSystem;
	std::shared_ptr<PlayerSpawnSystem> mPlayerSpawnSystem;
	std::shared_ptr<EntitySpawnSystem> mEntitySpawnSystem;
	std::shared_ptr<EntityDestroySystem> mEntityDestroySystem;
	std::shared_ptr<BulletHitSystem> mBulletHitSystem;
	std::shared_ptr<PlayerStateSystem> mPlayerStateSystem;
	std::shared_ptr<SceneChangeSystem> mSceneChangeSystem;
	std::shared_ptr<RollbackSystem> mRollbackSystem;

public:

	XTankWorld() = default;
	XTankWorld(const XTankWorld&) = default;
	XTankWorld& operator = (XTankWorld&&) noexcept = default;	
	
	virtual void Init() override;

	//系统初始化
	void SystemInit();

	//各个系统更新
	void SystemTick(float dt);

	//回滚过程中各个系统更新 部分系统不会像正常情况一样更新
	void SystemTickInRollback(const PlayersCommand& playersCmd);

	void Start();

};