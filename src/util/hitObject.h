#pragma once

class cMaterialBase;

struct sHitData
{
    cVec3 pos;
    cVec3 norm;
    shared_ptr<cMaterialBase> mat;
    double t;
    bool front_face;

    void SetFaceNormal(const cRay& ray, const cVec3& unit_outward_norm)
    {
        front_face = dot(ray.direction(), unit_outward_norm) < 0;
        norm = front_face ? unit_outward_norm : -unit_outward_norm;
    }
};


class cHitObject
{

public:
    virtual ~cHitObject() = default;
    virtual bool Hit(const cRay& ray, cInterval ray_t, sHitData& data) const = 0;

};
