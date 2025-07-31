#shader vertex
#version 330 core

// Vertex and Fragment shader for instanced rendering with phong light and velocity applied

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 3) in vec3 a_Normal;
layout(location = 4) in mat4 a_model;
layout(location = 8) in vec3 a_dir;

uniform mat4 u_projection;
uniform mat4 u_view;

uniform float u_delta;
uniform int u_nCubes;
uniform mat4 u_models[100];
uniform vec3 u_dirs[100];

out vec4 v_Color;
out vec3 v_FragPos;
out vec3 v_Normal;

bool collisions(in vec3 p_pos)
{
    vec4 p_cube = a_model * vec4(p_pos, 1.0f);
    for (int i = 0; i < u_nCubes; i++)
    {
        if (i != gl_InstanceID)
        {
            vec4 p_other = u_models[i] * vec4(a_Position + (u_delta * u_dirs[i]), 1.0f);
        
            if (distance(vec3(p_cube), vec3(p_other)) <= 1)
            {
                return true;
            }
        }
        
    }
    return false;
};

void main()
{
    v_Color = a_Color;
    vec3 pos = a_Position + (u_delta * a_dir);
    bool coll = collisions(pos);
    if (coll == true)
    {
        v_Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
    gl_Position = u_projection * u_view * a_model * vec4(pos, 1.0f);
    v_FragPos = vec3(a_model * vec4(pos, 1.0f));
    v_Normal = mat3(transpose(inverse(a_model))) * a_Normal;
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

void main()
{
    // Ambient
    vec3 ambient = u_ambientStrenght * u_lightColor;
    
    // Diffuse
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = u_diffuseStrenght * diff * u_lightColor;

    // Specular
    vec3 viewDir = normalize(u_viewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_shininess); // convert 32 to scalar uni
    vec3 specular = u_specularStrenght * spec * u_lightColor;
    
    color = vec4(vec4(ambient + diffuse + specular, 1.0) * v_Color);
};