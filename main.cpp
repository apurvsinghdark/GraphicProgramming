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
	int frameBufferWidth = 0;
	int frameBufferHeight = 0;

	GLFWwindow* window;
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GP_UP", nullptr, nullptr);

	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
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

	//Texture
	int imageHeight = 0;
	int imageWidth = 0;

	//Texture0 INIT
	unsigned char* image = SOIL_load_image("Image/mars.jpg", &imageWidth, &imageHeight, NULL, SOIL_LOAD_RGBA);

	GLuint texture0;
	glGenTextures(1, &texture0); //Genrating Texture At 0 point
	glBindTexture(GL_TEXTURE_2D, texture0); //Bind Texture properties with 0 point

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Scaling at x-axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Scaling at y-axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Scaling at Z-axis (Zoom-In) anti-alising
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Scaling at Z-axis (Zoom-Out) aniti-alising

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); //getting image data
		glGenerateMipmap(GL_TEXTURE_2D); //scaling with mipmap
	}
	else
	{
		std::cout << "ENABLE_TO_LOAD_TEXTURES" << std::endl;
	}

	//Unloading
	glActiveTexture(0); //deactivating
	glBindTexture(GL_TEXTURE_2D,0); //unibinding
	SOIL_free_image_data(image);

	//Texture1 INIT
	image = SOIL_load_image("Image/img_test.png", &imageWidth, &imageHeight, NULL, SOIL_LOAD_RGBA);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "TEXTTURE_NOT_LOADED_SUCCESSFULLY" << std::endl;
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	//ModelLoading
	glm::mat4 ModelMatrix(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f));

	glm::vec3 camPos(0.0f, 0.0f, 1.0f);
	glm::vec3 worldUp (0.0f, 1.0f, 0.0f);
	glm::vec3 camFront(0.0f, 0.0f, -1.0f);

	glm::mat4 ViewMatrix(1.0f);
	ViewMatrix = glm::lookAt(camPos, camPos + camFront, worldUp);

	float fov = 90.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;

	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	glm::mat4 ProjectionMatrix(1.0f);
	ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(frameBufferWidth) / frameBufferHeight,
		nearPlane,
		farPlane
	);

	glUseProgram(program);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	glUseProgram(0);
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
		glUniform1i(glGetUniformLocation(program, "texture1"), 1);

		//Uniform Location Of matrix form vertex.glsl
		glm::mat4 ModelMatrix(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f));

		glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		
		glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
		//glm::mat4 ProjectionMatrix(1.0f);
		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(frameBufferWidth) / frameBufferHeight,
			nearPlane,
			farPlane
		);

		glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
		glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		
		//Activate TEXTURE
		glActiveTexture(GL_TEXTURE0); // activating textures at O coordinate in GPU
		glBindTexture(GL_TEXTURE_2D, texture0); //binding
		
		glActiveTexture(GL_TEXTURE1); // activating textures at 1 coordinate in GPU
		glBindTexture(GL_TEXTURE_2D, texture1); //binding
		
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

	return 0;
}

void FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight) //Defination FrameBUfferFunction
{
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