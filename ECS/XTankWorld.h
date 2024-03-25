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

	//����ϵͳָ��
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

	//ϵͳ��ʼ��
	void SystemInit();

	//��ʵ�������ϵͳ����
	void SystemTickInLogic(float dt);

	//Ԥ�������и���ϵͳ����
	void SystemTickInDuplicate(float dt,const std::vector<PlayersCommand>& playersCmds);

	//��Ԥ�����������Ⱦϵͳ
	void DuplicateRenderTick(float dt);

	//��Ϸѭ��
	void GameTick();

	//�����;���� ׷֡
	void ChaseUpToServer();

	void Start(const bool& isEnd);

};