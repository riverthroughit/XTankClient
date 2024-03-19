#include "SceneChangeSystem.h"
#include "ECS/World.h"
#include "ECS/Component/RandComponent.h"
#include "ECS/Component/DestroyComponent.h"
#include "ECS/Component/EntitySpawnComponent.h"
#include "Config.h"
#include "TypeConfig.h"
#include <memory>

void SceneChangeSystem::Init()
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


			const static float offest[4][2] = { {0.25,0.25},{0.75,0.25},{0.25,0.75},{0.75,0.75} };

			//边界
			if (row == 0 || col == 0 || row == SCENE_SIDE_NUM - 1 || col == SCENE_SIDE_NUM - 1) {

				for (int i = 0; i < 4; ++i) {
					
					auto args = std::make_shared<ENTITY_SPAWN_ARGS::Block>();
					args->blockType = BLOCK::HARD;
					args->direc = { FixedPoint(0),FixedPoint(-1) };
					args->pos = { FixedPoint((col + offest[i][0]) * CUBE_SIDE_LENTH),FixedPoint((row + offest[i][1]) * CUBE_SIDE_LENTH)};
					entitySpawnComp.entitySpawnMap[ENTITY::BLOCK].push_back({ ENTITY::BLOCK, args });
				}

				continue;
			}

			//概率满足
			if (randComp.IsTrueByProb(BLOCK_DENSITY.first, BLOCK_DENSITY.second)) {
				
				for (int i = 0; i < 4; ++i) {

					auto args = std::make_shared<ENTITY_SPAWN_ARGS::Block>();
					args->blockType = BLOCK::FRAGILE;
					args->direc = { FixedPoint(0),FixedPoint(-1) };
					args->pos = { FixedPoint((col + offest[i][0]) * CUBE_SIDE_LENTH),FixedPoint((row + offest[i][1]) * CUBE_SIDE_LENTH) };
					entitySpawnComp.entitySpawnMap[ENTITY::BLOCK].push_back({ ENTITY::BLOCK, args });
				}
			}
		}
	}
}
