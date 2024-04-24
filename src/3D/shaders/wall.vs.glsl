#version 330
layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec3 vFragTexture;
out vec2 vFragPosition;

void main() {
  vFragTexture = vec3(aVertexTexture,1.);
  vFragPosition = aVertexPosition;
  gl_Position = vec4(vFragPosition, 0., 1.);
}