// OpenGL 3.3
#version 330

// Input from Vertex Shader
in vec4 frag_UV;
in vec4 frag_Position;

// Texture
uniform sampler2D u_Texture_Map;

// Output from Fragment Shader
out vec4 pixel_Colour;

void main () {
    
	//----------------------------------------------
	// Fragment Colour
	//----------------------------------------------
    pixel_Colour = texture(u_Texture_Map, frag_UV.xy);
}
