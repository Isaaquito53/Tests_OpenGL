#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_Color;
layout(location = 4) in mat4 a_model;

out vec4 v_Color;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
    gl_Position = u_projection * u_view * a_model * vec4(a_position, 1.0);
    v_Color = a_Color;
};

#shader fragment
#version 330 core

out vec4 color;

in vec4 v_Color;

void main()
{
    color = v_Color;
};