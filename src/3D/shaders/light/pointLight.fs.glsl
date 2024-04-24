
in vec3 vPosition_vs; // dans l'espace View (vs)
in vec3 vNormal_vs; // dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(vPosition_vs, uLightPos_vs){
    w_i = normalize(uLightPos_vs - vPosition_vs);
    w_0 = normalize(-vPosition_vs); // pointe vers la caméra

    return (uLightIntensity / (distance(uLightPos_vs) * distance(uLightPos_vs))) 
    * (uKd * dot(normalize(uLightPos_vs))) 
    + uKs * (pow(dot((w_0 + w_i) / 2.), uShininess));
}

void main(){
    fFragColor = blinnPhong();
}

//  variables uniformes vec3 uLightPos_vs 
//  => nous allons travailler dans le view space 
//  multiplier la direction de la lumière par la View Matrix avant de l'envoyer au shader.
//  et vec3 uLightIntensity =>  par rapport à l'intensité et la distance à la source de lumière.