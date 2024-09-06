#include "gui.h"

#include "esp.h"
#include "data.h"
#include "hooks.h"
#include "hack.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl2.h"
#include "../imgui/imgui_impl_win32.h"

#include <gl/GL.h>

//#pragma comment(lib, "opengl32.lib")

namespace gui
{
    bool isContextCreated{ false };
    HGLRC gameContext{ nullptr };
    HGLRC myContext{ nullptr };
    bool showMenu{ false };

    void createContext(HDC hdc)
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
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(data::hWndGame); // fix data::hWndGame, we might want to move it somewhere else
        ImGui_ImplOpenGL2_Init();

        isContextCreated = { true };
    }

    void startRender()
    {
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void menu()
    {
        ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);

        ImGui::Begin("Rebirth", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginTabBar("Menu", ImGuiTabBarFlags_DrawSelectedOverline | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton))
        {
            if (ImGui::BeginTabItem("hack"))
            {
                if (ImGui::CollapsingHeader("aim"))
                {
                    ImGui::Checkbox("enable##aim", &hack::toggle::enableAimbot);
                }

                if (ImGui::CollapsingHeader("map"))
                {
                    ImGui::Checkbox("show enemy on map", &hack::toggle::map);
                    ImGui::Checkbox("show enemy on minimap", &hack::toggle::minimap);
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("visual"))
            {
                if (ImGui::CollapsingHeader("esp"))
                {
                    ImGui::Checkbox("draw##esp", &esp::toggle::drawESP);
                    ImGui::Checkbox("box##esp", &esp::toggle::box);

                    ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::ColorEdit3("box color##esp", esp::color::box, ImGuiColorEditFlags_NoLabel);
                    ImGui::Checkbox("name##esp", &esp::toggle::name);

                    ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::ColorEdit3("name color##esp", esp::color::name, ImGuiColorEditFlags_NoLabel);

                    ImGui::Checkbox("health##esp", &esp::toggle::health);
                    ImGui::Checkbox("armor##esp", &esp::toggle::armor);
                    ImGui::Checkbox("distance##esp", &esp::toggle::distance);

                    ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::ColorEdit3("distance color##esp", esp::color::distance, ImGuiColorEditFlags_NoLabel);
                }

                if (ImGui::CollapsingHeader("fov"))
                {
                    ImGui::Checkbox("draw##fov", &esp::toggle::drawFOV);
                    ImGui::RadioButton("circle##fov", &esp::setting::fovType, 0);

                    ImGui::SameLine();
                    ImGui::RadioButton("rectangle##fov", &esp::setting::fovType, 1);

                    ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.5f);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::ColorEdit3("fov color##fov", esp::color::fov, ImGuiColorEditFlags_NoLabel);

                    // use ## to add id into label, carefull about same label id it can cause a conflict (## can be used to make no label)
                    // see https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-about-the-id-stack-system
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::SliderFloat("##fov thickness", &esp::setting::fovThickness, 1.0f, 10.0f, "thickness: %.3f", ImGuiSliderFlags_AlwaysClamp);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    ImGui::SliderFloat("##fov rounding", &esp::setting::fovRounding, 0.0f, 10.0f, "rouding: %.3f", ImGuiSliderFlags_AlwaysClamp);
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

    void endRender()
    {
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    void shutdownContext()
    {
        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void toggleMenu()
    {
        showMenu = !showMenu;
        ImGui::GetIO().WantCaptureMouse = showMenu;
        hooks::unhookMouse(!showMenu);
    }
}
