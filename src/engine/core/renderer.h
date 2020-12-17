//
// Created by JunhaoW on 2020/12/13.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class Object;
class DirectionalLight;
class PointLight;

class Renderer
{
public:
    Renderer(const Shader& shader);

    virtual ~Renderer() { };

    virtual void Render(const Object& object, bool drawCoordinate = true) = 0;
    virtual void Render(const PointLight& pointLight) = 0;

    void UpdateRenderer(glm::mat4 spaceMatrix, glm::vec3 viewPos,
        const DirectionalLight& directionalLight,
        const PointLight& pointLight);

protected:
    Shader       objectShader;
    Shader       coordinateShader;
    Shader       lightCubeShader;
    unsigned int coordinateVAO;

    virtual void initRenderData() = 0;

    void initCoordinateRenderData();
    void renderCoordinate();
};

#endif //_RENDERER_H_
