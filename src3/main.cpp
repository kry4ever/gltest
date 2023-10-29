#include <iostream>
#include <iterator>

#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "cube_shader.h"
#include "light_shader.h"
#include "camera.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// extern glm::vec3 cameraPos;
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// void processInput(GLFWwindow *window)
// {
// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 	{
// 		glfwSetWindowShouldClose(window, true);
// 	}

// 	float currentFrame = glfwGetTime();
// 	deltaTime = currentFrame - lastFrame;
// 	lastFrame = currentFrame;
// 	float cameraSpeed = 2.5f * deltaTime;
// 	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
// 		cameraPos += cameraSpeed * cameraFront;
// 	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
// 		cameraPos -= cameraSpeed * cameraFront;
// 	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
// 		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
// 	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
// 		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
// }

float lastX = 400, lastY = 300;
float yaw = 0.0f;
float pitch = 0.0f;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// void mouse_callback(GLFWwindow *window, double xpos, double ypos)
// {
// 	if (firstMouse)
// 	{
// 		lastX = xpos;
// 		lastY = ypos;
// 		firstMouse = false;
// 	}

// 	float xoffset = xpos - lastX;
// 	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
// 	lastX = xpos;
// 	lastY = ypos;

// 	float sensitivity = 0.05f;
// 	xoffset *= sensitivity;
// 	yoffset *= sensitivity;

// 	yaw += xoffset;
// 	pitch += yoffset;

// 	if (pitch > 89.0f)
// 	{
// 		pitch = 89.0f;
// 	}

// 	if (pitch < -89.0f)
// 	{
// 		pitch = -89.0f;
// 	}

// 	glm::vec3 front;
// 	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
// 	front.y = sin(glm::radians(pitch));
// 	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
// 	cameraFront = glm::normalize(front);
// }

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

int main(int argc, char *argv[])
{
	std::cout << "start program...." << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
	// macOS 特定代码
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		// GLEW初始化失败的处理
		glfwTerminate();
		return -1;
	}

	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	cube_shader cubeShader;
	cubeShader.init();

	LightShader lightShader;
	lightShader.init();


	glEnable(GL_DEPTH_TEST);

	// glm::mat4 projection;
	// projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	
		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		cubeShader.use();
		cubeShader.setVec3("light.diffuse", diffuseColor);
		cubeShader.setVec3("light.ambient", ambientColor);
		cubeShader.draw(view, projection);

		lightShader.use();
		lightShader.draw(view, projection);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}