//
// Created by JunhaoW on 2020/12/12.
//

#include "gui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../forker_engine.h"
#include "../core/resource_manager.h"
#include "../core/cube.h"

// constants
const char* IMGUI_FONT_FAMILY = "resources/fonts/Roboto-Regular.ttf";
const float IMGUI_FONT_SIZE = 16.0f;

const float IMGUI_FRAME_CORNER_SIZE = 2.0f;
const float IMGUI_VIEW_MARGIN       = 20.0f;

const float IMGUI_DRAG_SPEED_SUPER_SLOW = 0.001f;
const float IMGUI_DRAG_SPEED_SLOW       = 0.005f;
const float IMGUI_DRAG_SPEED_MEDIUM     = 0.01f;
const float IMGUI_DRAG_SPEED_FAST       = 0.1f;
const float IMGUI_DRAG_SPEED_SUPER_FAST = 1.0f;

const ImVec2 IMGUI_ITEM_SPACING   = ImVec2(8.0f, 8.0f);
const ImVec2 IMGUI_INFO_VIEW_SIZE = ImVec2(200.0f, 58.0f);

float ImGuiGetItemWidth();
void HelpMarker(const char* desc);

GUI::GUI(ForkerEngine& engine)
    : engine(engine),
      ImGuiShowDemo(false), ImGuiShowStyleEditor(false), ImGuiShowMetrics(false),
      ImGuiShowAbout(false), ImGuiShowInfo(true), ImGuiWindowBgAlpha(0.9f)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(engine
        .MainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiInfoViewPos = ImVec2(engine.Width - (IMGUI_INFO_VIEW_SIZE.x + IMGUI_VIEW_MARGIN), IMGUI_VIEW_MARGIN);
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::InitStyle()
{
    ImGuiIO& io = ImGui::GetIO();
    // (void) io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // font
    const float dpi = 2.0f;
    ImGui::GetIO().FontGlobalScale = 1 / dpi;
    ImGui::GetIO().Fonts->AddFontFromFileTTF(IMGUI_FONT_FAMILY, IMGUI_FONT_SIZE * dpi);

    // style
    ImGui::GetStyle().FrameRounding = IMGUI_FRAME_CORNER_SIZE;
    ImGui::GetStyle().ItemSpacing   = IMGUI_ITEM_SPACING;
}

void GUI::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    this->renderSceneInspector();
    this->renderExtraView();
    this->renderInfoView();
    this->renderBuiltInView();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#include <iostream>
#include <sstream>

void GUI::renderSceneInspector()
{
    ImGui::SetNextWindowBgAlpha(ImGuiWindowBgAlpha);
    ImGui::Begin("Scene Inspector");
    {
        // Object
        if (ImGui::CollapsingHeader("Object Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushID("Object Settings");

            int& index = engine.CurrentSelectedObjectIndex;

            if (index >= 0)
            {
                Object* currentObject = engine.SceneObjects[index];

                ImGui::PushItemWidth(ImGui::GetWindowSize().x - 100.0f);
                std::stringstream ss;
                ss << index + 1 << ". " << currentObject->Name;
                if (ImGui::BeginCombo("##SceneObjectCombo", ss.str().c_str()))
                {
                    for (unsigned int i = 0; i < engine.SceneObjects.size(); ++i)
                    {
                        bool selected = (i == engine.CurrentSelectedObjectIndex);
                        std::stringstream ss;
                        ss << i + 1 << ". " << engine.SceneObjects[i]->Name;
                        if (ImGui::Selectable(ss.str().c_str(), selected))
                        {
                            engine.CurrentSelectedObjectIndex = i;
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopItemWidth();

                ImGui::SameLine();

                ImGui::Checkbox("##EnableObject", &currentObject->Enabled);

                ImGui::SameLine();

                if (ImGui::Button("Reset"))
                {
                    currentObject->Reset();
                }

                ImGui::Separator();

                ImGui::DragFloat3("Position", glm::value_ptr(currentObject->Position), IMGUI_DRAG_SPEED_SLOW, 0.0f, 0.0f, "%.3f");
                ImGui::DragFloat3("Rotation", glm::value_ptr(currentObject->Rotation), IMGUI_DRAG_SPEED_FAST, 0.0f, 0.0f, "%.2f");
                ImGui::DragFloat3("Scale", glm::value_ptr(currentObject->Scale), IMGUI_DRAG_SPEED_MEDIUM, 0.0f, 0.0f, "%.2f");

                ImGui::Separator();

                ImGui::ColorEdit3("Color", glm::value_ptr(currentObject->Color));
                ImGui::Checkbox("Coordinate", &engine.IsObjectCoordinateShown);

                ImGui::Separator();

                // image
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right

                ImVec4 tint_col   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
                ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

                ImGui::Text("Diffuse:");
                ImGui::SameLine();
                ImGui::Image((void*) (intptr_t) currentObject->DiffuseTexture.ID, ImVec2(38, 38), uv_min, uv_max, tint_col, border_col);
                ImGui::SameLine();
                ImGui::Text("Specular:");
                ImGui::SameLine();
                ImGui::Image((void*) (intptr_t) currentObject->SpecularTexture.ID, ImVec2(38, 38), uv_min, uv_max, tint_col, border_col);
            }



            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2.0f - 100.0f, ImGui::GetCursorPos().y));
            if (ImGui::Button("Add Cube", ImVec2(80.0f, 0.0f)))
            {
                Object* cube = new Cube("Container Cube");
                cube->DiffuseTexture  = ResourceManager::GetTexture("container_diffuse");
                cube->SpecularTexture = ResourceManager::GetTexture("container_specular");
                engine.SceneObjects.push_back(cube);
                engine.CurrentSelectedObjectIndex = engine.SceneObjects.size() - 1;
            }
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.44f, 0.17f, 0.17f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.70f, 0.27f, 0.27f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85f, 0.31f, 0.31f, 1.0f));
            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2.0f + 20.0f, ImGui::GetCursorPos().y));

            if (ImGui::Button("Delete :(", ImVec2(80.0f, 0.0f)))
            {
                if (index >= 0)
                {
                    delete engine.SceneObjects[index];
                    engine.SceneObjects.erase(engine.SceneObjects.begin() + index);
                    if (index == engine.SceneObjects.size())
                        --index;
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::PopID();
        }

        // Camera
        if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushID("Camera Settings");

            ImGui::DragFloat3("Position", glm::value_ptr(engine.MainCamera.Position), IMGUI_DRAG_SPEED_SLOW);

            // ImGui::DragFloat3("Front", glm::value_ptr(engine.MainCamera.Front), IMGUI_DRAG_SPEED_SLOW);

            ImGui::PushItemWidth(ImGuiGetItemWidth());
            ImGui::DragFloat("Yaw", &engine.MainCamera.Yaw, IMGUI_DRAG_SPEED_FAST, 0.0f, 0.0f, "%.1f");
            ImGui::SameLine();
            ImGui::DragFloat("Pitch", &engine.MainCamera.Pitch, IMGUI_DRAG_SPEED_FAST, 0.0f, 0.0f, "%.1f");
            ImGui::PopItemWidth();

            ImGui::Text("LookAt Direction: (%.3f, %.3f, %.3f)",
                engine.MainCamera.Front.x, engine.MainCamera.Front.y,
                engine.MainCamera.Front.z);

            // viewing
            ImGui::Separator();

            if (engine.MainCamera.ProjectionType == PERSPECTIVE)
            {
                ImGui::SliderFloat("Y-Fov", &engine.MainCamera.Fov, MIN_FOV, MAX_FOV, "%.1f");
            }
            else
            {
                ImGui::SliderFloat("Scale", &engine.MainCamera.OrthoScale, MIN_ORTHO_SCALE, MAX_ORTHO_SCALE, "%.2f");
            }

            ImGui::PushItemWidth(ImGuiGetItemWidth());
            ImGui::DragFloat("Near", &engine.MainCamera.NearPlane, IMGUI_DRAG_SPEED_FAST, 0.0f, 0.0f, "%.1f");
            ImGui::SameLine();
            ImGui::DragFloat("Far", &engine.MainCamera.FarPlane, IMGUI_DRAG_SPEED_FAST, 0.0f, 0.0f, "%.1f");
            ImGui::PopItemWidth();
            ImGui::SameLine();
            HelpMarker("Near Plane / Far Plane");

            ImGui::RadioButton("Perspective", (int*) &engine.MainCamera.ProjectionType, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Orthogonal", (int*) &engine.MainCamera.ProjectionType, 1);

            ImGui::Separator();

            // control
            ImGui::PushItemWidth(ImGuiGetItemWidth());
            ImGui::DragFloat("Speed", &engine.MainCamera.MovementSpeed, IMGUI_DRAG_SPEED_MEDIUM, 0.0f, 0.0f, "%.2f");
            ImGui::SameLine();
            ImGui::DragFloat("Sensitivity", &engine.MainCamera.MouseSensitivity, IMGUI_DRAG_SPEED_MEDIUM, 0.0f, 0.0f, "%.2f");
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2.0f - 50.0f, ImGui::GetCursorPos().y));
            if (ImGui::Button("Reset Camera"))
            {
                engine.MainCamera.Reset();
            }

            ImGui::PopID();
        }

        // Lighting
        if (ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushID("Light Settings");

            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar("LightTypeTabBar", tab_bar_flags))
            {
                if (ImGui::BeginTabItem("Point Light"))
                {
                    PointLight& pointLight = engine.MainPointLight;

                    ImGui::Text("Enable Point Light:");
                    ImGui::SameLine();
                    ImGui::Checkbox("##EnablePointLight", &pointLight.Enabled);
                    ImGui::SameLine();
                    if (ImGui::Button("Reset"))
                    {
                        pointLight.Reset();
                    }

                    ImGui::Separator();

                    ImGui::ColorEdit3("Ambient", glm::value_ptr(pointLight.AmbientColor));
                    ImGui::ColorEdit3("Diffuse", glm::value_ptr(pointLight.DiffuseColor));
                    ImGui::ColorEdit3("Specular", glm::value_ptr(pointLight.SpecularColor));

                    ImGui::DragFloat3("Factors", glm::value_ptr(pointLight.AttenuationFactors), IMGUI_DRAG_SPEED_SUPER_SLOW, 0.0f, 1.0f);
                    ImGui::SameLine();
                    HelpMarker("Attenuation Factors: constant / linear / quadratic");

                    ImGui::Separator();

                    ImGui::DragFloat3("Position", glm::value_ptr(pointLight.Position), IMGUI_DRAG_SPEED_SLOW);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Directional Light"))
                {
                    DirectionalLight& dirLight = engine.MainDirectionalLight;

                    ImGui::Text("Enable Directional Light:");
                    ImGui::SameLine();
                    ImGui::Checkbox("##EnableDirectionalLight", &dirLight.Enabled);
                    ImGui::SameLine();
                    if (ImGui::Button("Reset"))
                    {
                        dirLight.Reset();
                    }

                    ImGui::Separator();

                    ImGui::ColorEdit3("Ambient", glm::value_ptr(dirLight.AmbientColor));
                    ImGui::ColorEdit3("Diffuse", glm::value_ptr(dirLight.DiffuseColor));
                    ImGui::ColorEdit3("Specular", glm::value_ptr(dirLight.SpecularColor));

                    ImGui::Separator();

                    ImGui::DragFloat3("Direction", glm::value_ptr(dirLight.Direction), IMGUI_DRAG_SPEED_SLOW);

                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }

            ImGui::PopID();
        }
    }
    ImGui::End();
}

void GUI::renderExtraView()
{
    // Extra Options
    // ---------------------------------------
    ImGui::SetNextWindowBgAlpha(ImGuiWindowBgAlpha);
    ImGui::Begin("Extra Options");
    {
        // GLFW
        if (ImGui::CollapsingHeader("GLFW", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // window size
            ImGui::Text("Window Size: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(ImGuiGetItemWidth());
            ImGui::InputInt("##ScreenWidth", (int*) &engine.Width, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::SameLine();
            ImGui::Text(" x ");
            ImGui::SameLine();
            ImGui::InputInt("##ScreenHeight", (int*) &engine.Height, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::PopItemWidth();

            ImGui::Checkbox("FPS", &ImGuiShowInfo);
            ImGui::SameLine();

            // mouse control
            ImGui::Checkbox("Mouse", &engine.IsMouseControlOn);
            ImGui::SameLine();
            ImGui::Checkbox("Scroll", &engine.IsScrollControlOn);
            ImGui::SameLine();
            HelpMarker("[Rotate Object] Hold CTRL + Hold Mouse Left Button\n"
                       "[Look Around] Hold Mouse Right Button\n"
                       "[Move Camera] Press W/A/S/D/Shift/Space\n"
                       "[Change Fov] Hold Mouse Right Button + Scroll");
        }

        // OpenGL
        if (ImGui::CollapsingHeader("OpenGL", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // version
            int majorVersion, minorVersion, revisionNumber;
            glfwGetVersion(&majorVersion, &minorVersion, &revisionNumber);
            ImGui::Text("Version: %d.%d.%d", majorVersion, minorVersion, revisionNumber);
            // color
            ImGui::ColorEdit3("Clear Color", glm::value_ptr(engine.ClearColor));
            // test
            ImGui::Checkbox("Depth Test", &engine.IsDepthTestOn);
            ImGui::SameLine();
            ImGui::Checkbox("Stencil Test", &engine.IsStencilTestOn);
        }

        // ImGui
        if (ImGui::CollapsingHeader("ImGui", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // version
            ImGui::Text("Version: %s", ImGui::GetVersion());

            ImGui::SliderFloat("Bg Alpha", &ImGuiWindowBgAlpha, 0.0f, 1.0f, "%.2f");

            // built-in views
            ImGui::Checkbox("Demo View", &ImGuiShowDemo);
            ImGui::SameLine();
            ImGui::Checkbox("Style Editor", &ImGuiShowStyleEditor);
            ImGui::Checkbox("Metrics View", &ImGuiShowMetrics);
            ImGui::SameLine();
            ImGui::Checkbox("About ImGui", &ImGuiShowAbout);
        }
    }
    ImGui::End();
}

void GUI::renderInfoView()
{
    if (ImGuiShowInfo)
    {
        ImGui::SetNextWindowSize(IMGUI_INFO_VIEW_SIZE);
        ImGui::SetNextWindowPos(ImGuiInfoViewPos);
        ImGui::SetNextWindowBgAlpha(ImGuiWindowBgAlpha);
        ImGui::Begin("Info View", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

        if (ImGui::BeginTable("##InfoTable", 2))
        {
            // row 0
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("fps");
            ImGui::SameLine();
            HelpMarker("Frames Per Second");
            ImGui::TableNextColumn();
            ImGui::Text(":  %d", (int) ImGui::GetIO().Framerate);

            // row 1
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("ms/frame");
            ImGui::TableNextColumn();
            ImGui::Text(":  %.2f ms", 1000 / ImGui::GetIO().Framerate);

            ImGui::EndTable();
        }

        ImGui::End();
    }
}

void GUI::renderBuiltInView()
{
    if (ImGuiShowDemo)
        ImGui::ShowDemoWindow(&ImGuiShowDemo);
    if (ImGuiShowStyleEditor)
        ImGui::ShowStyleEditor();
    if (ImGuiShowMetrics)
        ImGui::ShowMetricsWindow(&ImGuiShowMetrics);
    if (ImGuiShowAbout)
        ImGui::ShowAboutWindow(&ImGuiShowAbout);
}

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

