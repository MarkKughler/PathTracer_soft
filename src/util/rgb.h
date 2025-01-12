#pragma once


class cRGB
{

public:
    double r;
    double g;
    double b;

    cRGB() : r(0.0), g(0.0), b(0.0) {}
    cRGB(double c0, double c1, double c2) : r(c0), g(c1), b(c2) {}

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

inline double LinearToGamma(double linear_component)
{
    if (linear_component > 0.f)
        return std::sqrt(linear_component);
    return 0.0;
}

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

inline cRGB operator * (const cRGB lhs, double t)
{
    return cRGB(lhs.r * t, lhs.g * t, lhs.b * t);
}

inline cRGB operator / (const cRGB& lhs, double t)
{
    return (1.0 / t) * lhs;
}
