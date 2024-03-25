#pragma once
#include "ECS/World.h"

#include "TypeConfig.h"



class CollisionSystem;
class CommandSystem;
class FireSystem;
class FrameSystem;
class InputSystem;
class MoveSystem;
class ObstacleSystem;
class PRenderBufferSystem;
class SocketSystem;
class SpeedChangeSystem;
class PlayerSpawnSystem;
class EntitySpawnSystem;
class EntityDestroySystem;
class BulletHitSystem;
class PlayerStateSystem;
class SceneChangeSystem;
class RollbackSystem;
class ObservedSystem;
class DamageSystem;

class XTankWorld :public World {

private:
	CollisionSystem* mCollisionSystem{};
	CommandSystem* mCommandSystem{};
	FireSystem* mFireSystem{};
	FrameSystem* mFrameSystem{};
	InputSystem* mInputSystem{};
	MoveSystem* mMoveSystem{};
	ObstacleSystem* mObstacleSystem{};
	PRenderBufferSystem* mPRenderBufferSystem{};
	SocketSystem* mSocketSystem{};
	SpeedChangeSystem* mSpeedChangeSystem{};
	PlayerSpawnSystem* mPlayerSpawnSystem{};
	EntitySpawnSystem* mEntitySpawnSystem{};
	EntityDestroySystem* mEntityDestroySystem{};
	BulletHitSystem* mBulletHitSystem{};
	PlayerStateSystem* mPlayerStateSystem{};
	SceneChangeSystem* mSceneChangeSystem{};
	RollbackSystem* mRollbackSystem{};
	ObservedSystem* mObservedSystem{};
	DamageSystem* mDamageSystem{};


private:

	//重置系统指针
	void ResetSystemPtr();

public:

	XTankWorld() = default;

	XTankWorld(const XTankWorld& other) :
		World(other) {

		ResetSystemPtr();
	}

	XTankWorld& operator = (XTankWorld&& other) noexcept {

		World::operator=(std::move(other));
		ResetSystemPtr();

		return *this;

	}

	XTankWorld& operator = (const XTankWorld& other) {

		World::operator=(other);
		ResetSystemPtr();
	}

	virtual void Init() override;

	//系统初始化
	void SystemInit();

	//真实世界各个系统更新
	void SystemTickInLogic(float dt);

	//预测世界中各个系统更新
	void SystemTickInDuplicate(float dt,const std::vector<PlayersCommand>& playersCmds);

	//用预测世界更新渲染系统
	void DuplicateRenderTick(float dt);

	//游戏循环
	void GameTick();

	//玩家中途加入 追帧
	void ChaseUpToServer();

	void Start(const bool& isEnd);

};