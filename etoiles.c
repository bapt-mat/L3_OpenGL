#include "./etoiles.h"

/*----------------------------------------*
|     < Fonction de déssin d'étoile >
*-----------------------------------------*/
/*        __________ 
        ."          ".
        | .--------. |   
       /` |_etoile__| `\  
      |  / / .\/. \ \  |
      \_/  \__/\__/  \_/
        \            /   
        /'._  --  _.'\  
       /_   `""""`   _\ 
     (_)_|           |_)
         \__________/   
          '---------'   
*/
void etoile(float size, float posX, float posY, float posZ) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ); 
    //glColor3f(BLANC); 
    
    // axe x
    glBegin(GL_LINES);
    glVertex3f(-size, 0.0, 0.0);
    glVertex3f(size, 0.0, 0.0);
    glEnd();
    
    // axe y
    glBegin(GL_LINES);
    glVertex3f(0.0, -size, 0.0);
    glVertex3f(0.0, size, 0.0);
    glEnd();
    
    // axe z
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, -size);
    glVertex3f(0.0, 0.0, size);
    glEnd();
    
    // diagonales
    glBegin(GL_LINES);
    glVertex3f(-size, -size, 0.0);
    glVertex3f(size, size, 0.0);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(-size, size, 0.0);
    glVertex3f(size, -size, 0.0);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(0.0, -size, -size);
    glVertex3f(0.0, size, size);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(0.0, size, -size);
    glVertex3f(0.0, -size, size);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(-size, 0.0, -size);
    glVertex3f(size, 0.0, size);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(size, 0.0, -size);
    glVertex3f(-size, 0.0, size);
    glEnd();
    
    glPopMatrix();
}

//█▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀█
//█░░                ░░█
//█░░  Intersection  ░░█
//█░░                ░░█
//█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█
int est_intersection_etoile(float x, float y, float z, float rayon) {
    float distance;
    for (int i = 0; i < num_etoiles; i++) {
        distance = sqrt(pow((x - etoiles[i].x), 2) + pow((y - etoiles[i].y), 2) + pow((z - etoiles[i].z), 2));
        if (distance < (rayon + rayon_etoile)) {
            puts("UNE ETOILE EST EN COLISION ON RECALCULE DES NOUVELLES CONFIGURATION");
            return 1; //intersection
        }
    }
    return 0;
}



/*----------------------------------------*
|Fonction de génération aléatoire d'étoile
*-----------------------------------------*/

void Generation_aléatoire_etoile() {
    float x, y, z;
    x = RandomFloat(-200000, 300000);
    y = RandomFloat(-200000, 300000);
    z = RandomFloat(-200000, 300000);

    Etoile new_etoile;
    new_etoile.x = x;
    new_etoile.y = y;
    new_etoile.z = z;
    etoiles[num_etoiles++] = new_etoile;
}

void genereEtoileGalaxy(float posX, float posY, float posZ) {
    float radius, angle;
    float taille, rotation_angle;

    for (int i = 0; i < MAX_ETOILE_GALAXY; i++) {
        radius = RandomFloat(16000, 24000);
        angle = RandomFloat(0.0, 2 * M_PI);
        int x = posX + radius * cos(angle);
        int z = posZ + radius * sin(angle);
        int y = posY + radius * sin(angle); 
        float c11=RandomFloat(0,1.0);
        float c22=RandomFloat(0,1.0);
        float c33=RandomFloat(0,1.0);
        taille = RandomFloat(5, 50);
        rotation_angle = RandomInt(0, 359);

        etoilegalaxy new_etoile_galaxie;
        new_etoile_galaxie.x = x;
        new_etoile_galaxie.y = y;
        new_etoile_galaxie.z = z;
        new_etoile_galaxie.taille = taille;
        new_etoile_galaxie.rotation_angle = rotation_angle;
        new_etoile_galaxie.c1 = c11;
        new_etoile_galaxie.c2 = c22;
        new_etoile_galaxie.c3 = c33;
        etoilesgalaxy[num_etoilegalaxy++] = new_etoile_galaxie;
    }
}