#ifndef SKYBOX_H
#define SKYBOX_H

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

#include "image.h"

using namespace std;

class Skybox
{
public:
    
    // Vertex and Index buffers (host)
    vector<glm::vec4> skybox_buffer;
    vector<glm::ivec3> skybox_indexes;
    GLuint texture;
    
    Skybox(){}
    
    // Draw function
    void Draw()
    {
        // Use Skybox Program
        glUseProgram(program);
        
        // Bind Vertex Array Object
        glBindVertexArray(skybox_vao);
        
        // Disable Depth-Testing
        glDisable(GL_DEPTH_TEST);
        
        // Set active Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        
        // Bind Texture Map
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        
        // Draw Elements (Triangles)
        glDrawElements(GL_TRIANGLES, skybox_indexes.size() * 3, GL_UNSIGNED_INT, NULL);
        
        // Renable Depth-Testing
        glEnable(GL_DEPTH_TEST);

        // Set active Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        
        // Unbind Texture Map
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    }
    
    void cleanUp() {
        // Delete VAO, VBO & EBO
        glDeleteVertexArrays(1, &skybox_vao);
        glDeleteBuffers(1, &skybox_vbo);
        glDeleteBuffers(1, &skybox_ebo);
        glDeleteProgram(program);
    }
    
    void setupSkybox(GLuint program)
    {
        this->program = program;
        createSkybox(skybox_buffer, skybox_indexes);

        
        // Create VAO, VBO & EBO
        glGenVertexArrays(1, &skybox_vao);
        glGenBuffers(1, &skybox_vbo);
        glGenBuffers(1, &skybox_ebo);
        
        // Bind VAO, VBO & EBO
        glBindVertexArray(skybox_vao);
        glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox_ebo);
        
        // Load Vertex Data
        glBufferData(GL_ARRAY_BUFFER, skybox_buffer.size() * sizeof(glm::vec4), skybox_buffer.data(), GL_STATIC_DRAW);
        
        // Load Element Data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, skybox_indexes.size() * sizeof(glm::ivec3), skybox_indexes.data(), GL_STATIC_DRAW);

        // Get Position Attribute location (must match name in shader)
        GLuint skybox_posLoc = glGetAttribLocation(program, "vert_Position");
        
        // Set Vertex Attribute Pointers
        glVertexAttribPointer(skybox_posLoc, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);
        
        // Enable Vertex Attribute Arrays
        glEnableVertexAttribArray(skybox_posLoc);
        
        // Unbind VAO, VBO & EBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        setupTexture();
        
        cout << "texture is: " << texture << endl;
    }
    
private:
    GLuint skybox_vao, skybox_vbo, skybox_ebo;
    GLuint program;
//    GLuint texture;
    
    void setupTexture() {
        
        // Load Texture Map
        int x, y, n;
        // Filenames
        // posz - up; negz - down; posx - left, negx - right, negy - front, posy - back;
        const char *filenames[6] = {"/Users/apple/huihui/Assignment3/Assignment3/images/purplenebula_lf.tga",
            "/Users/apple/huihui/Assignment3/Assignment3/images/purplenebula_rt.tga",
            "/Users/apple/huihui/Assignment3/Assignment3/images/purplenebula_ft.tga",
            "/Users/apple/huihui/Assignment3/Assignment3/images/purplenebula_bk.tga",
            "/Users/apple/huihui/Assignment3/Assignment3/images/purplenebula_up.tga",
            "/Users/apple/huihui/Assignment3/Assignment3/images/purplenebula_dn.tga"};
        
        // Load Cubemap
        texture = loadTextureCubeMap(filenames, x, y, n);
        
        // Set Texture Unit
        glUseProgram(program);
        GLuint textureMapLoc  = glGetUniformLocation(program, "u_Texture_Map");
        
        glUniform1i(textureMapLoc,  0);
//        glUniform1i(glGetUniformLocation(program, "u_texture_Map"), 0);
    }
    
    // Create Skybox (Cube with Positions only)
    void createSkybox(vector<glm::vec4> &buffer, vector<glm::ivec3> &indexes) {
        // Vertices
        buffer.push_back(glm::vec4(-1.0f,  1.0f, -1.0f,  1.0f)); // Left  - Top    - Back  - 0
        buffer.push_back(glm::vec4( 1.0f,  1.0f, -1.0f,  1.0f)); // Right - Top    - Back  - 1
        buffer.push_back(glm::vec4(-1.0f,  1.0f,  1.0f,  1.0f)); // Left  - Top    - Front - 2
        buffer.push_back(glm::vec4( 1.0f,  1.0f,  1.0f,  1.0f)); // Right - Top    - Front - 3
        buffer.push_back(glm::vec4(-1.0f, -1.0f, -1.0f,  1.0f)); // Left  - Bottom - Back  - 4
        buffer.push_back(glm::vec4( 1.0f, -1.0f, -1.0f,  1.0f)); // Right - Bottom - Back  - 5
        buffer.push_back(glm::vec4(-1.0f, -1.0f,  1.0f,  1.0f)); // Left  - Bottom - Front - 6
        buffer.push_back(glm::vec4( 1.0f, -1.0f,  1.0f,  1.0f)); // Right - Bottom - Front - 7
        
        // Indexes
        indexes.push_back(glm::ivec3(0, 2, 3)); // Top
        indexes.push_back(glm::ivec3(0, 3, 1));
        indexes.push_back(glm::ivec3(4, 5, 7)); // Bottom
        indexes.push_back(glm::ivec3(4, 7, 6));
        indexes.push_back(glm::ivec3(0, 2, 6)); // Left
        indexes.push_back(glm::ivec3(0, 6, 4));
        indexes.push_back(glm::ivec3(3, 1, 5)); // Right
        indexes.push_back(glm::ivec3(3, 5, 7));
        indexes.push_back(glm::ivec3(2, 3, 7)); // Front
        indexes.push_back(glm::ivec3(2, 7, 6));
        indexes.push_back(glm::ivec3(1, 0, 4)); // Back
        indexes.push_back(glm::ivec3(1, 4, 5));
    }
};
#endif

