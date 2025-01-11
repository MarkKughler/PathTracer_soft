#pragma once

class cTexture
{
public:
    virtual ~cTexture() = default;
    virtual cRGB Value(double u, double v, const cVec3& p) const = 0;
};


// ------------------------------------------------------------------
class cSolidColor : public cTexture
{
public: 
    cSolidColor(const cRGB& _albedo) : albedo(_albedo) {}

    cSolidColor(float red, float green, float blue) : cSolidColor(cRGB(red, green, blue)) {}

    cRGB Value(double u, double v, const cVec3& p) const override
    {
        return albedo;
    }

private:
    cRGB albedo;
};


// ------------------------------------------------------------------
class cCheckerTexture : public cTexture
{
public:
    cCheckerTexture(double scale, shared_ptr<cSolidColor> _even, shared_ptr<cSolidColor> _odd)
        : inv_scale(1.0 / scale), even(_even), odd(_odd) {}

    cRGB Value(double u, double v, const cVec3& p) const override
    {
        int xInt = int(std::floor(inv_scale * p.x));
        int yInt = int(std::floor(inv_scale * p.y));
        int zInt = int(std::floor(inv_scale * p.z));
        bool is_even = ((xInt + yInt + zInt) % 2 == 0);
        return is_even ? even->Value(u, v, p) : odd->Value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<cSolidColor> even;
    shared_ptr<cSolidColor> odd;
};
