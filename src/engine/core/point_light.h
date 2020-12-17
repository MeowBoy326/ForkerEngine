//
// Created by JunhaoW on 2020/12/17.
//

#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "light.h"

const glm::vec3 INITIAL_LIGHT_POSITION(2.5f, 2.0f, -0.2f);
const glm::vec3 INITIAL_LIGHT_ATTENUATION_FACTORS(1.0f, 0.0f, 0.0f);

class PointLight : public Light
{
public:
    glm::vec3 Position;
    glm::vec3 AttenuationFactors;

    PointLight(glm::vec3 position = INITIAL_LIGHT_POSITION,
        glm::vec3 ambientColor = INITIAL_LIGHT_AMBIENT,
        glm::vec3 diffuseColor = INITIAL_LIGHT_DIFFUSE,
        glm::vec3 specularColor = INITIAL_LIGHT_SPECULAR,
        glm::vec3 attenuationFactors = INITIAL_LIGHT_ATTENUATION_FACTORS);

    void Draw(Renderer& renderer) override;
    void Reset() override;
};

#endif //_POINT_LIGHT_H_
