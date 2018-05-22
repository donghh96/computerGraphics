#ifndef model_h
#define model_h

// OpenGL Headers
#if defined(_WIN32)
#include <GL/glew.h>
#if defined(GLEW_EGL)
#include <GL/eglew.h>
#elif defined(GLEW_OSMESA)
#define GLAPI extern
#include <GL/osmesa.h>
#elif defined(_WIN32)
#include <GL/wglew.h>
#elif !defined(__APPLE__) && !defined(__HAIKU__) || defined(GLEW_APPLE_GLX)
#include <GL/glxew.h>
#endif

// OpenGL Headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#elif defined(__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#endif

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// System Header
#include <iostream>

enum orientation_mode{GIMBAL, GLOBAL_MATRIX, LOCAL_MATRIX, GLOBAL_QUATERNION, LOCAL_QUATERNION};

void update_keyboard_modelMatrix(GLFWwindow *window, glm::mat4 &modelMatrix, orientation_mode mode);

void update_keyboard_viewMatrix(GLFWwindow *window, glm::mat4 &viewMatrix, glm::vec3 &viewPosition, const glm::vec3 &viewUp, const glm::vec3 &viewForward);

void setupMatrixes(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &viewPosition, glm::vec3 &viewUp, glm::vec3 &viewForward);

void copyMatrixesToProgram(GLuint program, glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);

#endif
