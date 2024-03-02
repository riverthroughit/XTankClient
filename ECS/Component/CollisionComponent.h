#pragma once

#include"Math/Vec2.h"
#include "ECS/Types.h"
#include <vector>
#include "TypeConfig.h"

struct CollisionComponent {


	union ShapeData
	{
		FixedPoint r;//圆形半径
		//Vec2Fixed vertex;//矩形的一个顶点
	};

	LOGIC_SHAPE::Type shape{ LOGIC_SHAPE::NONE };
	ShapeData shapeData{ FixedPoint()};

	std::vector<Entity> hitEntities;
};