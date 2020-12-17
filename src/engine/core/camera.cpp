//
// Created by JunhaoW on 2020/12/12.
//

#include "camera.h"

// Constructor w/ Vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch),
      Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY),
      Fov(INITIAL_FOV), OrthoScale(INITIAL_ORTHO_SCALE),
      NearPlane(NEAR_PLANE), FarPlane(FAR_PLANE), ProjectionType(PERSPECTIVE)
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

#include <iostream>
glm::mat4 Camera::GetProjectionMatrix(float viewWidth, float viewHeight)
{
    if (ProjectionType == PERSPECTIVE)
        return glm::perspective(glm::radians(Fov), viewWidth / viewHeight, NearPlane, FarPlane);
    else if (ProjectionType == ORTHOGONAL)
    {
        float scale = 1.0f / OrthoScale;
        float right = viewWidth / viewWidth * scale;
        float top   = viewHeight / viewWidth * scale;  // keep the ratio
        return glm::ortho(-right, right, -top, top, NearPlane, FarPlane);
    }
}

void Camera::Reset()
{
    Position         = POSITION;
    Up               = UP_VECTOR;
    Yaw              = YAW;
    Pitch            = PITCH;
    MovementSpeed    = SPEED;
    MouseSensitivity = SENSITIVITY;
    Fov              = INITIAL_FOV;
    OrthoScale       = INITIAL_ORTHO_SCALE;
    NearPlane        = NEAR_PLANE;
    FarPlane         = FAR_PLANE;
    updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += Up * velocity;
    if (direction == DOWN)
        Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constraintPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    Yaw += xoffset;
    Pitch += yoffset;

    if (constraintPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();  // update Front, Right, Up
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Fov -= (float) yoffset;
    if (Fov < MIN_FOV)
        Fov = MIN_FOV;
    if (Fov > MAX_FOV)
        Fov = MAX_FOV;
}

void Camera::ProcessRotationUpdate()
{
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}