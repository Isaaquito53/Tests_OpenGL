#shader vertex
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;

uniform mat4 u_MVP;

out vec4 v_color;

void main()
{
    v_color = a_color;
    gl_Position = u_MVP * a_position;
};

#shader fragment
#version 330 core

in vec4 v_color;
layout(location = 0) out vec4 color;

void main()
{
    color = v_color;
};