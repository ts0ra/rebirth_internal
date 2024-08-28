#include "menu.h"
#include "data.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl2.h"
#include "../imgui/imgui_impl_win32.h"

#include <Windows.h>
#include <gl/GL.h>

#pragma comment(lib, "opengl32.lib")

void gui::createContext(HDC hdc)
{
	data::hWindow = WindowFromDC(hdc);
    gui::myContext = wglCreateContext(hdc);
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

    ImGui_ImplWin32_Init(data::hWindow);
    ImGui_ImplOpenGL2_Init();

    gui::isContextCreated = { true };
}

void gui::startRender()
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::menu()
{
    ImGui::Begin("Example Menu");
    ImGui::Text("Hello, World!");
    ImGui::End();
}

void gui::endRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void gui::shutdownContext()
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
