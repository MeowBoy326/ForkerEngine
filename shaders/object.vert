#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 modelMatrix;
uniform mat4 spaceMatrix;  // projection * view

void main()
{
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0f));  // reduce from 4 to 3
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;  // normal matrix
    TexCoords = aTexCoords;

    gl_Position = spaceMatrix * vec4(FragPos, 1.0f);  // save computing capacity
}
