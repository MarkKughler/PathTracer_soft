#pragma once
#include "hitObject.h"
#include "material.h"

class cCamera
{

public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_recursive_depth = 10;
    double vfov = 90;
    cVec3 look_from = { 0.0, 0.0, 1.0 };
    cVec3 look_at = { 0.0, 0.0, 0.0 };
    cVec3 up = { 0.0, 1.0, 0.0 };
    double defocus_angle = 0.0;
    double focus_dist = 10.0;

    void Render(const cHitObject& world)
    {
        Initialize();
        
        char szProgress[8] = "\0";
        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++)
        {
            sprintf_s(szProgress, 8, " %.0f", float(image_height - (image_height - j)) / image_height * 100);
            std::clog << "\rProgress:|" << szProgress << "% " << std::flush;
            for (int i = 0; i < image_width; i++)
            {
                cRGB px_color = { 0.f, 0.f, 0.f };
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    cRay ray = GetRay(i, j);
                    px_color += RayColor(ray, max_recursive_depth, world);
                }

                float r = LinearToGamma(px_color.r * px_sample_scale);
                float g = LinearToGamma(px_color.g * px_sample_scale);
                float b = LinearToGamma(px_color.b * px_sample_scale);
                
                static const cInterval intensity(0, 0.999);
                std::cout << int(256 * intensity.clamp_t(r)) << ' '
                          << int(256 * intensity.clamp_t(g)) << ' '
                          << int(256 * intensity.clamp_t(b)) << ' ';
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
    cVec3 u, v, w;
    cVec3 defocus_disk_u;
    cVec3 defocus_disk_v;


    void Initialize()
    {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        px_sample_scale = 1.0 / samples_per_pixel;
        eye_pt = look_from;
        double theta = deg_to_rad(vfov);
        double h = std::tan(theta / 2.0);
        double viewport_height = 2.0 * h * focus_dist;
        double viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(up, w));
        v = cross(w, u);
        cVec3 viewport_u = viewport_width * u;                // viewport edge vectors
        cVec3 viewport_v = viewport_height * -v;
        px_delta_u = viewport_u / image_width;                // pixel spacing vectors
        px_delta_v = viewport_v / image_height;
        cVec3 viewport_upper_left = eye_pt - (focus_dist * w) - viewport_u / 2.0 - viewport_v / 2.0;
        px_pos_upper_left = viewport_upper_left + 0.5 * (px_delta_u + px_delta_v);

        double defocus_radius = focus_dist * std::tan(deg_to_rad(defocus_angle / 2.0));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
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
        }

        cVec3 unit_dir = unit_vector(ray.Direction());
        double a = 0.5 * (unit_dir.y + 1.0);
        return (1.0 - a) * cRGB(1.0, 1.0, 1.0) + a * cRGB(0.4, 0.6, 1.0);
    }


    cRay GetRay(int i, int j) const 
    {
        cVec3 offset = cVec3(Rnd() - 0.5, Rnd() - 0.5, 0.0);
        cVec3 px_sample = px_pos_upper_left + ((i + offset.x) * px_delta_u) + ((j + offset.y) * px_delta_v);
        cVec3 ray_origin = (defocus_angle <= 0.0) ? eye_pt : defocus_disk_sample();
        cVec3 ray_dir = px_sample - ray_origin;
        return cRay(ray_origin, ray_dir);
    }

    cVec3 defocus_disk_sample() const
    {
        cVec3 p = random_in_unit_disk();
        return eye_pt + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
    }

    cVec3 Sample_disk(double radius)
    {
        return radius * random_in_unit_disk();
    }

    cVec3 Sample_square() const
    {
        return cVec3(Rnd() - 0.5, Rnd() - 0.5, 0);
    }
};
