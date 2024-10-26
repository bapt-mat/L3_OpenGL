#include "./planetes.c"

/****************************
|   Aleatoire etoile
*****************************/

typedef struct {
    float x;
    float y;
    float z;
} Etoile;

#define MAX_ETOILES 10000
#define rayon_etoile 50
Etoile etoiles[MAX_ETOILES];
int num_etoiles = 0;

/****************************
|   Structure de l'etoilegalaxy
*****************************/
typedef struct {
    float x;
    float y;
    float z;
    float taille;
    float rotation_angle;
    float c1;
    float c2;
    float c3;
} etoilegalaxy;

#define MAX_ETOILE_GALAXY 4000
etoilegalaxy etoilesgalaxy[MAX_ETOILE_GALAXY];
int num_etoilegalaxy = 0;

