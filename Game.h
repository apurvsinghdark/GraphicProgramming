#pragma once

#include"libs.h"
#include"Camera.h"

//Enumetrator
enum shader_enum { ENUM_SHADER_CORE_PROGRAM = 0 };
enum texture_enum { ENUM_TEXTURE0 = 0, ENUM_TEXTURE0_SPECULAR, ENUM_TEXTURE1, ENUM_TEXTURE1_SPECULAR };
enum material_enum { ENUM_MATERIAL0 = 0 };
enum mesh_enum { ENUM_MESH0 = 0, ENUM_MESH1};

class Game
{
private:
	//variables
	GLFWwindow* window;
	
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	const int GL_MajorVersion;
	const int GL_MinorVersion;
	int frameBufferWidth;
	int frameBufferHeight;

	//Inputs/Time
	float deltaTime;
	float curFrameTime;
	float lastFrameTime;

	double mouseX;
	double mouseY;
	double lastMouseX;
	double lastMouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	//Camera
	Camera camera;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	
	glm::vec3 camPos;
	glm::vec3 worldUp;
	glm::vec3 camFront;

	float fov;
	float nearPlane;
	float farPlane;

	std::vector<Shader*> shaders;
	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	std::vector<Model*> models;
	std::vector<glm::vec3*> lights;

	//private funcs
	void initGLFW();
	void InitiWindow(
		const std::string title,
		bool resizeable
	);
	void initGLEW();
	
	void InitOpenGLOptions();
	void InitMartices();
	void InitShaders();
	void InitTextures();
	void InitMaterials();
	void InitModels();
	void InitLights();
	void InitUniforms();

	void UpdateUniforms();

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
	int GetWindowShouldClose();
	
	//Modifiers
	void SetWindowShouldClose();
	
	//funcs
	void Update();

	void Render();

	void UpdateDeltaTime();
	void UpdateInput();
	void UpdateMouseInput();
	void UpdateKeyBoardInput();

	//Static funcs

	static void FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight); //Decleration FrameBUfferFunction
};

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

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	this->textures.push_back(new Texture("Image/container.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Image/container_specular.png", GL_TEXTURE_2D));
	
	this->textures.push_back(new Texture("Image/mars.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Image/download.png", GL_TEXTURE_2D));
}

void Game::InitMaterials()
{
	materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f),
		0, 1));
}

void Game::InitModels()
{
	std::vector<Mesh*> meshes;

	meshes.push_back(new Mesh(
		&Pyramid(),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f)
	)
	);
	this->models.push_back(new Model(glm::vec3(0.0f),
		this->materials[ENUM_MATERIAL0],
		this->textures[ENUM_TEXTURE0],
		this->textures[ENUM_TEXTURE0_SPECULAR],
		meshes
	)
	);this->models.push_back(new Model(glm::vec3(1.0f, 1.0f, 1.0f),
		this->materials[ENUM_MATERIAL0],
		this->textures[ENUM_TEXTURE0],
		this->textures[ENUM_TEXTURE0_SPECULAR],
		meshes
	)
	);this->models.push_back(new Model(glm::vec3(2.0f),
		this->materials[ENUM_MATERIAL0],
		this->textures[ENUM_TEXTURE0],
		this->textures[ENUM_TEXTURE0_SPECULAR],
		meshes
	)
	);
	
	//this->models.push_back(new Model(glm::vec3(0.0f), this->materials[0], this->textures[ENUM_TEXTURE0], this->textures[ENUM_TEXTURE0_SPECULAR], meshes));

	for (auto*& i : meshes)
		delete i;

	meshes.clear();
}

void Game::InitLights()
{
	this->lights.push_back(new glm::vec3(0.0f, 0.0f, 2.0f));
}

void Game::InitUniforms()
{
	this->shaders[ENUM_SHADER_CORE_PROGRAM]->SetMat4("ViewMatrix", this->ViewMatrix);
	this->shaders[ENUM_SHADER_CORE_PROGRAM]->SetMat4("ProjectionMatrix", this->ProjectionMatrix);

	this->shaders[ENUM_SHADER_CORE_PROGRAM]->SetVec3("lightPos0", *this->lights[0]);
}

void Game::UpdateUniforms()
{
	glfwGetFramebufferSize(this->window, &this->frameBufferWidth, &this->frameBufferHeight);

	this->ViewMatrix = this->camera.GetViewMatrix();
	//this->ViewMatrix = glm::lookAt(this->camPos, this->camPos + this->camFront, this->worldUp);

	this->shaders[ENUM_SHADER_CORE_PROGRAM]->SetMat4("ViewMatrix", this->ViewMatrix);
	this->shaders[ENUM_SHADER_CORE_PROGRAM]->SetVec3("camPos", this->camPos);

	//Projection(PerspectiveVision)
	this->ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(this->frameBufferWidth) / this->frameBufferHeight,
		this->nearPlane,
		this->farPlane
	);

	this->shaders[ENUM_SHADER_CORE_PROGRAM]->SetMat4("ProjectionMatrix", this->ProjectionMatrix);
}

//constructors / desturctors
Game::Game(
	const std::string title,
	const int width, const int height,
	const int majorVersion, const int minorVersion,
	bool resizeable
)
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height),
	GL_MajorVersion(majorVersion), GL_MinorVersion(minorVersion),
	camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
	this->window = nullptr;
	this->frameBufferWidth = this->WINDOW_WIDTH;
	this->frameBufferHeight = this->WINDOW_HEIGHT;

	this->deltaTime = 0.0f;
	this->curFrameTime = 0.0f;
	this->lastFrameTime = 0.0f;

	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->camPos = this->camera.GetPosition();
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
	this->InitModels();
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
	UpdateInput();
	UpdateDeltaTime();

	//Update Roatation
	this->models[0]->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
	this->models[1]->Rotate(glm::vec3(1.0f, 1.0f, 0.0f));
}

void Game::Render()
{
	//CLEAR
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update Uniforms
	this->UpdateUniforms();
	
	for(auto&i : this->models)
		i->Render(this->shaders[ENUM_SHADER_CORE_PROGRAM]);

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
	
	for (auto&i : this->models)
		delete i;

	for (size_t i = 0; i < this->lights.size(); i++)
		delete this->lights[i];

	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void Game::FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight) //Defination FrameBUfferFunction
{
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}

void Game::UpdateDeltaTime()
{
	this->curFrameTime = static_cast<float>(glfwGetTime());
	this->deltaTime = this->curFrameTime - this->lastFrameTime;
	this->lastFrameTime = this->curFrameTime;
}

void Game::UpdateInput()
{
	glfwPollEvents();

	this->UpdateKeyBoardInput();
	this->UpdateMouseInput();
	this->camera.UpdateInput(deltaTime, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::UpdateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

void Game::UpdateKeyBoardInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GL_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camera.Move(deltaTime, FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camera.Move(deltaTime, BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camera.Move(deltaTime, LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camera.Move(deltaTime, RIGHT);
	}
	/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		this->camPos.y += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		this->camPos.y -= 0.1f;
	}*/
}
