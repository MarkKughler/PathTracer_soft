#pragma once
#include "util/rgb.h"


void WritePPM(int w, int h)
{
    cRGB color;
    char szProgress[8] = "\0";

    std::cout << "P3\n" << w << " " << h << "\n255\n";
    for (int j = 0; j < h; j++)
    {
        sprintf_s(szProgress, 8, "%.0f", float(h-(h-j))/h*100);
        std::clog << "\rProgress: " << szProgress << "%" << std::flush;
        for (int i = 0; i < w; i++)
        {
            std::cout << int(255.999f * (float(i) / (w - 1))) << ' '
                      << int(255.999f * (float(j) / (h - 1))) << ' '
                      << int(0.f) << ' ';
            for (int n = 0; n < 10000; n++) int a = i + j;
        }
    }
    std::clog << "\rPPM file written to disk \n";
}