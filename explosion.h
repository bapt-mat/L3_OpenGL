#include"./meteorite.c"
#define MAX_PARTICULES 150
#define MAX_EXPLOSIONS 20
#define VITESSE 100
#define EXPLOSION_ROUGE 0
#define EXPLOSION_BLEU 1

typedef struct {
    float position[3];
    float couleur[3];
    float vitesse[3];
    float rayon;
    int active;
    int compteurTemps;
} Particule;

Particule particules[MAX_EXPLOSIONS][MAX_PARTICULES];

int numExplosion = -1;
