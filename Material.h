#ifndef Material.h

#include<glew.h>
#include<glfw3.h>
#include"Shader.h"

class Material
{
private:

	glm::vec3 ambident;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;
	GLint specularTex;

public:
	Material(
	glm::vec3 ambident,
	glm::vec3 diffuse,
	glm::vec3 specular,
	GLint diffuseTex,
	GLint specularTex
	);
	~Material();

	void SendToShader(Shader& program);

};

Material::Material(
	glm::vec3 ambident,
	glm::vec3 diffuse,
	glm::vec3 specular,
	GLint diffuseTex,
	GLint specularTex
)
{
	this->ambident = ambident;
	this->diffuse = diffuse;
	this->specular = specular;
	this->diffuseTex = diffuseTex;
	this->specularTex = specularTex;
}

Material::~Material()
{
}

void Material::SendToShader(Shader& program)
{
	program.SetVec3("material.ambident", this->ambident);
	program.SetVec3("material.diffuse", this->diffuse);
	program.SetVec3("material.specular", this->specular);

	program.SetVec1i("material.diffuseTex", this->diffuseTex);
	program.SetVec1i("material.specularTex", this->specularTex);
}

#endif // !Material.h
