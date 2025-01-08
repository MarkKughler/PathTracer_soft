#pragma once
#include "vec3.h"

class cRay
{

public:
    cRay() {}

    cRay(const cVec3& origin, const cVec3& direction) : _pt(origin), _dir(direction) {}

    const cVec3& origin() const { return _pt; }
    const cVec3& direction() const { return _dir; }

    cVec3 at(double t) const { return _pt + (t * _dir); }

private:
    cVec3 _pt;     // ray start position
    cVec3 _dir;    // ray direction

};
