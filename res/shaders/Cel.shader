#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
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
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
};

#shader fragment
#version 330 core

in vec4 v_Color;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform float u_ambientStrenght;
uniform float u_diffuseStrenght;
uniform float u_specularStrenght;
uniform float u_shininess;

out vec4 color;

const float levels = 10.0f;

void main()
{
    // Ambient
    vec3 ambient = u_ambientStrenght * u_lightColor;
    
    // Diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    float level = floor(diff * levels);
    diff = level / levels;
    vec3 diffuse = u_diffuseStrenght * diff * u_lightColor;

    // Specular
    vec3 viewDir = normalize(u_viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_shininess); // convert 32 to scalar uni
    level = floor(spec * levels);
    spec = level / levels;
    vec3 specular = u_specularStrenght * spec * u_lightColor;
    
    color = vec4(vec4(ambient + diffuse + specular, 1.0) * v_Color);
};