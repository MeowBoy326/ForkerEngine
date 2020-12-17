//
// Created by JunhaoW on 2020/12/13.
//

#include "renderer.h"
#include "resource_manager.h"
#include "directional_light.h"
#include "point_light.h"

Renderer::Renderer(const Shader& shader)
    : coordinateVAO(0)
{
    this->objectShader = shader;

    // create a coordinate shader
    ResourceManager::LoadShader("shaders/coordinate.vert", "shaders/coordinate.frag", nullptr, "coordinate");
    this->coordinateShader = ResourceManager::GetShader("coordinate");

    // create a light cube shader
    ResourceManager::LoadShader("shaders/light_cube.vert", "shaders/light_cube.frag", nullptr, "light_cube");
    this->lightCubeShader = ResourceManager::GetShader("light_cube");

    this->initCoordinateRenderData();
}

void Renderer::UpdateRenderer(glm::mat4 spaceMatrix, glm::vec3 viewPos,
    const DirectionalLight& directionalLight, const PointLight& pointLight)
{
    // object shader
    this->objectShader.SetMatrix4("spaceMatrix", spaceMatrix, true);
    this->objectShader.SetVector3f("viewPos", viewPos);

    this->objectShader.SetVector3f("dirLight.direction", directionalLight.Direction);
    this->objectShader.SetVector3f("dirLight.ambient", directionalLight.AmbientColor);
    this->objectShader.SetVector3f("dirLight.diffuse", directionalLight.DiffuseColor);
    this->objectShader.SetVector3f("dirLight.specular", directionalLight.SpecularColor);

    this->objectShader.SetVector3f("pointLight.position", pointLight.Position);
    this->objectShader.SetVector3f("pointLight.ambient", pointLight.AmbientColor);
    this->objectShader.SetVector3f("pointLight.diffuse", pointLight.DiffuseColor);
    this->objectShader.SetVector3f("pointLight.specular", pointLight.SpecularColor);
    this->objectShader.SetFloat("pointLight.constant", pointLight.AttenuationFactors.x);
    this->objectShader.SetFloat("pointLight.linear", pointLight.AttenuationFactors.y);
    this->objectShader.SetFloat("pointLight.quadratic", pointLight.AttenuationFactors.z);

    this->objectShader.SetBool("dirLight.enabled", directionalLight.Enabled);
    this->objectShader.SetBool("pointLight.enabled", pointLight.Enabled);

    // coordinate shader
    this->coordinateShader.SetMatrix4("spaceMatrix", spaceMatrix, true);

    // light cube shader
    this->lightCubeShader.SetMatrix4("spaceMatrix", spaceMatrix, true);
}

void Renderer::renderCoordinate()
{
    glDisable(GL_DEPTH_TEST);

    this->coordinateShader.Use();
    glBindVertexArray(this->coordinateVAO);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::initCoordinateRenderData()
{
    float vertices[] = {
        // position, color
        // x-axis (red)
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // y-axis (green)
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        // z-axis (blue)
        0.0f, 0.0f, 0.0f, 0.0f, 0.36f, 0.95f,
        0.0f, 0.0f, 0.5f, 0.0f, 0.36f, 0.95f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &this->coordinateVAO);
    glGenBuffers(1, &VBO);
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(this->coordinateVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}