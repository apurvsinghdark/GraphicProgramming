#pragma once

#include<iostream>
#include"Texture.h"
#include"Material.h"
#include<vector>
#include"Vertex.h"

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void InitVertexData(
		Vertex* vertexArray,
		const unsigned& noOfVertices,
		GLuint* indicesArray,
		const unsigned& noOfIndices
	);

	void InitVAO();

	void InitModelMatrix();

	void UpdateUniforms(Shader* shader);
public:
	
	Mesh(
		Vertex* vertexArray,
		const unsigned& noOfVertices,
		GLuint* indicesArray,
		const unsigned& noOfIndices
	);

	~Mesh();

	void Update();
	
	void Render(Shader* shader);
};

void Mesh::InitVertexData(
		Vertex* vertexArray,
		const unsigned& noOfVertices,
		GLuint* indicesArray,
		const unsigned& noOfIndices
)
{
	for (size_t i = 0; i < noOfVertices; i++)
	{
		this->vertices.push_back(vertexArray[i]);
	}
	for (size_t i = 0; i < noOfIndices; i++)
	{
		this->indices.push_back(indicesArray[i]);
	}
}

void Mesh::InitVAO()
{
	//init VAO(Vertex Array) and Bind it
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//init VBO and bind
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

	//init EBO and bind
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), GL_STATIC_DRAW);

	// Set ATTRIBPOINTERS and ATTRIBARRAYS (Input Assembly)
	///FOR position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	/// For Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	/// For texture(Texcoord)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	//Bind Array
	glBindVertexArray(this->VAO);
}

void Mesh::InitModelMatrix()
{
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);

	this->ModelMatrix = glm::mat4(1.0f);
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
}

void Mesh::UpdateUniforms(Shader* shader)
{
	shader->SetMat4("ModelMatrix", this->ModelMatrix);
}

Mesh::Mesh(
	Vertex* vertexArray,
	const unsigned& noOfVertices,
	GLuint* indicesArray,
	const unsigned& noOfIndices
)
{
	this->InitVertexData(vertexArray, noOfVertices, indicesArray, noOfIndices);
	this->InitVAO();
	this->InitModelMatrix();
}

void Mesh::Render(Shader* shader)
{
	//UpdateUniforms
	this->UpdateUniforms(shader);
	
	//BindingShader
	shader->Use();

	//Arrays BINDING
	glBindVertexArray(this->VAO);

	//DRAW
	if (indices.empty())
	{
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	}
	else
	{
		//std::cout << sizeof(indices.data()) << std::endl;
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

void Mesh::Update()
{
}
