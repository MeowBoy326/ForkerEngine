//
// Created by JunhaoW on 2020/12/13.
//

#include "object.h"

Object::Object(std::string name)
    : Name(name), Position(0.0f), Rotation(0.0f), Scale(1.0f), Color(1.0f),
      Enabled(true), Selected(false),
      DiffuseTexture(), SpecularTexture(), Shininess(32.0f)
{

}

Object::Object(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color, std::string name)
    : Name(name), Position(position), Rotation(rotation), Scale(scale), Color(color),
      Enabled(true), Selected(false),
      DiffuseTexture(), SpecularTexture(), Shininess(32.0f)
{

}

void Object::Reset()
{
    Position = glm::vec3(0.0f);
    Rotation = glm::vec3(0.0f);
    Scale = glm::vec3(1.0f);
    Color = glm::vec3(1.0f);
    Enabled = true;
    Selected = false;
    Shininess = 32.0f;
}