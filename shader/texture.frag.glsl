// OpenGL 3.3
#version 330

// Input from Vertex Shader
in vec4 frag_UV;
in vec4 frag_Position;
in vec4 frag_Normal;
//in vec4 frag_Light_Direction;
in vec4 frag_Light_Position;

// Light Source
uniform vec4 Ia = vec4(0.3f, 0.3f, 0.3f, 1.0f);
uniform vec4 Id = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);

// Light Attenuation
float Kc = 1.0f; // constant;
float Kl = 0.09f; // linear;
float Kq = 0.032f; // quadratic;

// Material Coefficients
uniform float a = 5;

// Texture
uniform sampler2D u_Texture_Map;
uniform sampler2D u_Specular_Map;

// For normal map
//uniform sampler2D u_Normal_Map;

// Output from Fragment Shader
out vec4 pixel_Colour;

void main () {
    
    vec4 Ka = texture(u_Texture_Map, frag_UV.xy);
    vec4 Kd = Ka;
//    vec4 Ks = texture(u_Specular_Map, frag_UV.xy);
    vec4 Ks = vec4(0.297254, 0.30729, 0.306678, 1.0f);

    // ---------- Calculate Vectors ----------
    // Surface Normal (normalised)
    vec4 n = normalize(frag_Normal);
    
    // Direction to Light (normalised)
//    vec4 l = normalize(-frag_Light_Direction);
    vec4 l = normalize(frag_Light_Position - frag_Position);
//    vec4 l = normalize(frag_Position - frag_Light_Position);
    
    // Reflected Vector
    vec4 r = reflect(-l, n);
    
    // View Vector
    vec4 v = normalize(-frag_Position);
    
    // ---------- Calculate Terms ----------
    // Ambient Term
    vec4 Ta = Ka * Ia;
    
    // Diffuse Term
    vec4 Td = Kd * max(dot(l, n), 0.0) * Id;
    
    // Specular Term
    vec4 Ts = Ks * pow((max(dot(r, v), 0.0)), a) * Is;
    
    // Attenuation
    float d    = length(frag_Light_Position - frag_Position);
    float Fatt = 1.0f / (Kc + Kl * d + Kq * (d * d));
    
    Ta *= Fatt;
    Td *= Fatt;
    Ts *= Fatt;
    
    // Great, the Point light calculation seems right. How to deal with sun?
    
	//----------------------------------------------
	// Fragment Colour
	//----------------------------------------------
//    pixel_Colour = texture(u_texture_Map, frag_UV.xy);
    pixel_Colour = Ta + Td + Ts;
//    pixel_Colour = Ta + Td;
}
