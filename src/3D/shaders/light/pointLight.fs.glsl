
in vec3 vPosition_vs; // dans l'espace View (vs)
in vec3 vNormal_vs; // dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(vec3 uLightPos_vs, vec3 vPosition_vs, vec3 uLightIntensity, vec3 uKd, vec3 vNormal_vs, vec3 uKs, float uShininess){
    vec3 w_i = normalize(uLightPos_vs - vPosition_vs);
    vec3 w_0 = normalize(-vPosition_vs); // pointe vers la caméra

    return (uLightIntensity / (distance(uLightPos_vs, vPosition_vs) * distance(uLightPos_vs, vPosition_vs))) 
    * (uKd * dot(normalize(uLightPos_vs), vNormal_vs)) 
    + uKs * (pow(dot((w_0 + w_i) / 2., vNormal_vs), uShininess));
}

void main(){
    fFragColor = blinnPhong(uLightPos_vs, vPosition_vs, uLightIntensity, uKd, vNormal_vs, uKs, uShininess);
};

//  variables uniformes vec3 uLightPos_vs 
//  => nous allons travailler dans le view space 
//  multiplier la direction de la lumière par la View Matrix avant de l'envoyer au shader.
//  et vec3 uLightIntensity =>  par rapport à l'intensité et la distance à la source de lumière.