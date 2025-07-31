#shader vertex
#version 330 core

// Vertex and Fragment shader for instanced rendering with phong light, velocity applied and support for light maps

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normal;
layout(location = 4) in mat4 a_model;

uniform mat4 u_projection;
uniform mat4 u_view;

out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec3 v_Normal;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_projection * u_view * a_model * vec4(a_Position, 1.0f);
    v_FragPos = vec3(a_model * vec4(a_Position, 1.0f));
    v_Normal = mat3(transpose(inverse(a_model))) * a_Normal;
};

#shader fragment
#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 v_TexCoord;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_viewPos;
uniform Material material;
uniform Light light;

out vec4 color;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * texture(material.diffuse, v_TexCoord).rgb;
    
    // Diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(light.position - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, v_TexCoord).rgb;

    // Specular
    vec3 viewDir = normalize(u_viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, v_TexCoord).rgb;
    
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
};