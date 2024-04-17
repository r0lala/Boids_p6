#version 330
in vec3 vFragPosition;

out vec3 fFragTexture;

uniform sampler2D uTexture;

void main()
{
    fFragTexture =  texture(uTexture, vFragPosition.xy).xyz;
}