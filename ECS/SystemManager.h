#pragma once

#include "System.h"
#include "Types.h"
#include <cassert>
#include <memory>
#include <unordered_map>

class World;

class SystemManager
{
public:

	SystemManager() = default;

	//拷贝构造
	SystemManager(const SystemManager& other):
		mSignatures(other.mSignatures){

		for (auto& [name, ptr] : other.mSystems) {
			mSystems.insert({ name,ptr->GetACopy()});
		}
	}

	//移动赋值
	SystemManager& operator = (SystemManager&& other)noexcept = default;

	template<typename T>
	T* RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		auto[ite,isNotExist] = mSystems.insert({ typeName, std::make_unique<T>() });
		
		assert(isNotExist && "Registering system more than once.");

		return static_cast<T*>(ite->second.get());
	}

	template<typename T>
	T* GetSystem()
	{
		const char* typeName = typeid(T).name();

		auto ite = mSystems.find(typeName);
		assert(ite != mSystems.end() && "system is not exist");

		return static_cast<T*>(ite->second.get());
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

		mSignatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity)
	{
		for (const auto& pair : mSystems)
		{
			const auto& system = pair.second;


			system->mEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (const auto& pair : mSystems)
		{
			const auto& type = pair.first;
			const auto& system = pair.second;
			const auto& systemSignature = mSignatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}


	void SetWorldOfSystems(World* world) {
		for (auto& [_, system] : mSystems) {
			system->SetWorld(world);
		}
	}

private:
	std::unordered_map<const char*, Signature> mSignatures{};
	std::unordered_map<const char*, std::unique_ptr<SystemAbstract>> mSystems{};
};

