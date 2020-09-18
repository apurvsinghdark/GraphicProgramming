#pragma once

#include<iostream>
#include<glew.h>
#include<glfw3.h>

#include<glm.hpp>
#include<vec3.hpp>
#include<mat4x4.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

class Camera
{
private:
	glm::mat4 ViewMatrix;

	GLfloat movementFloat;
	GLfloat sensitivity;

	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	void UpdateCameraVectors();

public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp);
	~Camera();

	glm::mat4 GetViewMatrix();

	const glm::vec3 GetPosition() const;

	void UpdateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY);
	void UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY);
	void UpdateKeyBoardInput(const float& dt, const int direction);
};

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
{

}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->worldUp);

	this->UpdateCameraVectors();

	return this->ViewMatrix;
}

const glm::vec3 Camera::GetPosition() const
{
	return this->position;
}

void Camera::UpdateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
	this->UpdateMouseInput(dt, offsetX, offsetY);
	this->UpdateKeyBoardInput(dt, direction);

}

void Camera::UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
}

void Camera::UpdateKeyBoardInput(const float& dt, const int direction)
{

}
