#ifndef ORBIT_H
#define ORBIT_H

// System Headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// OpenGL Headers
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Orbit
{
public:
    
    vector<glm::vec4> orbit_buffer;
    vector<glm::ivec2> orbit_indexes;
    
    float orbitRadius;
    
    Orbit(){}
    
    Orbit(float orbitRadius) {
        this->orbitRadius = orbitRadius;
    }
    
    // Draw function
    void Draw()
    {
        glUseProgram(program);
        
        glBindVertexArray(vao);
        
        glDrawElements(GL_LINES, orbit_indexes.size() * 2, GL_UNSIGNED_INT, NULL);
        
        glBindVertexArray(0);
        
    }
    
    void cleanUp() {
        // Delete VAO, VBO & EBO
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteProgram(program);
    }
    
    void setupOrbit(GLuint program) {
        this->program = program;
        
        createOrbit(orbit_buffer, orbit_indexes, orbitRadius, 100);
        
        // Setup vao, vbo, ebo
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        
        glBufferData(GL_ARRAY_BUFFER, orbit_buffer.size() * sizeof(glm::vec4), orbit_buffer.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, orbit_indexes.size() * sizeof(glm::ivec2), orbit_indexes.data(), GL_STATIC_DRAW);
        
        GLuint posLoc = glGetAttribLocation(program, "vert_Position");
        GLuint colLoc = glGetAttribLocation(program, "vert_Colour");
        
        glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
        glVertexAttribPointer(colLoc, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(4*sizeof(float)));
        
        glEnableVertexAttribArray(posLoc);
        glEnableVertexAttribArray(colLoc);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    }
    
private:
    GLuint vao, vbo, ebo;
    GLuint program;
    
    void createOrbit(vector<glm::vec4> &buffer, vector<glm::ivec2> &indexes, float r, int sub) {
        
        for (int i=0; i<sub; i++) {
            
            // [0,2pi)
            float theta = i * M_PI * 2 / sub;
            
            buffer.push_back(glm::vec4(r*cos(theta), 0.0f, r*sin(theta), 1.0f)); // Position
            buffer.push_back(glm::vec4(0.8, 0.8, 0.8, 0)); // Color
            
            if (i > 0) {
                indexes.push_back(glm::ivec2(i - 1, i));
            }
        }
        indexes.push_back(glm::ivec2(sub - 1, 0));
    }
};
#endif
