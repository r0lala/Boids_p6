#version 330 core

// in vec3 vPosition_vs; // dans l'espace View (vs)
// in vec3 vNormal_vs; // dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

uniform sampler2D uTexture;

void main() {
  fFragColor = texture(uTexture, vTexCoords);
};