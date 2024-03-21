#pragma once
#include "Math/Vec2.h"
#include <memory>

//��Ⱦˢ����
constexpr int RENDER_FPS{ 60 };
//�߼�֡ˢ����
constexpr int LOCKSTEP_FPS{ 20 };
//׷֡ʱˢ����
constexpr int MAX_CHASING_FPS{ LOCKSTEP_FPS * 2 };
//�߼�֡����
constexpr float LOCKSTEP_TICK{ 1.0 / LOCKSTEP_FPS * 1000 };
//��Ⱦ֡����
constexpr float RENDER_TICK{ 1.0 / RENDER_FPS * 1000 };
//׷֡����
constexpr float MIN_CHASING_TICK{ 1.0 / MAX_CHASING_FPS * 1000 };
//���Ԥ��֡��
constexpr int MAX_PREDICT_FRAME{ 20 };

//������ƿ�
constexpr int SCREEN_DESIGN_WIDTH{ 1280 };
//������Ƹ�
constexpr int SCREEN_DESIGN_HEIGHT{ 720 };
//���ڿ�
extern int SCREEN_WIDTH;
//���ڸ�
extern int SCREEN_HEIGHT;


//����߳�
constexpr int CUBE_SIDE_LENTH{ 10 };
//�ϰ���뾶
constexpr float BLOCK_RADIUS{ 2.5 };
//̹�˰뾶
constexpr int TANK_RADIUS{ 3 };
//�ڵ��뾶
constexpr int BULLET_RADIUS{ 1 };
//�������Ϸ�������(�������Ե�ϰ���)
constexpr int SCENE_SIDE_NUM{ 14 };
//�����߳�
constexpr int SCENE_SIDE_LENTH{ SCENE_SIDE_NUM * CUBE_SIDE_LENTH };
//�ϰ����ܶ� ���� ��ĸ
constexpr std::pair<int, int> BLOCK_DENSITY{ 1,2 };

//̹���ٶ� ע�ⵥλ��ÿ���߼�֡
constexpr float TANK_SPEED{ (float)CUBE_SIDE_LENTH * 2 / LOCKSTEP_FPS };
//�ӵ��ٶ�
constexpr float BULLET_SPEED{ (float)CUBE_SIDE_LENTH * 3 / LOCKSTEP_FPS };

//̹�˼��ٶ� ע�ⵥλ��ÿ���߼�֡
constexpr float TANK_ACCSPEED{ 1.f / LOCKSTEP_FPS };
//�ӵ����ٶ�
constexpr float BULLET_ACCSPEED{ 100.f / LOCKSTEP_FPS };

//���hp
constexpr int PLAYER_HP = 3;
constexpr int TANK_HP = 1;
constexpr int BULLET_HP = 1;
constexpr int BLOCK_HP = 1;
constexpr int BULLET_DAMAGE = 1;

//������ȴʱ��
constexpr int RESPAWN_TIME = LOCKSTEP_FPS;
//����ӳ�����ʱ��
constexpr int PLAYER_DESTROY_LATENCY = 10 * LOCKSTEP_FPS;
//�������
constexpr int PLAYER_NUM = 4;

//���ٶȵ��ĸ�����
constexpr Vec2f ACCSPEED_DIREC[4]{ {1,0},{-1,0},{0,1},{0,-1} };

//�ĸ���ҵĳ�����
constexpr Vec2f PLAYERS_SPAWN_POS[PLAYER_NUM]{
	{1.5 * CUBE_SIDE_LENTH,1.5 * CUBE_SIDE_LENTH},
	{(SCENE_SIDE_NUM - 1.5) * CUBE_SIDE_LENTH ,1.5 * CUBE_SIDE_LENTH},
	{1.5 * CUBE_SIDE_LENTH,(SCENE_SIDE_NUM - 1.5) * CUBE_SIDE_LENTH },
	{ (SCENE_SIDE_NUM - 1.5) * CUBE_SIDE_LENTH ,(SCENE_SIDE_NUM - 1.5) * CUBE_SIDE_LENTH },
};

//�ĸ���ҳ�ʼ����
constexpr Vec2f PLAYERS_SPAWN_DIREC[PLAYER_NUM]{
	{0,1},
	{0,1},
	{0,-1},
	{ 0,-1},
};

//��������ʾ
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

//���ٶȵ��ĸ�����
constexpr Vec2Fixed ACCSPEED_DIREC_FIXED[4] = {
	{FixedPoint(ACCSPEED_DIREC[0].x),FixedPoint(ACCSPEED_DIREC[0].y)},
	{FixedPoint(ACCSPEED_DIREC[1].x),FixedPoint(ACCSPEED_DIREC[1].y)},
	{FixedPoint(ACCSPEED_DIREC[2].x),FixedPoint(ACCSPEED_DIREC[2].y)},
	{FixedPoint(ACCSPEED_DIREC[3].x),FixedPoint(ACCSPEED_DIREC[3].y)},
};