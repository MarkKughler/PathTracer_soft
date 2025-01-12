#pragma once
#include "image.h"


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
    cCheckerTexture(double scale, shared_ptr<cTexture> _even, shared_ptr<cTexture> _odd)
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
    shared_ptr<cTexture> even;
    shared_ptr<cTexture> odd;
};


class cImageTexture : public cTexture
{
public:
    cImageTexture(const char* filename) : image(filename) {}

    cRGB Value(double u, double v, const cVec3& p) const override
    {
        if (image.GetHeight() <= 0) 
            return cRGB(0.f, 0.f, 1.f);

        u = cInterval(0, 1).clamp_t(u);
        v = 1.0 - cInterval(0, 1).clamp_t(v);
        int i = int(u * image.GetWidth());
        int j = int(v * image.GetHeight());
        const unsigned char* px = image.PixelData(i, j);

        float color_scale = 1.f / 255.f;
        return cRGB(color_scale * px[0], color_scale * px[1], color_scale * px[2]);
    }

private:
    cImage image;
};
