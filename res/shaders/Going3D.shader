#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normal;

uniform mat4 u_MVP;
uniform mat4 u_Model;

out vec4 v_Color;
out vec3 v_FragPos;
out vec3 v_Normal;

void main()
{
    v_Color = a_Color;
    gl_Position = u_MVP * vec4(a_Position, 1.0f);
    v_FragPos = vec3(u_Model * vec4(a_Position, 1.0f));
    v_Normal = vec3(u_Model * vec4(a_Normal, 1.0f));
};

#shader fragment
#version 330 core

in vec4 v_Color;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_lightColor;
uniform vec3 u_lightPos;

out vec4 color;

void main()
{
    // Ambient
    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * u_lightColor;
    
    // Diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * u_lightColor;
    
    color = vec4(vec4(ambient + diffuse, 1.0) * v_Color);
};