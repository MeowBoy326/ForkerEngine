//
// Created by JunhaoW on 2020/12/12.
//

#include "gui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../forker_engine.h"

// constants
const char* IMGUI_FONT_FAMILY = "resources/fonts/Roboto-Regular.ttf";
const float IMGUI_FONT_SIZE = 16.0f;

const float IMGUI_FRAME_CORNER_SIZE = 2.0f;
const float IMGUI_VIEW_MARGIN       = 20.0f;

const float IMGUI_DRAG_SPEED_SLOW       = 0.001f;
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

void GUI::renderSceneInspector()
{
    ImGui::SetNextWindowBgAlpha(ImGuiWindowBgAlpha);
    ImGui::Begin("Scene Inspector");
    {
        // Object
        if (ImGui::CollapsingHeader("Object Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("1");
        }

        // Camera
        if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // position
            ImGui::Text("Transform");
            ImGui::Separator();
            ImGui::Indent();
            ImGui::DragFloat3("Position", glm::value_ptr(engine.MainCamera.Position), IMGUI_DRAG_SPEED_SLOW);
            ImGui::SliderFloat("Y-Fov", &engine.MainCamera.Fov, 1.0f, 45.0f, "%.1f");
            ImGui::Unindent();

            // yaw / pitch
            ImGui::Text("Rotation");
            ImGui::Separator();
            ImGui::Indent();

            ImGui::PushItemWidth(ImGuiGetItemWidth());
            ImGui::DragFloat("Yaw", &engine.MainCamera.Yaw, IMGUI_DRAG_SPEED_FAST, 0.0f, 0.0f, "%.1f");
            ImGui::SameLine();
            ImGui::DragFloat("Pitch", &engine.MainCamera.Pitch, IMGUI_DRAG_SPEED_FAST, 0.0f, 0.0f, "%.1f");
            ImGui::PopItemWidth();

            ImGui::DragFloat3("Front", glm::value_ptr(engine.MainCamera.Front), IMGUI_DRAG_SPEED_SLOW);
            ImGui::DragFloat3("Up", glm::value_ptr(engine.MainCamera.Up), IMGUI_DRAG_SPEED_SLOW);
            ImGui::DragFloat3("Right", glm::value_ptr(engine.MainCamera.Right), IMGUI_DRAG_SPEED_SLOW);

            ImGui::Unindent();

            //
            // // viewing
            // ImGui::Text("Viewing");
            // ImGui::Separator();
            // ImGui::Indent();
            //
            // ImGui::Unindent();

            // control
            ImGui::Text("Control");
            ImGui::Separator();
            ImGui::Indent();

            ImGui::PushItemWidth(ImGuiGetItemWidth());
            ImGui::DragFloat("Speed", &engine.MainCamera.MovementSpeed, IMGUI_DRAG_SPEED_MEDIUM, 0.0f, 0.0f, "%.2f");
            ImGui::SameLine();
            ImGui::DragFloat("Sensitivity", &engine.MainCamera.MouseSensitivity, IMGUI_DRAG_SPEED_MEDIUM, 0.0f, 0.0f, "%.2f");
            ImGui::PopItemWidth();
            ImGui::Unindent();

            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x / 2.0f - 50.0f, ImGui::GetCursorPos().y));
            if (ImGui::Button("Reset Camera"))
            {
                engine.MainCamera.Reset();
            }
        }

        // Lighting
        if (ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("1");
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
        {i
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

            // mouse control
            ImGui::Checkbox("Mouse", &engine.IsMouseControlOn);
            ImGui::SameLine();
            ImGui::Checkbox("Scroll", &engine.IsScrollControlOn);
            ImGui::SameLine();
            ImGui::Checkbox("Show FPS", &ImGuiShowInfo);
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

