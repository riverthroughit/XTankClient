#pragma once

#include"FixedMath/Vec2i.h"

class Shape {

public:
	enum Type
	{
		TRIANGLE,
		RECTANGLE,
		CIRCLE,
	};

private:
	int speed;
	Vec2i pos;
	Type type;

	friend class CollisionDetection;

public:
	bool IsCollideWith(Shape* shape);

	Type GetType() { return type; };
};


