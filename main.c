#include "./missiles.c"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define LONGUEUR_FENETRE 1600
#define LARGEUR_FENETRE 800

struct timespec delay;

/* Spécifications (champ de vision) */
float G = -0.8; // gauche
float D = 0.8; // droite
float B = -0.4;  // bas
float H = 0.4; // haut
float N_DIST = 0.8; // near 
float F = 250000; // far

/*Déplacement avec souris*/
int xSave = 800;
int ySave = 400;
int curseur = 0;

float bouleEnglobanteVaisseau = 2.8;
float distanceCameraVaisseau = 10;

int boolMenu = 1;
int boolStop = 0;

char seconde_texte[1000000];

int seconde = 10800;  //10800 pour 3 minutes
int vrai_temps = 0;

int point_jeu = NB_ENNEMIS_A_ELIMINER;
char point_jeu_texte[1000000];

int boolVictoire;

Mix_Chunk *son_missile = NULL; 
Mix_Chunk *son_jeu = NULL; 
Mix_Chunk *son_explosion = NULL; 

/*----------------------------------------*
| Fonction d'animation 
*-----------------------------------------*/
void animation(){
    //Gestion de l'arret du vaisseau
    if (vitesseDeCroisiere <= 0){
        boolStop = 0;
    }
    if (boolStop == 1){
        vitesseDeCroisiere -= 20;
        distanceCameraVaisseau -= .4;
        G += .04;
        D -= .04;
        B += .02;
        H -= .02;
    }
    glutPostRedisplay();
}

/*----------------------------------------*
| Fonction affichage texte 
*-----------------------------------------*/
void texte2d(float x, float y, const char *texte, float c1,float c2,float c3, void * font) {
    glColor3f(c1, c2, c3); //couleurs
    glRasterPos2f(x, y);
    while (*texte) {
        glutBitmapCharacter(font, *texte);
        texte++;
    }
}

void texte3d(int x,int y, int z, char *texte,float c1,float c2,float c3){
    glColor3f(c1, c2, c3); //couleurs
    glRasterPos3f(x,y,z);  //position
    while (*texte) { //texte
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *texte);
        texte++;
    }
    glFlush();
}

/*----------------------------------------*
| Réinitialisation de la caméra 
*-----------------------------------------*/
void reinitCamera(){
    distanceCameraVaisseau = 10;
    G = -0.8;
    D = 0.8; 
    B = -0.4;
    H = 0.4;
}

/*----------------------------------------*
| Permet de maintenir les vecteurs normés 
*-----------------------------------------*/
void normalisationVecteurs(){
    // Normalisation du vecteur X
    float longueur = sqrt(vecVaisseauX[0]*vecVaisseauX[0] + vecVaisseauX[1]*vecVaisseauX[1] + vecVaisseauX[2]*vecVaisseauX[2]);
    vecVaisseauX[0] /= longueur;
    vecVaisseauX[1] /= longueur;
    vecVaisseauX[2] /= longueur;
    // Normalisation du vecteur Y
    longueur = sqrt(vecVaisseauY[0]*vecVaisseauY[0] + vecVaisseauY[1]*vecVaisseauY[1] + vecVaisseauY[2]*vecVaisseauY[2]);
    vecVaisseauY[0] /= longueur;
    vecVaisseauY[1] /= longueur;
    vecVaisseauY[2] /= longueur;
    // Normalisation du vecteur Z
    longueur = sqrt(vecVaisseauZ[0]*vecVaisseauZ[0] + vecVaisseauZ[1]*vecVaisseauZ[1] + vecVaisseauZ[2]*vecVaisseauZ[2]);
    vecVaisseauZ[0] /= longueur;
    vecVaisseauZ[1] /= longueur;
    vecVaisseauZ[2] /= longueur;
}

/*----------------------------------------*
| Permet de maintenir les vecteurs orthogonaux 
*-----------------------------------------*/
void orthoVecteurs(GLfloat vec1[3], GLfloat vec2[3], GLfloat vec3[3]){
    // Projection orthogonale de vec2 sur vec1
    float prod_scalaire = vec2[0]*vec1[0] + vec2[1]*vec1[1] + vec2[2]*vec1[2];
    vec2[0] -= prod_scalaire * vec1[0];
    vec2[1] -= prod_scalaire * vec1[1];
    vec2[2] -= prod_scalaire * vec1[2];

    // Projection orthogonale de vec1 sur vec3
    prod_scalaire = vec3[0]*vec1[0] + vec3[1]*vec1[1] + vec3[2]*vec1[2];
    vec3[0] -= prod_scalaire * vec1[0];
    vec3[1] -= prod_scalaire * vec1[1];
    vec3[2] -= prod_scalaire * vec1[2];

    // Projection orthogonale de vec2 sur vec3
    prod_scalaire = vec3[0]*vec2[0] + vec3[1]*vec2[1] + vec3[2]*vec2[2];
    vec3[0] -= prod_scalaire * vec2[0];
    vec3[1] -= prod_scalaire * vec2[1];
    vec3[2] -= prod_scalaire * vec2[2];
}

/*----------------------------------------*
| Gestion du clavier  
*-----------------------------------------*/
void GestionClavier(unsigned char touche, int x, int y){
    switch (touche){
        case 27 : //Sortir du jeu
            exit(0);
            break;

        case 13 : //Touche entrée -> on/off plein écran
            glutFullScreen();
            break;

        case 32 : //Touche espace -> s'arreter
            if (vitesseDeCroisiere != 0) {
                vitesseDeCroisiere = 0;
                boolStop = 1; 
                reinitCamera();
            }
            else {
                vitesseDeCroisiere = 1;
            }
            break;

        case 'm' : //Afficher/enlever le menu
            if (boolMenu) boolMenu = 0;
            else boolMenu = 1;
            break;
        
        case 'c' : // Gestion curseur on/off
            if (curseur) {
                curseur = 0;
                glutSetCursor(GLUT_CURSOR_NONE);
            }
            else {
                curseur = 1;
                glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            }
            break;

        case 'z' : //Accélérer
            if (vitesseDeCroisiere < 300){
                vitesseDeCroisiere += 5;
                distanceCameraVaisseau += .1;
                G -= .01;
                D += .01;
                B -= .005;
                H += .005;
            }
            break;
        
        case 's' : //Ralentir
            if (vitesseDeCroisiere > 5){
                vitesseDeCroisiere -= 5;
                distanceCameraVaisseau -= .1;
                G += .01;
                D -= .01;
                B += .005;
                H -= .005;
            }
            break;

        case 'h' : //Hyperespace
            vitesseDeCroisiere += 20;
            distanceCameraVaisseau += .001;
            G -= .1;
            D += .1;
            B -= .05;
            H += .05;
            break;
        
        case 'r' : //Reinit camera
            reinitCamera();
            break;

        case 'x' : //Monter
            posX += vitesse*vecVaisseauZ[0]*10;
            posY += vitesse*vecVaisseauZ[1]*10;
            posZ += vitesse*vecVaisseauZ[2]*10;
            break;

        case 'w' : //Descendre
            posX -= vitesse*vecVaisseauZ[0]*10;
            posY -= vitesse*vecVaisseauZ[1]*10;
            posZ -= vitesse*vecVaisseauZ[2]*10;
            break;
        
        case 'f' : // Missile
            new_missile(posX, posY, posZ-500, vecVaisseauY[0], vecVaisseauY[1], vecVaisseauY[2], vitesseDeCroisiere+5000, 200);    
            Mix_PlayChannel(-1, son_missile, 0);
            break;

        case 't' : // Couleur meteorites
            couleurMeteorites++;
            if (couleurMeteorites == 5){
                couleurMeteorites = 0;
            }
            break;
        /*----------------------------------------*
        | Rotation autour de l'axe Y, donc recalcule des vecteurs X et Z
        | Sens anti-horaire
        *-----------------------------------------*/
        case 'a' : 
            GLfloat newZX = vecVaisseauZ[0] * cos(angle) + vecVaisseauZ[2] * -sin(angle);
            GLfloat newZZ = vecVaisseauZ[0] * sin(angle) + vecVaisseauZ[2] * cos(angle);
            vecVaisseauZ[0] = newZX;
            vecVaisseauZ[2] = newZZ;
            normalisationVecteurs();
            orthoVecteurs(vecVaisseauY,vecVaisseauZ,vecVaisseauX);
            break;
        
        case 'e' : 
            newZX = vecVaisseauZ[0] * cos(-angle) + vecVaisseauZ[2] * -sin(-angle);
            newZZ = vecVaisseauZ[0] * sin(-angle) + vecVaisseauZ[2] * cos(-angle);
            vecVaisseauZ[0] = newZX;
            vecVaisseauZ[2] = newZZ;
            normalisationVecteurs();
            orthoVecteurs(vecVaisseauY,vecVaisseauZ,vecVaisseauX);
            break;
    }

    glutPostRedisplay();
}

void GestionSouris(int x, int y){
    //Désactiver si le curseur est actif
    if (curseur){
        return;
    }
    angle = 0.003;
    
    if (xSave<x){
        // Rotation du vecteur X
            GLfloat newXX = vecVaisseauX[0] * cos(-angle) + vecVaisseauX[1] * -sin(-angle);
            GLfloat newXY = vecVaisseauX[0] * sin(-angle) + vecVaisseauX[1] * cos(-angle);
            vecVaisseauX[0] = newXX;
            vecVaisseauX[1] = newXY;

            normalisationVecteurs();
            orthoVecteurs(vecVaisseauX, vecVaisseauY, vecVaisseauZ);
        
    }
    if (xSave>x){
        // Rotation du vecteur X
            GLfloat newXX = vecVaisseauX[0] * cos(angle) + vecVaisseauX[1] * -sin(angle);
            GLfloat newXY = vecVaisseauX[0] * sin(angle) + vecVaisseauX[1] * cos(angle);
            vecVaisseauX[0] = newXX;
            vecVaisseauX[1] = newXY;

            normalisationVecteurs();
            orthoVecteurs(vecVaisseauX, vecVaisseauY, vecVaisseauZ);
        
    }
    if (ySave<y){
        // Rotation du vecteur Y
        GLfloat newYY;
        GLfloat newYZ;   
        if (fabs(vecVaisseauY[0]) > 0.5){
            angle*=fabs(vecVaisseauY[0])*2;
        }
        if (vecVaisseauY[1] > 0){
            newYY = vecVaisseauY[1] * cos(-angle) + vecVaisseauY[2] * -sin(-angle);
            newYZ = vecVaisseauY[1] * sin(-angle) + vecVaisseauY[2] * cos(-angle);
        }
        else{
            newYY = vecVaisseauY[1] * cos(angle) + vecVaisseauY[2] * -sin(angle);
            newYZ = vecVaisseauY[1] * sin(angle) + vecVaisseauY[2] * cos(angle);
        }
        vecVaisseauY[1] = newYY;
        vecVaisseauY[2] = newYZ;
        normalisationVecteurs();
        orthoVecteurs(vecVaisseauX, vecVaisseauY, vecVaisseauZ);
        
    }
    if (ySave>y){
        // Rotation du vecteur Y
        GLfloat newYY;
        GLfloat newYZ;   
        if (fabs(vecVaisseauY[0]) > 0.5){
            angle*=fabs(vecVaisseauY[0])*2;
        }
        if (vecVaisseauY[1] > 0){
            newYY = vecVaisseauY[1] * cos(angle) + vecVaisseauY[2] * -sin(angle);
            newYZ = vecVaisseauY[1] * sin(angle) + vecVaisseauY[2] * cos(angle);
        }
        else{
            newYY = vecVaisseauY[1] * cos(-angle) + vecVaisseauY[2] * -sin(-angle);
            newYZ = vecVaisseauY[1] * sin(-angle) + vecVaisseauY[2] * cos(-angle);
        }
        vecVaisseauY[1] = newYY;
        vecVaisseauY[2] = newYZ;
        normalisationVecteurs();
        orthoVecteurs(vecVaisseauX, vecVaisseauY, vecVaisseauZ);
        
    }
    xSave = x;
    ySave = y;

    //On remet la souris au centre si elle sort de la fenêtre
    if (x>LONGUEUR_FENETRE-LONGUEUR_FENETRE/6 || x<LONGUEUR_FENETRE/6 || y>LARGEUR_FENETRE-LARGEUR_FENETRE/6 || y<LARGEUR_FENETRE/6){
        glutWarpPointer(LONGUEUR_FENETRE/2, LARGEUR_FENETRE/2);
    }
}

void GestionSourisDeux(int bouton, int etat, int x, int y){
    if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN) {
        new_missile(posX, posY, posZ-500, vecVaisseauY[0], vecVaisseauY[1], vecVaisseauY[2], vitesseDeCroisiere+5000, 200);    
        Mix_PlayChannel(-1, son_missile, 0);
    }
    if (bouton == 3){
        distanceCameraVaisseau--;
        G += .01;
        D -= .01;
        B += .005;
        H -= .005;
    }
    if (bouton == 4){
        distanceCameraVaisseau++;
        G -= .01;
        D += .01;
        B -= .005;
        H += .005;
    }
}

void reinitPosVaisseau(){
    reinitCamera();
    vitesseDeCroisiere = 1;
    vecVaisseauX[0] = 1;
    vecVaisseauX[1] = 0;
    vecVaisseauX[2] = 0;
    vecVaisseauY[0] = 0;
    vecVaisseauY[1] = 1;
    vecVaisseauY[2] = 0;
    vecVaisseauZ[0] = 0;
    vecVaisseauZ[1] = 0;
    vecVaisseauZ[2] = 1;
    posX=0;
    posY=0;
    posZ=0;
    point_jeu = NB_ENNEMIS_A_ELIMINER;
    generationExplosion(posX, posY, posZ, nouvelleExplosion(), EXPLOSION_ROUGE);
    for (int i=0; i<NB_MAX_ENNEMIS; i++){
        liste_ennemis[i].active = 1;
    }
}

/*----------------------------------------*
| Fonction d'ajustement de la caméra 
*-----------------------------------------*/
void ajuster_camera() {
    gluLookAt(posX-distanceCameraVaisseau*vecVaisseauY[0], posY-distanceCameraVaisseau*vecVaisseauY[1], posZ-distanceCameraVaisseau*vecVaisseauY[2]+0.15*distanceCameraVaisseau, posX + vecVaisseauY[0], posY + vecVaisseauY[1], posZ + vecVaisseauY[2], vecVaisseauZ[0], vecVaisseauZ[1], 1);
}

/*----------------------------------------*
| Ecran de fin 
*-----------------------------------------*/
void affichageFin(){
    /*----------------------------------------*
    | Remplit la fenêtre avec la couleur par défaut (noir) 
    *-----------------------------------------*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*----------------------------------------*
    | Affichage des contrôles et du score
    *-----------------------------------------*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, LONGUEUR_FENETRE, LARGEUR_FENETRE, 0); 
    if (boolVictoire){
        texte2d(LONGUEUR_FENETRE / 2-50, LARGEUR_FENETRE/2-50, "VICTOIRE !", 1, 1, 1,GLUT_BITMAP_TIMES_ROMAN_24);
        texte2d(LONGUEUR_FENETRE / 2-50, LARGEUR_FENETRE/2, "Echap pour quitter :-)", 1, 1, 1,GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else{
        texte2d(LONGUEUR_FENETRE / 2-50, LARGEUR_FENETRE/2-50, "DEFAITE !", 1, 1, 1,GLUT_BITMAP_TIMES_ROMAN_24);
        texte2d(LONGUEUR_FENETRE / 2-50, LARGEUR_FENETRE/2, "Echap pour quitter :'(", 1, 1, 1,GLUT_BITMAP_TIMES_ROMAN_24);
    }

    glutSwapBuffers();
}

/*----------------------------------------*
| Fonction d'affichage 
*-----------------------------------------*/
void affichage(){
    sprintf(seconde_texte, "%d", vrai_temps);
    sprintf(point_jeu_texte, "%d", point_jeu);

    seconde--;
    vrai_temps = seconde / 59;
    if (seconde <= 0){
        boolVictoire = 0;
        glutDisplayFunc(affichageFin);
    }

    posX += vitesseDeCroisiere*vecVaisseauY[0];
    posY += vitesseDeCroisiere*vecVaisseauY[1];
    posZ += vitesseDeCroisiere*vecVaisseauY[2];
    /*----------------------------------------*
    | Remplit la fenêtre avec la couleur par défaut (noir) 
    *-----------------------------------------*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*----------------------------------------*
    | Affichage des contrôles et du score
    *-----------------------------------------*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, LONGUEUR_FENETRE, LARGEUR_FENETRE, 0); // Proj ortho en 2D
    if (boolMenu){
        texte2d(10, 20, "M - Menu on/off", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18); 
        texte2d(10, 40, "ESC - Quitter le Jeu", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18); 
        texte2d(10, 60, "ENTER - Plein ecran", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18); 
        texte2d(10, 80, "C - Curseur on/off", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 100, "T - Couleur meteorites", 1.0, 1.0, 1.0, GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 120, "R - Reinitialiser camera", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 140, "Molette - Zoom/Dezoom", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 160, "--------------", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 180, "Z - Acceler", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 200, "S - Ralentir", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 220, "SPACE - Stop", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 240, "H - Hyper-Espace", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(10, 260, "F/Clic-gauche - Tirer", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
    }
    if (point_jeu > 0){
        texte2d(500, 20, "Temps :", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(570, 20, seconde_texte, 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(620, 20, "Ennemis a eliminer :", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
        texte2d(790, 20, point_jeu_texte, 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
    }
    else{
        texte2d(790, 20, "Va sur la zone d'arrivee !", 1.0, 1.0, 1.0,GLUT_BITMAP_HELVETICA_18);
    }
    
    /*----------------------------------------*
    | Définit la matrice de projection, charge l'identité et redéfinit le volume de projection
    *-----------------------------------------*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(G, D, B, H, N_DIST, F);
    /*----------------------------------------*
    | On place la caméra derrière le vaisseau, et on lui fait regarder vers le centre du vaisseau 
    *-----------------------------------------*/
    ajuster_camera();

    /*----------------------------------------*
    | Définit la matrice modelView et charge l'identité 
    *-----------------------------------------*/
    glMatrixMode(GL_MODELVIEW);

    /*----------------------------------------*
    | Appels de nos fonctions de dessins
    *-----------------------------------------*/
    /*----------------------------------------*
    | Vaisseau 
    *-----------------------------------------*/
    glLoadIdentity();

    dessiner_vaisseau();

    /*----------------------------------------*
    | Planètes 
    *-----------------------------------------*/
    for (int i = 0; i < num_planets; i++) {
        Planet planet = planets[i];
        /*----------------------------------------*
        | Intersection entre planètes et vaisseau 
        *-----------------------------------------*/
        float distance = distance_A_B(posX, posY, posZ, planet.x, planet.y, planet.z);
        float somme_rayons = distance_rayon_necessaire(planet.t, bouleEnglobanteVaisseau/2);
        if (distance < somme_rayons){
            reinitPosVaisseau();
        }
        glPushMatrix();
        glTranslatef(planet.x, planet.y, planet.z);
        dessinPlanete(planet.t, 35, planet.c1, planet.c2, planet.c3);

        // On dessine un drapeau sur le haut de la planète n°0, cible a atteindre pour gagner
        if (i == 0){
            int zoneArrivee = 1000;
            glTranslatef(0,0,planet.t);
            glColor3f(0,0.7,0);
            glutWireSphere(200,20,20);
            if (point_jeu <= 0){
                glTranslatef(0,0,zoneArrivee);
                texte3d(0,0,0,"Zone d'arrivee", 1,1,1);
            }
            //On check si le vaisseau est dans la boule dans la théière
            float distance = distance_A_B(posX, posY, posZ, planet.x, planet.y, planet.z+planet.t);
            if (distance < 200)
            {
                if(point_jeu <= 0){
                    boolVictoire = 1;
                    glutDisplayFunc(affichageFin);
                }
            }
        }

        glPopMatrix();

        /*----------------------------------------*
        | Intersections entre planètes et missiles 
        *-----------------------------------------*/
        for (int i=0; i<NB_MAX_MISSILES; i++){
            distance = distance_A_B(planet.x, planet.y, planet.z, liste_missiles[i].x, liste_missiles[i].y, liste_missiles[i].z);
            somme_rayons = distance_rayon_necessaire(planet.t, liste_missiles[i].taille);
            if (distance < somme_rayons){
                generationExplosion(liste_missiles[i].x, liste_missiles[i].y, liste_missiles[i].z, nouvelleExplosion(), EXPLOSION_BLEU);
                reinitMissile(i);
                Mix_PlayChannel(-1, son_explosion, 0);
            }
        }
    }

    /*----------------------------------------*
    | Etoiles 
    *-----------------------------------------*/
    for (int i = 0; i < num_etoiles; i++) {
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0); 
        glTranslatef(etoiles[i].x, etoiles[i].y, etoiles[i].z);
        etoile(rayon_etoile, 0.0, 0.0, 0.0); 
        glPopMatrix();
    }

    /*----------------------------------------*
    | Météorites 
    *-----------------------------------------*/
    float distance;
    float somme_rayons;
    for (int i=0; i<NB_MAX_METEORITE; i++){
        if (liste_meteorites[i].active){
            /*----------------------------------------*
            | Vérif des intersections avec les météorites pour le vaisseau 
            *-----------------------------------------*/
            distance = distance_A_B(posX, posY, posZ, liste_meteorites[i].x, liste_meteorites[i].y, liste_meteorites[i].z);
            somme_rayons = distance_rayon_necessaire(liste_meteorites[i].taille, bouleEnglobanteVaisseau/2);
            if (distance < somme_rayons){
                reinitPosVaisseau();
            }

            for (int j = 0; j < num_planets; j++) {
                Planet planet = planets[j];
                /*----------------------------------------*
                | Vérif des intersections avec les planètes pour les météorites
                *-----------------------------------------*/
                distance = distance_A_B(planet.x, planet.y, planet.z, liste_meteorites[i].x, liste_meteorites[i].y, liste_meteorites[i].z);
                somme_rayons = distance_rayon_necessaire(planet.t, liste_meteorites[i].taille);
                if (distance < somme_rayons){
                    //On fait reapparaitre la météorite a un autre endroit de la map
                    generationExplosion(liste_meteorites[i].x, liste_meteorites[i].y, liste_meteorites[i].z, nouvelleExplosion(), EXPLOSION_ROUGE);
                    liste_meteorites[i].x = RandomFloat(-50000,200000);
                    liste_meteorites[i].y = RandomFloat(-50000,200000);
                    liste_meteorites[i].z = RandomFloat(-50000,200000);
                }
            }

            // On fait déplacer la météorite
            liste_meteorites[i].x += liste_meteorites[i].vitesse*liste_meteorites[i].vecX;
            liste_meteorites[i].y += liste_meteorites[i].vitesse*liste_meteorites[i].vecY;
            liste_meteorites[i].z += liste_meteorites[i].vitesse*liste_meteorites[i].vecZ;

            // On fait rebondir la météorite sur un "mur" imaginaire pour la ramener dans la zone de jeu
            if (liste_meteorites[i].x < -50000 || liste_meteorites[i].x > 200000){
                liste_meteorites[i].vecX = -liste_meteorites[i].vecX;
            }
            if (liste_meteorites[i].y < -50000 || liste_meteorites[i].y > 200000){
                liste_meteorites[i].vecY = -liste_meteorites[i].vecY;
            }
            if (liste_meteorites[i].z < -50000 || liste_meteorites[i].z > 200000){
                liste_meteorites[i].vecZ = -liste_meteorites[i].vecZ;
            }

            //On dessine la météorite
            glPushMatrix();
            glRotatef(i,liste_meteorites[i].x, liste_meteorites[i].y, liste_meteorites[i].z);
            dessin_meteorite(liste_meteorites[i].taille, liste_meteorites[i].x, liste_meteorites[i].y, liste_meteorites[i].z, liste_meteorites[i].nbFacettes);
            glPopMatrix();

            /*----------------------------------------*
            | Intersections entre météorites et missiles 
            *-----------------------------------------*/
            for (int j=0; j<NB_MAX_MISSILES; j++){
                distance = distance_A_B(liste_meteorites[i].x, liste_meteorites[i].y, liste_meteorites[i].z, liste_missiles[j].x, liste_missiles[j].y, liste_missiles[j].z);
                somme_rayons = distance_rayon_necessaire(liste_meteorites[i].taille, liste_missiles[j].taille);
                if (distance < somme_rayons){
                    generationExplosion(liste_missiles[j].x, liste_missiles[j].y, liste_missiles[j].z, nouvelleExplosion(), EXPLOSION_BLEU);
                    reinitMissile(j);
                    liste_meteorites[i].active = 0;
                    Mix_PlayChannel(-1, son_explosion, 0);
                }
            }
        }
    }

    /*----------------------------------------*
    | Galaxies cc
    *-----------------------------------------*/
    for (int i = 0; i < MAX_ETOILE_GALAXY; i++) {
        glPushMatrix();
        glColor3f(etoilesgalaxy[i].c1, etoilesgalaxy[i].c2, etoilesgalaxy[i].c3);
        glTranslatef(etoilesgalaxy[i].x, etoilesgalaxy[i].y, etoilesgalaxy[i].z);
        glRotatef(etoilesgalaxy[i].rotation_angle, 1.0, 1.0, 1.0);
        etoile(etoilesgalaxy[i].taille,0,0,0);
        glPopMatrix();
    }
    glPushMatrix();
    glTranslatef(50000,50000,-50000);
    dessinPlanete(15000, 40, 0.4,0.4,0.4);
    glPopMatrix();

    /*----------------------------------------*
    | Ennemis
    *-----------------------------------------*/
    if (point_jeu > 0){
        for (int i = 0; i < NB_MAX_ENNEMIS; i++){
            if (liste_ennemis[i].active){
                /*----------------------------------------*
                | Intersections entre vaisseau et ennemis 
                *-----------------------------------------*/
                distance = distance_A_B(liste_ennemis[i].x, liste_ennemis[i].y, liste_ennemis[i].z, posX, posY, posZ);
                somme_rayons = distance_rayon_necessaire(liste_ennemis[i].taille, bouleEnglobanteVaisseau/2);
                if (distance < somme_rayons){
                    reinitPosVaisseau();
                }
                liste_ennemis[i].x += liste_ennemis[i].vitesse * liste_ennemis[i].vecX;
                liste_ennemis[i].y += liste_ennemis[i].vitesse * liste_ennemis[i].vecY;
                liste_ennemis[i].z += liste_ennemis[i].vitesse * liste_ennemis[i].vecZ;
                glColor3f(1, 1, 1);
                texte3d(liste_ennemis[i].x + 50, liste_ennemis[i].y + 50, (liste_ennemis[i].z) + 50, "+1 pt.", 1, 1, 1);

                if (liste_ennemis[i].x < -50000 || liste_ennemis[i].x > 200000){
                    liste_ennemis[i].vecX = -liste_ennemis[i].vecX + RandomFloat(-.5,.5);
                }
                if (liste_ennemis[i].y < -50000 || liste_ennemis[i].y > 200000){
                    liste_ennemis[i].vecY = -liste_ennemis[i].vecY + RandomFloat(-.5,.5);
                }
                if (liste_ennemis[i].z < -50000 || liste_ennemis[i].z > 200000){
                    liste_ennemis[i].vecZ = -liste_ennemis[i].vecZ + RandomFloat(-.5,.5);
                }

                glPushMatrix();
                dessin_ennemi(liste_ennemis[i].taille, liste_ennemis[i].x, liste_ennemis[i].y, liste_ennemis[i].z);
                glPopMatrix();
                
                /*----------------------------------------*
                | Intersection entre ennemis et missiles 
                *-----------------------------------------*/
                for (int j=0; j<NB_MAX_MISSILES; j++){
                    distance = distance_A_B(liste_ennemis[i].x, liste_ennemis[i].y, liste_ennemis[i].z, liste_missiles[j].x, liste_missiles[j].y, liste_missiles[j].z);
                    somme_rayons = distance_rayon_necessaire(liste_ennemis[i].taille, liste_missiles[j].taille);
                    if (distance < somme_rayons){
                        generationExplosion(liste_missiles[j].x, liste_missiles[j].y, liste_missiles[j].z, nouvelleExplosion(), EXPLOSION_BLEU);
                        reinitMissile(j);
                        point_jeu--;
                        liste_ennemis[i].active = 0;
                        Mix_PlayChannel(-1, son_explosion, 0);
                    }
                }
            }
        }
    }

    /*----------------------------------------*
    | Missiles 
    *-----------------------------------------*/
    for (int i=0; i<NB_MAX_MISSILES; i++){
        liste_missiles[i].x += liste_missiles[i].vitesse * liste_missiles[i].vecX;
        liste_missiles[i].y += liste_missiles[i].vitesse * liste_missiles[i].vecY;
        liste_missiles[i].z += liste_missiles[i].vitesse * liste_missiles[i].vecZ;

        glPushMatrix();
        dessin_missile(liste_missiles[i].x, liste_missiles[i].y, liste_missiles[i].z, liste_missiles[i].taille);
        glPopMatrix();
    }

    /*----------------------------------------*
    | Explosions
    *-----------------------------------------*/
    majParticules();
    affichageParticules();

    /*----------------------------------------*
    | Swap entre les deux buffers (de calcul et d'affichage) pour l'animation 
    | -> appel a glFlush() dans glutSwapBuffers() pour envoyer sur le buffer d'affichage
    *-----------------------------------------*/
    glutSwapBuffers();
}

/*----------------------------------------*
| Libère les sons 
*-----------------------------------------*/
void cleanup() {
    Mix_FreeChunk(son_missile);
    Mix_FreeChunk(son_jeu);
    Mix_FreeChunk(son_explosion);
    son_missile = NULL;
    son_jeu = NULL;
    son_explosion = NULL;
    Mix_Quit();
    SDL_Quit();
}

int main(int argc, char * argv[]){
    srand(time(NULL)); 
    delay.tv_nsec = 10000000;

    //Génération des planètes
    for(int i = 0; i < MAX_PLANETS; i++){
        Generation_aléatoire_planet();
    }

    //Génération des étoiles
    for(int i = 0; i < MAX_ETOILES; i++){
        Generation_aléatoire_etoile();
    }

    //Génération des météorites
    generation_aleatoire_meteorites();

    //Génération des ennemis
    generation_aleatoire_ennemi();

    //Génération d'une galaxie
    genereEtoileGalaxy(50000,50000,-50000);

    //Génération des missiles
    generationMissile();

    /*----------------------------------------*
    | Fonction init glut 
    *-----------------------------------------*/
    glutInit(&argc, argv);

    /*----------------------------------------*
    | Init du mode d'affichage (double buffer)
    *-----------------------------------------*/
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    /*----------------------------------------*
    | Fixe la taille de la fenetre et la position 
    *-----------------------------------------*/
    glutInitWindowSize(LONGUEUR_FENETRE, LARGEUR_FENETRE);
    glutInitWindowPosition(50, 50);

    /*----------------------------------------*
    | Creation de la fenêtre 
    *-----------------------------------------*/
    glutCreateWindow("Star Wars !");

    /*----------------------------------------*
    | Pour la perspective 
    *-----------------------------------------*/
    glEnable(GL_DEPTH_TEST);

    /*----------------------------------------*
    | On précise ici le nom de notre fonction d'affichage 
    *-----------------------------------------*/
    glutDisplayFunc(affichage);

    /*----------------------------------------*
    | Gestion du clavier 
    *-----------------------------------------*/
    glutKeyboardFunc(GestionClavier);

    /*----------------------------------------*
    | Gestion de la souris 
    *-----------------------------------------*/
    glutPassiveMotionFunc(GestionSouris);
    glutMouseFunc(GestionSourisDeux);

    /*----------------------------------------*
    | On précise ici le nom de notre fonction d'animation (Idle est appelé en permanence)
    *-----------------------------------------*/
    glutIdleFunc(animation);

    glutSetCursor(GLUT_CURSOR_NONE);

    /*----------------------------------------*
    | Sons 
    *-----------------------------------------*/
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Probleme SDL: %s\n", SDL_GetError());
        exit(1);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Problème SDL mixer: %s\n", Mix_GetError());
        exit(1);
    }

    son_missile = Mix_LoadWAV("./sons/missile.wav");
    if (son_missile == NULL) {
        fprintf(stderr, "Problème chargement son: %s\n", Mix_GetError());
        return 1;
    }

    son_jeu = Mix_LoadWAV("./sons/imperial_march.wav");
    if (son_jeu == NULL) {
        fprintf(stderr, "Problème chargement son: %s\n", Mix_GetError());
        return 1;
    }

    son_explosion = Mix_LoadWAV("./sons/explosion.wav");
    if (son_explosion == NULL) {
        fprintf(stderr, "Problème chargement son: %s\n", Mix_GetError());
        return 1;
    }

    Mix_PlayChannel(-1, son_jeu, 0);

    /*----------------------------------------*
    | Lancement du gestionnaire GLUT, boucle infinie qui écoute les évènements 
    *-----------------------------------------*/
    glutMainLoop();

    cleanup();

    return 0;
}
