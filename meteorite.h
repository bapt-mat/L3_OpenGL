#include "./etoiles.c"

typedef struct{
    float x;
    float y;
    float z;
    float vecX;
    float vecY;
    float vecZ;
    float vitesse;
    float taille;
    int nbFacettes;
    int active;
} meteorite;

#define NB_MAX_METEORITE 900
int couleurMeteorites = 0;
meteorite liste_meteorites[NB_MAX_METEORITE];

void dessin_meteorite(float r, float posX, float posY, float posZ, int nbFacettes);
meteorite new_meteorite(float posX, float posY, float posZ, float vX, float vY, float vZ, float vit, float t, int nbFacettes);

