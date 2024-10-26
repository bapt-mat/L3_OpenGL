#include "./explosion.h"

void initParticle(int i, float posX, float posY, float posZ, int numExplosion, int couleur) {
    particules[numExplosion][i].active = 1;
    particules[numExplosion][i].position[0] = posX;
    particules[numExplosion][i].position[1] = posY;
    particules[numExplosion][i].position[2] = posZ;

    if (couleur == 0){ // explosion rouge
        particules[numExplosion][i].couleur[0] = RandomFloat(0.8,1);
        particules[numExplosion][i].couleur[1] = RandomFloat(0,0.2);
        particules[numExplosion][i].couleur[2] = RandomFloat(0,0.2);
    } 
    else if (couleur == 1){ //explosion bleue
        particules[numExplosion][i].couleur[0] = RandomFloat(0,0.2);
        particules[numExplosion][i].couleur[1] = RandomFloat(0.6,0.8);
        particules[numExplosion][i].couleur[2] = RandomFloat(0.8,1);
    }
    

    particules[numExplosion][i].rayon = RandomFloat(100,200);

    float theta = 2 * M_PI * ((float) rand() / RAND_MAX);
    float phi = acos(2 * ((float) rand() / RAND_MAX) - 1);

    particules[numExplosion][i].vitesse[0] = sin(phi) * cos(theta) * VITESSE;
    particules[numExplosion][i].vitesse[1] = sin(phi) * sin(theta) * VITESSE;
    particules[numExplosion][i].vitesse[2] = cos(phi) * VITESSE;
}

void generationExplosion(float posX, float posY, float posZ, int numExplosion, int couleur) {
    for (int i = 0; i < MAX_PARTICULES; i++) {
        initParticle(i, posX, posY, posZ, numExplosion, couleur);
    }
}

void majParticules() {
    for (int j = 0; j < MAX_EXPLOSIONS; j++){
        for (int i = 0; i < MAX_PARTICULES; i++) {
            if (particules[j][i].active) {
                particules[j][i].position[0] += particules[j][i].vitesse[0];
                particules[j][i].position[1] += particules[j][i].vitesse[1];
                particules[j][i].position[2] += particules[j][i].vitesse[2];
                particules[j][i].compteurTemps ++;

                if (particules[j][i].compteurTemps == 100) {
                    particules[j][i].active = 0;
                }
            }
        }
    }
}

void affichageParticules() {
    for (int j = 0; j < MAX_EXPLOSIONS; j++){
        for (int i = 0; i < MAX_PARTICULES; i++) {
            if (particules[j][i].active) {
                glPushMatrix();
                glColor3fv(particules[j][i].couleur);
                glRotatef(i, particules[j][i].position[0], particules[j][i].position[1], particules[j][i].position[2]);
                glTranslatef(particules[j][i].position[0], particules[j][i].position[1], particules[j][i].position[2]);
                int nbFacettes = RandomInt(3,4);
                glutSolidSphere(particules[j][i].rayon, nbFacettes, nbFacettes);
                glPopMatrix();
            }
        }
    }
}

int nouvelleExplosion(){
    numExplosion++;
    if (numExplosion == MAX_EXPLOSIONS){
        numExplosion = 0;
    }
    return numExplosion;
}


