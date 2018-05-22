
#ifndef PLANET_H
#define PLANET_H

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

class Planet
{
public:
    
    vector<Vertex> vertices;
    vector<glm::ivec3> indices;
    GLuint texture;

    string planetName;
    float planetRadius;
    float orbitRadius; // Better use this to calculate model matrix, transform.
    string textureFile;
    
    Planet(){}
        
    Planet(string planetName, float planetRadius, float orbitRadius, string textureFile) {
        this->planetName = planetName;
        this->planetRadius = planetRadius;
        this->orbitRadius = orbitRadius;
        this->textureFile = textureFile;
    }
    
    // Draw function
    void Draw()
    {
        glUseProgram(program);
        
        glBindVertexArray(vao);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, specularTex);
        
        glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, NULL);
        
        glBindVertexArray(0);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void cleanUp() {
        // Delete VAO, VBO & EBO
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteProgram(program);
    }
    
    void setupPlanet(GLuint program) {
        this->program = program;
        createPlanet22(vertices, indices, planetRadius, 30);
        
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
        cout << "planet texture:" << texture << endl;
    }
    
    void setupMatrixes() {
        
    }
    
private:
    GLuint vao, vbo, ebo;
    GLuint program;
    
    void setupTexture() {
        
        texture = loadTexture(textureFile);
//        int x, y, n;
//        diffuseTex = loadTexture2D(textureFile, x, y, n);
//        specularTex = loadTexture("/Users/apple/huihui/Assignment3/Assignment3/planetTextures/earthspec1k.jpg");
        GLuint textureMapLoc  = glGetUniformLocation(program, "u_Texture_Map");
//        GLuint specularMapLoc = glGetUniformLocation(program, "u_Specular_Map");
        
        glUniform1i(textureMapLoc,  0);
//        glUniform1i(specularMapLoc, 1);
    }
    
    void createPlanet(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes, float r, int sub) {
        for (int i1 = 0; i1 < sub; i1++) {
            // Theta [0, pi)
            float theta = i1 * M_PI / sub;
            
            int offset1 = ((i1 < (sub - 1)) ? 1 : -(sub - 1)) * sub;
            
            for(int i2 = 0; i2 < sub; i2++) {
                // Phi [0, 2pi)
                float phi = i2 * M_PI * 2.0 / sub;
                
                // Cauculate position
                glm::vec4 p = glm::vec4(r * cos(theta) * cos(phi), r * sin(phi), r * sin(theta) * cos(phi), 1.0f);

                // Compute normal
                glm::vec4 u = glm::normalize(glm::vec4(p.x, p.y, p.z, 1.0f));
                
                // Add Position and Normal to buffer
                buffer.push_back(p);
                buffer.push_back(u);
                
                // Calculate index
                int k = i1 * sub + i2;
                
                int offset2 = (i2 < (sub - 1)) ? 1 : -(sub - 1);
                
                // Added indexes
                indexes.push_back(glm::ivec3(k, k + offset2, k + offset1 + offset2));
                indexes.push_back(glm::ivec3(k, k + offset1 + offset2, k + offset1));
            }
        }
    }
 
    // Todo - fix the extra vertex issue.
    void createPlanet2(std::vector<Vertex> &vertices, std::vector<glm::ivec3> &indexes, float r, int sub) {
        for (int i1 = 0; i1 < sub; i1++) {
            // Theta [0, pi) angle with y
            float theta = i1 * M_PI / (sub - 1);
            
//            int offset1 = ((i1 < (sub - 1)) ? 1 : -(sub - 1)) * sub;
            int offset1 = -sub;
            
            for(int i2 = 0; i2 < sub; i2++) {
                // Phi [0, 2pi) angle with x
                float phi = i2 * M_PI * 2.0 / sub;
                
                Vertex v;
                v.Position = glm::vec4(r * sin(theta) * cos(phi), r * cos(theta), r * sin(theta) * sin(phi), 1.0f);
                v.Normal = glm::normalize(glm::vec4(v.Position.x, v.Position.y, v.Position.z, 0.0f));
                
                // copy from example09
                v.TexCoords = glm::vec4(phi / (M_PI*2.0f), theta / M_PI, 0.0f, 1.0f);
                
                // Add Position and Normal to vertices
                vertices.push_back(v);
                
                // Calculate index
                int offset2 = (i2 < (sub - 1)) ? 1 : -(sub - 1);
                
                // Added indexes
                if (i1 > 0 ) {
                    int k = i1 * sub + i2;
                
                indexes.push_back(glm::ivec3(k + offset1, k,           k + offset2));
                indexes.push_back(glm::ivec3(k + offset1, k + offset2, k + offset1 + offset2));
                }
            }
        }
    }

    // Try to fix the texture issue.
    void createPlanet22(std::vector<Vertex> &vertices, std::vector<glm::ivec3> &indexes, float r, int sub) {
        
        
        // north pole, (0, r, 0)
        // is there south pople (0, -r, 0)?
        for (int i1 = 0; i1 < sub; i1++) {
            // Theta [0, pi) angle with y
            float theta = i1 * M_PI / (sub - 1);
            
//             int offset1 = ((i1 < (sub - 1)) ? 1 : -(sub - 1)) * sub;
            int offset1 = sub;
            
            for(int i2 = 0; i2 <= sub; i2++) {
                // Phi [0, 2pi) angle with x
                float phi = i2 * M_PI * 2.0 / sub;
                
                Vertex v;
                v.Position = glm::vec4(r * sin(theta) * cos(phi), r * cos(theta), r * sin(theta) * sin(phi), 1.0f);
                v.Normal = glm::normalize(glm::vec4(v.Position.x, v.Position.y, v.Position.z, 0.0f));
                
                // copy from example09
                v.TexCoords = glm::vec4(phi / (M_PI*2.0f), theta / M_PI, 0.0f, 1.0f);
                
                // Add Position and Normal to vertices
                vertices.push_back(v);
                
//                cout << "v:(" << v.Position.x << "," << v.Position.y << "," << v.Position.z << ")"<< endl;
//                cout << "i1:" << i1 << "," << "i2:" << i2 << endl;
                
                // Calculate index
//                int offset2 = (i2 < (sub - 1)) ? 1 : -(sub - 1);
                int offset2 = 1;
                
                // Added indexes
                if (i1 > 0) {
                    int k = i1 * (sub + 1) + i2;
                    
                    indexes.push_back(glm::ivec3(k - offset1, k,           k + offset2));
                    indexes.push_back(glm::ivec3(k - offset1, k + offset2, k - offset1 + offset2));
                    
//                    cout << "indexes:(" << k - offset1 << "," << k << "," << k+offset2 << ")" <<endl;
//                    cout << "indexes:(" << k - offset1 << "," << k  + offset2 << "," << k-offset1+offset2 << ")" <<endl;
                }
            }
        }
    }

    void createPlanet222(std::vector<Vertex> &vertices, std::vector<glm::ivec3> &indexes, float r, int sub) {
        
        for (int i1 = 0; i1 < sub; i1++) {
            // Theta [0, pi) angle with y
            float theta = i1 * M_PI / (sub - 1);
            
            // int offset1 = ((i1 < (sub - 1)) ? 1 : -(sub - 1)) * sub;
            int offset1 = -sub;
            
            for(int i2 = 0; i2 <= sub; i2++) {
                // Phi [0, 2pi) angle with x
                float phi = i2 * M_PI * 2.0 / sub;
                
                Vertex v;
                v.Position = glm::vec4(r * sin(theta) * cos(phi), r * cos(theta), r * sin(theta) * sin(phi), 1.0f);
                v.Normal = glm::normalize(glm::vec4(v.Position.x, v.Position.y, v.Position.z, 0.0f));
                
                // copy from example09
                v.TexCoords = glm::vec4(phi / (M_PI*2.0f), theta / M_PI, 0.0f, 1.0f);
                
                // Add Position and Normal to vertices
                vertices.push_back(v);
                
                // Calculate index
                //                int offset2 = (i2 < (sub - 1)) ? 1 : -(sub - 1);
                int offset2 = 1;
                
                // Added indexes
                if (i1 > 0) {
                    int k = i1 * sub + i2;
                    
                    indexes.push_back(glm::ivec3(k + offset1, k,           k + offset2));
                    indexes.push_back(glm::ivec3(k + offset1, k + offset2, k + offset1 + offset2));
                }
            }
        }
    }
    
    void createSphere3(std::vector<glm::vec4> &buffer, std::vector<glm::ivec3> &indexes, float r, int sub1, int sub2) {
        // Longitude
        for(int i1 = 0; i1 < sub1; i1++) {
            // Theta [0, pi]
            float theta = i1 * M_PI / (sub1-1);
            
            // Longitude offset
            int offset1 = -sub2;
            
            // Latitude
            for(int i2 = 0; i2 < sub2; i2++) {
                // Phi [0, 2pi)
                float phi = i2 * M_PI * 2.0 / sub2;
                
                // Calculate point
                glm::vec4 p = glm::vec4(r*sin(theta)*cos(phi), r*cos(theta), r*sin(theta)*sin(phi), 1.0f);
                
                // Calculate normal
                glm::vec4 u = glm::normalize(glm::vec4(p.x, p.y, p.z, 0.0f));
                
                // Add position, normal and UV to buffer
                buffer.push_back(p);
                buffer.push_back(u);
                buffer.push_back(glm::vec4(phi / (M_PI*2.0f), theta / M_PI, 0.0f, 1.0f));
                
                // Latitude offset
                int offset2 = (i2 < (sub2 - 1)) ? 1 : -(sub2 - 1);
                
                // Add triangles between layers
                if(i1 > 0) {
                    // Index of current vertex
                    int k = i1*sub2 + i2;
                    
                    // Add Indexes
                    indexes.push_back(glm::ivec3(k + offset1, k,           k + offset2));
                    indexes.push_back(glm::ivec3(k + offset1, k + offset2, k + offset1 + offset2));
                }
            }
        }
    }
};
#endif
