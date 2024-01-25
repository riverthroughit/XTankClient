#pragma once

#include"Shape.h"

class Rectangle;
class Circle;

class CollisionDetection {

	static const int RECTANGLE_RECTANGLE = 1 << Shape::RECTANGLE | 1 << Shape::RECTANGLE;
	static const int RECTANGLE_CIRCLE = 1 << Shape::RECTANGLE | 1 << Shape::CIRCLE;
	static const int CIRCLE_CIRCLE = 1 << Shape::CIRCLE | 1 << Shape::CIRCLE;

private:
	static bool isRecRecCollide(Rectangle* rec1, Rectangle* rec2);
	static bool isRecCircCollide(Rectangle* rec, Circle* circ);
	static bool isCircCircCollide(Circle* circ1, Circle* circ2);

public:
	static bool isCollide(Shape* shape1, Shape* shape2);

};