#pragma once
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


class Object
{
public:
	Object();
	void SetVerticles(std::vector<float> verticesData);
	void SetTexture(const char *filename);
	void SetRotate(float angle, glm::vec3 axis);
	void SetTranslate(glm::vec3 translate);
	void SetScale(glm::vec3 scale);
	void Draw(Shader &shader);
	GLuint VAO, VBO;
private:
	GLuint Texture;
	glm::mat4 Model;
	int verticlesCount;
};

