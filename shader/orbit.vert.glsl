// OpenGL 3.3
#version 330

// Input to Vertex Shader
in vec4 vert_Position;
in vec4 vert_Colour;

// Transform Matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// Output to Fragment Shader
out vec4 frag_Colour;

void main() {
	// Frag Colour
	frag_Colour = vert_Colour;

	//----------------------------------------------
	// Vertex Position
	//----------------------------------------------
	gl_Position = u_Projection * u_View * u_Model * vert_Position;
}