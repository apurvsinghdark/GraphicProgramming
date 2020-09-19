#pragma once

#include<iostream>
#include<glew.h>
#include<glfw3.h>

#include<glm.hpp>
#include<vec3.hpp>
#include<mat4x4.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

enum direction { FORWARD = 0, BACKWARD, LEFT, RIGHT };

class Camera
{
private:
	glm::mat4 ViewMatrix;

	GLfloat movementSpeed;
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
	void Move(const float& dt, const int direction);
};

void Camera::UpdateCameraVectors()
{
	this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front.y = sin(glm::radians(this->pitch));
	this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
{
	this->ViewMatrix = glm::mat4(1.0f);

	this->movementSpeed = 3.0f;
	this->sensitivity = 3.0f;

	this->worldUp = worldUp;
	this->position = position;
	this->up = worldUp;
	this->right = glm::vec3(1.0f);

	this->pitch = 0.0f;
	this->yaw = -90.0f;
	this->roll = 0.0f;

	this->UpdateCameraVectors();
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
}

void Camera::UpdateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
	this->pitch += static_cast<GLfloat>(offsetY) * this->sensitivity * dt;
	this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity * dt;

	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	if (this->yaw > 360.0f || this->yaw < -360.0f)
		this->yaw = 0.0f;
}

void Camera::Move(const float& dt, const int direction)
{
	switch (direction)
	{
	case FORWARD:
		this->position += this->front * this->movementSpeed * dt;
		break;
	case BACKWARD:
		this->position -= this->front * this->movementSpeed * dt;
		break;
	case LEFT:
		this->position -= this->right * this->movementSpeed * dt;
		break;
	case RIGHT:
		this->position += this->right * this->movementSpeed * dt;
		break;
	default:
		break;
	}
}
