// OpenGL 3.3
#version 330


#version 330 core
struct Material {
    sampler2D diffuse; //Kd
    sampler2D specular; //Ks
    float     shininess; //a
};

struct Light {
    vec3 position; //light_position
    
    vec3 ambient;  //Ia
    vec3 diffuse;  //Id
    vec3 specular; //Is
    
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    // Attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    color = vec4(ambient + diffuse + specular, 1.0f);
}




//// Input from Vertex Shader
//in vec4 frag_UV;
//in vec4 frag_Position;
//in vec4 frag_Normal;
//in vec4 frag_Light_Direction;
//
//// Light Source
//uniform vec4 Ia = vec4(0.3f, 0.3f, 0.3f, 1.0f);
//uniform vec4 Id = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//uniform vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//
//
//// // Material - Brass
//// uniform vec4 Ka = vec4(0.329412, 0.223529, 0.027451, 1.0);
//// uniform vec4 Kd = vec4(0.780392, 0.568627, 0.113725, 1.0);
//// uniform vec4 Ks = vec4(0.992157, 0.941176, 0.807843, 1.0);
//// uniform float a = 27.89743616;
//
// // Material - Pearl
// uniform vec4 Ka = vec4(0.25,     0.20725,  0.20725,  1.0);
// uniform vec4 Kd = vec4(1.0,      0.829,    0.82900,  1.0);
// uniform vec4 Ks = vec4(0.296648, 0.296648, 0.296648, 1.0);
// uniform float a = 11.264;
//
//// // Material - Emerald
//// uniform vec4 Ka = vec4(0.0215,  0.1745,   0.0215,  1.0f);
//// uniform vec4 Kd = vec4(0.07568, 0.61424,  0.07568, 1.0f);
//// uniform vec4 Ks = vec4(0.633,   0.727811, 0.633,   1.0f);
//// uniform float a = 76.8;
//
//// Material - Turquoise
////uniform vec4 Ka = vec4(0.1,      0.18725, 0.1745,   1.0f);
////uniform vec4 Kd = vec4(0.396,    0.74151, 0.69102,  1.0f);
////uniform vec4 Ks = vec4(0.297254, 0.30729, 0.306678, 1.0f);
////uniform float a = 12.8;
//
//
//// Texture
////uniform sampler2D u_Texture_Map;
////uniform sampler2D u_Specular_Map;
//
//// For normal map
////uniform sampler2D u_Normal_Map;
//
//// Output from Fragment Shader
//out vec4 pixel_Colour;
//
//void main () {
//
//    // ---------- Calculate Vectors ----------
//    // Surface Normal (normalised)
//    vec4 n = normalize(frag_Normal);
//
//    // Direction to Light (normalised)
//    vec4 l = normalize(-frag_Light_Direction);
//
//    // Reflected Vector
//    vec4 r = reflect(-l, n);
//
//    // View Vector
//    vec4 v = normalize(-frag_Position);
//
//    // ---------- Calculate Terms ----------
//    // Ambient Term
//    vec4 Ta = Ka * Ia;
//
//    // Diffuse Term
//    vec4 Td = Kd * max(dot(l, n), 0.0) * Id;
//
//    // Specular Term
//    vec4 Ts = Ks * pow((max(dot(r, v), 0.0)), a) * Is;
//
//    //----------------------------------------------
//    // Fragment Colour
//    //----------------------------------------------
//    pixel_Colour = Ta + Td + Ts;
//}

