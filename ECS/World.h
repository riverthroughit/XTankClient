#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "SystemManager.h"
#include "Types.h"
#include <memory>
#include "Util/Macro.h"
#include "Event/EventQueue.h"

class World
{

private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;


public:

	World() = default;

	World(const World& other):
		mComponentManager(std::make_unique<ComponentManager>(*other.mComponentManager)),
		mEntityManager(std::make_unique<EntityManager>(*other.mEntityManager)),
		mSystemManager(std::make_unique<SystemManager>(*other.mSystemManager)){

		mSystemManager->SetWorldOfSystems(this);

	}

	World& operator = (World&& other) noexcept {

		mComponentManager = std::move(other.mComponentManager);
		mEntityManager = std::move(other.mEntityManager);
		mSystemManager = std::move(other.mSystemManager);
		mSystemManager->SetWorldOfSystems(this);

		return *this;
	}

	World& operator = (const World& other) {

		mComponentManager = std::make_unique<ComponentManager>(*other.mComponentManager);
		mEntityManager = std::make_unique<EntityManager>(*other.mEntityManager);
		mSystemManager = std::make_unique<SystemManager>(*other.mSystemManager);
		mSystemManager->SetWorldOfSystems(this);

		return *this;
	}

	virtual void Init()
	{
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}

	// Entity methods
	Entity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);

		mComponentManager->EntityDestroyed(entity);

		mSystemManager->EntityDestroyed(entity);
	}


	// Component methods
	template<typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void RegisterSingletonComponent()
	{
		mComponentManager->RegisterSingletonComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		mComponentManager->AddComponent<T>(entity, component);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	bool HasComponent(Entity entity)
	{
		return mComponentManager->HasComponent<T>(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	// System methods
	template<typename T>
	T* RegisterSystem()
	{
		auto system = mSystemManager->RegisterSystem<T>();
		system->SetWorld(this);
		return system;
	}

	template<typename T>
	T* GetSystem()
	{
		return mSystemManager->GetSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}

	template<typename T>
	T& GetSingletonComponent() {
		return mComponentManager->GetSingletonComponent<T>();
	}


};

