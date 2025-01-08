#pragma once
#include "../util/hitObject.h"


class cSphere : public cHitObject
{

public:
    cSphere(const cVec3& center, double radius) : center(center), radius(max(0.0, radius)) {}

    bool Hit(const cRay& ray, cInterval ray_t, sHitData& data) const override
    {
        cVec3 oc = center - ray.origin();
        double a = ray.direction().length_squared();
        double h = dot(ray.direction(), oc);
        double c = oc.length_squared() - radius * radius;
        double discriminant = h * h - a * c;
        if (discriminant < 0) return false;

        double discriminant_sqrt = std::sqrt(discriminant);
        double nearest_root = (h - discriminant_sqrt) / a;
        if (!ray_t.Surrounds(nearest_root))
        {
            nearest_root = (h + discriminant_sqrt) / a;
            if (!ray_t.Surrounds(nearest_root))
                return false;
        }
        data.t = nearest_root;
        data.pos = ray.at(data.t);
        cVec3 unit_outward_normal = (data.pos - center) / radius;
        data.SetFaceNormal(ray, unit_outward_normal);
        return true;
    }

private:

    cVec3 center;
    double radius;
};

