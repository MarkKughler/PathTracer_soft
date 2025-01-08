
#define WIN32_LEAN_AND_MEAN
#define WIN_32_EXTRA_LEAN
#include "res/targetver.h"
#include <Windows.h>
#include "nublog.h"
#include "constants.h"
#include "util/camera.h"
#include "util/hitObject.h"
#include "hitObject_list.h"
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
    world.Add(make_shared<cSphere>(cVec3(0.0, 0.0, -1.0), 0.5));
    world.Add(make_shared<cSphere>(cVec3(0.0, -100.5, -1.0), 100.0));
   
    cCamera camera;
    camera.aspect_ratio = 16.0 / 9.0;
    camera.image_width = 800;
    camera.samples_per_pixel = 500;
    camera.Render(world);
    
    return 0;
}
