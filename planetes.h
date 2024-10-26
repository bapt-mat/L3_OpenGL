#include "GL/gl.h"
#include "GL/glut.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "time.h"
#include "math.h"

/****************************
|   Aleatoire point struct
*****************************/
typedef struct {
    float x;
    float y;
    float z;
    float c1;
    float c2;
    float c3;
    int h;
    int v;
    int t;
} Planet;

#define MAX_PLANETS 100
Planet planets[MAX_PLANETS];
int num_planets = 0;
int test_incrementation = 0;
int fun = 0;
