#include "CollisionDetection.h"
#include<utility>
#include<cassert>
#include"Rectangle.h"
#include"Circle.h"

bool CollisionDetection::isCollide(Shape* shape1, Shape* shape2)
{

	int types = 1 << shape1->type | 1 << shape2->type;
	bool res = false;

	switch (types)
	{
	case RECTANGLE_RECTANGLE:	
		res = isRecRecCollide((Rectangle*)shape1, (Rectangle*)shape2);
		break;

	case RECTANGLE_CIRCLE:
		if (shape1->type != Shape::RECTANGLE) {
			std::swap(shape1, shape2);
		}
		res = isRecCircCollide((Rectangle*)shape1, (Circle*)shape2);
		break;

	case CIRCLE_CIRCLE:
		res = isCircCircCollide((Circle*)shape1, (Circle*)shape2);
		break;

	default:
		assert(false && "No matched collision kind");
		break;
	}

	return res;
}

bool CollisionDetection::isRecRecCollide(Rectangle* rec1, Rectangle* rec2)
{
	return false;
}

bool CollisionDetection::isRecCircCollide(Rectangle* rec, Circle* circ)
{
	return false;
}

bool CollisionDetection::isCircCircCollide(Circle* circ1, Circle* circ2)
{
	return false;
}

