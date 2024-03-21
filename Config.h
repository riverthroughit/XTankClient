#pragma once
#include "Math/Vec2.h"
#include <memory>

//渲染刷新率
constexpr int RENDER_FPS{ 60 };
//逻辑帧刷新率
constexpr int LOCKSTEP_FPS{ 20 };
//追帧时刷新率
constexpr int MAX_CHASING_FPS{ LOCKSTEP_FPS * 2 };
//逻辑帧周期
constexpr float LOCKSTEP_TICK{ 1.0 / LOCKSTEP_FPS * 1000 };
//渲染帧周期
constexpr float RENDER_TICK{ 1.0 / RENDER_FPS * 1000 };
//追帧周期
constexpr float MIN_CHASING_TICK{ 1.0 / MAX_CHASING_FPS * 1000 };
//最多预测帧数
constexpr int MAX_PREDICT_FRAME{ 20 };

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
//障碍物半径
constexpr float BLOCK_RADIUS{ 2.5 };
//坦克半径
constexpr int TANK_RADIUS{ 3 };
//炮弹半径
constexpr int BULLET_RADIUS{ 1 };
//场景边上方格数量(包括最边缘障碍物)
constexpr int SCENE_SIDE_NUM{ 14 };
//场景边长
constexpr int SCENE_SIDE_LENTH{ SCENE_SIDE_NUM * CUBE_SIDE_LENTH };
//障碍物密度 分子 分母
constexpr std::pair<int, int> BLOCK_DENSITY{ 1,2 };

//坦克速度 注意单位是每个逻辑帧
constexpr float TANK_SPEED{ (float)CUBE_SIDE_LENTH * 2 / LOCKSTEP_FPS };
//子弹速度
constexpr float BULLET_SPEED{ (float)CUBE_SIDE_LENTH * 3 / LOCKSTEP_FPS };

//坦克加速度 注意单位是每个逻辑帧
constexpr float TANK_ACCSPEED{ 1.f / LOCKSTEP_FPS };
//子弹加速度
constexpr float BULLET_ACCSPEED{ 100.f / LOCKSTEP_FPS };

//玩家hp
constexpr int PLAYER_HP = 3;
constexpr int TANK_HP = 1;
constexpr int BULLET_HP = 1;
constexpr int BLOCK_HP = 1;
constexpr int BULLET_DAMAGE = 1;

//复活冷却时间
constexpr int RESPAWN_TIME = LOCKSTEP_FPS;
//玩家延迟销毁时间
constexpr int PLAYER_DESTROY_LATENCY = 10 * LOCKSTEP_FPS;
//玩家数量
constexpr int PLAYER_NUM = 4;

//加速度的四个方向
constexpr Vec2f ACCSPEED_DIREC[4]{ {1,0},{-1,0},{0,1},{0,-1} };

//四个玩家的出生点
constexpr Vec2f PLAYERS_SPAWN_POS[PLAYER_NUM]{
	{1.5 * CUBE_SIDE_LENTH,1.5 * CUBE_SIDE_LENTH},
	{(SCENE_SIDE_NUM - 1.5) * CUBE_SIDE_LENTH ,1.5 * CUBE_SIDE_LENTH},
	{1.5 * CUBE_SIDE_LENTH,(SCENE_SIDE_NUM - 1.5) * CUBE_SIDE_LENTH },
	{ (SCENE_SIDE_NUM - 1.5) * CUBE_SIDE_LENTH ,(SCENE_SIDE_NUM - 1.5) * CUBE_SIDE_LENTH },
};

//四个玩家初始方向
constexpr Vec2f PLAYERS_SPAWN_DIREC[PLAYER_NUM]{
	{0,1},
	{0,1},
	{0,-1},
	{ 0,-1},
};

//定点数表示
constexpr FixedPoint CUBE_SIDE_LENTH_FIXED{ CUBE_SIDE_LENTH };
constexpr FixedPoint BLOCK_RADIUS_FIXED{ BLOCK_RADIUS };
constexpr FixedPoint TANK_RADIUS_FIXED{ TANK_RADIUS };
constexpr FixedPoint BULLET_RADIUS_FIXED{ BULLET_RADIUS };
constexpr FixedPoint SCENE_SIDE_NUM_FIXED{ SCENE_SIDE_NUM };
constexpr FixedPoint SCENE_SIDE_LENTH_FIXED{ SCENE_SIDE_LENTH }; 
constexpr FixedPoint TANK_SPEED_FIXED{ TANK_SPEED };
constexpr FixedPoint BULLET_SPEED_FIXED{ BULLET_SPEED };
constexpr FixedPoint TANK_ACCSPEED_FIXED{ TANK_ACCSPEED };
constexpr FixedPoint BULLET_ACCSPEED_FIXED{ BULLET_ACCSPEED };


constexpr Vec2Fixed PLAYERS_SPAWN_POS_FIXED[4]{
	{FixedPoint(PLAYERS_SPAWN_POS[0].x),FixedPoint(PLAYERS_SPAWN_POS[0].y)},
	{FixedPoint(PLAYERS_SPAWN_POS[1].x),FixedPoint(PLAYERS_SPAWN_POS[1].y)},
	{FixedPoint(PLAYERS_SPAWN_POS[2].x),FixedPoint(PLAYERS_SPAWN_POS[2].y)},
	{FixedPoint(PLAYERS_SPAWN_POS[3].x),FixedPoint(PLAYERS_SPAWN_POS[3].y)},
};

constexpr Vec2Fixed PLAYERS_SPAWN_DIREC_FIXED[4]{
	{FixedPoint(PLAYERS_SPAWN_DIREC[0].x),FixedPoint(PLAYERS_SPAWN_DIREC[0].y)},
	{FixedPoint(PLAYERS_SPAWN_DIREC[1].x),FixedPoint(PLAYERS_SPAWN_DIREC[1].y)},
	{FixedPoint(PLAYERS_SPAWN_DIREC[2].x),FixedPoint(PLAYERS_SPAWN_DIREC[2].y)},
	{FixedPoint(PLAYERS_SPAWN_DIREC[3].x),FixedPoint(PLAYERS_SPAWN_DIREC[3].y)},
};

//加速度的四个方向
constexpr Vec2Fixed ACCSPEED_DIREC_FIXED[4] = {
	{FixedPoint(ACCSPEED_DIREC[0].x),FixedPoint(ACCSPEED_DIREC[0].y)},
	{FixedPoint(ACCSPEED_DIREC[1].x),FixedPoint(ACCSPEED_DIREC[1].y)},
	{FixedPoint(ACCSPEED_DIREC[2].x),FixedPoint(ACCSPEED_DIREC[2].y)},
	{FixedPoint(ACCSPEED_DIREC[3].x),FixedPoint(ACCSPEED_DIREC[3].y)},
};