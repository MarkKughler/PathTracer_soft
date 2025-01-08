#pragma once


class cRGB
{

public:
    float r;
    float g;
    float b;

    cRGB() : r(0.f), g(0.f), b(0.f) {}
    cRGB(float c0, float c1, float c2) : r(c0), g(c1), b(c2) {}

    cRGB operator - () const { return cRGB(-r, -g, -b); }

    cRGB& operator += (const cRGB& c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }

    cRGB& operator *= (float t)
    {
        r *= t;
        g *= t;
        b *= t;
        return *this;
    }

};


inline std::ostream& operator << (std::ostream& out, const cRGB& rhs)
{
    return out << rhs.r << " " << rhs.g << " " << rhs.b;
}

inline cRGB operator + (const cRGB& lhs, const cRGB& rhs)
{
    return cRGB(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);
}

inline cRGB operator - (const cRGB& lhs, const cRGB& rhs)
{
    return cRGB(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b);
}

inline cRGB operator * (const cRGB& lhs, const cRGB& rhs)
{
    return cRGB(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
}

inline cRGB operator * (double t, const cRGB& rhs)
{
    return cRGB(t * rhs.r, t * rhs.g, t * rhs.b);
}

inline cRGB operator * (const cRGB lhs, float t)
{
    return cRGB(lhs.r * t, lhs.g * t, lhs.b * t);
}

inline cRGB operator / (const cRGB& lhs, float t)
{
    return (1.f / t) * lhs;
}
