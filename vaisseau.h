#include "./explosion.c"

/*Position initiale du vaisseau*/
float posX = 0;
float posY = 0;
float posZ = 0;

/*Vecteur d'orientation du vaisseau*/
GLfloat vecVaisseauX[3] = {1,0,0};
GLfloat vecVaisseauY[3] = {0,1,0};
GLfloat vecVaisseauZ[3] = {0,0,1};

/*Caractéristiques du vaisseau*/
float vitesse = 5;
float vitesseDeCroisiere = 1;
float angle = 0.01;