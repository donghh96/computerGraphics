/*********************************
 Stdent name: Dong Huihui
 Student ID: 17383655
 159.709 Computer Graphics, 2018 S1
 Assignment3 - solar system
 
 http://hubblesite.org/images/gallery
 http://www.custommapmakers.org/skyboxes.php
 
 Next:
 Draw a obit.
 Add more planets.
 Fixed the sphere texture issue, maybe try tesselation shader
 Consider the real spin speed, size, orbit size
 If consider the axle angle, seems I should use "LOCAL_QUATERNION" mode for rotation
**********************************/
// System Headers
#include <iostream>
#include <fstream>
#include <vector>

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

#include "utils.h"
#include "shader.h"
#include "planet.h"
#include "camera.h"
#include "skybox.h"
#include "orbit.h"

using namespace std;

// Camera
Camera *camera;

// --------------------------------------------------------------------------------
// Mouse Input
// --------------------------------------------------------------------------------
void onMouseButton(GLFWwindow *window, int button, int action, int mods) {
    // Update Camera
    camera->onMouseButton(window, button, action, mods);
}

void onCursorPosition(GLFWwindow *window, double x, double y) {
    // Update Camera
    camera->onCursorPosition(window, x, y);
}

static void copyPlanetMatrix(glm::mat4 &projectionMatrix, glm::mat4 &planetModelMatrix, const Planet &planet, GLuint planetprogram) {
    
    planetModelMatrix = glm::translate(planetModelMatrix, glm::vec3(planet.orbitRadius, 0.0f, 0.0f));
    glUseProgram(planetprogram);
    glUniformMatrix4fv(glGetUniformLocation(planetprogram, "u_Model"), 1, GL_FALSE, glm::value_ptr(planetModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(planetprogram, "u_View"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(planetprogram, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

static void rotationAndObit(const Planet &planet, glm::mat4 &planetModelMatrix, GLuint planetprogram, float rotationSpeed, float orbitSpeed) {
    glUseProgram(planetprogram);
    planetModelMatrix = glm::rotate(planetModelMatrix, rotationSpeed * 0.02f, glm::vec3(0,1,0));
    
    // Orbit the sun，update X, Z positions
    planetModelMatrix[3][0] = glm::cos(glfwGetTime() * orbitSpeed * 0.2f) * planet.orbitRadius;
    planetModelMatrix[3][2] = -glm::sin(glfwGetTime() * orbitSpeed * 0.2f) * planet.orbitRadius;
    glUniformMatrix4fv(glGetUniformLocation(planetprogram, "u_Model"), 1, GL_FALSE, glm::value_ptr(planetModelMatrix));
}

int main() {
    
    // ----------------------------------------
    // Setup glfw and Initialise OpenGL
    // ----------------------------------------

    // Set Error Callback
    glfwSetErrorCallback(onError);

    // Initialise GLFW
    if (!glfwInit()) {
        // Return Error
        return 1;
    }

    // Create Window
    GLFWwindow *window = createWindow(900, 900, "Assignment3 - Solar System", 3, 2);

    // Check Window
    if (window == NULL) {
        // Print Error Message
        std::cerr << "Error: create window or context failed." << std::endl;

        // Return Error
        return 1;
    }

#if defined(_WIN32)
    // Initialise GLEW
    if (glewInit() != GLEW_OK) {
        // Return Error
        return 1;
    }
#endif

    // Set window callback functions
    glfwSetFramebufferSizeCallback(window, onFramebufferSize);
    glfwSetWindowCloseCallback(window, onWindowClose);
    
    // Set mouse input callback functions
    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onCursorPosition);

    // Initialise OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    
    // Camera
    camera = new GimbalFreeLookCamera(window);
//    camera = new FreeLookCamera(window);
    
    // Load programs
    GLuint skybox_program = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/skybox.vert.glsl",  NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/skybox.frag.glsl");
    GLuint sunprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/sun.vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/sun.frag.glsl");
    
    GLuint mercuryprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/texture.frag.glsl");
    GLuint venusprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/texture.frag.glsl");
    GLuint earthprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/texture.frag.glsl");
    GLuint marsprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/texture.frag.glsl");
    GLuint jupiterprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/texture.frag.glsl");
    GLuint saturnprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/texture.frag.glsl");
    GLuint uranusprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/texture.frag.glsl");
    GLuint neptuneprogram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/texture.frag.glsl");

    GLuint orbitProgram = loadProgram("/Users/apple/huihui/Assignment3/Assignment3/shader/orbit.vert.glsl", NULL, NULL, NULL, "/Users/apple/huihui/Assignment3/Assignment3/shader/orbit.frag.glsl");
    
    
    Skybox skybox;
    Planet sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune;
    Orbit mercuryOrbit, venusOrbit, earthOrbit,marsOrbit, jupiterOrbit, saturnOrbit, uranusOrbit, neptuneOrbit;
    
    // Setup
    sun = Planet("sun", 0.6f, 0.0f, "/Users/apple/huihui/Assignment3/Assignment3/images/sunmap.jpg");
    
    // Setup each planet, (planet name, radius, orbit radius, texture file)
    mercury = Planet("mercury", 0.1f, 1.0f, "/Users/apple/huihui/Assignment3/Assignment3/images/mercurymap.jpg");
    venus   = Planet("venus",   0.2f, 1.5f, "/Users/apple/huihui/Assignment3/Assignment3/images/venusmap.jpg");
    earth   = Planet("earth",   0.3f, 2.0f, "/Users/apple/huihui/Assignment3/Assignment3/images/earthmap1k.jpg");
    mars    = Planet("mars",    0.2f, 2.5f, "/Users/apple/huihui/Assignment3/Assignment3/images/mars_1k_color.jpg");
    jupiter = Planet("jupiter", 0.5f, 3.5f, "/Users/apple/huihui/Assignment3/Assignment3/images/jupitermap.jpg");
    saturn  = Planet("saturn",  0.4f, 4.5f, "/Users/apple/huihui/Assignment3/Assignment3/images/saturnmap.jpg");
    uranus  = Planet("uranus",  0.3f, 5.3f, "/Users/apple/huihui/Assignment3/Assignment3/images/uranusmap.jpg");
    neptune = Planet("neptune", 0.3f, 6.1f, "/Users/apple/huihui/Assignment3/Assignment3/images/neptunemap.jpg");
    
    // orbits
    mercuryOrbit = Orbit(mercury.orbitRadius);
    venusOrbit = Orbit(venus.orbitRadius);
    earthOrbit = Orbit(earth.orbitRadius);
    marsOrbit = Orbit(mars.orbitRadius);
    jupiterOrbit = Orbit(jupiter.orbitRadius);
    saturnOrbit = Orbit(saturn.orbitRadius);
    uranusOrbit = Orbit(uranus.orbitRadius);
    neptuneOrbit = Orbit(neptune.orbitRadius);
    

    // Setup all Model, Projection, View matrixes in main.cpp for now, to make it clear.
    
    // Projection Matrix, shared by skybox, sun and all other planets.
    // Calculate Perspective Projection
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(67.0f), 1.0f, 0.2f, 30.0f);
    
    // Copy Skybox View, Projection Matrixes to Shader
    glUseProgram(skybox_program);
    glUniformMatrix4fv(glGetUniformLocation(skybox_program, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getOrientationMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(skybox_program, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

 
    // Copy Sun Model, View, Projection Matrixes to Shader
    glm::mat4 sunModelMatrix = glm::mat4();
    
    glUseProgram(sunprogram);
    glUniformMatrix4fv(glGetUniformLocation(sunprogram, "u_Model"), 1, GL_FALSE, glm::value_ptr(sunModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(sunprogram, "u_View"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(sunprogram, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    // Copy Mercury Model, View, Projection Matrixes to Shader
    glm::mat4 mercuryModelMatrix = glm::mat4();
    copyPlanetMatrix(projectionMatrix, mercuryModelMatrix, mercury, mercuryprogram);

    // Copy Venus Model, View, Projection Matrixes to Shader
    glm::mat4 venusModelMatrix = glm::mat4();
    copyPlanetMatrix(projectionMatrix, venusModelMatrix, venus, venusprogram);

    // Copy Earth Model, View, Projection Matrixes to Shader
    glm::mat4 earthModelMatrix = glm::mat4();
    copyPlanetMatrix(projectionMatrix, earthModelMatrix, earth, earthprogram);

    // Copy Mars Model, View, Projection Matrixes to Shader
    glm::mat4 marsModelMatrix = glm::mat4();
    copyPlanetMatrix(projectionMatrix, marsModelMatrix, mars, marsprogram);
   
    // Copy Jupiter Model, View, Projection Matrixes to Shader
    glm::mat4 jupiterModelMatrix = glm::mat4();
    copyPlanetMatrix(projectionMatrix, jupiterModelMatrix, jupiter, jupiterprogram);
    
    // Copy Saturn Model, View, Projection Matrixes to Shader
    glm::mat4 saturnModelMatrix = glm::mat4();
    copyPlanetMatrix(projectionMatrix, saturnModelMatrix, saturn, saturnprogram);

    // Copy Uranus Model, View, Projection Matrixes to Shader
    glm::mat4 uranusModelMatrix = glm::mat4();
    copyPlanetMatrix(projectionMatrix, uranusModelMatrix, uranus, uranusprogram);
    
    // Copy Neptune Model, View, Projection Matrixes to Shader
    glm::mat4 neptuneModelMatrix = glm::mat4();
    copyPlanetMatrix(projectionMatrix, neptuneModelMatrix, neptune, neptuneprogram);

    // Copy orbit Model, View, Projection Matrixes to Shader
    glm::mat4 orbitModelMatrix = glm::mat4();
    glUseProgram(orbitProgram);
    glUniformMatrix4fv(glGetUniformLocation(orbitProgram, "u_Model"), 1, GL_FALSE, glm::value_ptr(orbitModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(orbitProgram, "u_View"), 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(orbitProgram, "u_Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    // Setup vao, vbo, veo
    skybox.setupSkybox(skybox_program);
    sun.setupPlanet(sunprogram);
    
    mercury.setupPlanet(mercuryprogram);
    venus.setupPlanet(venusprogram);
    earth.setupPlanet(earthprogram);
    mars.setupPlanet(marsprogram);
    jupiter.setupPlanet(jupiterprogram);
    saturn.setupPlanet(saturnprogram);
    uranus.setupPlanet(uranusprogram);
    neptune.setupPlanet(neptuneprogram);
    
    mercuryOrbit.setupOrbit(orbitProgram);
    venusOrbit.setupOrbit(orbitProgram);
    earthOrbit.setupOrbit(orbitProgram);
    marsOrbit.setupOrbit(orbitProgram);
    jupiterOrbit.setupOrbit(orbitProgram);
    saturnOrbit.setupOrbit(orbitProgram);
    uranusOrbit.setupOrbit(orbitProgram);
    neptuneOrbit.setupOrbit(orbitProgram);
    
    float time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // Make the context of the given window current on the calling thread
        glfwMakeContextCurrent(window);
        
        // Set clear (background) colour to dark grey
        glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
        
        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Update Time
        float current_time = glfwGetTime();
        float dt = current_time - time;
        time = current_time;
        
        // Update Camera
        camera->update(dt);
        
        skybox.Draw();
        
        glUseProgram(sunprogram);
        glUniformMatrix4fv(glGetUniformLocation(sunprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(mercuryprogram);
        glUniformMatrix4fv(glGetUniformLocation(mercuryprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(venusprogram);
        glUniformMatrix4fv(glGetUniformLocation(venusprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(earthprogram);
        glUniformMatrix4fv(glGetUniformLocation(earthprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(marsprogram);
        glUniformMatrix4fv(glGetUniformLocation(marsprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(jupiterprogram);
        glUniformMatrix4fv(glGetUniformLocation(jupiterprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(saturnprogram);
        glUniformMatrix4fv(glGetUniformLocation(saturnprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(uranusprogram);
        glUniformMatrix4fv(glGetUniformLocation(uranusprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(neptuneprogram);
        glUniformMatrix4fv(glGetUniformLocation(neptuneprogram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

        glUseProgram(orbitProgram);
        glUniformMatrix4fv(glGetUniformLocation(orbitProgram, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
        
        glUseProgram(skybox_program);
        glUniformMatrix4fv(glGetUniformLocation(skybox_program, "u_View"),  1, GL_FALSE, glm::value_ptr(camera->getOrientationMatrix()));

        // Draw sun
        sun.Draw();
        
        // Draw Planets
        mercury.Draw();
        venus.Draw();
        earth.Draw();
        mars.Draw();
        jupiter.Draw();
        saturn.Draw();
        uranus.Draw();
        neptune.Draw();
        
        // Draw Orbits
        mercuryOrbit.Draw();
        venusOrbit.Draw();
        earthOrbit.Draw();
        marsOrbit.Draw();
        jupiterOrbit.Draw();
        saturnOrbit.Draw();
        uranusOrbit.Draw();
        neptuneOrbit.Draw();
        
        // Sun rotation, update its model matrix
        glUseProgram(sunprogram);
        sunModelMatrix = sunModelMatrix * glm::rotate(glm::mat4(), 0.02f, glm::vec3(0,1,0));
        glUniformMatrix4fv(glGetUniformLocation(sunprogram, "u_Model"), 1, GL_FALSE, glm::value_ptr(sunModelMatrix));
        
        // Mercury rotation and orbit, update its model matrix
        rotationAndObit(mercury, mercuryModelMatrix, mercuryprogram, 0.2f, 4.15f);

        // Venus rotation and orbit, update its model matrix
        rotationAndObit(venus, venusModelMatrix, venusprogram, 0.1f, 1.63f);

        // Earth rotation and orbit, update its model matrix
        rotationAndObit(earth, earthModelMatrix, earthprogram, 1.0f, 1.0f);

        // Mars rotation and orbit, update its model matrix
        rotationAndObit(mars, marsModelMatrix, marsprogram, 0.97f, 0.53f);

        // Jupiter rotation and orbit, update its model matrix
        rotationAndObit(jupiter, jupiterModelMatrix, jupiterprogram, 2.44f, 0.08f);

        // Saturn rotation and orbit, update its model matrix
        rotationAndObit(saturn, saturnModelMatrix, saturnprogram, 2.25f, 0.03f);

        // Uranus rotation and orbit, update its model matrix
        rotationAndObit(uranus, uranusModelMatrix, uranusprogram, 1.39f, 0.02f);

        // Neptune rotation and orbit, update its model matrix
        rotationAndObit(neptune, neptuneModelMatrix, neptuneprogram, 1.49f, 0.01f);

        // Swap the back and front buffers
        glfwSwapBuffers(window);
        
        // Poll window events
        glfwPollEvents();
        
    }
    
    // Delete all vaos, vbos, ebos, Programs
    skybox.cleanUp();
    sun.cleanUp();
    
    mercury.cleanUp();
    venus.cleanUp();
    earth.cleanUp();
    mars.cleanUp();
    jupiter.cleanUp();
    saturn.cleanUp();
    uranus.cleanUp();
    neptune.cleanUp();
    
    mercuryOrbit.cleanUp();
    venusOrbit.cleanUp();
    earthOrbit.cleanUp();
    marsOrbit.cleanUp();
    jupiterOrbit.cleanUp();
    saturnOrbit.cleanUp();
    uranusOrbit.cleanUp();
    neptuneOrbit.cleanUp();
    
    // Stop receiving events for the window and free resources; this must be
    // called from the main thread and should not be invoked from a callback
    glfwDestroyWindow(window);
    
    // Terminate GLFW
    glfwTerminate();
    
    return 0;
}
