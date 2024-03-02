#pragma once

#include "Math/FixedPoint.h"

template <typename T>
struct Vec2 {
	T x, y;

public:
	constexpr Vec2(const T& xx = T(0), const T& yy = T(0)) :
		x(xx), y(yy) 
	{

	}

	T X() {
		return x;
	}

	T Y() {
		return y;
	}

	T operator*(const Vec2& other)const {
		return x * other.x + y * other.y;
	}

	Vec2& operator*=(T num) {
		x *= num;
		y *= num;
		return *this;
	}

	Vec2 operator*(T num) const{
		Vec2 Vec2 = *this;
		Vec2 *= num;
		return Vec2;
	}

	Vec2& operator/=(T num) {
		x /= num;
		y /= num;
		return *this;
	}

	Vec2 operator/(T num) const {
		Vec2 Vec2 = *this;
		Vec2 /= num;
		return Vec2;
	}

	Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2 operator+(const Vec2& other) const {
		Vec2 v = *this;
		return v += other;
	}

	Vec2& operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vec2 operator-(const Vec2& other) const {
		Vec2 v = *this;
		return v -= other;
	}

	Vec2 operator-()const {
		Vec2 Vec2;
		Vec2.x = -Vec2.x;
		Vec2.y = -Vec2.y;
		return Vec2;
	}

	T squareLenth() const {
		return *this * *this;
	}

	T lenth() const {
		return sqrt(squareLenth());
	}

	Vec2 normalize() const {
		Vec2 Vec2(*this);
		T len = Vec2.lenth();
		Vec2.x /= len;
		Vec2.y /= len;
		return Vec2;
	}
};

using Vec2Fixed = Vec2<FixedPoint>; 
using Vec2f = Vec2<float>;
