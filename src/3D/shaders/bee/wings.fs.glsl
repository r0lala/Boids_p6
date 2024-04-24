#version 330 core

in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
// in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
// in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

void main() {
  fFragColor = vec4(1., 1., 1., 1.); 

  float alpha = 1.; // intensité du centre
  float beta = 0.5;  // vitesse de dégradé
  float a = alpha * exp(-beta * distance(vec2(0., 0.), vPosition_vs.xy) * distance(vec2(0., 0.), vPosition_vs.xy));

  fFragColor = vec4(0., 0.8, 1., 0.3) * a;
};