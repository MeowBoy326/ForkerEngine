//
// Created by JunhaoW on 2020/12/13.
//

#include "cube.h"
#include "renderer.h"

Cube::Cube(std::string name)
    : Object(name)
{

}

Cube::Cube(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color, std::string name)
    : Object(position, rotation, scale, color, name)
{

}

Cube::~Cube() { }

void Cube::Draw(Renderer& renderer, bool drawCoordinate)
{
    renderer.Render(*this, drawCoordinate);
}