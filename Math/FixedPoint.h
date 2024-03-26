#pragma once

#include <cmath>

class FixedPoint {

	const static int FractionalBits = 10;
	const static int FractionalMultiplier = 1 << FractionalBits;

	int val;

public:

    FixedPoint() :
        val(0)
    {

    }

    constexpr explicit FixedPoint(int intValue):
    val(intValue << FractionalBits)
    {

    }

    constexpr explicit FixedPoint(float floatValue):
        val(static_cast<int>(floatValue* FractionalMultiplier))
    {

    }

    explicit operator int() const
    {
        return val >> FractionalBits;
    }

    explicit operator float() const
    {
        return static_cast<float>(val) / FractionalMultiplier;
    }

     FixedPoint& operator += (const FixedPoint& other)
     {
         val += other.val;
         return *this;
     }

     FixedPoint& operator -= (const FixedPoint& other)
     {
         val -= other.val;
         return *this;
     }

     FixedPoint& operator *= (const FixedPoint& other)
    {
         val = (val * other.val) >> FractionalBits;
         return *this;
    }

     FixedPoint& operator /= (const FixedPoint& other)
    {
         val = (val << FractionalBits) / other.val;
         return *this;
    }

     FixedPoint operator-() const {
         FixedPoint fixedPoint(*this);
         fixedPoint.val = -fixedPoint.val;
         return fixedPoint;
     }

     FixedPoint sqrt() const {
         float floatVal = static_cast<float>(*this);
         FixedPoint fixedPoint(std::sqrt(floatVal));
         return fixedPoint;
     }

     FixedPoint abs() const {
     
         FixedPoint ans;
         ans.val = std::abs(val);
         return ans;
     }

     friend FixedPoint operator + (const FixedPoint& a, const FixedPoint& b);
     friend FixedPoint operator - (const FixedPoint& a, const FixedPoint& b);
     friend FixedPoint operator * (const FixedPoint& a, const FixedPoint& b);
     friend FixedPoint operator / (const FixedPoint& a, const FixedPoint& b);
     friend bool operator == (const FixedPoint& a, const FixedPoint& b);
     friend bool operator != (const FixedPoint& a, const FixedPoint& b);
     friend bool operator <(const FixedPoint& a, const FixedPoint& b);
     friend bool operator >(const FixedPoint& a, const FixedPoint& b);

};

inline FixedPoint operator + (const FixedPoint& a, const FixedPoint& b) {
    FixedPoint fixedPoint(a);
    fixedPoint += b;
    return fixedPoint;
}

inline FixedPoint operator - (const FixedPoint& a, const FixedPoint& b) {
    FixedPoint fixedPoint(a);
    fixedPoint -= b;
    return fixedPoint;
}

inline FixedPoint operator * (const FixedPoint& a, const FixedPoint& b) {
    FixedPoint fixedPoint(a);
    fixedPoint *= b;
    return fixedPoint;
}

inline FixedPoint operator / (const FixedPoint& a, const FixedPoint& b){
    FixedPoint fixedPoint(a);
    fixedPoint /= b;
    return fixedPoint;
}

inline bool operator==(const FixedPoint& a, const FixedPoint& b)
{
    return a.val == b.val;
}

inline bool operator!=(const FixedPoint& a, const FixedPoint& b)
{
    return !(a == b);
}

inline bool operator<(const FixedPoint& a, const FixedPoint& b)
{
    return a.val < b.val;
}

inline bool operator>(const FixedPoint& a, const FixedPoint& b)
{
    return a.val > b.val;
}

inline FixedPoint sqrt(const FixedPoint& fixedPoint)
{
    return fixedPoint.sqrt();
}

inline FixedPoint abs(const FixedPoint& fixedPoint) 
{

    return fixedPoint.abs();
}

