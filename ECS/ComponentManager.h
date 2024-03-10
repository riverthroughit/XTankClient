#pragma once

#include "ComponentArray.h"
#include "Types.h"
#include <any>
#include <memory>
#include <unordered_map>
#include "SingleCompContainer.h"

class ComponentManager
{
public:

	ComponentManager() = default;

	//自定义拷贝构造 用于回滚系统
	ComponentManager(const ComponentManager& other) :
		mComponentTypes(other.mComponentTypes),
		mNextComponentType(other.mNextComponentType) {

		mSingleCompContainers.reserve(other.mSingleCompContainers.size());

		for (const auto& [type, ptr] : other.mSingleCompContainers) {
			mSingleCompContainers.insert({ type ,ptr->GetACopy() });
		}

		mComponentArrays.reserve(other.mComponentArrays.size());

		for (const auto& [type, ptr] : other.mComponentArrays) {
			mComponentArrays.insert({ type ,ptr->GetACopy() });
		}
	}

	ComponentManager& operator = (ComponentManager&&)noexcept = default;

	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		mComponentTypes.insert({ typeName, mNextComponentType });
		mComponentArrays.insert({ typeName, std::make_unique<ComponentArray<T>>() });

		++mNextComponentType;
	}

	template<typename T>
	void RegisterSingletonComponent()
	{
		const char* typeName = typeid(T).name();

		assert(mSingleCompContainers.find(typeName) == mSingleCompContainers.end() && "Registering component type more than once.");

		mSingleCompContainers.insert({ typeName ,std::make_unique<SingleCompContainer<T>>() });
	}

	template<typename T>
	T& GetSingletonComponent()
	{
		const char* typeName = typeid(T).name();

		auto ite = mSingleCompContainers.find(typeName);
		assert(ite != mSingleCompContainers.end() && "Singleton Component is not exist");

		return GetSingleCompContainer<T>().singletonComp;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return mComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>().InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>().RemoveData(entity);
	}

	template<typename T>
	bool HasComponent(Entity entity)
	{
		return GetComponentArray<T>().HasData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>().GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (const auto& pair : mComponentArrays)
		{
			const auto& componentArray = pair.second;

			componentArray->EntityDestroyed(entity);
		}
	}


private:
	std::unordered_map<const char*, ComponentType> mComponentTypes{};
	std::unordered_map<const char*, std::unique_ptr<ISingleCompContainer>> mSingleCompContainers{};
	std::unordered_map<const char*, std::unique_ptr<IComponentArray>> mComponentArrays{};
	ComponentType mNextComponentType{};


	template<typename T>
	ComponentArray<T>& GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return static_cast<ComponentArray<T>&>(*mComponentArrays[typeName]);
	}

	template<typename T>
	SingleCompContainer<T>& GetSingleCompContainer()
	{
		const char* typeName = typeid(T).name();

		assert(mSingleCompContainers.find(typeName) != mSingleCompContainers.end() && "Component not registered before use.");

		return static_cast<SingleCompContainer<T>&>(*mSingleCompContainers[typeName]);
	}
};

