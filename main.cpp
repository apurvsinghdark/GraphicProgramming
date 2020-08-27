#include"libs.h"

Vertex vertices[] =
{
	///position						//color							//texcoord
	glm::vec3(-0.5f,0.5f,0.0f),		glm::vec3(1.0f,0.0f,0.0f),		glm::vec2(0.0f,1.0f),
	glm::vec3(-0.5f,-0.5f,0.0f),	glm::vec3(0.0f,1.0f,0.0f),		glm::vec2(0.0f,0.0f),
	glm::vec3( 0.5f,-0.5f,0.0f),	glm::vec3(0.0f,0.0f,1.0f),		glm::vec2(1.0f,0.0f),
	glm::vec3( 0.5f, 0.5f,0.0f),	glm::vec3(1.0f,1.0f,0.0f),		glm::vec2(1.0f,1.0f),
	
};

unsigned noOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indicies[] =
{
	0, 1, 2, //Triangle 1
	2, 3, 0, //Triangle 2
};

unsigned noOfIndicies = sizeof(indicies) / sizeof(GLuint);

void FrameBufferSizeCallBack(GLFWwindow*window,int frameBufferWidth,int frameBufferHeight); //Declaration FrameBUfferFunction

bool loadShader(GLuint& program); //vetex and shader loading

void Input(GLFWwindow* window); //Input declaration

int main()
{
	// GLFW INITIALIZATION
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 640;
	
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

	//OpenGL (OPTIMIZATION)
	glEnable(GL_DEPTH);
	
	/// <summary>
	/// CULLING BAcK FACE What we are not watching
	/// </summary>
	/// <returns></returns>
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//set the polygon into Fill or Skeleton(GL_LINES)
	
	//vertex & fragment Shaders
	GLuint program;
	if (!loadShader(program))
	{
		glfwTerminate();
	}

	//init VAO(Vertex Array) and Bind it
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//init VBO and bind
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//init EBO and bind
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

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

	//Bind Array
	glBindVertexArray(VAO);

	//Texture INIT
	int imageHeight = 0;
	int imageWidth = 0;

	unsigned char* image = SOIL_load_image("Image/mars.jpg", &imageWidth, &imageHeight, NULL, SOIL_LOAD_RGBA);

	GLuint texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ENABLE_TO_LOAD_TEXTURES" << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D,0);

	//UpdateLoop
	while (!glfwWindowShouldClose(window))
	{
		//UPDATE INPUTS
		Input(window);

		//CALL INPUT EVENTS
		glfwPollEvents();

		//UPDATE

		//CLEAR
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//PROGRAM
		glUseProgram(program);
		
		//Arrays BINDING
		glBindVertexArray(VAO);
		

		//Uniformtexture Update
		glUniform1i(glGetUniformLocation(program, "texture0"), 0);

		//Activate TEXTURE
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		
		//DRAW
		glDrawElements(GL_TRIANGLES, noOfIndicies, GL_UNSIGNED_INT, 0);
		
		//END OF DRAW
		glfwSwapBuffers(window);
		glFlush();

		//Unbinding
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
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

void Input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}