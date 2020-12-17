#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 Color;

uniform mat4 modelMatrix;
uniform mat4 spaceMatrix;  // projection * view

void main()
{
    Color = aColor;
    gl_Position = spaceMatrix * modelMatrix * vec4(aPos, 1.0f);
}
