#pragma once
#include "../util/hitObject.h"



class cSphere : public cHitObject
{

public:
    cSphere(const cVec3& center, double radius, shared_ptr<cMaterialBase> mat) 
        : center(center), radius(fmax(0.0, radius)), material(mat)
    { }

    bool Hit(const cRay& ray, cInterval ray_t, sHitData& data) const override
    {
        cVec3 oc = center - ray.Origin();
        double a = ray.Direction().LengthSquared();
        double h = dot(ray.Direction(), oc);
        double c = oc.LengthSquared() - radius * radius;
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
        data.pos = ray.At(data.t);
        cVec3 unit_outward_normal = (data.pos - center) / radius;
        data.SetFaceNormal(ray, unit_outward_normal);
        SetUV(unit_outward_normal, data.u, data.v);
        data.mat = material;

        return true;
    }

    cAABB GetBoundingBox() const override { return bbox; }

private:

    static void SetUV(const cVec3& p, double& u, double& v)
    {
        // p: point on radius one centered at origin
        // u [0, 1] of angle around Y axis from X=-1 (CW looking Y up)
        // v [0, 1] of angle from Y=-1 to Y=+1
        double theta = std::acos(-p.y);
        double phi = std::atan2(-p.z, p.x) + pi;
        u = phi / (2 * pi);
        v = theta / pi;
    }

    cVec3 center;
    double radius;
    shared_ptr<cMaterialBase> material;
    cAABB bbox;
};

