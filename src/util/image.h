#pragma once

#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../lib/stb_image.h"

#include "../nublog.h"
#include <cstdlib>

class cImage
{

public:
    cImage() {}

    cImage(const char* filename) 
    {
        std::string filepath = "data/textures/";
        filepath += filename;
        if (Load(filepath.c_str())) LOG_INFO(filename, "loaded sucessfully");
        else LOG_ERROR(filename, "not found");
    }

    ~cImage()
    {
        delete[] bdata;
        STBI_FREE(fdata);
    }

    bool Load(const char* filename)
    {
        int n = bytes_per_pixel; // ignored out param
        bdata = stbi_load(filename, &width, &height, &n, bytes_per_pixel);
        if(bdata == nullptr) return false;

        bytes_per_scanline = width * bytes_per_pixel;
        return true;
    }

    int GetWidth() const { return (bdata == nullptr) ? 0 : width; }
    int GetHeight() const { return (bdata == nullptr) ? 0 : height; }

    const unsigned char* GetTexel(int x, int y)
    {
        static unsigned char texel[] = { 255,0,255 };
        if (bdata == nullptr) return texel;

        x = clamp(x, 0, width);
        y = clamp(y, 0, height);
        return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
    }

private:
    const int      bytes_per_pixel = 3;
    unsigned char* bdata = nullptr;
    int width = 0;
    int height = 0;
    int bytes_per_scanline = 0;

    static int clamp(int x, int low, int high)
    {
        if (x < low) return low;
        if (x < high) return x;
        return high;
    }    

    /*
    static unsigned char float_to_byte(float value) {
        if (value <= 0.0)
            return 0;
        if (1.0 <= value)
            return 255;
        return static_cast<unsigned char>(256.0 * value);
    }
    void convert_to_bytes() {
        // Convert the linear floating point pixel data to bytes, storing the resulting byte
        // data in the `bdata` member.
        int total_bytes = image_width * image_height * bytes_per_pixel;
        bdata = new unsigned char[total_bytes];
        // Iterate through all pixel components, converting from [0.0, 1.0] float values to
        // unsigned [0, 255] byte values.
        auto *bptr = bdata;
        auto *fptr = fdata;
        for (auto i=0; i < total_bytes; i++, fptr++, bptr++)
            *bptr = float_to_byte(*fptr);
    }
    */
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif
