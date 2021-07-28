#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec3 rectColor;

void main()
{
    color = vec4(rectColor, 1.0);
}