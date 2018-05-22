// OpenGL 3.3
#version 330

// Input to Vertex Shader
in vec4 vert_Position;
in vec4 vert_Normal;
in vec4 vert_UV;
// For normal map
//in vec4 vert_Tangent;

// Transform Matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Light Source - Directional
//uniform vec4 u_Light_Direction = vec4(0.0f, 0.0f, -0.2f, 0.0f);

// light Source - Point Light
uniform vec4 u_Light_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);

// Output to Fragment Shader
out vec4 frag_Position;
out vec4 frag_UV;
out vec4 frag_Light_Position;
//out vec4 frag_Light_Direction;

out vec4 frag_Normal;

//For normal map
//out vec3 frag_Tangent;
//out vec3 frag_Bitangent;

void main() {

    //----------------------------------------------
    // Output to Fragment Shader
    //----------------------------------------------
    
    // Frag Position
    frag_Position = u_View * u_Model * vert_Position;
    
    // Frag Normal
    frag_Normal = u_View * u_Model * vert_Normal;

    // Frag UV
    frag_UV = vert_UV;
    
    // Frag Light Position ??
//    frag_Light_Direction = u_View * u_Light_Direction;
    frag_Light_Position = u_View * u_Light_Position;
    
    
	//----------------------------------------------
	// Vertex Position
	//----------------------------------------------
    gl_Position = u_Projection * u_View * u_Model * vert_Position;
}

