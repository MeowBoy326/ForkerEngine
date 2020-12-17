//
// Created by JunhaoW on 2020/12/13.
//

#ifndef _CUBE_H_
#define _CUBE_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "object.h"

class Cube : public Object
{
public:
    Cube(std::string name = "Cube");

    Cube(glm::vec3 position,
        glm::vec3 rotation = glm::vec3(0.0f),
        glm::vec3 scale = glm::vec3(1.0f),
        glm::vec3 color = glm::vec3(1.0f), std::string name = "Cube");

    ~Cube() override;

    void Draw(Renderer& renderer, bool drawCoordinate = true) override;
};

#endif //_CUBE_H_
