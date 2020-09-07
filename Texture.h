#pragma once

#include<iostream>
#include<glew.h>
#include<glfw3.h>

//SOIL Libs
#include<SOIL2.h>

class Texture
{
private:

	GLuint texture;
	int imageWidth;
	int imageHeight;

	GLuint type;
	GLint texture_unit;

public:
	Texture(const std::string name, GLenum type, GLint texture_unit);
		
		~Texture();

		GLuint getID() const;

		GLint GetTextureUnit();

		void bind();
		
		void unbind();
};

Texture::Texture(const std::string name, GLenum type, GLint texture_unit)
{
	this->texture_unit = texture_unit;
	this->type = type;
	//Texture INIT
	unsigned char* image = SOIL_load_image(name.c_str(), &this->imageWidth, &this->imageHeight, NULL, SOIL_LOAD_RGBA);

	//GLuint texture0;
	glGenTextures(1, &this->texture); //Genrating Texture At Derived point
	glBindTexture(type, this->texture); //Bind Texture properties with Derived point

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT); //Scaling at x-axis
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT); //Scaling at y-axis
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Scaling at Z-axis (Zoom-In) anti-alising
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Scaling at Z-axis (Zoom-Out) aniti-alising

	if (image)
	{
		glTexImage2D(type, 0, GL_RGBA, this->imageWidth, this->imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); //getting image data
		glGenerateMipmap(type); //scaling with mipmap
	}
	else
	{
		std::cout << "UNABLE_TO_LOAD_TEXTURES" << std::endl;
	}

	//Unloading
	glActiveTexture(0); //deactivating
	glBindTexture(type, 0); //unibinding
	SOIL_free_image_data(image);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->texture);
}

inline GLuint Texture::getID() const { return this->texture; }

inline GLint Texture::GetTextureUnit() { return this->texture_unit; }

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + this->texture_unit);
	glBindTexture(this->type, this->texture);
}

void Texture::unbind()
{
	glActiveTexture(0);
	glBindTexture(this->type, 0);
}
