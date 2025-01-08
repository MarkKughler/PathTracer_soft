
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
    cHitObject_List world;
    auto mat_ground  = make_shared<cLambertian>(cRGB(0.1f, 0.8f, 0.1f));
    auto mat_midGray = make_shared<cLambertian>(cRGB(0.5f, 0.5f, 0.5f));
    auto mat_metal   = make_shared<cMetalic>(cRGB(0.8f, 0.8f, 0.8f), 0.3);
    auto mat_brass   = make_shared<cMetalic>(cRGB(0.8f, 0.6f, 0.2f), 1.0);

    world.Add(make_shared<cSphere>(cVec3(0.0, -100.5, -1.0), 100.0, mat_ground));
    world.Add(make_shared<cSphere>(cVec3(0.0, 0.0, -1.2), 0.5, mat_midGray));
    world.Add(make_shared<cSphere>(cVec3(-1.0, 0.0, -1.0), 0.5, mat_metal));
    world.Add(make_shared<cSphere>(cVec3(1.0, 0.0, -1.0), 0.5, mat_brass));
    
   
    cCamera camera;
    camera.aspect_ratio = 16.0 / 9.0;
    camera.image_width = 800;
    camera.samples_per_pixel = 500;
    camera.max_recursive_depth = 50;
    camera.Render(world);
    
    return 0;
}

//10.6
