//
// Created by JunhaoW on 2020/12/13.
//

#ifndef _CUBE_RENDERER_H_
#define _CUBE_RENDERER_H_

#include "renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CubeRenderer : public Renderer
{
public:
    CubeRenderer(const Shader& shader);

    ~CubeRenderer();

    void Render(const Object& object, bool drawCoordinate = true) override;
    void Render(const PointLight& pointLight) override;

private:
    unsigned int cubeVAO;
    void initRenderData() override;
};

#endif //_CUBE_RENDERER_H_
