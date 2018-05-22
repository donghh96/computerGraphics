// Project Headers 
#include "utils.h"

// --------------------------------------------------------------------------------
// GLFW Functions
// --------------------------------------------------------------------------------

// Create a GLFW Window
GLFWwindow* createWindow(int width, int height, const char *title, int major, int minor, GLFWmonitor *monitor, GLFWwindow *share) {
	// Request an OpenGL context with specific features
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

	// If Version is 3 or higher
	if (major >= 3) {
		// Request Forward Compatibility
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		// If version is 3.2 or higher
		if (major > 3 || minor >= 2) {
			// Request Core Profile
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		}
	}

	// Create GLFW Window
	GLFWwindow *window = glfwCreateWindow(width, height, title, monitor, share);

	// Check Returned Window
	if (window == NULL) {
		return NULL;
	}

	// Set Context
	glfwMakeContextCurrent(window);

	// Return GLFW window
	return window;
}

// --------------------------------------------------------------------------------
// GLFW Callbacks
// --------------------------------------------------------------------------------

// Called on Error Event
void onError(int error, const char *description) {
	// Print Error message
	std::cerr << "Error: " << error << " : " << description << std::endl;
}

// Called on Window Close Event
void onWindowClose(GLFWwindow *window) {
	// Nothing to do right now
	// Do not call glfwDestroyWindow from here
}

// Called on Window Size Event
void onFramebufferSize(GLFWwindow *window, int width, int height) {
	// Set-up the window/screen coordinates
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
	glfwMakeContextCurrent(NULL);
}

