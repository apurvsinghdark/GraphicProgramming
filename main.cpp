#include "Game.h"

int main()
{
	//Game Class INIT
	Game game("GP_UP", 800, 640, 4, 5, false);
	
	//UpdateLoop
	while (!game.GetWindowShouldClose())
	{
		//UPDATE INPUTS
		game.Update();

		game.Render();
	}

	return 0;
}

//void FrameBufferSizeCallBack(GLFWwindow* window, int frameBufferWidth, int frameBufferHeight) //Defination FrameBUfferFunction
//{
//	glViewport(0, 0, frameBufferWidth, frameBufferHeight);
//}
//
//void Input(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, GL_TRUE);
//	}
//}
//
//void Input(GLFWwindow* window, Mesh& mesh)
//{
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//	{
//		mesh.Move(glm::vec3(0.0f, 0.0f, 0.1f));
//	}
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//	{
//		mesh.Move(glm::vec3(0.0f, 0.0f, -0.1f));
//	}
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//	{
//		mesh.Move(glm::vec3(0.1f, 0.0f, 0.0f));
//	}
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//	{
//		mesh.Move(glm::vec3(-0.1f, 0.0f, 0.0f));
//	}
//	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//	{
//		mesh.Rotate(glm::vec3(0.f, -1.0f, 0.0f));
//	}
//	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//	{
//		mesh.Rotate(glm::vec3(0.f, 1.0f, 0.0f));
//	}
//	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
//	{
//		mesh.Scale(glm::vec3(0.1f));
//	}
//	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
//	{
//		mesh.Scale(glm::vec3(-0.1f));
//	}
//}
