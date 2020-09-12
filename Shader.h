#pragma once

#include<iostream>
#include<glew.h>
#include<glfw3.h>
#include<fstream>
#include<string>

#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

class Shader
{
private:

	const int majorVersion;
	const int minorVersion;

	GLuint ID;

	std::string LoadShaderSource(const char* fileName);
	
	GLuint LoadShader(GLenum type, const char* fileName);

	void LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader);

	void CompilingErrorCheck(GLuint shader, std::string type);
	
public:
	
	Shader(const int majorVersion, const int minorVersion, const char* vertexFile, const char* fragmentFile, const char* geometryFile);
	
	~Shader();

	void Use();

	void Unuse();

	void SetVec1i(const std::string& name, GLint count);

	void SetVec3(const std::string& name, glm::vec3 vector);
	
	void SetMat4(const std::string& name, glm::mat4 matrix);

};

Shader::Shader(const int majorVersion, const int minorVersion, const char* vertexFile, const char* fragmentFile, const char* geometryFile = "")
	: majorVersion(majorVersion) , minorVersion(minorVersion)
{
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint geometryShader = 0;

	vertexShader = LoadShader(GL_VERTEX_SHADER, vertexFile);
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentFile);

	if (geometryFile != "")
	{
		geometryShader = LoadShader(GL_GEOMETRY_SHADER, geometryFile);
	}

	this->LinkProgram(vertexShader, fragmentShader, geometryShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);	
}

Shader::~Shader()
{
	glDeleteProgram(this->ID);
}

void Shader::Use()
{
	glUseProgram(this->ID);
}

void Shader::Unuse()
{
	glUseProgram(0);
}

void Shader::SetVec1i(const std::string& name, GLint count)
{
	Use();
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), count);
	Unuse();
}

void Shader::SetVec3(const std::string& name, glm::vec3 vector)
{
	Use();
	glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, glm::value_ptr(vector));
	Unuse();
}

void Shader::SetMat4(const std::string& name, glm::mat4 matrix)
{
	Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	Unuse();
}

std::string Shader::LoadShaderSource(const char* fileName)
{
	std::string temp = "";
	std::string src = "";

	std::ifstream shaderFile;

	//For Shader
	shaderFile.open(fileName);

	if (shaderFile.is_open())
	{
		while (std::getline(shaderFile, temp))
		{
			src += temp + "\n";
		}
	}
	else
	{
		std::cout << "ERROR_LOADING_SHADER_FILE" << std::endl;
	}

	shaderFile.close();

	std::string versionNo = std::to_string(this->majorVersion) + std::to_string(this->minorVersion) + "0";
	src.replace(src.find("#version"), 12, ("#version " + versionNo));

	return src;
}

GLuint Shader::LoadShader(GLenum type, const char* fileName)
{
	GLuint SHADER = glCreateShader(type);
	std::string str_src = this->LoadShaderSource(fileName);
	const char* src = str_src.c_str();

	glShaderSource(SHADER, 1, &src, nullptr);
	glCompileShader(SHADER);

	CompilingErrorCheck(SHADER, "SHADER");

	return SHADER;
}

void Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader)
{
	this->ID = glCreateProgram();

	glAttachShader(this->ID, vertexShader);
	glAttachShader(this->ID, fragmentShader);

	if (geometryShader)
	{
		glAttachShader(this->ID, geometryShader);
	}

	glLinkProgram(this->ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	glUseProgram(0);

	CompilingErrorCheck(ID, "PROGRAM");
}

void Shader::CompilingErrorCheck(GLuint shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}