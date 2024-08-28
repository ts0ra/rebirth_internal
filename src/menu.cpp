#include "esp.h"
#include "menu.h"
#include "data.h"
#include "hooks.h"

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
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
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
    ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);

    ImGui::Begin("Rebirth", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
    
    if (ImGui::BeginTabBar("Menu", ImGuiTabBarFlags_DrawSelectedOverline | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton))
    {
        if (ImGui::BeginTabItem("hack"))
        {
			

			ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("visual"))
        {
			if (ImGui::CollapsingHeader("esp"))
			{
				ImGui::Checkbox("draw", &esp::toggle::drawESP);

                ImGui::Checkbox("box", &esp::toggle::box); ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f); ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::ColorEdit3("box color", esp::color::box, ImGuiColorEditFlags_NoLabel);
				ImGui::Checkbox("name", &esp::toggle::name); ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f); ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::ColorEdit3("name color", esp::color::name, ImGuiColorEditFlags_NoLabel);
				ImGui::Checkbox("health", &esp::toggle::health);
				ImGui::Checkbox("armor", &esp::toggle::armor);
				ImGui::Checkbox("distance", &esp::toggle::distance); ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f); ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::ColorEdit3("distance color", esp::color::distance, ImGuiColorEditFlags_NoLabel);
			}

            if (ImGui::CollapsingHeader("fov"))
            {
                ImGui::Checkbox("draw", &esp::toggle::drawFov);
				ImGui::RadioButton("circle", &esp::setting::fovType, 0); ImGui::SameLine();
				ImGui::RadioButton("rectangle", &esp::setting::fovType, 1); ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f); ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::ColorEdit3("fov color", esp::color::fov, ImGuiColorEditFlags_NoLabel);
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("misc"))
        {


            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("test"))
        {


            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

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

void gui::toggleMenu()
{
	gui::showMenu = !gui::showMenu;
	//ImGui::GetIO().WantCaptureMouse = gui::showMenu;
	//ImGui::GetIO().MouseDrawCursor = gui::showMenu;
    hooks::unhookMouse(!gui::showMenu);
}
