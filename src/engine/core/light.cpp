//
// Created by JunhaoW on 2020/12/17.
//

#include "light.h"

Light::Light(glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
    : AmbientColor(ambientColor), DiffuseColor(diffuseColor), SpecularColor(specularColor),
      Enabled(true)
{

}