#version 330

layout(location = 0) in vec3 aVertexPosition;
//layout(location = 1) in vec2 aVertexTexture;

//out vec2 vFragTexture;
out vec3 vFragPosition;

void main() {
  //vFragTexture = aVertexTexture;
  vFragPosition = aVertexPosition;
  gl_Position = vec4(vFragPosition, 1.);
}