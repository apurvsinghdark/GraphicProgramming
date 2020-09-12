#pragma once

#include"libs.h"

//Enumetrator
enum shader_enum { ENUM_SHADER0 = 0 };
enum texture_enum { ENUM_TEXTURE0 = 0, ENUM_TEXTURE1 = 1 };
enum material_enum { ENUM_MATERIAL0 = 0 };
enum mesh_enum { ENUM_MESH0 = 0 };

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
	std::vector<Mesh*> meshes;
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
	void InitMeshes();
	void InitLights();
	void InitUniforms();

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

	//Static funcs

	static void FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight); //Decleration FrameBUfferFunction
};



