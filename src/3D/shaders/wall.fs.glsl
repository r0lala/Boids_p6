#version 330
in vec2 vFragPosition;

out vec3 fFragTexture;

uniform sampler2D uTexture;

void main()
{
    fFragTexture =  texture(uTexture, vFragPosition).xyz;
}