//
// Created by JunhaoW on 2020/12/12.
//

#ifndef _FORKER_ENGINE_H_
#define _FORKER_ENGINE_H_

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/camera.h"
#include "core/object.h"
#include "core/directional_light.h"
#include "core/point_light.h"

class ForkerEngine
{
public:
    unsigned int Width, Height;
    GLFWwindow* MainWindow;

    std::vector<Object*> SceneObjects;
    Camera               MainCamera;
    PointLight           MainPointLight;
    DirectionalLight     MainDirectionalLight;

    int CurrentSelectedObjectIndex;

    bool KeysPressed[1024];
    bool KeysProcessed[1024];

    // State
    bool IsDepthTestOn;
    bool IsStencilTestOn;
    bool IsMouseControlOn;
    bool IsScrollControlOn;
    bool IsFreeLookingModeOn;
    bool IsObjectRotationModeOn;
    bool IsObjectCoordinateShown;

    glm::vec3 ClearColor;

    // Constructor
    ForkerEngine(GLFWwindow* window, unsigned int width, unsigned int height);
    ~ForkerEngine();

    // Init
    void Init();

    // Input
    void ProcessKeyboardInput(float dt);
    void ProcessMouseInput(float xoffset, float yoffset);
    void ProcessScrollInput(float offset);

    // Loop
    void Update(float dt);
    void Render();

private:
    void configureRenderSetup();
};

#endif //_FORKER_ENGINE_H_
