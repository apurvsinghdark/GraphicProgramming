#pragma once

#include"Mesh.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"

class Model
{
private:

	Material* material;
	Texture* overerideTextureDiffuse;
	Texture* overerideTextureSpecular;
	std::vector<Mesh*> meshes;
	glm::vec3 position;

	void UpdateUniforms();

public:
	Model(glm::vec3 position, Material* material, Texture* textureDiffuse, Texture* textureSpecular, std::vector<Mesh*>meshes);
	~Model();

	void Rotate(const glm::vec3 rotation);

	void Render(Shader* shader);
};

void Model::UpdateUniforms()
{
}

Model::Model(glm::vec3 position, Material* material, Texture* textureDiffuse, Texture* textureSpecular, std::vector<Mesh*>meshes)
{
	this->position = position;
	this->material = material;
	this->overerideTextureDiffuse = textureDiffuse;
	this->overerideTextureSpecular = textureSpecular;

	for (auto*& i : meshes)
	{
		this->meshes.push_back(new Mesh(*i));
	}
	
	for (auto& i : this->meshes)
	{
		i->Move(this->position);
		i->SetOrigin(this->position);
	}
}

Model::~Model()
{
	for (auto*& i : this->meshes)
	{
		delete i;
	}
}

void Model::Rotate(const glm::vec3 rotation)
{
	for (auto& i : this->meshes)
		i->Rotate(rotation);
}

void Model::Render(Shader* shader)
{
	//Update Uniforms
	this->UpdateUniforms();

	this->material->SendToShader(*shader);

	shader->Use();//re-Init for becoz of SetMat

	//bind N activate texture
	this->overerideTextureDiffuse->bind(0);
	this->overerideTextureSpecular->bind(1);

	//DRAW
	for (auto& i : this->meshes)
	{
		i->Render(shader);
	}

	//Unbinding
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
