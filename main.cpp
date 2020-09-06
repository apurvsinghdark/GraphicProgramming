#include"libs.h"
#include"Shader.h"
#include"Texture.h"

Vertex vertices[] =
{
	///position						//color							//texcoord					//Normal
	glm::vec3(-0.5f,0.5f,0.0f),		glm::vec3(1.0f,1.0f,1.0f),		glm::vec2(0.0f,1.0f),		glm::vec3(0.0f,0.0f,-1.0f),
	glm::vec3(-0.5f,-0.5f,0.0f),	glm::vec3(1.0f,1.0f,1.0f),		glm::vec2(0.0f,0.0f),		glm::vec3(0.0f,0.0f,-1.0f),
	glm::vec3(0.5f,-0.5f,0.0f),		glm::vec3(1.0f,1.0f,1.0f),		glm::vec2(1.0f,0.0f),		glm::vec3(0.0f,0.0f,-1.0f),
	glm::vec3(0.5f, 0.5f,0.0f),		glm::vec3(1.0f,1.0f,1.0f),		glm::vec2(1.0f,1.0f),		glm::vec3(0.0f,0.0f,-1.0f),
																	
};

unsigned noOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indicies[] =
{
	0, 1, 2, //Triangle 1
	2, 3, 0, //Triangle 2
};

unsigned noOfIndicies = sizeof(indicies) / sizeof(GLuint);

void FrameBufferSizeCallBack(GLFWwindow*window,int frameBufferWidth,int frameBufferHeight); //Declaration FrameBUfferFunction

void Input(GLFWwindow* window); //Input declaration

void Input(GLFWwindow* window, glm::vec3 &position, glm::vec3 &rotation, glm::vec3 &scale);

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
	Shader core_program("vertex.glsl", "fragment.glsl");
		
	#pragma region BUFFERREADING
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
	
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	//Bind Array
	glBindVertexArray(VAO);
	#pragma endregion

	#pragma region TEXTURE

	Texture TEXTURE0("Image/mars.jpg", GL_TEXTURE_2D, 0);
	Texture TEXTURE1("Image/img_test.png", GL_TEXTURE_2D, 1);
	
	#pragma endregion

	#pragma region MODELLOADING
	//ModelLoading
	glm::vec3 position(0.0f);
	glm::vec3 rotation(0.0f);
	glm::vec3 scale(1.0f);


	glm::mat4 ModelMatrix(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, position);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, scale);

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

	glm::vec3 lightPos0(0.0f, 0.0f, 2.0f);

	core_program.Use();

	core_program.SetMat4("ModelMatrix", ModelMatrix);
	core_program.SetMat4("ViewMatrix", ViewMatrix);
	core_program.SetMat4("ProjectionMatrix", ProjectionMatrix);

	core_program.SetVec3("lightPos0", lightPos0);
	core_program.SetVec3("camPos", camPos);
	
	core_program.Unuse();
	#pragma endregion

	//UpdateLoop
	while (!glfwWindowShouldClose(window))
	{
		//PROGRAM
		core_program.Use();

		//UPDATE INPUTS
		Input(window);

		//CALL INPUT EVENTS
		glfwPollEvents();

		//UPDATE
		Input(window, position, rotation, scale);
		//CLEAR
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//Arrays BINDING
		glBindVertexArray(VAO);
				
		//Uniformtexture Update
		core_program.SetVec1i("texture0", 0);
		core_program.SetVec1i("texture1", 1);
				
		//Uniform Location Of matrix form vertex.glsl
		//rotation.y += 1.0f;

		glm::mat4 ModelMatrix(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, position);
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, scale);

		core_program.SetMat4("ModelMatrix", ModelMatrix);
		
		//glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
		//glm::mat4 ProjectionMatrix(1.0f);
		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(frameBufferWidth) / frameBufferHeight,
			nearPlane,
			farPlane
		);

		glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
		
		core_program.SetMat4("ProjectionMatrix", ProjectionMatrix);
		
		core_program.Use();//re-Init for becoz of SetMat
				
		//bind N activate texture
		TEXTURE0.bind();
		TEXTURE1.bind();
		
		//DRAW
		glDrawElements(GL_TRIANGLES, noOfIndicies, GL_UNSIGNED_INT, 0);

		//END OF DRAW
		glfwSwapBuffers(window);
		glFlush();

		//Unbinding
		glBindVertexArray(0);
		core_program.Unuse();
		TEXTURE0.unbind();
		TEXTURE1.unbind();
	}

	//FINAL TERMINATE
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight) //Defination FrameBUfferFunction
{
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}

void Input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void Input(GLFWwindow* window, glm::vec3 &position,glm::vec3 &rotation, glm::vec3 &scale)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.z += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.z -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.y -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		scale += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		scale -= 0.01f;
	}
}