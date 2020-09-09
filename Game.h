#pragma once

#include"libs.h"
#include"Mesh.h"
#include"Primitives.h"

class Game
{
private:
	//variables
	GLFWwindow* window;
	
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 640;
	const int GL_MajorVersion = 4;
	const int GL_MinorVersion = 5;
	int frameBufferWidth;
	int frameBufferHeight;


	//private funcs
	void InitiWindow(
		const std::string title,
		bool resizeable
	);

	//Static variables


public:
	//constructors / desturctors
	Game(
		const std::string title,
		const int width, const int height,
		const int majorVersion, const int minorVersion,
		bool resizeable
	);
	virtual ~Game();

	//Accessors

	
	//Modifiers

	
	//funcs


	//Static funcs


};

void FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight); //Decleration FrameBUfferFunction


