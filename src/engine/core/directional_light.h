//
// Created by JunhaoW on 2020/12/17.
//

#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

#include "light.h"

const glm::vec3 INITIAL_LIGHT_DIRECTION(0.0f, 0.0f, -1.0f);

class DirectionalLight : public Light
{
public:
    glm::vec3 Direction;

    DirectionalLight(glm::vec3 direction = INITIAL_LIGHT_DIRECTION,
        glm::vec3 ambientColor = INITIAL_LIGHT_AMBIENT,
        glm::vec3 diffuseColor = INITIAL_LIGHT_DIFFUSE,
        glm::vec3 specularColor = INITIAL_LIGHT_SPECULAR);

    void Draw(Renderer &renderer) override;
    void Reset() override;
};

#endif //_DIRECTIONAL_LIGHT_H_
