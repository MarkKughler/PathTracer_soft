#pragma once


class cVec3
{

public:

    double x;
    double y;
    double z;

    cVec3() : x(0), y(0), z(0) {}
    cVec3(double v0, double v1, double v2) : x(v0), y(v1), z(v2) {}


    cVec3  operator -  () const  { return cVec3(-x, -y, -z); }
    cVec3& operator /= (double t) { return *this *= 1/t; }

    cVec3& operator += (const cVec3 &vec)
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
    
    double Length() const { return std::sqrt(LengthSquared()); }
    double LengthSquared() const { return x*x + y*y + z*z; }

    bool IsNearZero() const
    {
        double s = 1e-8;
        return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
    }

    static cVec3 Random()
    {
        return { Rnd(), Rnd(), Rnd() };
    }

    static cVec3 Random(double _min, double _max)
    {
        return { Rnd(_min, _max), Rnd(_min, _max), Rnd(_min, _max) };
    }
};


inline std::ostream& operator << (std::ostream& out, const cVec3& rhs)
{
    return out << rhs.x << " " << rhs.y << " " << rhs.z << " ";
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
    return (1/t) * lhs;
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
    return rhs / rhs.Length();
}

inline cVec3 random_in_unit_disk()
{
    while (1)
    {
        cVec3 p = { Rnd(-1.0, 1.0), Rnd(-1.0, 1.0), 0 };
        if (p.LengthSquared() < 1.0)
            return p;
    }
}

inline cVec3 random_unit_vector()
{
    while (1)
    {
        cVec3 p = cVec3::Random(-1.0, 1.0);
        double lensq = p.LengthSquared();
        if (1e-160 < lensq && lensq <= 1.0) return p / sqrt(lensq);
    }
}

inline cVec3 random_on_hemisphere(const cVec3& normal)
{
    cVec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline cVec3 Reflect(const cVec3& v, const cVec3& n)
{
    return v - (2 * dot(v, n)) * n;
}

inline cVec3 Refract(const cVec3& uv, const cVec3& n, double etai_over_etat)
{
    double cos_theta = std::fmin(dot(-uv, n), 1.0);
    cVec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    cVec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.LengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}