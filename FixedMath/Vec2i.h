#pragma once


class Vec2i {
	int x, y;

public:
	Vec2i(int xx = 0, int yy = 0) :x(xx), y(yy) {

	}

	int X() {
		return x;
	}

	int Y() {
		return y;
	}

	int operator*(const Vec2i& other);

	Vec2i& operator*=(int num);

	Vec2i operator*(int num);

	Vec2i& operator/=(int num);

	Vec2i operator/(int num);

	Vec2i& operator+=(const Vec2i& other);

	Vec2i operator+(const Vec2i& other);

	Vec2i& operator-=(const Vec2i& other);

	Vec2i operator-(const Vec2i& other);

	Vec2i operator-()const;

	int squareLenth();

	int lenth();

	Vec2i& normalize();
};