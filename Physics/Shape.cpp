#include "Shape.h"
#include "CollisionDetection.h"

bool Shape::IsCollideWith(Shape* shape)
{
	return CollisionDetection::IsCollide(this, shape);
}
