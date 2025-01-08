#pragma once
#include "hitObject.h"
#include "material.h"

class cCamera
{

public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 800;
    int samples_per_pixel = 10;
    int max_recursive_depth = 10;


    void Render(const cHitObject& world)
    {
        Initialize();
        
        char szProgress[8] = "\0";
        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        for (int j = 0; j < image_height; ++j)
        {
            sprintf_s(szProgress, 8, "%.0f", float(image_height - (image_height - j)) / image_height * 100);
            std::clog << "\rProgress: " << szProgress << "%" << std::flush;
            for (int i = 0; i < image_width; ++i)
            {
                cRGB px_color = { 0.f, 0.f, 0.f };
                for (int sample = 0; sample < samples_per_pixel; ++sample)
                {
                    cRay ray = GetRay(i, j);
                    px_color += RayColor(ray, max_recursive_depth, world);
                }

                float r = linear_to_gamma(px_color.r * px_sample_scale);
                float g = linear_to_gamma(px_color.g * px_sample_scale);
                float b = linear_to_gamma(px_color.b * px_sample_scale);
                
                static const cInterval intensity(0.f, 0.999f);
                std::cout << int(256.0 * intensity.clamp_t(r)) << ' '
                          << int(256.0 * intensity.clamp_t(g)) << ' '
                          << int(256.0 * intensity.clamp_t(b)) << ' ';
            }
        }
        std::clog << "\rPPM file written to disk \n";
    }


private:
    int image_height;
    double px_sample_scale;
    cVec3 eye_pt;
    cVec3 px_pos_upper_left;
    cVec3 px_delta_u;
    cVec3 px_delta_v;


    void Initialize()
    {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        px_sample_scale = 1.0 / samples_per_pixel;
        eye_pt = { 0.0, 0.0, 0.0 };
        double focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width = viewport_height * (double(image_width) / image_height);

        cVec3 viewport_u = { viewport_width, 0.0, 0.0 };      // viewport edge vectors
        cVec3 viewport_v = { 0.0, -viewport_height, 0.0 };
        px_delta_u = viewport_u / image_width;                // pixel spacing vectors
        px_delta_v = viewport_v / image_height;
        cVec3 viewport_upper_left = eye_pt - cVec3(0.0, 0.0, focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
        px_pos_upper_left = viewport_upper_left + (0.5 * (px_delta_u + px_delta_v));
    }


    cRGB RayColor(const cRay& ray, int depth, const cHitObject& world) const
    {
        if (depth <= 0) return cRGB(0.0, 0.0, 0.0);              // bounce limiter

        sHitData data;
        if (world.Hit(ray, cInterval(0.001, infinity), data)) 
        {
            cRay scattered;
            cRGB attenuation;
            if (data.mat->Scatter(ray, data, attenuation, scattered))
                return attenuation * RayColor(scattered, depth - 1, world);
            return cRGB(0.f, 0.f, 0.f);

            //cVec3 direction = random_on_hemisphere(data.norm); // naive lambert
            //cVec3 direction = data.norm + random_unit_vector();  //  true lambert

            //return 0.3f * RayColor(cRay(data.pos, direction), depth-1, world);
        }

        cVec3 unit_dir = unit_vector(ray.direction());
        double a = 0.5 * (unit_dir.y + 1.0);
        return (1.0 - a) * cRGB(1.0, 1.0, 1.0) + a * cRGB(0.4, 0.6, 1.0);
    }


    cRay GetRay(int i, int j) const 
    {
        cVec3 offset = cVec3(rnd() - 0.5, rnd() - 0.5, 0.0);
        cVec3 px_sample = px_pos_upper_left + ((i + offset.x) * px_delta_u) + ((j + offset.y) * px_delta_v);
        cVec3 ray_origin = eye_pt;
        cVec3 ray_dir = px_sample - ray_origin;
        return cRay(ray_origin, ray_dir);
    }

    cVec3 Sample_square() const
    {
        return cVec3(rnd() - 0.5, rnd() - 0.5, 0);
    }
};
