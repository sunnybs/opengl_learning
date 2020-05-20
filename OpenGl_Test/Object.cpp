#include "pch.h"
#include "Object.h"

Object::Object()
{
}

void Object::SetVerticles(std::vector<float> verticesData) {
	verticlesCount = verticesData.size() / 5;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	// Копируем вершины в буфер для OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(float), verticesData.data(), GL_STATIC_DRAW);
	// Устанавливаем указатели на вершинные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

void Object::SetTexture(const char *filename) {
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture); 
										
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Фильтрация текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int textureWidth, textureHeight;
	unsigned char* image = SOIL_load_image(filename, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); 
}

void Object::SetRotate(float angle, glm::vec3 axis) {
	Model = glm::rotate(Model, (GLfloat)(angle * glfwGetTime()), axis);
}

void Object::SetScale(glm::vec3 scale) {
	Model = glm::scale(Model, scale);
}

void Object::SetTranslate(glm::vec3 translate) {
	Model = glm::translate(Model, translate);
}

void Object::Draw(Shader &shader) {

	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(VAO);
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model));
	glDrawArrays(GL_TRIANGLES, 0, verticlesCount);
	glBindVertexArray(0);

	Model = glm::mat4();
}
