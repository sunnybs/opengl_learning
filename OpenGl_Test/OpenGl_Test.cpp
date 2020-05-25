#include "pch.h"
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Object.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void Do_Movement();

void ListenEvents();

void SetView(int width, int height, Shader &ourShader);

void getPlane(std::vector<float> &floorVerticesData, float repeatTextureCount);

void getCube(std::vector<float> &verticesData);

void getPrism(std::vector<float> &verticesData, float lowerBase, float upperBase);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 10.0f, 2.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGl_test", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader ourShader("Shaders/shader.vs", "Shaders/shader.frag");

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ОБЪЕКТЫ СЦЕНЫ
	std::vector<float> floorVerticesData;
	getPlane(floorVerticesData, 20);
	Object floor;
	floor.SetVerticles(floorVerticesData);
	floor.SetTexture("Textures/grass.jpg");

	std::vector<float> containerVertData;
	getCube(containerVertData);
	Object container;
	container.SetVerticles(containerVertData);
	container.SetTexture("Textures/container.jpg");

	std::vector<float> tankCubeData;
	getCube(tankCubeData);
	Object tankCube;
	tankCube.SetVerticles(tankCubeData);
	tankCube.SetTexture("Textures/armor.jpg");

	std::vector<float> tankWheelData;
	getPrism(tankWheelData, 4, 5);
	Object tankWheel;
	tankWheel.SetVerticles(tankWheelData);
	tankWheel.SetTexture("Textures/tank-wheel.jpg");

	std::vector<float> tankWingData;
	getPrism(tankWingData, 5, 3);
	Object tankWing;
	tankWing.SetVerticles(tankWingData);
	tankWing.SetTexture("Textures/armor.jpg");

	float r, g, b;
	r = 117.0f / 255.0f;
	g = 187.0f / 255.0f;
	b = 253.0f / 255.0f;

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		ListenEvents();
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();
		SetView(width, height, ourShader);

		floor.SetTranslate(glm::vec3(0, -5, 0));
		floor.SetScale(glm::vec3(200, 200, 200));
		floor.Draw(ourShader);

		float yLevel = -4.5;

		container.SetTranslate(glm::vec3(0, yLevel, 0));
		container.SetScale(glm::vec3(1, 1, 1));
		container.Draw(ourShader);

		//Гусеницы
		tankWheel.SetTranslate(glm::vec3(3.75, yLevel - 0.25, 0));
		tankWheel.SetScale(glm::vec3(0.5, 0.5, 4.25));
		tankWheel.Draw(ourShader);

		tankWheel.SetTranslate(glm::vec3(6.25, yLevel - 0.25, 0));
		tankWheel.SetScale(glm::vec3(0.5, 0.5, 4.25));
		tankWheel.Draw(ourShader);

		//Кузов
		tankCube.SetTranslate(glm::vec3(5, yLevel + 0.5, 0));
		tankCube.SetScale(glm::vec3(2, 1, 4));
		tankCube.Draw(ourShader);

		tankCube.SetTranslate(glm::vec3(5, yLevel + 1.5, 0));
		tankCube.SetScale(glm::vec3(2, 1, 2));
		tankCube.Draw(ourShader);

		//Крылья
		tankCube.SetTranslate(glm::vec3(3.75, yLevel + 0.5, 0));
		tankCube.SetScale(glm::vec3(0.5, 1, 4.5));
		tankCube.Draw(ourShader);

		tankCube.SetTranslate(glm::vec3(6.25, yLevel + 0.5, 0));
		tankCube.SetScale(glm::vec3(0.5, 1, 4.5));
		tankCube.Draw(ourShader);

		tankWing.SetTranslate(glm::vec3(3.75, yLevel + 1.125, 0));
		tankWing.SetScale(glm::vec3(0.5, 0.25, 4.5));
		tankWing.Draw(ourShader);

		tankWing.SetTranslate(glm::vec3(6.25, yLevel + 1.125, 0));
		tankWing.SetScale(glm::vec3(0.5, 0.25, 4.5));
		tankWing.Draw(ourShader);

		//Крышка
		tankCube.SetTranslate(glm::vec3(5, yLevel + 2, 0));
		tankCube.SetScale(glm::vec3(1, 0.3, 1));
		tankCube.Draw(ourShader);

		//Дуло
		tankCube.SetTranslate(glm::vec3(5, yLevel + 1.5, 2.5));
		tankCube.SetScale(glm::vec3(0.1, 0.1, 3));
		tankCube.Draw(ourShader);
		
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &floor.VAO);
	glDeleteBuffers(1, &floor.VBO);

	glfwTerminate();
	return 0;
}

void getCube(std::vector<float> &verticesData) {
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // задняя
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // передняя
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // левая
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // правая
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // нижняя
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // верхняя
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	for (float vertex : vertices) {
		verticesData.push_back(vertex);
	}
}

void getPrism(std::vector<float> &verticesData, float lowerBaseLength, float upperBaseLength) {
	float lowerBase = lowerBaseLength;
	float upperBase = upperBaseLength;

	float lowerTextureOffset = (1.0f - lowerBase * 0.2f) / 2.0f;
	float upperTextureOffset = (1.0f - upperBase * 0.2f) / 2.0f;

	float a = upperBase < lowerBase ? 1.0f: -1.0f;
	float b = upperBase < lowerBase ? upperBase : lowerBase;

	float vertices[] = {
	-0.5f, -0.5f, -0.1f * lowerBase,  0.0f,  a * 0.5f, -0.1f * b,  0.5f, 0.0f, // задняя
	 0.5f, -0.5f, -0.1f * lowerBase,  0.0f,  a * 0.5f, -0.1f * b,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.1f * upperBase,  0.0f,  a * 0.5f, -0.1f * b,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.1f * upperBase,  0.0f,  a * 0.5f, -0.1f * b,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.1f * upperBase,  0.0f,  a * 0.5f, -0.1f * b,  0.5f, 1.0f,
	-0.5f, -0.5f, -0.1f * lowerBase,  0.0f,  a * 0.5f, -0.1f * b,  0.5f, 0.0f,

	-0.5f, -0.5f,  0.1f * lowerBase,  0.0f,  a * 0.5f,  0.1f * b,  0.5f, 0.0f, // передняя
	 0.5f, -0.5f,  0.1f * lowerBase,  0.0f,  a * 0.5f,  0.1f * b,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.1f * upperBase,  0.0f,  a * 0.5f,  0.1f * b,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.1f * upperBase,  0.0f,  a * 0.5f,  0.1f * b,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.1f * upperBase,  0.0f,  a * 0.5f,  0.1f * b,  0.5f, 1.0f,
	-0.5f, -0.5f,  0.1f * lowerBase,  0.0f,  a * 0.5f,  0.1f * b,  0.5f, 0.0f,

	-0.5f,  0.5f,  0.1f * upperBase,  -1.0f,  0.0f,  0.0f,  0.5f, upperTextureOffset, // левая
	-0.5f,  0.5f, -0.1f * upperBase,  -1.0f,  0.0f,  0.0f,  0.5f, 1.0f - upperTextureOffset,
	-0.5f, -0.5f, -0.1f * lowerBase,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f - lowerTextureOffset,
	-0.5f, -0.5f, -0.1f * lowerBase,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f - lowerTextureOffset,
	-0.5f, -0.5f,  0.1f * lowerBase,  -1.0f,  0.0f,  0.0f,  0.0f, lowerTextureOffset,
	-0.5f,  0.5f,  0.1f * upperBase,  -1.0f,  0.0f,  0.0f,  0.5f, upperTextureOffset,

	 0.5f,  0.5f,  0.1f * upperBase,  1.0f,  0.0f,  0.0f,  0.5f, upperTextureOffset, // правая
	 0.5f,  0.5f, -0.1f * upperBase,  1.0f,  0.0f,  0.0f,  0.5f, 1.0f - upperTextureOffset,
	 0.5f, -0.5f, -0.1f * lowerBase,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f - lowerTextureOffset,
	 0.5f, -0.5f, -0.1f * lowerBase,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f - lowerTextureOffset,
	 0.5f, -0.5f,  0.1f * lowerBase,  1.0f,  0.0f,  0.0f,  0.0f, lowerTextureOffset,
	 0.5f,  0.5f,  0.1f * upperBase,  1.0f,  0.0f,  0.0f,  0.5f, upperTextureOffset,

	-0.5f, -0.5f, -0.1f * lowerBase,  0.0f, -1.0f,  0.0f,  0.5f, 1.0f, // нижняя
	 0.5f, -0.5f, -0.1f * lowerBase,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.1f * lowerBase,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.1f * lowerBase,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.1f * lowerBase,  0.0f, -1.0f,  0.0f,  0.5f, 0.0f,
	-0.5f, -0.5f, -0.1f * lowerBase,  0.0f, -1.0f,  0.0f,  0.5f, 1.0f,

	-0.5f,  0.5f, -0.1f * upperBase,  0.0f,  1.0f,  0.0f,  0.5f, 1.0f, // верхняя
	 0.5f,  0.5f, -0.1f * upperBase,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.1f * upperBase,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.1f * upperBase,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.1f * upperBase,  0.0f,  1.0f,  0.0f,  0.5f, 0.0f,
	-0.5f,  0.5f, -0.1f * upperBase,  0.0f,  1.0f,  0.0f,  0.5f, 1.0f
	};

	for (float vertex : vertices) {
		verticesData.push_back(vertex);
	}
}

void getPlane(std::vector<float> &floorVerticesData, float repeatTextureCount)
{
	float floorVertices[] = {
		-0.5f, 0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f * repeatTextureCount,
		0.5f, 0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f * repeatTextureCount, 1.0f * repeatTextureCount,
		0.5f, 0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f * repeatTextureCount, 0.0f,
		0.5f, 0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f * repeatTextureCount, 0.0f,
		-0.5f, 0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f * repeatTextureCount,
	};

	for (float vertex : floorVertices) {
		floorVerticesData.push_back(vertex);
	}
}

void SetView(int width, int height, Shader &ourShader)
{
	glm::mat4 view;
	view = camera.GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera.Zoom, (float)width / (float)height, 0.1f, 1000.0f);

	GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
	GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void ListenEvents()
{
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glfwPollEvents();
	Do_Movement();
}

void Do_Movement()
{
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; 

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}