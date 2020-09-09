#include "Game.h"

//Private funcs
void Game::InitiWindow(
	const std::string title,
	bool resizeable
)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_MajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_MinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizeable);

	GLFWwindow* window;
	window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title.c_str(), nullptr, nullptr);

	glfwGetFramebufferSize(window, &this->frameBufferWidth, &this->frameBufferHeight);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack); //Give CallBack Of recent activity of Window Resizing (Calling)

	glfwMakeContextCurrent(window);
}

//constructors / desturctors
Game::Game(
	const std::string title,
	const int width, const int height,
	const int majorVersion, const int minorVersion,
	bool resizeable
)
	: WINDOW_WIDTH(width),WINDOW_HEIGHT(height),
	GL_MajorVersion(majorVersion),GL_MinorVersion(minorVersion)
{
	this->frameBufferWidth = this->WINDOW_WIDTH;
	this->frameBufferHeight = this->WINDOW_HEIGHT;
	this->InitiWindow(title, resizeable);
}

Game::~Game()
{
}

void FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight) //Defination FrameBUfferFunction
{
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}