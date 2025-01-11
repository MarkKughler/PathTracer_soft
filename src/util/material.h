#pragma once
#include "hitObject.h"
#include "ray.h"
#include "rgb.h"
#include "texture.h"


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
class cAlbedo : public cMaterialBase
{
public:
    cAlbedo(const cRGB& _albedo) : texture(make_shared<cSolidColor>(_albedo)) {}
    cAlbedo(shared_ptr<cTexture> _texture) : texture(_texture) {}

    bool Scatter(const cRay& ray, const sHitData& data, cRGB& attenuation, cRay& scattered) const override
    {
        cVec3 scatter_dir = data.norm + random_unit_vector();
        if (scatter_dir.IsNearZero()) scatter_dir = data.norm;

        scattered = cRay(data.pos, scatter_dir);
        attenuation = texture->Value(data.u, data.v, data.pos);
        return true;
    }

private:
    shared_ptr<cTexture> texture;
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
        cVec3 reflected = Reflect(ray.Direction(), data.norm);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

        scattered = cRay(data.pos, reflected);
        attenuation = albedo;
        return (dot(scattered.Direction(), data.norm) > 0);
    }

private:
    cRGB albedo;
    double fuzz;
};


// ------------------------------------------------------------------------
class cDielectric : public cMaterialBase
{
public:
    cDielectric(double _refraction_index) : refraction_index(_refraction_index) {}

    bool Scatter(const cRay& ray, const sHitData& data, cRGB& attenuation, cRay& scattered) const override
    {
        attenuation = cRGB(1.0f, 1.0f, 1.0f);
        double ri = data.front_face ? (1.0 / refraction_index) : refraction_index;

        cVec3 unit_direction = unit_vector(ray.Direction());
        double cos_theta = std::fmin(dot(-unit_direction, data.norm), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        
        bool cannot_refract = ri * sin_theta > 1.0;
        cVec3 direction;
        if (cannot_refract || Reflectance(cos_theta, ri) > Rnd())
            direction = Reflect(unit_direction, data.norm);
        else
            direction = Refract(unit_direction, data.norm, ri);
;
        scattered = cRay(data.pos, direction);
        return true;
    }

private:
    double refraction_index;

    static double Reflectance(double cosine, double _refraction_index)
    {
        double r0 = (1.0 - _refraction_index) / (1.0 + _refraction_index);
        r0 = r0 * r0;
        return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5.0);
    }
};
