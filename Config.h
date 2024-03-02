#pragma once
#include "Math/Vec2.h"

//渲染刷新率
constexpr int RENDER_FPS{ 60 };
//逻辑帧刷新率
constexpr int LOCKSTEP_FPS{ 30 };
//逻辑帧周期
constexpr float LOCKSTEP_TICK{ 1.0 / LOCKSTEP_FPS * 1000 };


//窗口设计宽
constexpr int SCREEN_DESIGN_WIDTH{ 1280 };
//窗口设计高
constexpr int SCREEN_DESIGN_HEIGHT{ 720 };
//窗口宽
extern int SCREEN_WIDTH;
//窗口高
extern int SCREEN_HEIGHT;


//方格边长
constexpr int CUBE_SIDE_LENTH{ 10 };
//坦克半径
constexpr int TANK_RADIUS{ 3 };
//炮弹半径
constexpr int BULLET_RADIUS{ 1 };
//场景边上方格数量
constexpr int SCENE_SIDE_NUM{ 13 };
//场景边长
constexpr int SCENE_SIDE_LENTH{ SCENE_SIDE_NUM * CUBE_SIDE_LENTH };

//四个玩家的出生点
constexpr Vec2f PLAYERS_SPAWN_POS[4]{
	{0,0},
	{(SCENE_SIDE_NUM - 0.5) * CUBE_SIDE_LENTH ,0},
	{0,(SCENE_SIDE_NUM - 0.5) * CUBE_SIDE_LENTH },
	{ (SCENE_SIDE_NUM - 0.5) * CUBE_SIDE_LENTH ,(SCENE_SIDE_NUM - 0.5) * CUBE_SIDE_LENTH },
};

//定点数表示
constexpr FixedPoint CUBE_SIDE_LENTH_FIXED{ CUBE_SIDE_LENTH };
constexpr FixedPoint TANK_RADIUS_FIXED{ TANK_RADIUS };
constexpr FixedPoint BULLET_RADIUS_FIXED{ BULLET_RADIUS };
constexpr FixedPoint SCENE_SIDE_NUM_FIXED{ SCENE_SIDE_NUM };
constexpr FixedPoint SCENE_SIDE_LENTH_FIXED{ SCENE_SIDE_LENTH };

constexpr Vec2Fixed PLAYERS_SPAWN_POS_FIXED[4]{
	{FixedPoint(PLAYERS_SPAWN_POS[0].x),FixedPoint(PLAYERS_SPAWN_POS[0].y)},
	{FixedPoint(PLAYERS_SPAWN_POS[1].x),FixedPoint(PLAYERS_SPAWN_POS[1].y)},
	{FixedPoint(PLAYERS_SPAWN_POS[2].x),FixedPoint(PLAYERS_SPAWN_POS[2].y)},
	{FixedPoint(PLAYERS_SPAWN_POS[3].x),FixedPoint(PLAYERS_SPAWN_POS[3].y)},
};

