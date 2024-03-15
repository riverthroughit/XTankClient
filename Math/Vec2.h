#pragma once

#include "Math/FixedPoint.h"


template <typename T>
struct Vec2 {
	T x, y;

public:
	constexpr Vec2(const T& xx = T(), const T& yy = T()) :
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

	Vec2& operator/=(T num) {
		x /= num;
		y /= num;
		return *this;
	}

	Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	//friend Vec2<T> operator * <T>(const Vec2<T>& a, T num);
	//friend Vec2<T> operator * <T>(T num, const Vec2& a);
	//friend Vec2<T> operator / <T>(const Vec2<T>& a, T num);
	//friend Vec2<T> operator + <T>(const Vec2<T>& a, const Vec2<T>& b);
	//friend Vec2<T> operator - <T>(const Vec2<T>& a, const Vec2<T>& b);
	//friend bool operator == <T>(const Vec2<T>& a, const Vec2<T>& b);
	//friend bool operator != <T>(const Vec2<T>& a, const Vec2<T>& b);

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

template<typename T>
Vec2<T> operator * (const Vec2<T>& a, T num) {
	Vec2<T> vec2 = a;
	return vec2 *= num;
}

template<typename T>
Vec2<T> operator * (T num, const Vec2<T>& a) {
	return a * num;
}

template<typename T>
Vec2<T> operator / (const Vec2<T>& a, T num) {
	Vec2<T> vec2 = a;
	return vec2 /= num;
}

template<typename T>
Vec2<T> operator + (const Vec2<T>& a, const Vec2<T>& b) {
	Vec2<T> vec2 = a;
	return vec2 += b;
}

template<typename T>
Vec2<T> operator - (const Vec2<T>& a, const Vec2<T>& b) {
	Vec2<T> vec2 = a;
	return vec2 -= b;
}

template<typename T>
bool operator == (const Vec2<T>& a, const Vec2<T>& b) {
	return a.x == b.x && a.y == b.y;
}

template<typename T>
bool operator != (const Vec2<T>& a, const Vec2<T>& b) {
	return !(a == b);
}

using Vec2Fixed = Vec2<FixedPoint>; 
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
