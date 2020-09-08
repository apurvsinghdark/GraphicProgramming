#pragma once
#include<glew.h>
#include<glfw3.h>

#include<vector>
#include"Vertex.h"

class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
public:
	Primitive();

	virtual ~Primitive();

	void SetVertex(
		const Vertex* vertex,
		const unsigned noOfVertices,
		const GLuint* index,
		const unsigned noOfIndices
	);

	Vertex* GetVertices();
	const unsigned GetNoOfVertices();
	GLuint* GetIndices();
	const unsigned GetNoOfIndices();

};

Primitive::Primitive()
{
}

Primitive::~Primitive()
{
}

void Primitive::SetVertex(const Vertex* vertex, const unsigned noOfVertices, const GLuint* index, const unsigned noOfIndices)
{
	for (size_t i = 0; i < noOfVertices; i++)
	{
		this->vertices.push_back(vertex[i]);
	}
	for (size_t i = 0; i < noOfIndices; i++)
	{
		this->indices.push_back(index[i]);
	}
}

inline Vertex* Primitive::GetVertices() { return this->vertices.data(); }

inline const unsigned Primitive::GetNoOfVertices() { return this->vertices.size(); }

inline GLuint* Primitive::GetIndices() { return this->indices.data(); }

inline const unsigned Primitive::GetNoOfIndices() { return this->indices.size(); }

class Quad : public Primitive
{
private:
public:
	Quad();
	~Quad();
};

Quad::Quad() : Primitive()
{
	Vertex vertices[] =
{
		///position						//color							//texcoord					//Normal
		glm::vec3(-0.5f,0.5f,0.0f),		glm::vec3(1.0f,1.0f,1.0f),		glm::vec2(0.0f,1.0f),		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(-0.5f,-0.5f,0.0f),	glm::vec3(1.0f,1.0f,1.0f),		glm::vec2(0.0f,0.0f),		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(0.5f,-0.5f,0.0f),		glm::vec3(1.0f,1.0f,1.0f),		glm::vec2(1.0f,0.0f),		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(0.5f, 0.5f,0.0f),		glm::vec3(1.0f,1.0f,1.0f),		glm::vec2(1.0f,1.0f),		glm::vec3(0.0f,0.0f,1.0f),

	};

	unsigned noOfVertices = sizeof(vertices) / sizeof(Vertex);

	GLuint indices[] =
	{
		0, 1, 2, //Triangle 1
		2, 3, 0, //Triangle 2
	};

	unsigned noOfIndices = sizeof(indices) / sizeof(GLuint);

	this->SetVertex(vertices, noOfVertices, indices, noOfIndices);
}

Quad::~Quad()
{
}