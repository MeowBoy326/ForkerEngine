//
// Created by JunhaoW on 2020/12/12.
//

#include "forker_engine.h"

#include <iostream>

#include "resource/resource_manager.h"

#include "gui/gui.h"

GUI* MainGui;

// Constructor
ForkerEngine::ForkerEngine(GLFWwindow* window, unsigned int width, unsigned int height)
    : MainWindow(window), Width(width), Height(height),
      MainCamera(),
      KeysPressed(), KeysProcessed(),
    // State
      IsDepthTestOn(true), IsStencilTestOn(false),
      IsMouseControlOn(true), IsScrollControlOn(true), IsFreeLookingModeOn(true),
      ClearColor(0.1f, 0.1f, 0.1f)
{

}

// Destructor
ForkerEngine::~ForkerEngine()
{
    delete MainGui;
}

void ForkerEngine::Init()
{
    // Shaders
    // ---------------------------------------
    // load shaders
    ResourceManager::LoadShader("shaders/light_cube.vert", "shaders/light_cube.frag", nullptr, "light_cube");
    ResourceManager::LoadShader("shaders/object.vert", "shaders/object.frag", nullptr, "object");

    // configure shaders


    // Textures
    // ----------------------------------------


    // GUI
    // ----------------------------------------
    MainGui = new GUI(*this);
    MainGui->InitStyle();
}

// Loop
// ----------------------------------------------------------------------
void ForkerEngine::Update(float dt)
{

}

void ForkerEngine::Render()
{
    this->configureRenderSetup();

    MainGui->Render();
}

// Input
// ----------------------------------------------------------------------
void ForkerEngine::ProcessInput(float dt)
{
    // quit
    if (KeysPressed[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(MainWindow, true);

    // movement
    if (KeysPressed[GLFW_KEY_W])
        MainCamera.ProcessKeyboard(FORWARD, dt);
    if (KeysPressed[GLFW_KEY_S])
        MainCamera.ProcessKeyboard(BACKWARD, dt);
    if (KeysPressed[GLFW_KEY_A])
        MainCamera.ProcessKeyboard(LEFT, dt);
    if (KeysPressed[GLFW_KEY_D])
        MainCamera.ProcessKeyboard(RIGHT, dt);
    if (KeysPressed[GLFW_KEY_SPACE])
        MainCamera.ProcessKeyboard(UP, dt);
    if (KeysPressed[GLFW_KEY_LEFT_SHIFT])
        MainCamera.ProcessKeyboard(DOWN, dt);

    // toggle mouse control
    if (KeysPressed[GLFW_KEY_T] && !KeysProcessed[GLFW_KEY_T])
    {
        this->IsMouseControlOn = !this->IsMouseControlOn;
        KeysProcessed[GLFW_KEY_T] = true;
    }

    this->IsFreeLookingModeOn = KeysPressed[GLFW_MOUSE_BUTTON_RIGHT];

    if (this->IsMouseControlOn && this->IsFreeLookingModeOn)
        MainCamera.ProcessMouseMovement(MouseOffset[0], MouseOffset[1]);

    if (this->IsScrollControlOn && this->IsFreeLookingModeOn)
        MainCamera.ProcessMouseScroll(this->ScrollOffset);

    // MainCamera.ProcessRotationUpdate();
}

void ForkerEngine::configureRenderSetup()
{
    glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    if (IsDepthTestOn)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    if (IsStencilTestOn)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);
}