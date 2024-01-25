#include "Vec2i.h"

#include "fixedMath.h"
#include<cassert>
#include<cmath>

int Vec2i::operator*(const Vec2i& other)
{
    return fixedMultiply(x,other.x) + fixedMultiply(y, other.y);
}

Vec2i& Vec2i::operator*=(int num)
{
    fixedMultiply(x, num);
    fixedMultiply(y, num);
    return *this;
}

Vec2i Vec2i::operator*(int num)
{
    Vec2i vec2i = *this;
    return vec2i *= num;
}

Vec2i& Vec2i::operator/=(int num)
{
    assert(num && "divided by zero");
    return *this *= fixedDivide(1, num);
}

Vec2i Vec2i::operator/(int num)
{
    Vec2i vec2i = *this;
    return vec2i /= num;
}

Vec2i& Vec2i::operator+=(const Vec2i& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vec2i Vec2i::operator+(const Vec2i& other)
{
    Vec2i v = *this;
    return v += other;
}

Vec2i& Vec2i::operator-=(const Vec2i& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2i Vec2i::operator-(const Vec2i& other)
{
    Vec2i v = *this;
    return v -= other;
}

Vec2i Vec2i::operator-() const
{
    return Vec2i(-x,-y);
}

int Vec2i::squareLenth()
{
    return *this * *this;
}

int Vec2i::lenth()
{
    //整数求平方根 待定
    return sqrt(squareLenth());
}

Vec2i& Vec2i::normalize()
{
    int len = lenth();
    fixedDivide(x, len);
    fixedDivide(y, len);
    return *this;
}
