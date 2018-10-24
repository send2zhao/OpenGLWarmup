#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName, GLenum type=GL_TEXTURE_2D);

	void bind(const GLint texture_unit=0);
	void unbind();

	virtual ~Texture();
	inline void print() { std::cout << m_texture << std::endl; }

protected:
private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	GLuint m_texture;
	int m_width;
	int m_height;
	unsigned int m_type;

};

#endif
