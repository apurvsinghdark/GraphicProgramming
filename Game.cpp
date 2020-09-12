#include "Game.h"

void Game::initGLFW()
{
	glfwInit();

	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR_INIT_GLEWINIT" << std::endl;
		glfwTerminate();
	}
}

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

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title.c_str(), nullptr, nullptr);

	if (this->window == nullptr)
	{
		std::cout << "ERROR_INIT_WINDOw" << std::endl;
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::FrameBufferSizeCallBack); //Give CallBack Of recent activity of Window Resizing (Calling)

	glfwMakeContextCurrent(this->window);
}

void Game::initGLEW()
{
	//GLEW INITIALIZATION
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW_NOT_OKAY_IN_MAIN_FUNCTION" << std::endl;

		glfwTerminate();
	}

}

void Game::InitOpenGLOptions()
{
	//OpenGL (OPTIMIZATION)
	glEnable(GL_DEPTH);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::InitMartices()
{
	this->ViewMatrix = glm::mat4(1.0f);
	this->ViewMatrix = glm::lookAt(this->camPos, this->camPos + this->camFront, this->worldUp);

	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);

	this->ProjectionMatrix = glm::mat4(1.0f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Game::InitShaders()
{
	this->shaders.push_back(new Shader(this->GL_MajorVersion, this->GL_MinorVersion, "vertex.glsl", "fragment.glsl"));
}

void Game::InitTextures()
{
	this->textures.push_back(new Texture("Image/mars.jpg", GL_TEXTURE_2D, 0));
	this->textures.push_back(new Texture("Image/img_test.png", GL_TEXTURE_2D, 1));
}

void Game::InitMaterials()
{
	materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f),
		this->textures[ENUM_TEXTURE0]->GetTextureUnit(), this->textures[ENUM_TEXTURE1]->GetTextureUnit()));
}

void Game::InitMeshes()
{
	this->meshes.push_back(new Mesh(
		&Quad(),
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f)
	)
	);
}

void Game::InitLights()
{
	this->lights.push_back(new glm::vec3(0.0f, 0.0f, 2.0f));
}

void Game::InitUniforms()
{
	this->shaders[ENUM_SHADER0]->SetMat4("ViewMatrix", this->ViewMatrix);
	this->shaders[ENUM_SHADER0]->SetMat4("ProjectionMatrix", this->ProjectionMatrix);

	this->shaders[ENUM_SHADER0]->SetVec3("lightPos0", *this->lights[0]);
	this->shaders[ENUM_SHADER0]->SetVec3("camPos", this->camPos);
}

//constructors / desturctors
Game::Game(
	const std::string title,
	const int width, const int height,
	const int majorVersion, const int minorVersion,
	bool resizeable
)
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height),
	GL_MajorVersion(majorVersion), GL_MinorVersion(minorVersion)
{
	this->window = nullptr;
	this->frameBufferWidth = this->WINDOW_WIDTH;
	this->frameBufferHeight = this->WINDOW_HEIGHT;

	this->camPos = glm::vec3(0.0f, 0.0f, 1.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->camFront = glm::vec3(0.0f, 0.0f, -1.0f);

	this->fov = 90.0f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.0f;

	this->initGLFW();
	this->InitiWindow(title, resizeable);
	this->initGLEW();

	this->InitOpenGLOptions();
	this->InitMartices();

	this->InitShaders();
	this->InitTextures();
	this->InitMaterials();
	this->InitMeshes();
	this->InitLights();
	this->InitUniforms();
}


int Game::GetWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Game::SetWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GL_TRUE);
}

void Game::Update()
{
	//CALL INPUT EVENTS
	glfwPollEvents();
}

void Game::Render()
{
	//UPDATE
	//Input(window, mesh);

	//CLEAR
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Uniformtexture Update
	this->shaders[ENUM_SHADER0]->SetVec1i("texture0", this->textures[ENUM_TEXTURE0]->GetTextureUnit());
	this->shaders[ENUM_SHADER0]->SetVec1i("texture1", this->textures[ENUM_TEXTURE1]->GetTextureUnit());
	this->materials[ENUM_MATERIAL0]->SendToShader(*this->shaders[ENUM_SHADER0]);

	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);
	//Projection(PerspectiveVision)
	ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(frameBufferWidth) / frameBufferHeight,
		nearPlane,
		farPlane
	);

	this->shaders[ENUM_SHADER0]->SetMat4("ProjectionMatrix", this->ProjectionMatrix);

	this->shaders[ENUM_SHADER0]->Use();//re-Init for becoz of SetMat

	//bind N activate texture
	this->textures[ENUM_TEXTURE0]->bind();
	this->textures[ENUM_TEXTURE1]->bind();

	//DRAW
	this->meshes[ENUM_MESH0]->Render(this->shaders[ENUM_SHADER0]);

	//END OF DRAW
	glfwSwapBuffers(this->window);
	glFlush();

	//Unbinding
	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Game::~Game()
{
	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (size_t i = 0; i < this->meshes.size(); i++)
		delete this->meshes[i];

	for (size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];

	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void Game::FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight) //Defination FrameBUfferFunction
{
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}