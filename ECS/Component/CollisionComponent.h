#pragma once

#include"Math/Vec2.h"
#include "ECS/Types.h"
#include <vector>
#include "TypeConfig.h"

struct CollisionComponent {


	union ShapeData
	{
		FixedPoint r;//Բ�ΰ뾶
		//Vec2Fixed vertex;//���ε�һ������
	};

	LOGIC_SHAPE::Type shape{ LOGIC_SHAPE::NONE };
	ShapeData shapeData{ FixedPoint()};

	std::vector<Entity> hitEntities;
};