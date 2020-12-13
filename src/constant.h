//
// Created by JunhaoW on 2020/12/10.
//

#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <glm/glm.hpp>

namespace Constant
{
    // Camera
    // ----------------------
    glm::vec3 InitialCameraPos(0.0f, 0.0f, 4.0f);

    // GLFW
    // ----------------------
    bool KeysPressed[1024];
    bool KeysProcessed[1024];

    unsigned int ScreenWidth              = 1440;
    unsigned int ScreenHeight             = 900;
    bool         IsScrollControlAvailable = true;
    bool         IsMouseControlAvailable  = true;
    bool         IsFreeLookingModeOn      = false;
    bool         ShowFPS                  = true;

    // OpenGL
    // ----------------------
    bool   IsDepthTestOn   = true;
    bool   IsStencilTestOn = false;
    ImVec4 ClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // ImGui
    // ----------------------
    bool IsImGuiShowDemoOn        = false;
    bool IsImGuiShowStyleEditorOn = false;
    bool IsImGuiShowMetricsOn     = false;
    bool IsImGuiShowAboutOn       = false;

    float       ImGuiWindowBgAlpha    = 0.9f;
    const float ImGuiFrameCornerWidth = 2.0f;
    const float ImGuiViewMargin       = 20.0f;
    ImVec2      ImGuiItemSpacing(8.0f, 8.0f);
    ImVec2      ImGuiSceneViewPos(ImGuiViewMargin, ImGuiViewMargin);
    ImVec2      ImGuiExtraViewPos(ImGuiViewMargin, 600);
    ImVec2      ImGuiInfoViewSize(200.0f, 58.0f);
    ImVec2      ImGuiInfoViewPos(ScreenWidth - (ImGuiInfoViewSize.x + ImGuiViewMargin), ImGuiViewMargin);

    const float ImGuiDragSpeedSlow   = 0.001f;
    const float ImGuiDragSpeedMedium = 0.01f;
    const float ImGuiDragSpeedFast   = 0.1f;

    float ImGuiGetItemWidth()
    {
        return ImGui::GetWindowSize().x / 4.65f;
    }

    void HelpMarker(const char* desc)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }
}

#endif //_CONSTANT_H_
