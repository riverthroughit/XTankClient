#include "Shape.h"
#include "CollisionDetection.h"

bool Shape::isCollideWith(Shape* shape)
{
	return CollisionDetection::isCollide(this, shape);
}
