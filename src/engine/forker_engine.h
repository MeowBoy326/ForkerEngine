//
// Created by JunhaoW on 2020/12/12.
//

#ifndef _FORKER_ENGINE_H_
#define _FORKER_ENGINE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"

class ForkerEngine
{
public:
    unsigned int Width, Height;
    GLFWwindow* MainWindow;

    Camera MainCamera;

    float MouseOffset[2];
    float ScrollOffset;
    bool  KeysPressed[1024];
    bool  KeysProcessed[1024];

    // State
    bool IsDepthTestOn;
    bool IsStencilTestOn;
    bool IsMouseControlOn;
    bool IsScrollControlOn;
    bool IsFreeLookingModeOn;

    glm::vec3 ClearColor;

    // Constructor
    ForkerEngine(GLFWwindow* window, unsigned int width, unsigned int height);
    ~ForkerEngine();

    // Init
    void Init();

    // Input
    void ProcessInput(float dt);

    // Loop
    void Update(float dt);
    void Render();

private:
    void configureRenderSetup();
};

#endif //_FORKER_ENGINE_H_
