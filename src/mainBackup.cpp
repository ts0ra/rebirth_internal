#include <Windows.h>
#include <thread>
#include <iostream>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "MinHook.h"
#include "data.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl2.h"
#include "../imgui/imgui_impl_win32.h"

#pragma comment(lib, "libMinHook.x86.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef LRESULT(CALLBACK* WndProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef BOOL(__stdcall* wglSwapBuffers)(HDC hdc);
typedef int(__fastcall* SDL_SetRelativeMouseMode)(int mode);
typedef void(__fastcall* mousemove)(int idx, int idy);

// Global
WndProc oWndProc = nullptr;
wglSwapBuffers owglSwapBuffers = nullptr;
wglSwapBuffers owglSwapBuffersTarget = (wglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
SDL_SetRelativeMouseMode oSDL_SetRelativeMouseMode = (SDL_SetRelativeMouseMode)GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode");
mousemove oMouseMove = nullptr;
mousemove oMouseMoveTarget = (mousemove)0x4BF780;

const ImVec2 initWindowSize = ImVec2(400, 400);
bool showMenu = false;
bool initialized = false;
bool contextCreated = false;
HWND gameWindow;
HGLRC myContext;
HGLRC gameContext;
HDC gHDC;

// Forward
BOOL WINAPI hkWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void mainThread(const HINSTANCE hinstDLL);
void toggleMenu();
void init();

void __fastcall hkMouseMove(int idx, int idy)
{
    if (showMenu)
        return;

    oMouseMove(idx, idy);
}

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

void startRender()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void render()
{
    if (!showMenu)
        return;

    ImGui::Begin("AssaultCube Hack", &showMenu);
    ImGui::Text("Hello, world!");
    ImGui::End();
}

void endRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

}

void SetupContext(HDC hdc)
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
    contextCreated = true;
}

BOOL __stdcall hkSwapBuffers(HDC hdc)
{
    if (!initialized)
    {
        init();
        owglSwapBuffers(hdc);
    }
    gameContext = wglGetCurrentContext();
    if (!contextCreated)
        SetupContext(hdc);
    wglMakeCurrent(hdc, myContext);
    startRender();
    render();
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(100, 100), IM_COL32(255, 0, 0, 255));
    endRender();

    wglMakeCurrent(hdc, gameContext);

    return owglSwapBuffers(hdc);
}

void mainThread(const HINSTANCE hinstDLL)
{
    bool enableHooking = false;

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
        (LPVOID)GetWindowLongPtr(hGameWindow, GWLP_WNDPROC),
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

    MH_STATUS mouseMoveHookStatus = MH_CreateHook(
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
    std::cout << "mouseMove hook enable\n";

    while (!GetAsyncKeyState(VK_END))
    {

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            toggleMenu();
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    MH_DisableHook(MH_ALL_HOOKS);

    MessageBox(NULL, L"DLL uninjected.", L"Uninjection Notice", MB_OK | MB_ICONINFORMATION);

    if (fDummy != 0)
        fclose(fDummy);

    if (status == MH_OK)
        MH_Uninitialize();
    FreeConsole();
    FreeLibraryAndExitThread(hinstDLL, 0);
}

void toggleMenu()
{
    showMenu = !showMenu;
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.WantCaptureMouse = showMenu;
    io.WantCaptureKeyboard = showMenu;
    io.MouseDrawCursor = showMenu;
    oSDL_SetRelativeMouseMode(!showMenu);
}

void init()
{
    ImGui::CreateContext();
    gameWindow = FindWindow(NULL, L"AssaultCube");
    ImGui_ImplWin32_Init(gameWindow);
    ImGui_ImplOpenGL2_Init();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

    ImGui::SetCurrentContext(ImGui::GetCurrentContext());
    ImGui::SetNextWindowSize(initWindowSize);

    initialized = true;
    std::cout << "Initialized" << std::endl;
}