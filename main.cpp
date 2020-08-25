#include"libs.h"

void FrameBufferSizeCallBack(GLFWwindow*window,int frameBufferWidth,int frameBufferHeight); //Declaration FrameBUfferFunction

bool loadShader(GLuint& program);

int main()
{
	// GLFW INITIALIZATION
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 800;
	
	GLFWwindow* window;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GP_UP", nullptr, nullptr);

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack); //Give CallBack Of recent activity of Window Resizing (Calling)
	
	glfwMakeContextCurrent(window);

	//GLEW INITIALIZATION
	glewInit();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW_NOT_OKAY_IN_MAIN_FUNCTION" << std::endl;

		glfwTerminate();
	}

	//vertex & fragment Shaders
	GLuint program;
	if (!loadShader(program))
	{
		glfwTerminate();
	}

	while (!glfwWindowShouldClose(window))
	{
		//UPDATE INPUTS
		
		//CALL INPUT EVENTS
		glfwPollEvents();

		//UPDATE

		//CLEAR
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//DRAW

		//END OF DRAW
		glfwSwapBuffers(window);
		glFlush();
	}

	//FINAL TERMINATE
	glfwDestroyWindow(window);
	glfwTerminate();

	//Delete Program
	glDeleteProgram(program);
	//GLFWwindow* window = glfwCreateWindow(800, 600, GLFWwindow * window, nullptr);
	return 0;
}

void FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight) //Defination FrameBUfferFunction
{
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}

bool loadShader(GLuint& program)
{
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream shaderFile;

	//For Vertex Shader
	shaderFile.open("vertex.glsl");

	if (shaderFile.is_open())
	{
		while (std::getline(shaderFile,temp))
		{
			src += temp + "\n";
		}
	}
	else
	{
		std::cout << "ERROR_LOADING_VERTEX_SHADER" << std::endl;
		loadSuccess = false;
	}

	shaderFile.close();

	GLuint VERTEXSHADER = glCreateShader(GL_VERTEX_SHADER);
	const char* vertStrc = src.c_str();

	glShaderSource(VERTEXSHADER, 1, &vertStrc, nullptr);
	glCompileShader(VERTEXSHADER);

	glGetShaderiv(VERTEXSHADER, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(VERTEXSHADER, 512, NULL, infoLog);
		std::cout << "ERROR_COMPILING_VERTEX_SHADER" << std::endl;
		std::cout << infoLog << std::endl;
		loadSuccess = false;
	}

	temp = "";
	src = "";
	
	//For Fragment Shader
	shaderFile.open("fragment.glsl");

	if (shaderFile.is_open())
	{
		while (std::getline(shaderFile,temp))
		{
			src += temp + "\n";
		}
	}
	else
	{
		std::cout << "ERROR_LOADING_FRAGMENT_SHADER" << std::endl;
		loadSuccess = false;
	}

	shaderFile.close();

	GLuint FRAGMENTSHADER = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragStr = src.c_str();
	glShaderSource(FRAGMENTSHADER, 1, &fragStr, nullptr);

	glCompileShader(FRAGMENTSHADER);
	glGetShaderiv(FRAGMENTSHADER, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(FRAGMENTSHADER, 512, nullptr, infoLog);
		std::cout << "ERROR_COMPILING_FRAGMENT_SHADER" << std::endl;
		std::cout << infoLog << std::endl;
		loadSuccess = false;
	}

	temp = "";
	src = "";

	//Linking/Program
	program = glCreateProgram();

	glAttachShader(program, VERTEXSHADER);
	glAttachShader(program, FRAGMENTSHADER);
	
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "ERROR_IN_LINKING_SHADERS" << std::endl;
		std::cout << infoLog << std::endl;
		loadSuccess = false;
	}

	//Delete Shader
	glUseProgram(0);
	glDeleteShader(VERTEXSHADER);
	glDeleteShader(FRAGMENTSHADER);

	return loadSuccess;
}