#pragma once

#include<iostream>
#include"Texture.h"
#include"Material.h"
#include<vector>
#include"Vertex.h"

class Mesh
{
private:
	unsigned noOfVertices;
	unsigned noOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void InitVAO(
		Vertex* vertexArray,
		const unsigned& noOfVertices,
		GLuint* indexArray,
		const unsigned& noOfIndices
	);

	void UpdateUniforms(Shader* shader);

	void UpdateModelMatrix();
public:
	
	Mesh(
		Vertex* vertexArray,
		const unsigned& noOfVertices,
		GLuint* indexArray,
		const unsigned& noOfIndices,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f)
	);

	~Mesh();

	void Update();
	
	void Render(Shader* shader);

	//Modifiers
	void SetPosition(const glm::vec3 position);
	void SetRotation(const glm::vec3 rotation);
	void SetScale(const glm::vec3 scale);
	
	//Translation/Rotation/Scaling
	void Move(const glm::vec3 position);
	void Rotate(const glm::vec3 rotation);
	void Scale(const glm::vec3 scale);
};

//void Mesh::InitVertexData(
//		Vertex* vertexArray,
//		const unsigned& noOfVertices,
//		GLuint* indicesArray,
//		const unsigned& noOfIndices
//)
//{
//	for (size_t i = 0; i < noOfVertices; i++)
//	{
//		this->vertices.push_back(vertexArray[i]);
//	}
//	for (size_t i = 0; i < noOfIndices; i++)
//	{
//		this->indices.push_back(indicesArray[i]);
//	}
//}

void Mesh::InitVAO(
	Vertex* vertexArray,
	const unsigned& noOfVertices,
	GLuint* indexArray,
	const unsigned& noOfIndices
)
{
	//init vertex
	this->noOfVertices = noOfVertices;
	this->noOfIndices = noOfIndices;

	//init VAO(Vertex Array) and Bind it
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//init VBO and bind
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->noOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

	//init EBO and bind
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->noOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);

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

void Mesh::UpdateUniforms(Shader* shader)
{
	shader->SetMat4("ModelMatrix", this->ModelMatrix);
}

void Mesh::UpdateModelMatrix()
{
	this->ModelMatrix = glm::mat4(1.0f);
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
}

Mesh::Mesh(
	Vertex* vertexArray,
	const unsigned& noOfVertices,
	GLuint* indexArray,
	const unsigned& noOfIndices,
	glm::vec3 position,
	glm::vec3 rotation,
	glm::vec3 scale 
)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->InitVAO(vertexArray, noOfVertices, indexArray, noOfIndices);
	this->UpdateModelMatrix();
}

void Mesh::Render(Shader* shader)
{
	//UpdateUniforms
	this->UpdateModelMatrix();
	this->UpdateUniforms(shader);
	
	//BindingShader
	shader->Use();

	//Arrays BINDING
	glBindVertexArray(this->VAO);

	//DRAW
	if (!noOfIndices)
	{
		glDrawArrays(GL_TRIANGLES, 0, this->noOfVertices);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, this->noOfIndices, GL_UNSIGNED_INT, 0);
	}
}

inline void Mesh::SetPosition(const glm::vec3 position) { this->position = position; }

inline void Mesh::SetRotation(const glm::vec3 rotation) { this->rotation = rotation; }

inline void Mesh::SetScale(const glm::vec3 scale) { this->scale = scale; }

inline void Mesh::Move(const glm::vec3 position) { this->position += position; }

inline void Mesh::Rotate(const glm::vec3 rotation) { this->rotation += rotation; }

inline void Mesh::Scale(const glm::vec3 scale) { this->scale += scale; }

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

void Mesh::Update()
{
}
