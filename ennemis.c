#include "ennemis.h"

void dessin_ennemi(float r, float posX, float posY, float posZ) {
    glPushMatrix();
    glTranslated(posX, posY, posZ);
    dessinPlanete(r, 10, 0.5, 0.5, 0.5);
    glPopMatrix();
}

ennemi new_ennemi(float posX, float posY, float posZ, float vX, float vY, float vZ, float vit, float t){
    ennemi * m = (ennemi*) malloc(sizeof(ennemi));
    m->x = posX;
    m->y = posY;
    m->z = posZ;
    m->vecX = vX;
    m->vecY = vY;
    m->vecZ = vZ;
    m->vitesse = vit;
    m->taille = t;
    m->active = 1;
    return *m;
}

void generation_aleatoire_ennemi(){
    for (int i=0; i<NB_MAX_ENNEMIS; i++){
        float posX = RandomFloat(-50000,200000);
        float posY = RandomFloat(-50000,200000);
        float posZ = RandomFloat(-50000,200000);
        float directionX = RandomFloat(-1,1);
        float directionY = RandomFloat(-1,1);
        float directionZ = RandomFloat(-1,1);
        float vit = RandomFloat(50, 150);
        float t = RandomFloat(50, 1000);

        ennemi m = new_ennemi(posX, posY, posZ, directionX, directionY, directionZ, vit, t);
        liste_ennemis[i] = m;
    }
}

