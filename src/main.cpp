
#define WIN32_LEAN_AND_MEAN
#define WIN_32_EXTRA_LEAN
#include "res/targetver.h"
#include <Windows.h>
#include "nublog.h"
#include "constants.h"
#include "util/camera.h"
#include "util/hitObject.h"
#include "hitObject_list.h"
#include "util/material.h"
#include "shapes/quad.h"
#include "shapes/sphere.h"



#include <crtdbg.h>
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
int main() {
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    //_CrtSetBreakAlloc(000);
    HINSTANCE instance = GetModuleHandle(0);


    // scene setup ----------------------------------------------
    auto tex_solidDrkRed   = make_shared<cSolidColor>(cRGB(0.05, 0.0, 0.0));
    auto tex_solidDrkGray  = make_shared<cSolidColor>(cRGB(0.4, 0.4, 0.4));
    auto tex_checkered     = make_shared<cCheckerTexture>(0.32, tex_solidDrkRed, tex_solidDrkGray);
    
    auto tex_uvgrid = make_shared<cImageTexture>("uvgrid.jpg");
    auto mat_uvgrid = make_shared<cAlbedo>(tex_uvgrid);

    auto tex_perlin = make_shared<cNoiseTexture>(4.0);
    auto mat_perlin = make_shared<cAlbedo>(tex_perlin);

    auto mat_checkered  = make_shared<cAlbedo>(tex_checkered);
    auto mat_midGray    = make_shared<cLambertian>(cRGB(0.5, 0.5, 0.6));
    auto mat_glass      = make_shared<cDielectric>(1.41);
    auto mat_bubble     = make_shared<cDielectric>(1.0 / 1.5);
    auto mat_brass      = make_shared<cMetalic>(cRGB(0.8, 0.6, 0.2), 0.7);
    auto mat_steel      = make_shared<cMetalic>(cRGB(0.9, 0.9, 0.9), 0.1);
    auto mat_blueSteel  = make_shared<cMetalic>(cRGB(0.3, 0.3, 0.85), 0.5);

    cHitObject_List world;
    world.Add(make_shared<cSphere>(cVec3(0.0, -700.0, -1.0), 700.0, mat_perlin)); // ground

    world.Add(make_shared<cSphere>(cVec3(-1.25, 0.5, -1.0), 0.5,  mat_checkered));
    world.Add(make_shared<cSphere>(cVec3(  0.0, 0.5, -1.2), 0.5,  mat_midGray));
    world.Add(make_shared<cSphere>(cVec3(  1.1, 0.5, -1.0), 0.5,  mat_uvgrid));

    world.Add(make_shared<cSphere>(cVec3(-1.2, 0.2, -0.05), 0.2,  mat_steel));
    world.Add(make_shared<cSphere>(cVec3(-0.8, 0.2,  0.45), 0.2,  mat_blueSteel));
    
    world.Add(make_shared<cSphere>(cVec3(-0.2, 0.2,  0.6),  0.2,  mat_glass));
    world.Add(make_shared<cSphere>(cVec3(-0.2, 0.2,  0.6),  0.18, mat_bubble));
    
   
    cCamera camera;
    camera.aspect_ratio = 16.0 / 9.0;
    camera.image_width = 800;
    camera.samples_per_pixel = 500;
    camera.max_recursive_depth = 50;
    camera.vfov = 20.0;
    camera.look_from = { -3.5, 1.0, 4.1 };
    camera.look_at = { 0.0, 0.5, -1.2 };
    camera.up = { 0.0, 1.0, 0.0 };
    camera.defocus_angle = 0.6;
    camera.focus_dist = 5.0;
    camera.Render(world);
    
    return 0;
}
