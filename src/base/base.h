#ifndef _BASE_H_
#define _BASE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void initGLFW();

GLFWwindow* createWindow();

void genTextureData(unsigned int &texture, const std::string &filePath, unsigned int format, GLint param);

void printMat4(const glm::mat4& matrix);

#endif