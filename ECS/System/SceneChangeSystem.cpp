#include "SceneChangeSystem.h"
#include "ECS/World.h"
#include "ECS/Component/BlockComponent.h"
#include "ECS/Component/RandComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "Config.h"
#include "TypeConfig.h"
#include <memory>

void SceneChangeSystem::Tick(float dt)
{
	for (Entity entity : mEntities) {
		auto& blockComp = mWorld->GetComponent<BlockComponent>(entity);
		if (blockComp.blockType == BLOCK::FRAGILE && blockComp.isHit == true) {
			mWorld->AddComponent(entity, DestroyComponent());
		}
	}
}

void SceneChangeSystem::InitScene()
{

	auto& randComp = mWorld->GetSingletonComponent<RandComponent>();
	auto& entitySpawnComp = mWorld->GetSingletonComponent<EntitySpawnComponent>();

	for (int row = 0; row < SCENE_SIDE_NUM; ++row) {
		for (int col = 0; col < SCENE_SIDE_NUM; ++col) {
			//坦克出生点直接跳过
			if ((row == 1 && col == 1)
				|| row == 1 && col == SCENE_SIDE_NUM - 2
				|| row == SCENE_SIDE_NUM - 2 && col == 1
				|| row == SCENE_SIDE_NUM - 2 && col == SCENE_SIDE_NUM - 2) {
				
				continue;
			}

			//边界
			if (row == 0 || col == 0 || row == SCENE_SIDE_NUM - 1 || col == SCENE_SIDE_NUM - 1) {
				auto args = std::make_shared<ENTITY_SPAWN_ARGS::Block>();
				args->blockType = BLOCK::HARD;
				args->direc = { FixedPoint(0),FixedPoint(-1) };
				args->pos = { FixedPoint((col + (float)0.5) * CUBE_SIDE_LENTH),FixedPoint((row + (float)0.5) * CUBE_SIDE_LENTH) };
				entitySpawnComp.entitySpawnMap[ENTITY::BLOCK].push_back({ ENTITY::BLOCK, args });
				continue;
			}

			//概率满足
			if (randComp.IsTrueByProb(BLOCK_DENSITY.first, BLOCK_DENSITY.second)) {
				auto args = std::make_shared<ENTITY_SPAWN_ARGS::Block>();
				args->blockType = BLOCK::FRAGILE;
				args->direc = { FixedPoint(0),FixedPoint(-1) };
				args->pos = { FixedPoint((col + (float)0.5) * CUBE_SIDE_LENTH),FixedPoint((row + (float)0.5) * CUBE_SIDE_LENTH) };
				entitySpawnComp.entitySpawnMap[ENTITY::BLOCK].push_back({ ENTITY::BLOCK, args });
			}
		}
	}
}
