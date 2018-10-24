#include "texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& fileName, GLenum type)
{
	this->m_type = type;
	int width, height, numComponents;
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);
	this->m_width = width; this->m_height = height;

    if(data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;
	
    glGenTextures(1, &m_texture);
    glBindTexture(type, m_texture);
       
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
    glTexParameterf(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glActiveTexture(0);
	glBindTexture(type, 0);
    stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
	std::cout << "[delete] Texture" << std::endl;
}

void Texture::bind(const GLint texture_unit)
{
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(this->m_type, this->m_texture);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(this->m_type, 0);
}
