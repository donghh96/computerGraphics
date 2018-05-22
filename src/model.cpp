#include "model.h"

using namespace std;

void setupMatrixes(glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &viewPosition, glm::vec3 &viewUp, glm::vec3 &viewForward) {
    
//    modelMatrix = glm::mat4();
    float modelThetaX = 0.8f;
    float modelThetaY = 0.0f;
    
    // Calculate Model Matrix
    modelMatrix = glm::rotate(modelMatrix, modelThetaX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, modelThetaY, glm::vec3(0.0f, 1.0f, 0.0f));
//    modelMatrix = glm::scale(modelMatrix, glm::vec3(0f,0.03f,0.03f));
    
    // View Matrix
    viewPosition = glm::vec3(0.0f,  0.0f,  5.0f);
    viewUp = glm::vec3(0.0f,  1.0f,  0.0f);
    viewForward = glm::vec3(0.0f,  0.0f, -1.0f);

    // Normalise Vectors
    viewUp      = glm::normalize(viewUp);
    viewForward = glm::normalize(viewForward);
    
    // Construct View Matrix
    viewMatrix = glm::lookAt(viewPosition, viewPosition + viewForward, viewUp);
    
    // Calculate Perspective Projection
    projectionMatrix = glm::perspective(glm::radians(67.0f), 1.0f, 0.2f, 10.0f);
}

void copyMatrixesToProgram(GLuint program, glm::mat4 &modelMatrix, glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix) {
    
    glUseProgram(program);
    // Get Model Matrix location
    GLint modelLoc = glGetUniformLocation(program, "u_Model");
    
    // Copy Rotation Matrix to Shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    // Get View Matrix location
    GLint viewLoc = glGetUniformLocation(program, "u_View");
    
    // Copy View Matrix to Shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    
    // Get Projection Matrix location
    GLint projectionLoc = glGetUniformLocation(program, "u_Projection");
    
    // Copy Projection Matrix to Shader
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void update_keyboard_viewMatrix(GLFWwindow *window, glm::mat4 &viewMatrix, glm::vec3 &viewPosition, const glm::vec3 &viewUp, const glm::vec3 &viewForward) {
    if(glfwGetKey(window, GLFW_KEY_LEFT)) {
        // Move Closer
        viewPosition.z -= 0.05f;
        
        // Construct View Matrix
        viewMatrix = glm::lookAt(viewPosition, viewPosition + viewForward, viewUp);
    }
    
    if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
        // Move Away
        viewPosition.z += 0.05f;
        
        // Construct View Matrix
        viewMatrix = glm::lookAt(viewPosition, viewPosition + viewForward, viewUp);
    }
    
    if(glfwGetKey(window, GLFW_KEY_UP)) {
        // Move Away
        viewPosition.y += 0.05f;
        
        // Construct View Matrix
        viewMatrix = glm::lookAt(viewPosition, viewPosition + viewForward, viewUp);
    }
    
    if(glfwGetKey(window, GLFW_KEY_DOWN)) {
        // Move Away
        viewPosition.y -= 0.05f;
        
        // Construct View Matrix
        viewMatrix = glm::lookAt(viewPosition, viewPosition + viewForward, viewUp);
    }
}

void update_keyboard_modelMatrix(GLFWwindow *window, glm::mat4 &modelMatrix, orientation_mode mode) {
    // Gimbal
    static glm::vec3 rotations(0.0f, 0.0f, 0.0f);
    
    // Quaternion
    static glm::quat modelQuaternion(1.0f, 0.0f, 0.0f, 0.0f);
    
    if(mode == GIMBAL) {
        // ----------------------------------------
        // Angles
        // ----------------------------------------
        // Key Inputs
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // Pitch +
            rotations.x += 0.05;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // Pitch -
            rotations.x -= 0.05;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // Yaw +
            rotations.y += 0.05;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // Yaw -
            rotations.y -= 0.05;
        }
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            // Roll -
            rotations.z += 0.05;
        }
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            // Roll +
            rotations.z -= 0.05;
        }
        // ----------------------------------------
        // Calculate Model Matrix
        glm::mat4 modelRx = glm::rotate(glm::mat4(), rotations.x, glm::vec3(1,0,0));
        glm::mat4 modelRy = glm::rotate(glm::mat4(), rotations.y, glm::vec3(0,1,0));
        glm::mat4 modelRz = glm::rotate(glm::mat4(), rotations.z, glm::vec3(0,0,1));
        
        modelMatrix *= modelRy * modelRx * modelRz;
        // ----------------------------------------
    } else if(mode == GLOBAL_MATRIX) {
        // ----------------------------------------
        // Matrix Transformation - Global
        // ----------------------------------------
        // Key Inputs
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // Pitch +
            modelMatrix = glm::rotate(glm::mat4(),  0.05f, glm::vec3(1,0,0)) * modelMatrix;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // Pitch -
            modelMatrix = glm::rotate(glm::mat4(), -0.05f, glm::vec3(1,0,0)) * modelMatrix;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // Yaw +
            modelMatrix = glm::rotate(glm::mat4(),  0.05f, glm::vec3(0,1,0)) * modelMatrix;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // Yaw -
            modelMatrix = glm::rotate(glm::mat4(), -0.05f, glm::vec3(0,1,0)) * modelMatrix;
        }
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            // Roll -
            modelMatrix = glm::rotate(glm::mat4(),  0.05f, glm::vec3(0,0,1)) * modelMatrix;
        }
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            // Roll +
            modelMatrix = glm::rotate(glm::mat4(), -0.05f, glm::vec3(0,0,1)) * modelMatrix;
        }
        // ----------------------------------------
    } else if(mode == LOCAL_MATRIX) {
        // ----------------------------------------
        // Matrix Transformation - Local
        // ----------------------------------------
        // Key Inputs
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // Pitch +
            modelMatrix = modelMatrix * glm::rotate(glm::mat4(),  0.05f, glm::vec3(1,0,0));
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // Pitch -
            modelMatrix = modelMatrix * glm::rotate(glm::mat4(), -0.05f, glm::vec3(1,0,0));
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // Yaw +
            modelMatrix = modelMatrix * glm::rotate(glm::mat4(),  0.05f, glm::vec3(0,1,0));
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // Yaw -
            modelMatrix = modelMatrix * glm::rotate(glm::mat4(), -0.05f, glm::vec3(0,1,0));
        }
//        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
//            // Roll -
//            modelMatrix = modelMatrix * glm::rotate(glm::mat4(),  0.05f, glm::vec3(0,0,1));
//        }
//        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
//            // Roll +
//            modelMatrix = modelMatrix * glm::rotate(glm::mat4(), -0.05f, glm::vec3(0,0,1));
//        }
    } else if(mode == GLOBAL_QUATERNION) {
        // ----------------------------------------
        // Quaternion Transformation - Global
        // ----------------------------------------
        
//        cout << "i'm here GLOBAL_QUATERNION" <<endl;
        // Key Inputs
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // Pitch +
            modelQuaternion = glm::quat(glm::cos(0.025f),  glm::sin(0.025f), 0.0f, 0.0f) * modelQuaternion;
            cout << "pressed W" << endl;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // Pitch -
            modelQuaternion = glm::quat(glm::cos(0.025f), -glm::sin(0.025f), 0.0f, 0.0f) * modelQuaternion;
            cout << "pressed S" << endl;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // Yaw +
            modelQuaternion = glm::quat(glm::cos(0.025f), 0.0f,  glm::sin(0.025f), 0.0f) * modelQuaternion;
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // Yaw -
            modelQuaternion = glm::quat(glm::cos(0.025f), 0.0f, -glm::sin(0.025f), 0.0f) * modelQuaternion;
            modelMatrix = modelMatrix * glm::mat4_cast(modelQuaternion);
        }
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            // Roll -
            modelQuaternion = glm::quat(glm::cos(0.025f), 0.0f, 0.0f,  glm::sin(0.025f)) * modelQuaternion;
        }
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            // Roll +
            modelQuaternion = glm::quat(glm::cos(0.025f), 0.0f, 0.0f, -glm::sin(0.025f)) * modelQuaternion;
        }
        
        // Check Quaternion
        float w = modelQuaternion.w;
        float x = modelQuaternion.x;
        float y = modelQuaternion.y;
        float z = modelQuaternion.z;
        
        // Check Quaternion Magnitude
        if(glm::abs((x*x + y*y + z*z + w*w) - 1.0f) > 0.00001) {
            // Renormalise Model Quaternion
            std::cout << "Renormalising Model Orientation" << std::endl;
            modelQuaternion = glm::normalize(modelQuaternion);
        }
        
        // Convert to 4x4 Matrix
//        modelMatrix = modelMatrix * glm::mat4_cast(modelQuaternion);
        
        // ----------------------------------------
    } else if(mode == LOCAL_QUATERNION) {
        // ----------------------------------------
        // Matrix Transformation - Local
        // ----------------------------------------
        // Key Inputs
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            // Pitch +
            modelQuaternion = modelQuaternion * glm::quat(glm::cos(0.025f),  glm::sin(0.025f), 0.0f, 0.0f);
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            // Pitch -
            modelQuaternion = modelQuaternion * glm::quat(glm::cos(0.025f), -glm::sin(0.025f), 0.0f, 0.0f);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            // Yaw +
            modelQuaternion = modelQuaternion * glm::quat(glm::cos(0.025f), 0.0f,  glm::sin(0.025f), 0.0f);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            // Yaw -
            modelQuaternion = modelQuaternion * glm::quat(glm::cos(0.025f), 0.0f, -glm::sin(0.025f), 0.0f);
        }
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            // Roll -
            modelQuaternion = modelQuaternion * glm::quat(glm::cos(0.025f), 0.0f, 0.0f,  glm::sin(0.025f));
        }
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            // Roll +
            modelQuaternion = modelQuaternion * glm::quat(glm::cos(0.025f), 0.0f, 0.0f, -glm::sin(0.025f));
        }
        
        // Check Quaternion
        float w = modelQuaternion.w;
        float x = modelQuaternion.x;
        float y = modelQuaternion.y;
        float z = modelQuaternion.z;
        
        // Check Quaternion Magnitude
        if(glm::abs((x*x + y*y + z*z + w*w) - 1.0f) > 0.00001) {
            // Renormalise Model Quaternion
            std::cout << "Renormalising Model Orientation" << std::endl;
            modelQuaternion = glm::normalize(modelQuaternion);
        }
        
        // Convert to 4x4 Matrix
        modelMatrix *= glm::mat4_cast(modelQuaternion);
    }
}

