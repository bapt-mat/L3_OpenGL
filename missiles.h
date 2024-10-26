#include "./ennemis.c"

typedef struct{
    float x;
    float y; 
    float z;
    float vecX;
    float vecY;
    float vecZ;
    float vitesse;
    float taille;
}missile;

#define NB_MAX_MISSILES 30
missile liste_missiles[NB_MAX_MISSILES];

void dessin_missile(float posX, float posY, float posZ, float taille);
void new_missile(float posX, float posY, float posZ, float vX, float vY, float vZ, float vit, float taille);
int nouveauMissile();
void generationMissile();

int numMissile = -1;