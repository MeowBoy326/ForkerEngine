//
// Created by JunhaoW on 2020/12/12.
//

#include "forker_engine.h"

#include <iostream>
#include <vector>

#include "core/resource_manager.h"

#include "gui/gui.h"

#include "core/cube.h"
#include "core/renderer.h"
#include "core/cube_renderer.h"

GUI         * MainGui;
CubeRenderer* Renderer;

// Constructor
ForkerEngine::ForkerEngine(GLFWwindow* window, unsigned int width, unsigned int height)
    : MainWindow(window), Width(width), Height(height),
      MainCamera(),
      MainDirectionalLight(),
      MainPointLight(),
      KeysPressed(), KeysProcessed(),
    // State
      IsDepthTestOn(true), IsStencilTestOn(false),
      IsMouseControlOn(true), IsScrollControlOn(true),
      IsFreeLookingModeOn(false), IsObjectRotationModeOn(false),
      IsObjectCoordinateShown(true),
      CurrentSelectedObjectIndex(-1),
      ClearColor(0.1f, 0.1f, 0.1f)
{

}

// Destructor
ForkerEngine::~ForkerEngine()
{
    delete MainGui;
    delete Renderer;
    for (unsigned int i = 0; i < SceneObjects.size(); ++i)
    {
        delete SceneObjects[i];
    }
    SceneObjects.clear();
}

void ForkerEngine::Init()
{
    // load shaders
    ResourceManager::LoadShader("shaders/light_cube.vert", "shaders/light_cube.frag", nullptr, "light_cube");
    ResourceManager::LoadShader("shaders/object.vert", "shaders/object.frag", nullptr, "object");

    // load textures
    ResourceManager::LoadTexture("resources/textures/container2.png", true, "container_diffuse");
    ResourceManager::LoadTexture("resources/textures/container2_specular.png", true, "container_specular");
    ResourceManager::LoadTexture("resources/textures/brickwall.jpg", false, "brick_wall_diffuse");

    // renderer
    Renderer = new CubeRenderer(ResourceManager::GetShader("object"));

    // objects
    Object* cube = new Cube("Container Cube");
    cube->Position        = glm::vec3(0.0f, 0.0f, -3.0f);
    cube->DiffuseTexture  = ResourceManager::GetTexture("container_diffuse");
    cube->SpecularTexture = ResourceManager::GetTexture("container_specular");
    cube->Shininess       = 64.0f;
    SceneObjects.push_back(cube);

    cube = new Cube("Brick Cube");
    cube->DiffuseTexture = ResourceManager::GetTexture("brick_wall_diffuse");
    cube->Position = glm::vec3(1.0f, 1.0f, 1.0f);
    cube->Enabled  = false;
    SceneObjects.push_back(cube);

    CurrentSelectedObjectIndex = SceneObjects.size() ? 0 : -1;

    // lights
    // MainPointLight.Position        = glm::vec3(2.5, 2.0f, -0.2f);
    // MainDirectionalLight.Direction = glm::vec3(0.0f, 0.0f, -1.0f);

    // GUI
    // ----------------------------------------
    MainGui = new GUI(*this);
    MainGui->InitStyle();
}


// Loop
// ----------------------------------------------------------------------
void ForkerEngine::Update(float dt)
{
    for (unsigned int i = 0; i < SceneObjects.size(); ++i)
    {
        SceneObjects[i]->Selected = (this->CurrentSelectedObjectIndex == i);
    }
}

void ForkerEngine::Render()
{
    this->configureRenderSetup();

    // camera
    glm::mat4 spaceMatrix = MainCamera.GetProjectionMatrix(this->Width, this->Height) * MainCamera.GetViewMatrix();

    // update renderer (e.g. spaceMatrix)
    Renderer->UpdateRenderer(spaceMatrix, MainCamera.Position, MainDirectionalLight, MainPointLight);

    // scene objects
    for (unsigned int i = 0; i < SceneObjects.size(); ++i)
    {
        if (SceneObjects[i]->Enabled)
        {
            SceneObjects[i]->Draw(*Renderer, IsObjectCoordinateShown);
        }
    }

    MainPointLight.Draw(*Renderer);  // draw point light as a cube

    // gui
    MainGui->Render();
}

// Input
// ----------------------------------------------------------------------
void ForkerEngine::ProcessKeyboardInput(float dt)
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

    this->IsFreeLookingModeOn    = KeysPressed[GLFW_MOUSE_BUTTON_RIGHT];
    this->IsObjectRotationModeOn = KeysPressed[GLFW_MOUSE_BUTTON_LEFT] && KeysPressed[GLFW_KEY_LEFT_CONTROL];

    MainCamera.ProcessRotationUpdate();
}

void ForkerEngine::ProcessMouseInput(float xoffset, float yoffset)
{
    if (this->IsMouseControlOn && this->IsFreeLookingModeOn)
        MainCamera.ProcessMouseMovement(xoffset, yoffset);
    if (this->IsMouseControlOn && this->IsObjectRotationModeOn)
    {
        SceneObjects[this->CurrentSelectedObjectIndex]->Rotation.y += xoffset * 0.1f;
        SceneObjects[this->CurrentSelectedObjectIndex]->Rotation.x -= yoffset * 0.1f;
    }
}

void ForkerEngine::ProcessScrollInput(float offset)
{
    if (this->IsScrollControlOn && this->IsFreeLookingModeOn)
        MainCamera.ProcessMouseScroll(offset);
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