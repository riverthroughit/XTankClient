#pragma once

#include "Vec2.h"
#include <cassert>


//点到射线距离
template<typename T>
T DistFromPointToRay(Vec2<T> point, Vec2<T> rayBegPos, Vec2<T> rayDirec) {

	assert(!(rayBegPos.x == 0 && rayBegPos.y == 0) && "ilegal direc");

	//一般式参数
	T a{}, b{}, c{};

	//与坐标轴平行的情况
	if (rayBegPos.x == T(0)) {
		a = T(1), b = T(0), c = -rayBegPos.x;
	}
	else if (rayBegPos.y == T(0)) {
		a = T(0), b = T(1), c = -rayBegPos.y;
	}
	else {
		a = rayDirec.y, b = -rayBegPos.x, c = rayDirec.x * rayBegPos.y - rayDirec.y * rayBegPos.x;
	}

	T dist = abs(a * point.x + b * point.y + c) / sqrt(a * a + b * b);

	return dist;
}
