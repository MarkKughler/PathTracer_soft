#pragma once
#include <cmath>

class cVec3
{

public:

    float x;
    float y;
    float z;

    cVec3() : x(0.0), y(0.0), z(0.0) {}
    cVec3(float v0, float v1, float v2) : x(v0), y(v1), z(v2) {}


    cVec3  operator -  () const  { return cVec3(-x, -y, -z); }
    cVec3& operator /= (float t) { return *this *= 1 / t;    }

    cVec3& operator += (const cVec3 & vec)
    {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }

    cVec3& operator *= (float t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    
    float length() const { return std::sqrtf(length_squared()); }
    float length_squared() const { return x*x + y*y + z*z; }

};


inline std::ostream& operator << (std::ostream& out, const cVec3& rhs)
{
    return out << rhs.x << " " << rhs.y << " " << rhs.z;
}

inline cVec3 operator + (const cVec3& lhs, const cVec3& rhs)
{
    return cVec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline cVec3 operator - (const cVec3& lhs, const cVec3& rhs)
{
    return cVec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline cVec3 operator * (const cVec3& lhs, const cVec3& rhs)
{
    return cVec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

inline cVec3 operator * (float t, const cVec3& rhs)
{
    return cVec3(t * rhs.x, t * rhs.y, t * rhs.z);
}

inline cVec3 operator * (const cVec3 lhs, float t)
{
    return cVec3(lhs.x * t, lhs.y * t, lhs.z * t);
}

inline cVec3 operator / (const cVec3& lhs, float t)
{
    return (1.f / t) * lhs;
}

inline double dot(const cVec3& lhs, const cVec3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline cVec3 cross(const cVec3& lhs, const cVec3& rhs)
{
    return cVec3(lhs.y * rhs.z - lhs.z * rhs.y,
                 lhs.z * rhs.x - lhs.x * rhs.z,
                 lhs.x * rhs.y - lhs.y * rhs.x);
}

inline cVec3 unit_vector(const cVec3& rhs)
{
    return rhs / rhs.length();
}
