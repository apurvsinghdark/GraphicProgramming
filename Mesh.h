#pragma once

#include<iostream>
#include"Texture.h"
#include"Material.h"
#include<vector>
#include"Vertex.h"
#include"Primitives.h"

class Mesh
{
private:
	Vertex* vertexArray;
	unsigned noOfVertices;
	GLuint* indexArray;
	unsigned noOfIndices;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void InitVAO();

	void UpdateUniforms(Shader* shader);

	void UpdateModelMatrix();
public:
	
	Mesh(
		Primitive* primitive,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 origin = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f)
	);

	Mesh(
		Vertex* vertexArray,
		const unsigned& noOfVertices,
		GLuint* indexArray,
		const unsigned& noOfIndices,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 origin = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f)
	);

	Mesh(const Mesh& obj);

	~Mesh();

	void Update();
	
	void Render(Shader* shader);

	//Modifiers
	void SetPosition(const glm::vec3 position);
	void SetOrigin(const glm::vec3 origin);
	void SetRotation(const glm::vec3 rotation);
	void SetScale(const glm::vec3 scale);
	
	//Translation/Rotation/Scaling
	void Move(const glm::vec3 position);
	void Rotate(const glm::vec3 rotation);
	void Scale(const glm::vec3 scale);
};


void Mesh::InitVAO()
{
	//init VAO(Vertex Array) and Bind it
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//init VBO and bind
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->noOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

	//init EBO and bind
	if ( this->noOfIndices > 0 )
	{
		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->noOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
	}

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
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
	this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
}

Mesh::Mesh(Primitive* primitive, glm::vec3 position, glm::vec3 origin, glm::vec3 rotation, glm::vec3 scale)
{
	this->position = position;
	this->origin = origin;
	this->rotation = rotation;
	this->scale = scale;

	this->noOfVertices = primitive->GetNoOfVertices();
	this->noOfIndices = primitive->GetNoOfIndices();

	this->vertexArray = new Vertex[this->noOfVertices];
	for (size_t i = 0; i < this->noOfVertices; i++)
	{
		this->vertexArray[i] = primitive->GetVertices()[i];
	}

	this->indexArray = new GLuint[this->noOfIndices];
	for (size_t i = 0; i < this->noOfIndices; i++)
	{
		this->indexArray[i] = primitive->GetIndices()[i];
	}

	this->InitVAO();
	this->UpdateModelMatrix();
}

Mesh::Mesh(
	Vertex* vertexArray,
	const unsigned& noOfVertices,
	GLuint* indexArray,
	const unsigned& noOfIndices,
	glm::vec3 position,
	glm::vec3 origin,
	glm::vec3 rotation,
	glm::vec3 scale 
)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->noOfVertices = noOfVertices;
	this->noOfIndices = noOfIndices;

	this->vertexArray = new Vertex[this->noOfVertices];
	for (size_t i = 0; i < noOfVertices; i++)
	{
		this->vertexArray[i] = vertexArray[i];
	}
	
	this->indexArray = new GLuint[this->noOfIndices];
	for (size_t i = 0; i < noOfIndices; i++)
	{
		this->indexArray[i] = indexArray[i];
	}

	this->InitVAO();
	this->UpdateModelMatrix();
}

Mesh::Mesh(const Mesh& obj)
{
	this->position = obj.position;
	this->origin = obj.origin;
	this->rotation = obj.rotation;
	this->scale = obj.scale;

	this->noOfVertices = obj.noOfVertices;
	this->noOfIndices = obj.noOfIndices;

	this->vertexArray = new Vertex[this->noOfVertices];
	for (size_t i = 0; i < this->noOfVertices; i++)
	{
		this->vertexArray[i] = obj.vertexArray[i];
	}

	this->indexArray = new GLuint[this->noOfIndices];
	for (size_t i = 0; i < this->noOfIndices; i++)
	{
		this->indexArray[i] = obj.indexArray[i];
	}

	this->InitVAO();
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
	if (this->noOfIndices == 0)
		glDrawArrays(GL_TRIANGLES, 0, this->noOfVertices);
	else
		glDrawElements(GL_TRIANGLES, this->noOfIndices, GL_UNSIGNED_INT, 0);
}

inline void Mesh::SetPosition(const glm::vec3 position) { this->position = position; }

inline void Mesh::SetOrigin(const glm::vec3 origin) { this->origin = origin; }

inline void Mesh::SetRotation(const glm::vec3 rotation) { this->rotation = rotation; }

inline void Mesh::SetScale(const glm::vec3 scale) { this->scale = scale; }

inline void Mesh::Move(const glm::vec3 position) { this->position += position; }

inline void Mesh::Rotate(const glm::vec3 rotation) { this->rotation += rotation; }

inline void Mesh::Scale(const glm::vec3 scale) { this->scale += scale; }

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	
	if (this->noOfIndices > 0)
		glDeleteBuffers(1, &this->EBO);

	delete[] this->vertexArray;
	delete[] this->indexArray;
}

void Mesh::Update()
{
}
