#include "./planetes.h"

/*----------------------------------------*
|
|  < Genération aléatoire de planets >
|
*-----------------------------------------*/
/*----------------------------------------*
| INT : position planet Float : Couleur 
*-----------------------------------------*/
int RandomInt(int min, int max) { 
    return min + rand() % (max - min + 1); 
}

float RandomFloat(float min, float max) {
    float d = max - min;
    float random = ((float)rand() / RAND_MAX) * d + min; 
    return random;
}

/*----------------------------------------*
|Fonction permetant un meilleur aléatoire
*-----------------------------------------*/

float distance_A_B(float x1,float y1,float z1,float x2,float y2,float z2){
   return sqrt(pow((x2-x1),2)+pow(y2-y1,2)+pow(z2-z1,2));
}

float distance_rayon_necessaire(float r,float r1){
    float res = r+r1;
    return res;
}

int est_intersection(int c,int rr){
    if (c<=rr){
        return 1;
    }
    return 0;
}


int bon_aleatoire(int x,int y,int z,int r){
    float distance_centre;
    float distance_rayon;
    for (int i = 0; i < num_planets; i++) {
       distance_centre=distance_A_B(x,y,z,planets[i].x,planets[i].y,planets[i].z);
       distance_rayon=distance_rayon_necessaire(r,planets[i].t);
        if (distance_centre < distance_rayon){
            return 1;
        }
    } 
    return 0;
}

/*----------------------------------------*
|Fonction de génération aléatoire de planet
*-----------------------------------------*/
void Generation_aléatoire_planet() {
    int x,y,z;
    float c1,c2,c3;
    int h,v,t;
    //On prepare nos couleurs, la position et le type de spheres
    c1=RandomFloat(0,1.0); x=RandomInt(50,150000); h=RandomInt(30,50);
    c2=RandomFloat(0,1.0); y=RandomInt(50,150000); v=RandomInt(30,50);
    c3=RandomFloat(0,1.0); z=RandomInt(50,150000); t=RandomInt(1000,5000);

    while(bon_aleatoire(x,y,z,t)==1){ 
        x=RandomInt(50, 150000); y=RandomInt(50, 150000); z=RandomInt(50, 150000);
    }
    if (num_planets < MAX_PLANETS) {
        Planet new_planet;
        new_planet.x = x; new_planet.c1 = c1; new_planet.h = h;
        new_planet.y = y; new_planet.c2 = c2; new_planet.v = v;
        new_planet.z = z; new_planet.c3 = c3; new_planet.t = t;     
        planets[num_planets++] = new_planet;
    }
}

/*----------------------------------------*
| Fonction de dessin d'une planète 
*-----------------------------------------*/
void dessinPlanete(float taille, int nombreFacettes, float col1, float col2, float col3) {
    float delta = (M_PI) / nombreFacettes;
    float deltaPhi = (2 * M_PI) / nombreFacettes;

    for (float teta = 0; teta < M_PI; teta += delta) {
        glBegin(GL_QUADS);
        for (float phi = 0; phi < M_PI; phi += deltaPhi) {
            // Points sur la sphère
            float x1 = taille * sin(teta) * cos(phi);
            float y1 = taille * sin(teta) * sin(phi);
            float z1 = taille * cos(teta);

            float x2 = taille * sin(teta + delta) * cos(phi);
            float y2 = taille * sin(teta + delta) * sin(phi);
            float z2 = taille * cos(teta + delta);

            float x3 = taille * sin(teta) * cos(phi + deltaPhi);
            float y3 = taille * sin(teta) * sin(phi + deltaPhi);
            float z3 = taille * cos(teta);

            float x4 = taille * sin(teta + delta) * cos(phi + deltaPhi);
            float y4 = taille * sin(teta + delta) * sin(phi + deltaPhi);
            float z4 = taille * cos(teta + delta);

            // Calcul de couleurs
            float r = col1 * (1 - (phi / (2 * M_PI))) ;  // Rouge
            float g = col2 * (1 - (teta / M_PI));  // Vert
            float b = col3 * (1 - (teta / M_PI));  // Bleu 

            // Assignation des couleurs
            glColor3f(r, g, b);

            // Dessiner les faces
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glVertex3f(x4, y4, z4);
            glVertex3f(x3, y3, z3);

            // Points symétriques (en inversant uniquement la coordonnée y)
            float y1_sym = -y1;
            float y2_sym = -y2;
            float y3_sym = -y3;
            float y4_sym = -y4;

            // Dessiner les faces symétriques
            glVertex3f(x1, y1_sym, z1);
            glVertex3f(x2, y2_sym, z2);
            glVertex3f(x4, y4_sym, z4);
            glVertex3f(x3, y3_sym, z3);
        }
        glEnd();
    }
}