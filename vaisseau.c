#include "./vaisseau.h"

/*----------------------------------------*
| Primitives de tracés
*-----------------------------------------*/
void affiche_cube(float x1, float y1, float z1, float x2, float y2, float z2){
    glBegin(GL_QUADS);

    glColor3f(0.9,0.9,0.9);
    glVertex3f(x1, y1, z1);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x1, y1, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x2, y1, z2);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(x2, y1, z1);

    glColor3f(0.9,0.9,0.9);
    glVertex3f(x2, y1, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x2, y2, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x2, y2, z1);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(x2, y1, z1);

    glColor3f(0.9,0.9,0.9);
    glVertex3f(x1, y1, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x1, y2, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x1, y2, z1);
     glColor3f(0.9,0.9,0.9);
    glVertex3f(x1, y1, z1);

    glColor3f(0.9,0.9,0.9);
    glVertex3f(x1, y2, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x2, y2, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x2, y2, z1);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(x1, y2, z1);

    glColor3f(0.9,0.9,0.9);
    glVertex3f(x1, y1, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x1, y2, z2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x2, y2, z2);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(x2, y1, z2);

    glColor3f(0.9,0.9,0.9);
    glVertex3f(x1, y1, z1);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x1, y2, z1);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(x2, y2, z1);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(x2, y1, z1);

    glEnd();
}

void dessiner_cylindre(float rayon, float hauteur, int nbFacettes) {
    glColor3f(0.4,0.4,0.4);
    // Dessiner les côtés du cylindre
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= nbFacettes; ++i) {
        glColor3f(i*0.04,i*0.04,i*0.04);
        float angle = 2.0 * M_PI * i / nbFacettes;
        float x = rayon * cos(angle);
        float z = rayon * sin(angle);

        glVertex3f(x, hauteur / 2, z);
        glVertex3f(x, -hauteur / 2, z);
    }
    glEnd();

    glColor3f(0.4,0.4,0.4);
    // Dessiner les bases du cylindre
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, hauteur / 2, 0); // Centre de la base supérieure
    for (int i = 0; i <= nbFacettes; ++i) {
        
        float angle = 2.0 * M_PI * i / nbFacettes;
        float x = rayon * cos(angle);
        float z = rayon * sin(angle);

        glVertex3f(x, hauteur / 2, z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, -hauteur / 2, 0); // Centre de la base inférieure
    for (int i = 0; i <= nbFacettes; ++i) {
        
        float angle = 2.0 * M_PI * i / nbFacettes;
        float x = rayon * cos(angle);
        float z = rayon * sin(angle);

        glVertex3f(x, -hauteur / 2, z);
    }
    glEnd();
}

void dessiner_cylindre_rouge(float rayon, float hauteur, int nbFacettes) {
    glColor3f(0.9/(vitesseDeCroisiere/10),0,0.5*(vitesseDeCroisiere/20));
    // Dessiner les côtés du cylindre en rouge
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= nbFacettes; ++i) {
        float angle = 2.0 * M_PI * i / nbFacettes;
        float x = rayon * cos(angle);
        float z = rayon * sin(angle);

        glVertex3f(x, 0, z);
        glVertex3f(x, -hauteur / 2, z);
    }
    glEnd();

    glColor3f(1,1,0);
    // Dessiner l'intérieur du cylindre en jaune
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= nbFacettes; ++i) {
        float angle = 2.0 * M_PI * i / nbFacettes;
        float x = (rayon-0.05) * cos(angle);
        float z = (rayon-0.05) * sin(angle);

        glVertex3f(x, 0, z);
        glVertex3f(x, -hauteur / 2, z);
    }
    glEnd();
}

/*----------------------------------------*
| Dessin du vaisseau 
*-----------------------------------------*/
void dessiner_vaisseau() {
    glPushMatrix();

    // Utilisation des vecteurs du repère pour déplacer le vaisseau
    GLfloat repere[16] = {
        vecVaisseauX[0], vecVaisseauX[1], vecVaisseauX[2], 0,
        vecVaisseauY[0], vecVaisseauY[1], vecVaisseauY[2], 0,
        vecVaisseauZ[0], vecVaisseauZ[1], vecVaisseauZ[2], 0,
        posX, posY, posZ, 1
    };

    // Charge la matrice de transformation
    glMultMatrixf(repere);

    glTranslatef(-.5,0,-.5);

    affiche_cube(0, 0, 0, 1, 1, 0.9); //cube centrale
    affiche_cube(0.2,1,0.2,0.8,7,0.7);

    glTranslated(-0.2,1,0.1); //pour avancer un peu les réacteurs
    dessiner_cylindre(0.25, 3, 20); //réacteur en bas a gauche
    glTranslated(0,-1.5,0);
    dessiner_cylindre_rouge(0.1, 0.01*vitesseDeCroisiere, 6);
    glTranslated(0,1.5,0);

    glTranslated(0,0,0.8); 
    dessiner_cylindre(0.25, 3, 20); //réacteur en haut a gauche
    glTranslated(0,-1.5,0);
    dessiner_cylindre_rouge(0.1, 0.01*vitesseDeCroisiere, 6);
    glTranslated(0,1.5,0);

    glTranslated(1.4,0,0);
    dessiner_cylindre(0.25, 3, 20); //réacteur en haut a droite
    glTranslated(0,-1.5,0);
    dessiner_cylindre_rouge(0.1, 0.01*vitesseDeCroisiere, 6);
    glTranslated(0,1.5,0);

    glTranslated(0,0,-0.8);
    dessiner_cylindre(0.25, 3, 20); //réacteur en bas a droite
    glTranslated(0,-1.5,0);
    dessiner_cylindre_rouge(0.1, 0.01*vitesseDeCroisiere, 6);
    glTranslated(0,1.5,0);

    //couleurs des ailes
    glColor3f(0.3,0.3,0.3);
    
    //aile en bas a gauche
    glLoadIdentity();
    glMultMatrixf(repere);
    glTranslatef(-.5,0,-.5);
    glBegin(GL_POLYGON);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(0,0,0.2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(-2.5,0.8,-0.2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(-2.5,2.2,-0.2);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(0,3,0.2);
    glEnd();

    glLoadIdentity();
    glMultMatrixf(repere);
    glTranslatef(-2.9,3,-0.7);
    dessiner_cylindre(0.05, 6, 20);
    glTranslated(0,-1.5,0);
    dessiner_cylindre(0.1, 2, 20);

    //aile en bas a droite
    glLoadIdentity();
    glMultMatrixf(repere);
    glTranslatef(-.5,0,-.5);
    glBegin(GL_POLYGON);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(1,0,0.2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(3.4,0.8,-0.2);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(3.4,2.2,-0.2);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(1,3,0.2);
    glEnd();

    glLoadIdentity();
    glMultMatrixf(repere);
    glTranslatef(2.9,3,-0.7);
    dessiner_cylindre(-0.05, 6, 20);
    glTranslated(0,-1.5,0);
    dessiner_cylindre(-0.1, 2, 20);

    //aile en haut a gauche
    glLoadIdentity();
    glMultMatrixf(repere);
    glTranslatef(-.5,0,-.5);
    glBegin(GL_POLYGON);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(0,0,0.8);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(-2.5,0.8,1);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(-2.5,2.2,1);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(0,3,0.8);
    glEnd();

    glLoadIdentity();
    glMultMatrixf(repere);
    glTranslatef(-2.9,3,0.5);
    dessiner_cylindre(0.05, 6, 20);
    glTranslated(0,-1.5,0);
    dessiner_cylindre(0.1, 2, 20);

    //aile en haut a droite
    glLoadIdentity();
    glMultMatrixf(repere);
    glTranslatef(-.5,0,-.5);
    glBegin(GL_POLYGON);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(1,0,0.8);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(3.4,0.8,1);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(3.4,2.2,1);
    glColor3f(0.9,0.9,0.9);
    glVertex3f(1,3,0.8);
    glEnd();

    glLoadIdentity();
    glMultMatrixf(repere);
    glTranslatef(2.9,3,0.5);
    dessiner_cylindre(-0.05, 6, 20);
    glTranslated(0,-1.5,0);
    dessiner_cylindre(-0.1, 2, 20);

    glPopMatrix();
}