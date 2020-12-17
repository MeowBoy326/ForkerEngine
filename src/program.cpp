//
// Created by JunhaoW on 2020/12/12.
//

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/forker_engine.h"

// glfw callback function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int key, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// utility
bool checkIfRunInRetina(GLFWwindow* window, float* dpi = nullptr);

// Constants
const unsigned int INITIAL_WINDOW_WIDTH  = 1440;
const unsigned int INITIAL_WINDOW_HEIGHT = 900;

// Engine
ForkerEngine* Engine;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // uncomment this statement to fix compilation on OS X
#endif

    // glfw: window creation
    GLFWwindow* window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "ForkerEngine v1.0", nullptr, nullptr);
    glfwMakeContextCurrent(window);  // must include this before glad

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // setup callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // init Engine
    Engine = new ForkerEngine(window, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
    Engine->Init();

    // render loop
    // ----------------------------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // window size
        glfwSetWindowSize(window, Engine->Width, Engine->Height);

        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();  // IO events

        // input
        Engine->ProcessKeyboardInput(deltaTime);

        // update
        Engine->Update(deltaTime);

        // render
        Engine->Render();

        // swap buffers
        glfwSwapBuffers(window);
    }

    delete Engine;

    return 0;
}

// glfw: whenever the window size changed, this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Engine's size is always coordinate/window size
    Engine->Width  = checkIfRunInRetina(window) ? width / 2 : width;
    Engine->Height = checkIfRunInRetina(window) ? height / 2 : height;
    std::cout << "Size updated: "
              << Engine->Width << " x " << Engine->Height << " (window) | "
              << width << " x " << height << " (display)" << std::endl;
    // Viewport's size is always framebuffer/display size
    glViewport(0, 0, width, height);
}

// glfw: whenever keys get pressed, this callback is called
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            Engine->KeysPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            Engine->KeysPressed[key]   = false;
            Engine->KeysProcessed[key] = false;
        }
    }
}

// glfw: whenever the mouse's buttons get clicked, this callback is called
void mouse_button_callback(GLFWwindow* window, int key, int action, int mode)
{
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            Engine->KeysPressed[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            Engine->KeysPressed[key]   = false;
            Engine->KeysProcessed[key] = false;
        }
    }
}

// glfw: whenever the mouse moves, this callback is called
float lastX      = INITIAL_WINDOW_WIDTH / 2.0f;
float lastY      = INITIAL_WINDOW_HEIGHT / 2.0f;
bool  firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = -(ypos - lastY);  // mouse coord goes from top to bottom
    lastX = xpos;
    lastY = ypos;

    Engine->ProcessMouseInput(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Engine->ProcessScrollInput(yoffset);
}

bool checkIfRunInRetina(GLFWwindow* window, float* dpi)
{
    int windowWidth, windowHeight;
    int bufferWidth, bufferHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    if (dpi != nullptr)
        *dpi = (float) bufferWidth / (float) windowWidth;
    return bufferWidth != windowWidth;
}

