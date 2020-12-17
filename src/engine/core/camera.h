//
// Created by JunhaoW on 2020/12/12.
//

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum Camera_Projection
{
    PERSPECTIVE,
    ORTHOGONAL
};

// Default camera values
const glm::vec3 POSITION(0.0f, 0.0f, 6.0f);
const glm::vec3 UP_VECTOR(0.0f, 1.0f, 0.0f);
const float     YAW                 = -90.0f;
const float     PITCH               = 0.0f;
const float     SPEED               = 2.5f;
const float     SENSITIVITY         = 0.08f;
const float     INITIAL_FOV         = 45.0f;  // y-fov
const float     MIN_FOV             = 1.0f;
const float     MAX_FOV             = 90.0f;
const float     INITIAL_ORTHO_SCALE = 0.5f;   // scale view
const float     MIN_ORTHO_SCALE     = 0.0f;
const float     MAX_ORTHO_SCALE     = 1.0f;
const float     NEAR_PLANE          = 0.1f;
const float     FAR_PLANE           = 100.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera attributes
    glm::vec3         Position;
    glm::vec3         Front;
    glm::vec3         Up;
    glm::vec3         Right;
    glm::vec3         WorldUp;
    // euler angles
    float             Yaw;
    float             Pitch;
    // camera options
    float             MovementSpeed;
    float             MouseSensitivity;
    // viewing
    float             Fov;
    float             OrthoScale;
    float             NearPlane;
    float             FarPlane;
    Camera_Projection ProjectionType;

    // Constructor w/ Vectors
    Camera(glm::vec3 position = POSITION, glm::vec3 up = UP_VECTOR,
        float yaw = YAW, float pitch = PITCH);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float viewWidth, float viewHeight);

    // Reset camera
    void Reset();

    // Processes input
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constraintPitch = true);
    void ProcessMouseScroll(float yoffset);
    void ProcessRotationUpdate();

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

#endif //_CAMERA_H_
