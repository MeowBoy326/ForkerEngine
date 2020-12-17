//
// Created by JunhaoW on 2020/12/17.
//

#include "point_light.h"
#include "renderer.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, glm::vec3 attenuationFactors)
    : Light(ambientColor, diffuseColor, specularColor),
      Position(position), AttenuationFactors(attenuationFactors)
{

}

void PointLight::Draw(Renderer& renderer)
{
    renderer.Render(*this);
}

void PointLight::Reset()
{
    Position = INITIAL_LIGHT_POSITION;
    AmbientColor = INITIAL_LIGHT_AMBIENT;
    DiffuseColor = INITIAL_LIGHT_DIFFUSE;
    SpecularColor = INITIAL_LIGHT_SPECULAR;
    AttenuationFactors = INITIAL_LIGHT_ATTENUATION_FACTORS;
}

