#include "vaisseau.c"
typedef struct{
    float x;
    float y;
    float z;
    float vecX;
    float vecY;
    float vecZ;
    float vitesse;
    float taille;
    int active;
}ennemi;

#define NB_MAX_ENNEMIS 20
#define NB_ENNEMIS_A_ELIMINER 15
ennemi liste_ennemis[NB_MAX_ENNEMIS];

void dessin_ennemi(float r, float posX, float posY, float posZ);
ennemi new_ennemi(float posX, float posY, float posZ, float vX, float vY, float vZ, float vit, float t);

