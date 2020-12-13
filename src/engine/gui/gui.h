//
// Created by JunhaoW on 2020/12/12.
//

#ifndef _GUI_H_
#define _GUI_H_

#include <imgui/imgui.h>

class ForkerEngine;

class GUI
{
public:
    // State
    bool  ImGuiShowDemo;
    bool  ImGuiShowStyleEditor;
    bool  ImGuiShowMetrics;
    bool  ImGuiShowAbout;
    bool  ImGuiShowInfo;
    float ImGuiWindowBgAlpha;

    ImVec2 ImGuiInfoViewPos;

    GUI(ForkerEngine& engine);
    ~GUI();

    void InitStyle();
    void Render();

private:
    ForkerEngine& engine;
    void renderSceneInspector();
    void renderExtraView();
    void renderInfoView();
    void renderBuiltInView();
};

#endif //_GUI_H_
