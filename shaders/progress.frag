#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec3 backColor;
uniform vec3 timeColor;
uniform float timeLeft;

void main() {
  if (TexCoords.x > timeLeft) {
    color = vec4(backColor, 1.0);
  } else {
    color = vec4(timeColor, 1.0);
  }
}