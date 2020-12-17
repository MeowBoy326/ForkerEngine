//
// Created by JunhaoW on 2020/12/12.
// Modified version from LearnOpenGL by Joey de Vries
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>

class Texture
{
public:
    unsigned int ID;
    // size
    unsigned int Width, Height;
    // format
    unsigned int Internal_Format;
    unsigned int Image_Format;
    // texture configuration
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;
    // constructor
    Texture();
    // generates texture from image data
    void Generate(unsigned int width, unsigned int height, const unsigned char* data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};

#endif //_TEXTURE_H_
