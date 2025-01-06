
#define WIN32_LEAN_AND_MEAN
#define WIN_32_EXTRA_LEAN
#include "res/targetver.h"
#include <Windows.h>
#include "nublog.h"
#include "util/vec3.h"
#include "util/rgb.h"
#include "ppm.h"

#ifdef NDEBUG
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ LPSTR cmd_line, _In_ int cmd_show) {
#else
#include <crtdbg.h>
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
int main() {
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    //_CrtSetBreakAlloc(000);
    HINSTANCE instance = GetModuleHandle(0);
#endif

    int image_width = 256;
    int image_height = 256;
    WritePPM(image_width, image_height);

    return 0;
}
