// OpenGL 3.3
#version 330

// Input to Vertex Shader
in vec4 vert_Position;
in vec4 vert_Normal;
in vec4 vert_UV;

// Transform Matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Output to Fragment Shader
out vec4 frag_Position;
out vec4 frag_UV;

void main() {

    //----------------------------------------------
    // Output to Fragment Shader
    //----------------------------------------------
    
    // Frag Position
    frag_Position = u_View * u_Model * vert_Position;
    
    // Frag Normal
//    frag_Normal = u_View * u_Model * vert_Normal;

    // Frag UV
    frag_UV = vert_UV;
    
    
	//----------------------------------------------
	// Vertex Position
	//----------------------------------------------
    gl_Position = u_Projection * u_View * u_Model * vert_Position;
}

