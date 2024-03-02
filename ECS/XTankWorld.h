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

public:

	virtual void Init() override;

	void Start();

};