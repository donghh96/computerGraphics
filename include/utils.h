#ifndef UTILS_H
#define UTILS_H

// System Headers
#include <iostream>
#include <fstream>
#include <cmath>

// OpenGL Headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// --------------------------------------------------------------------------------
// GLFW Functions
// --------------------------------------------------------------------------------

// Create a GLFW Window
GLFWwindow* createWindow(int width, int height, const char *title, int major = 3, int minor = 2, GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL);

// Called on Error Event
void onError(int error, const char *description);

// Called on Window Close Event
void onWindowClose(GLFWwindow *window);

// Called on Window Size Event
void onFramebufferSize(GLFWwindow *window, int width, int height);

#endif // UTILS_H
