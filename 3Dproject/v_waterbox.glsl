#version 330 core
layout (location = 0) in vec3 vertex;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
    TexCoords = vertex;
    vec4 pos = P * V * M * vec4(vertex, 1.0);
    gl_Position = pos.xyww;
}  