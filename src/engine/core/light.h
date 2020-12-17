//
// Created by JunhaoW on 2020/12/17.
//

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

const glm::vec3 INITIAL_LIGHT_AMBIENT(0.1f, 0.1f, 0.1f);
const glm::vec3 INITIAL_LIGHT_DIFFUSE(0.6f, 0.6f, 0.6f);
const glm::vec3 INITIAL_LIGHT_SPECULAR(1.0f, 1.0f, 1.0f);

class Renderer;

class Light
{
public:
    glm::vec3 AmbientColor;
    glm::vec3 DiffuseColor;
    glm::vec3 SpecularColor;
    bool      Enabled;

    Light(
        glm::vec3 ambientColor = INITIAL_LIGHT_AMBIENT,
        glm::vec3 diffuseColor = INITIAL_LIGHT_DIFFUSE,
        glm::vec3 specularColor = INITIAL_LIGHT_SPECULAR);
    virtual ~Light() { }

    virtual void Draw(Renderer& renderer) = 0;

    virtual void Reset() = 0;
};

#endif //_LIGHT_H_
