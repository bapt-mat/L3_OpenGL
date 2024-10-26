#include "./meteorite.h"

void dessin_meteorite(float r, float posX, float posY, float posZ, int nbFacettes) {
    glTranslatef(posX, posY, posZ);
    float red=0,green=0,blue=0;
    switch(couleurMeteorites){
        case 0 : //jaune
            red=0.2*nbFacettes;
            green=0.2*nbFacettes;
            blue=0.1;
            break;
        case 1 : //rouge
            red=0.1*nbFacettes;
            green=0.1;
            blue=0.1;
            break;
        case 2 : //bleu
            red=0.1;
            green=0.2*nbFacettes;
            blue=0.2*nbFacettes;
            break;
        case 3 : //violet
            red=0.1*nbFacettes;
            green=0.1;
            blue=0.25*nbFacettes;
            break;
        case 4 : //orange
            red=0.2*nbFacettes;
            green=0.1*nbFacettes;
            blue=0.1;
            break;
    }
    dessinPlanete(r, nbFacettes, red, green, blue);
}

meteorite new_meteorite(float posX, float posY, float posZ, float vX, float vY, float vZ, float vit, float t, int nbFacettes){
    meteorite * m = (meteorite*) malloc(sizeof(meteorite));
    m->x = posX;
    m->y = posY;
    m->z = posZ;
    m->vecX = vX;
    m->vecY = vY;
    m->vecZ = vZ;
    m->vitesse = vit;
    m->taille = t;
    m->nbFacettes = nbFacettes;
    m->active = 1;
    return *m;
}


void generation_aleatoire_meteorites(){
    for (int i=0; i<NB_MAX_METEORITE; i++){
        float posX = RandomFloat(-50000,200000);
        float posY = RandomFloat(-50000,200000);
        float posZ = RandomFloat(-50000,200000);
        float directionX = RandomFloat(-1,1);
        float directionY = RandomFloat(-1,1);
        float directionZ = RandomFloat(-1,1);
        float vit = RandomFloat(100, 500)/* RandomFloat(10, 60) */;
        float t = RandomFloat(50, 3000);
        int nbFacettes = RandomInt(3,5);

        meteorite m = new_meteorite(posX, posY, posZ, directionX, directionY, directionZ, vit, t, nbFacettes);
        liste_meteorites[i] = m;
    }
}

