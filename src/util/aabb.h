#pragma once

class cAABB
{
public:
    cInterval x, y, z;

    cAABB() {}
    cAABB(const cInterval& _x, const cInterval& _y, const cInterval& _z) : x(_x), y(_y), z(_z)
    {
        PadToMinimums();
    }

    cAABB(const cVec3& a, const cVec3& b)
    {
        x = (a.x <= b.x) ? cInterval(a.x, b.x) : cInterval(b.x, a.x);
        y = (a.y <= b.y) ? cInterval(a.y, b.y) : cInterval(b.y, a.y);
        z = (a.z <= b.z) ? cInterval(a.z, b.z) : cInterval(b.z, a.z);
        PadToMinimums();
    }

    
    cAABB(const cAABB& b0, const cAABB& b1)
    {
        x = cInterval(b0.x, b1.x);
        y = cInterval(b0.y, b1.y);
        z = cInterval(b0.z, b1.z);
    }
    
    const cInterval& AxisInterval(int n) const
    {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool Hit(const cRay& ray, cInterval ray_t) const
    {
        const cVec3& ray_origin = ray.Origin();
        const cVec3& ray_dir = ray.Direction();
        const double dir_x_inv = 1.0 / ray_dir.x;
        const double dir_y_inv = 1.0 / ray_dir.y;
        const double dir_z_inv = 1.0 / ray_dir.z;

        auto tx0 = (x.min_t - ray_origin.x) * dir_x_inv;
        auto tx1 = (x.max_t - ray_origin.x) * dir_x_inv;
        auto ty0 = (y.min_t - ray_origin.y) * dir_y_inv;
        auto ty1 = (y.max_t - ray_origin.y) * dir_y_inv;
        auto tz0 = (z.min_t - ray_origin.z) * dir_z_inv;
        auto tz1 = (z.max_t - ray_origin.z) * dir_z_inv;

        if (tx0 < tx1)
        {
            if (tx0 > ray_t.min_t) ray_t.min_t = tx0;
            if (tx1 < ray_t.max_t) ray_t.max_t = tx1;
        }
        else {
            if (tx1 > ray_t.min_t) ray_t.min_t = tx1;
            if (tx0 < ray_t.max_t) ray_t.max_t = tx0;
        }
        if (ray_t.max_t <= ray_t.min_t) return false;

        if (ty0 < ty1)
        {
            if (ty0 > ray_t.min_t) ray_t.min_t = ty0;
            if (ty1 < ray_t.max_t) ray_t.max_t = ty1;
        }
        else {
            if (ty1 > ray_t.min_t) ray_t.min_t = ty1;
            if (ty0 < ray_t.max_t) ray_t.max_t = ty0;
        }
        if (ray_t.max_t <= ray_t.min_t) return false;

        if (tz0 < tz1)
        {
            if (tz0 > ray_t.min_t) ray_t.min_t = tz0;
            if (tz1 < ray_t.max_t) ray_t.max_t = tz1;
        }
        else {
            if (tz1 > ray_t.min_t) ray_t.min_t = tz1;
            if (tz0 < ray_t.max_t) ray_t.max_t = tz0;
        }
        if (ray_t.max_t <= ray_t.min_t) return false;

        return true;
    }

    int LongestAxisIndex() const
    {
        if (x.Size() > y.Size())
            return x.Size() > z.Size() ? 0 : 2;
        else
            return y.Size() > z.Size() ? 1 : 2;
    }
    

    static const cAABB empty, universe;

private:

    void PadToMinimums()
    {
        double delta = 0.0001;
        if (x.Size() < delta) x = x.Expand(delta);
        if (y.Size() < delta) y = y.Expand(delta);
        if (z.Size() < delta) z = z.Expand(delta);
    }

};

const cAABB cAABB::empty    = cAABB(cInterval::empty, cInterval::empty, cInterval::empty);
const cAABB cAABB::universe = cAABB(cInterval::universe, cInterval::universe, cInterval::universe);

/*
cAABB operator + (const cAABB& bbox, const cVec3& offset)
{
    return cAABB(bbox.x + offset.x, bbox.y + offset.y, bbox.z + offset.z);
}

cAABB operator + (const cVec3& offset, const cAABB& bbox)
{
    return bbox + offset;
}
*/