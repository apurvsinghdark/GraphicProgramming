#pragma once

#include<iostream>
#include<glew.h>
#include<glfw3.h>
#include<fstream>
#include<string>
#include<vector>

//Math Includes Library
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

//image libs
#include<SOIL2.h>

//vertex points
struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
};