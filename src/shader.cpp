#include "shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(const std::string& fileName)
{
	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for(unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(m_program, m_shaders[i]);

	//// Now we use layout to specified position explicitly
	//glBindAttribLocation(m_program, 0, "position");
	//glBindAttribLocation(m_program, 1, "texCoord");
	//glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");


	int loc = glGetAttribLocation(m_program, "position");
	std::cout << "position: " << loc << std::endl;
	loc = glGetAttribLocation(m_program, "texCoord");
	std::cout << "textCoord: " << loc << std::endl;
	loc = glGetAttribLocation(m_program, "normal");
	std::cout << "normal: " << loc << std::endl;


	m_uniforms[0] = glGetUniformLocation(m_program, "MVP");
	m_uniforms[1] = glGetUniformLocation(m_program, "Normal");
	m_uniforms[2] = glGetUniformLocation(m_program, "lightDirection");
	std::cout << "uniform location: " << std::endl;
	for (size_t i = 0; i < 3; i++)
	{
		std::cout << m_uniforms[i] << std::endl;
	}
}

Shader::~Shader()
{
	for(unsigned int i = 0; i < NUM_SHADERS; i++)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }

	glDeleteProgram(m_program);
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

void Shader::Update(const Transform& transform, const Camera& camera, Texture& texture)
{
	glm::mat4 MVP = transform.GetMVP(camera);
	glm::mat4 Normal = transform.GetModel();

	glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
	glUniform3f(m_uniforms[2], 0.0f, 0.0f, 1.0f);

	// set up the sampler
	//glActiveTexture(GL_TEXTURE3);  // pick up the first texture buffer (use third)
	//glEnable(GL_TEXTURE_2D); // set to be 2D
	texture.bind(3); // bind the actual data to the texture
	glUniform1i(glGetUniformLocation(m_program, "sampler"), 3); // set to the shader, 3rd texture buffer

	glDrawElementsBaseVertex(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 0);
}

void Shader::Update(const Transform& transform, const Camera& camera, std::vector<std::shared_ptr<Texture>>& textures)
{
	glm::mat4 MVP = transform.GetMVP(camera);
	glm::mat4 Normal = transform.GetModel();

	// set up the sampler
	for (int i = 0; i < textures.size(); i++)
	{
		//glActiveTexture(GL_TEXTURE3);  // pick up the first texture buffer (use third)
		//glEnable(GL_TEXTURE_2D); // set to be 2D
		textures[i]->bind(3);
		glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
		glUniform1i(glGetUniformLocation(m_program, "sampler"), 3); // set to the shader, 3rd texture buffer

		// every four vetexes form a quad(face), so skip 4 each.
		glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 4 * i);
	}
}


std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
    GLuint shader = glCreateShader(type);

    if(shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = text.length();

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

    return shader;
}
