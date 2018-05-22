#ifndef SHADER_H
#define SHADER_H

// System Headers
#include <iostream>
#include <fstream>

// OpenGL Headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

// --------------------------------------------------------------------------------
// Shader Functions
// --------------------------------------------------------------------------------

// Read file contents
char* readFile(const char *filename);

// Check the status of a shader
GLuint checkShader(GLuint shader);

// Check the status of a program
GLuint checkProgram(GLuint program);

// Load and compile shader from source file
GLuint loadShader(GLuint type, const char *filename);

// Load and compiler program from source files
GLuint loadProgram(const char *vert_file, const char *ctrl_file, const char *eval_file, const char *geom_file, const char *frag_file);

// --------------------------------------------------------------------------------

#endif // SHADER_H