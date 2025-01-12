#pragma once

#include "../util/aabb.h"
#include "../util/hitObject.h"

class cQuad : public cHitObject
{
public:
    cQuad(const cVec3& _Q, const cVec3& _u, const cVec3& _v, shared_ptr<cMaterialBase> _mat)
        : Q(_Q), u(_u), v(_v), mat(_mat)
    {
        SetBoundingBox();
    }

    virtual void SetBoundingBox()
    {
        auto bbox_diag1 = cAABB(Q, Q + u + v);
        auto bbox_diag2 = cAABB(Q + u, Q + v);
        bbox = cAABB(bbox_diag1, bbox_diag2);
    }

    cAABB GetBoundingBox() const override { return bbox; }

    bool Hit(const cRay& ray, cInterval ray_t, sHitData& data) const override
    {
        return false;// todo----------------------
    }

private:

    cVec3 Q;
    cVec3 u, v;
    shared_ptr<cMaterialBase> mat;
    cAABB bbox;
};
