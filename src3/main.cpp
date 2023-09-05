#include <iostream>
#include <iterator>

#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <cmath>
#include "glm/glm.hpp"
#include "cube_shader.h"

glm::vec3 cameraPos = glm::vec3(0.6f, 1.0f, 3.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLuint load_image(const char *path, GLint location, GLint index);


static std::string light_v_shader = R"(
	#version 330 core
	layout (location = 0) in vec3 position;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
    	gl_Position = projection * view * model * vec4(position, 1.0);
	}
)";

static std::string light_f_shader = R"(
	#version 330 core
	out vec4 FragColor;

	void main()
	{
		FragColor = vec4(1.0); // 将向量的四个分量全部设置为1.0
	}
)";

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float lastX = 400, lastY = 300;
float yaw = 0.0f;
float pitch = 0.0f;
bool firstMouse = true;
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
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
	// glfwSetCursorPosCallback(window, mouse_callback);

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

	// // glUseProgram(lightShaderProgram);
	// unsigned int lightVAO;
	// glGenVertexArrays(1, &lightVAO);
	// glBindVertexArray(lightVAO);
	// // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	// glEnableVertexAttribArray(0);

	// glBindVertexArray(0);

	// // glUniform3f(glGetUniformLocation(lightShaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
	// glm::vec3 lightPos(1.2f, 1.0f, 1.2f);

	// float angle = glm::radians(45.0f);
	// glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	// glm::vec4 rotatedVector = rotation * glm::vec4(cameraUp, 1.0f);
	// glm::vec3 cameraUp2(rotatedVector.x, rotatedVector.y, rotatedVector.z);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// GLfloat timeValue = glfwGetTime();
		// GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		// std::cout << "greenValue = " << greenValue << std::endl;
		// GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		// glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		// glBindTexture(GL_TEXTURE_2D, texture1);

		// glm::mat4 view;
		// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		glm::mat4 view;
		view = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), cameraUp);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		cubeShader.use();
		cubeShader.draw(view, projection);

		// {
		// 	glBindVertexArray(VAO);
		// 	glActiveTexture(GL_TEXTURE0);
		// 	glBindTexture(GL_TEXTURE_2D, texture1);
		// 	glActiveTexture(GL_TEXTURE1);
		// 	glBindTexture(GL_TEXTURE_2D, texture2);

		// 	glUseProgram(shaderProgram);
		// 	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		// 	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		// 	GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		// 	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// 	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// 	glm::mat4 model;
		// 	model = glm::translate(model, cubePositions[0]);
		// 	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		// 	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// 	glDrawArrays(GL_TRIANGLES, 0, 36);
		// }

		// {
		// 	glBindVertexArray(lightVAO);
		// 	glUseProgram(lightShaderProgram);
		// 	GLint modelLoc = glGetUniformLocation(lightShaderProgram, "model");
		// 	GLint viewLoc = glGetUniformLocation(lightShaderProgram, "view");
		// 	GLint projectionLoc = glGetUniformLocation(lightShaderProgram, "projection");
		// 	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// 	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// 	glm::mat4 model;
		// 	model = glm::mat4();
		// 	model = glm::translate(model, lightPos);
		// 	model = glm::scale(model, glm::vec3(0.2f));
		// 	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// 	glDrawArrays(GL_TRIANGLES, 0, 36);
		// }

		// glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}