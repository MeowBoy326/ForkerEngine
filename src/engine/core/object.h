//
// Created by JunhaoW on 2020/12/13.
//

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"

class Renderer;

class Object
{
public:
    // object state
    glm::vec3   Position, Rotation, Scale;
    glm::vec3   Color;
    std::string Name;
    bool        Enabled;
    bool        Selected;

    // texture
    Texture DiffuseTexture;
    Texture SpecularTexture;
    float   Shininess;

    Object(std::string name = "Object");
    Object(glm::vec3 position, glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = glm::vec3(1.0f),
        std::string name = "Object");
    virtual ~Object() { }

    // draw
    virtual void Draw(Renderer& renderer, bool drawCoordinate = true) = 0;

    void Reset();
};

#endif //_OBJECT_H_
