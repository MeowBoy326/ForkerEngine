//
// Created by JunhaoW on 2020/12/17.
//

#include "directional_light.h"
#include "renderer.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
    : Light(ambientColor, diffuseColor, specularColor), Direction(direction)
{

}

void DirectionalLight::Draw(Renderer& renderer)
{

}

void DirectionalLight::Reset()
{
    Direction = INITIAL_LIGHT_DIRECTION;
    AmbientColor = INITIAL_LIGHT_AMBIENT;
    DiffuseColor = INITIAL_LIGHT_DIFFUSE;
    SpecularColor = INITIAL_LIGHT_SPECULAR;
}
