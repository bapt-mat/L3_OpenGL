#include "./missiles.h"

void dessin_missile(float posX, float posY, float posZ, float taille){
    glTranslatef(posX, posY, posZ);
    glColor3f(0.1,0.7,0.8);
    //glutSolidSphere(taille, 5, 5);
    glutSolidTorus(taille/2, taille, 10, 10);
}

void new_missile(float posX, float posY, float posZ, float vX, float vY, float vZ, float vit, float taille){
    missile * miss = (missile*)malloc(sizeof(missile));
    miss->x = posX;
    miss->y = posY;
    miss->z = posZ;
    miss->vecX = vX;
    miss->vecY = vY;
    miss->vecZ = vZ;
    miss->vitesse = vit;
    miss->taille = taille;
    liste_missiles[nouveauMissile()] = *miss;
}

int nouveauMissile(){
    numMissile++;
    if (numMissile == NB_MAX_MISSILES){
        numMissile = 0;
    }
    return numMissile;
}

void generationMissile(){
    for (int i=0; i<NB_MAX_MISSILES; i++){
        new_missile(-100000, 0, 0, 0, 0, 0, 0, 0);
    }
}

void reinitMissile(int numMiss){
    liste_missiles[numMiss].x = -100000;
    liste_missiles[numMiss].y = 0;
    liste_missiles[numMiss].z = 0;
    liste_missiles[numMiss].vecX = 0;
    liste_missiles[numMiss].vecY = 0;
    liste_missiles[numMiss].vecZ = 0;
    liste_missiles[numMiss].taille = 0;
}
