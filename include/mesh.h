#ifndef MESH_H
#define MESH_H

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

struct Vertex
{
    glm::vec4 Position;
    glm::vec4 Normal;
    glm::vec4 TexCoords;
};

// only need to support Ka, Kd, Ks, Ns, map_Kd, map_Ks.
struct Material
{
    Material()
    {
//        name;
        Ns = 0.0f;
    }
    // Material Name
    std::string name;
    // Ambient Color
    glm::vec3 Ka;
    // Diffuse Color
    glm::vec3 Kd;
    // Specular Color
    glm::vec3 Ks;
    // Specular Exponent
    float Ns;
    // Diffuse Texture Map
    std::string map_Kd;
    // Specular Texture Map
    std::string map_Ks;
};

class Mesh
{
public:
    
    vector<Vertex> vertices;
    Material material;
    vector<glm::ivec3> indices;
    GLuint diffuseTex;
    GLuint specularTex;
    
    string meshName;
    GLuint hasTextures;
    
    Mesh(){}
    
    Mesh(vector<Vertex> vertices, vector<glm::ivec3> indices) {
        this->vertices = vertices;
        this->indices = indices;
    }
    
    // Draw function
    void Draw(GLuint pro)
    {
        glUseProgram(program);
        
        glBindVertexArray(vao);
        
        // Some meshes have textures(map_Kd, map_Ks), their Ka, Kd, Ks, Ns values are empty.
        // Some meshes don't have textures, then use Ka, Kd, Ks, Ns values in the .mtl file.
        // Use a uniform variable u_Has_Textures
        
        if (hasTextures == 1) {
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseTex);
            
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularTex);
        }

        glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, NULL);
        
        glBindVertexArray(0);
        
        if (hasTextures == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    void ifHasTextures() {
        if (material.map_Kd != "" && material.map_Ks != "") {
            hasTextures = 1;
        } else {
            hasTextures = 0;
        }
    }
    
    void cleanUp() {
        // Delete VAO, VBO & EBO
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteProgram(program);
    }

    void setupMesh(GLuint program)
    {
        this->program = program;
        
        // Setup vao, vbo, ebo
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::ivec3), indices.data(), GL_STATIC_DRAW);
        
        GLuint posLoc = glGetAttribLocation(program, "vert_Position");
        GLuint norLoc = glGetAttribLocation(program, "vert_Normal");
        GLuint texLoc = glGetAttribLocation(program, "vert_UV");
        
        glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
        glVertexAttribPointer(norLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(4*sizeof(float)));
        glVertexAttribPointer(texLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(8*sizeof(float)));
        
        glEnableVertexAttribArray(posLoc);
        glEnableVertexAttribArray(norLoc);
        glEnableVertexAttribArray(texLoc);
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        setupTexture();
    }
    
private:
    GLuint vao, vbo, ebo;
    GLuint program;
    
    void setupTexture() {
        
        if (hasTextures == 1) {
//            cout << "has textures" << endl;
            diffuseTex = loadTexture("./Old_man/" + material.map_Kd);
            specularTex = loadTexture("./Old_man/" + material.map_Ks);
            
            GLuint textureMapLoc  = glGetUniformLocation(program, "u_Texture_Map");
            GLuint specularMapLoc = glGetUniformLocation(program, "u_Specular_Map");
            
            glUniform1i(textureMapLoc,  0);
            glUniform1i(specularMapLoc, 1);
            
        } else {
//            cout << "no textures" <<endl;
            // Set Ka, Kd, Ks, Ns values
            glUniform4f(glGetUniformLocation(program, "Ka"), material.Ka.x, material.Ka.y, material.Ka.z, 1.0f);
            glUniform4f(glGetUniformLocation(program, "Kd"), material.Kd.x, material.Kd.y, material.Kd.z, 1.0f);
            glUniform4f(glGetUniformLocation(program, "Ks"), material.Ks.x, material.Ks.y, material.Ks.z, 1.0f);
            glUniform1i(glGetUniformLocation(program, "Ns"), material.Ns);

        }
    }

};
#endif
