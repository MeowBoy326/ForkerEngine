//
// Created by JunhaoW on 2020/12/12.
// Modified version from LearnOpenGL by Joey de Vries
//

#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <map>
#include <string>

#include "texture.h"
#include "shader.h"

// Singleton class that loads textures and shaders
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>  Shaders;
    static std::map<std::string, Texture> Textures;

    // loads (and generates) a shader program
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);

    // retrieves a stored shader
    static Shader GetShader(std::string name);

    // loads (and generates) a texture from file
    static Texture LoadTexture(const char* file, bool alpha, std::string name);

    // retrieves a stored texture
    static Texture GetTexture(std::string name);

    // de-allocates all loaded resources
    static void Clear();

private:
    // private constructor
    ResourceManager() { }

    // loads and generates a shader from file
    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

    // gets source code from path
    static std::string getSourceCodeFromPath(const char* shaderPath, std::string type);

    // loads a single texture from file
    static Texture loadTextureFromFile(const char* file, bool alpha);
};

#endif //_RESOURCE_MANAGER_H_
