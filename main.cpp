#include"libs.h"
#include"Mesh.h"
#include"Primitives.h"

void FrameBufferSizeCallBack(GLFWwindow*window,int frameBufferWidth,int frameBufferHeight); //Declaration FrameBUfferFunction

void Input(GLFWwindow* window); //Input declaration

void Input(GLFWwindow* window, Mesh &mesh);

int main()
{
	//Init Primitive
	Quad Quad;
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

	Mesh mesh( 
		&Quad,
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f)
		);

	#pragma endregion

	#pragma region TEXTURE/MATERIAL

	Texture TEXTURE0("Image/mars.jpg", GL_TEXTURE_2D, 0);
	Texture TEXTURE1("Image/img_test.png", GL_TEXTURE_2D, 1);

	Material material0(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), TEXTURE0.GetTextureUnit(), TEXTURE1.GetTextureUnit());
	
	#pragma endregion

	#pragma region CAMERA


	//ModelLoading
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
		Input(window, mesh);
		//CLEAR
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		mesh.Render(&core_program);
		
		//Uniformtexture Update
		core_program.SetVec1i("texture0", TEXTURE0.GetTextureUnit());
		core_program.SetVec1i("texture1", TEXTURE1.GetTextureUnit());
		material0.SendToShader(core_program);
				
		//Projection(PerspectiveVision)
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
		//glDrawElements(GL_TRIANGLES, noOfIndices, GL_UNSIGNED_INT, 0);

		//END OF DRAW
		glfwSwapBuffers(window);
		glFlush();

		//Unbinding
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

void Input(GLFWwindow* window, Mesh& mesh)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mesh.Move(glm::vec3(0.0f, 0.0f, 0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mesh.Move(glm::vec3(0.0f, 0.0f, -0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mesh.Move(glm::vec3(0.1f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mesh.Move(glm::vec3(-0.1f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mesh.Rotate(glm::vec3(0.f, -1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		mesh.Rotate(glm::vec3(0.f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		mesh.Scale(glm::vec3(0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		mesh.Scale(glm::vec3(-0.1f));
	}
}