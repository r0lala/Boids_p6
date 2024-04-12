#version 330 core

in vec3 vPosition_vs; // dans l'espace View (vs)
// in vec3 vNormal_vs; // dans l'espace View (vs)
// in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

void main() {
  fFragColor = vec4(0.9, 0.8, 0., 1.) * mod(floor(2.5 * vPosition_vs.x), 2.0);
};