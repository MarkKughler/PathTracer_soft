#pragma once
#include "hitObject.h"
#include "ray.h"
#include "rgb.h"


class cMaterialBase
{

public:
    virtual ~cMaterialBase() = default;

    virtual bool Scatter(const cRay& ray, const sHitData& data, cRGB& attenuation, cRay& scattered) const
    {
        return false;
    }

};


// ------------------------------------------------------------------------
class cLambertian : public cMaterialBase
{

public:
    cLambertian(const cRGB& _albedo) : albedo(_albedo) {}

    bool Scatter(const cRay& ray, const sHitData& data, cRGB& attenuation, cRay& scattered) const override
    {
        cVec3 scatter_dir = data.norm + random_unit_vector();
        if (scatter_dir.IsNearZero()) scatter_dir = data.norm;

        scattered = cRay(data.pos, scatter_dir);
        attenuation = albedo;
        return true;
    }

private:
    cRGB albedo;
};


// ------------------------------------------------------------------------
class cMetalic : public cMaterialBase
{
public:
    cMetalic(const cRGB& _albedo, double _fuzz) : albedo(_albedo), fuzz(_fuzz < 1 ? _fuzz : 1) {}

    bool Scatter(const cRay& ray, const sHitData& data, cRGB& attenuation, cRay& scattered) const override
    {
        cVec3 reflected = Reflect(ray.direction(), data.norm);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

        scattered = cRay(data.pos, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), data.norm) > 0);
    }

private:
    cRGB albedo;
    double fuzz;
};
