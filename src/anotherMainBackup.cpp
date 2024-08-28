#include <Windows.h>
#include <thread>
#include <iostream>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "MinHook.h"
#include "data.h"
#include "typedefhooks.h"
#include "console.h"
#include "hooks.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl2.h"
#include "../imgui/imgui_impl_win32.h"

#pragma comment(lib, "libMinHook.x86.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Global
WndProc oWndProc = nullptr;
WndProc oWndProcTarget = (WndProc)GetWindowLongPtr(FindWindow(NULL, L"AssaultCube"), GWLP_WNDPROC);
wglSwapBuffers owglSwapBuffers = nullptr;
wglSwapBuffers owglSwapBuffersTarget = (wglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
SDL_SetRelativeMouseMode oSDL_SetRelativeMouseMode = (SDL_SetRelativeMouseMode)GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode");
SDL_SetRelativeMouseMode oSDL_SetRelativeMouseModeTarget = (SDL_SetRelativeMouseMode)GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode");
SDL_GetRelativeMouseMode oSDL_GetRelativeMouseMode = (SDL_GetRelativeMouseMode)GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_GetRelativeMouseMode");
mousemove oMouseMove = nullptr;
mousemove oMouseMoveTarget = (mousemove)0x4BF780;

bool showMenu = false;
bool myContextCreated = false;
HGLRC gameContext;
HGLRC myContext;
HWND gameWindow = FindWindow(NULL, L"AssaultCube");

// Forward
BOOL WINAPI hkWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void mainThread(const HINSTANCE hinstDLL);
BOOL __stdcall hkSwapBuffers(HDC hdc);
void __fastcall hkMouseMove(int idx, int idy);
void RenderImgui();

BOOL WINAPI DllMain(
    _In_ HINSTANCE hinstDLL,
    _In_ DWORD     fdwReason,
    _In_ LPVOID    lpvReserved
)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

        const HANDLE mainThreadHandle = CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(mainThread),
            hinstDLL,
            0,
            nullptr
        );

        if (mainThreadHandle)
            CloseHandle(mainThreadHandle);
    }

    return TRUE;
}

void RenderImgui()
{
    ImGui_ImplOpenGL2_NewFrame();  // or ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Example ImGui window
    ImGui::Begin("Example Menu");
    ImGui::Text("Hello, World!");
    ImGui::End();

    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(100, 100), IM_COL32(255, 0, 0, 255));

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}


BOOL WINAPI hkWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if (showMenu)
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
            return 0;

        switch (msg)
        { // can be used to detect key presses to bypass some anticheat
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            return 0;
        }
    }

    return CallWindowProc(oWndProc, hwnd, msg, wParam, lParam);
}

BOOL __stdcall hkSwapBuffers(HDC hdc)
{

    gameContext = wglGetCurrentContext();

    if (!myContextCreated)
    {
        myContext = wglCreateContext(hdc);
        wglMakeCurrent(hdc, myContext);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        glOrtho(0, viewport[2], viewport[3], 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;
        io.LogFilename = NULL;
        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(gameWindow);
        ImGui_ImplOpenGL2_Init();

        myContextCreated = true;
    }

    wglMakeCurrent(hdc, myContext);

    if (showMenu)
    {
        RenderImgui();
    }

    wglMakeCurrent(hdc, gameContext);


    return owglSwapBuffers(hdc);
}

void __fastcall hkMouseMove(int idx, int idy)
{
    if (showMenu)
        return;
    oMouseMove(idx, idy);
}

void mainThread(const HINSTANCE hinstDLL)
{
    //debug::initConsole();

    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

    std::cout << "DLL injected." << std::endl;

    MH_STATUS status = MH_Initialize();

    if (status != MH_OK)
        std::cout << "MinHook failed to initialize." << std::endl;

    std::cout << "MinHook initialized." << std::endl;

    HWND hGameWindow = FindWindow(NULL, L"AssaultCube");

    MH_STATUS WndProcHookStatus = MH_CreateHook(
        (LPVOID)oWndProcTarget,
        (LPVOID)hkWndProc,
        (LPVOID*)&oWndProc
    );
    if (WndProcHookStatus != MH_OK)
        std::cout << "Failed to create WndProc hook." << std::endl;
    std::cout << "WndProc hook created." << std::endl;
    MH_STATUS WndProcenableHookStatus = MH_EnableHook((LPVOID)GetWindowLongPtr(hGameWindow, GWLP_WNDPROC));
    if (WndProcenableHookStatus != MH_OK)
        std::cout << "Failed to enable WndProc hook\n";
    std::cout << "WndProc hook enable\n";

    MH_STATUS createHookStatus = MH_CreateHook(
        (LPVOID)owglSwapBuffersTarget,
        &hkSwapBuffers,
        (LPVOID*)&owglSwapBuffers
    );
    if (createHookStatus != MH_OK)
        std::cout << "Failed to create wglSwapBuffers hook." << std::endl;
    std::cout << "wglSwapBuffers hook created." << std::endl;
    MH_STATUS enableHookStatus = MH_EnableHook((LPVOID)owglSwapBuffersTarget);
    if (enableHookStatus != MH_OK)
        std::cout << "Failed to enable wglSwapBuffers hook\n";
    std::cout << "wglSwapBuffers hook enable\n";

    /*MH_STATUS mouseMoveHookStatus = MH_CreateHook(
        (LPVOID)oMouseMoveTarget,
        &hkMouseMove,
        (LPVOID*)&oMouseMove
    );
    if (mouseMoveHookStatus != MH_OK)
        std::cout << "Failed to create mouseMove hook." << std::endl;
    std::cout << "mouseMove hook created." << std::endl;
    MH_STATUS mouseMoveEnableHookStatus = MH_EnableHook((LPVOID)oMouseMoveTarget);
    if (mouseMoveEnableHookStatus != MH_OK)
        std::cout << "Failed to enable mouseMove hook\n";
    std::cout << "mouseMove hook enable\n";*/

    while (!GetAsyncKeyState(VK_END))
    {

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            showMenu = !showMenu;
            oSDL_SetRelativeMouseMode(!showMenu);
            //std::cout << oSDL_SetRelativeMouseMode(!showMenu) << std::endl;
            std::cout << "Menu toggle: " << showMenu << '\n';
            std::cout << "Relative Mouse toggle: " << oSDL_GetRelativeMouseMode() << '\n';
        }

        //std::cout << "Relative Mouse toggle: " << oSDL_GetRelativeMouseMode() << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    MH_DisableHook(MH_ALL_HOOKS);



    ImGui_ImplOpenGL2_Shutdown();  // or ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();





    MessageBox(NULL, L"DLL uninjected.", L"Uninjection Notice", MB_OK | MB_ICONINFORMATION);

    if (status == MH_OK)
        MH_Uninitialize();

    //debug::shutdownConsole();
    if (fDummy != 0)
        fclose(fDummy);
    FreeConsole();

    FreeLibraryAndExitThread(hinstDLL, 0);
}

void mainThreadDummy(const HINSTANCE hinstDLL)
{
    debug::initConsole();
    hooks::initHooks();



    hooks::shutdownHooks();
    debug::shutdownConsole();
}