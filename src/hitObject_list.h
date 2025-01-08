#pragma once
#include "util/hitObject.h"
#include <vector>


class cHitObject_List : public cHitObject
{

public:
    std::vector<shared_ptr<cHitObject>> objects;

    cHitObject_List() {}
    cHitObject_List(shared_ptr<cHitObject> object) { Add(object); }

    void Clear() { objects.clear(); }

    void Add(shared_ptr<cHitObject> object) { objects.push_back(object); }

    bool Hit(const cRay& ray, cInterval ray_t, sHitData& data) const override
    {
        sHitData temp_data;
        bool hit_anything = false;
        double closest_so_far = ray_t.max_t;

        for (const auto& object : objects)
        {
            if (object->Hit(ray, cInterval(ray_t.min_t, closest_so_far), temp_data))
            {
                hit_anything = true;
                closest_so_far = temp_data.t;
                data = temp_data;
            }
        }
        return hit_anything;
    }

};
