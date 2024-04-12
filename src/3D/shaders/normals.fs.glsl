#version 330 core

in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

void main() {
  // TODO pour le debug
  // fFragColor = vec4(normalize(vNormal_vs), 1.);
  // TODO renommer le fichier en Bee / abeille
  fFragColor = vec4(0.9, 0.8, 0., 1.); 
  fFragColor = fFragColor * mod(floor(2.5 * vPosition_vs.x), 2.0);
};