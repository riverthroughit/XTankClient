#pragma once
#include "Math/Vec2.h"

//��Ⱦˢ����
constexpr int RENDER_FPS{ 60 };
//�߼�֡ˢ����
constexpr int LOCKSTEP_FPS{ 30 };
//�߼�֡����
constexpr float LOCKSTEP_TICK{ 1.0 / LOCKSTEP_FPS * 1000 };


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
//̹�˰뾶
constexpr int TANK_RADIUS{ 3 };
//�ڵ��뾶
constexpr int BULLET_RADIUS{ 1 };
//�������Ϸ�������
constexpr int SCENE_SIDE_NUM{ 13 };
//�����߳�
constexpr int SCENE_SIDE_LENTH{ SCENE_SIDE_NUM * CUBE_SIDE_LENTH };

//�ĸ���ҵĳ�����
constexpr Vec2f PLAYERS_SPAWN_POS[4]{
	{0,0},
	{(SCENE_SIDE_NUM - 0.5) * CUBE_SIDE_LENTH ,0},
	{0,(SCENE_SIDE_NUM - 0.5) * CUBE_SIDE_LENTH },
	{ (SCENE_SIDE_NUM - 0.5) * CUBE_SIDE_LENTH ,(SCENE_SIDE_NUM - 0.5) * CUBE_SIDE_LENTH },
};

//��������ʾ
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

