#pragma once


class cVec3
{

public:

    double x;
    double y;
    double z;

    cVec3() : x(0.0), y(0.0), z(0.0) {}
    cVec3(double v0, double v1, double v2) : x(v0), y(v1), z(v2) {}


    cVec3  operator -  () const  { return cVec3(-x, -y, -z); }
    cVec3& operator /= (double t) { return *this *= 1.0 / t;    }

    cVec3& operator += (const cVec3 & vec)
    {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }

    cVec3& operator *= (double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    
    double length() const { return std::sqrt(length_squared()); }
    double length_squared() const { return x*x + y*y + z*z; }

    static cVec3 random()
    {
        return { rnd(), rnd(), rnd() };
    }

    static cVec3 random(float _min, float _max)
    {
        return { rnd(_min, _max), rnd(_min, _max), rnd(_min, _max) };
    }
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

inline cVec3 operator * (double t, const cVec3& rhs)
{
    return cVec3(t * rhs.x, t * rhs.y, t * rhs.z);
}

inline cVec3 operator * (const cVec3 lhs, double t)
{
    return cVec3(lhs.x * t, lhs.y * t, lhs.z * t);
}

inline cVec3 operator / (const cVec3& lhs, double t)
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

inline cVec3 random_unit_vector()
{
    while (1)
    {
        cVec3 p = { rnd(-1.0, 1.0), rnd(-1.0, 1.0), rnd(-1.0, 1.0) };
        double lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1) return p / sqrt(lensq);
    }
}

inline cVec3 random_on_hemisphere(const cVec3& normal)
{
    cVec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.f)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}