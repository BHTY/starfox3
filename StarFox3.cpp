#define _CRT_SECURE_NO_DEPRECATE
#pragma check_stack(off)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl/glut.h>
#include <math.h>

int refreshMills = 16;
float cameraZ = -100.0;
float cameraY = 0.0;
float cameraX = 0.0;
int cameraRotX = 0;
int cameraRotY = 0;
char keys[256];

struct Vertex {
    float x;
    float y;
    float z;
};

struct Triangle {
    Vertex vert1;
    Vertex vert2;
    Vertex vert3;
    float shadeR;
    float shadeG;
    float shadeB;
};

struct Entity {
    Vertex coords;
    //Triangle* tris;
    Triangle tris[5000];
    int numTris;
};

size_t* objects;
int numObjects = 0;

double r2() {
    return (double)rand() / (double)RAND_MAX;
}

Entity loadOBJ(char* fileName) {
    Entity object;
    FILE* fp = fopen(fileName, "r");
    int eof = 0;
    Vertex verts[15000];
    int currentVert = 0;
    int currentFace = 0;
    float currentR = 0.0;
    float currentG = 0.0;
    float currentB = 0.0;

    while (!eof) {
        char lineHeader[512];
        int res = fscanf(fp, "%s", lineHeader);

        if (res == -1) { eof = 1; break; }

        if(strcmp(lineHeader, "v") == 0){
            Vertex temp;
            fscanf(fp, "%f %f %f\n", &temp.x, &temp.y, &temp.z);
            verts[currentVert] = temp;
            currentVert++;
        }

        else if (strcmp(lineHeader, "f") == 0) {
            int face1 = 0;
            int face2 = 0;
            int face3 = 0;
            fscanf(fp, "%d %d %d\n", &face1, &face2, &face3);
            Triangle currentTri;
            currentTri.vert1 = verts[face1-1];
            currentTri.vert2 = verts[face2-1];
            currentTri.vert3 = verts[face3-1];
            currentTri.shadeR = currentR;
            currentTri.shadeG = currentG;
            currentTri.shadeB = currentB;
            object.tris[currentFace] = currentTri;
            currentFace++;
        }
        else if (strcmp(lineHeader, "usemtl") == 0) {
            fscanf(fp, "%f %f %f\n", &currentR, &currentG, &currentB);
        }
        object.numTris = currentFace;


    }

    fclose(fp);
    return object;
}

void initGL() {
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glDisable(GL_CULL_FACE); //default, two-sided rendering
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void display() {
    //Entity obj = loadOBJ((char*)"C:\\Users\\Will\\Documents\\Custom Edited - Star Fox Customs - Arwing SNES\\arwing\\arwing SNES.obj");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(cameraX, cameraY, cameraZ);
    glRotatef(cameraRotY, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraRotX, 0.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);

    for (int current = 0; current < numObjects; current++) {
        (*(Entity*)objects[0]).coords.z += 0.1;
        Entity obj = *(Entity*)objects[current];

        for (int i = 0; i < obj.numTris; i++) {
            glColor3f(obj.tris[i].shadeR, obj.tris[i].shadeG, obj.tris[i].shadeB);
            glVertex3f(obj.tris[i].vert1.x + obj.coords.x, obj.tris[i].vert1.y + obj.coords.y, obj.tris[i].vert1.z + obj.coords.z);
            glVertex3f(obj.tris[i].vert2.x + obj.coords.x, obj.tris[i].vert2.y + obj.coords.y, obj.tris[i].vert2.z + obj.coords.z);
            glVertex3f(obj.tris[i].vert3.x + obj.coords.x, obj.tris[i].vert3.y + obj.coords.y, obj.tris[i].vert3.z + obj.coords.z);
        }
    }

    glEnd();
    glutSwapBuffers();
    //cameraRot++;

    if (keys[115]) { cameraY++; }
    if (keys[119]) { cameraY--; }
    if (keys[97]) { cameraX--; }
    if (keys[100]) { cameraX++; }
    if (keys[101]) { cameraZ++; }
    if (keys[113]) { cameraZ--; }
    if (keys[105]) { cameraRotY--; }
    if (keys[107]) { cameraRotY++; }
    if (keys[106]) { cameraRotX--; }
    if (keys[108]) { cameraRotX++; }
}

void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(refreshMills, timer, 0);
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 10000.0f);
}

void handleKeyDown(unsigned char key, int x, int y) {
    keys[key] = 1;
}

void handleKeyUp(unsigned char key, int x, int y) {
    keys[key] = 0;
}

int main(int argc, char** argv)
{
    //objects = (Entity *)malloc(50 * sizeof(Entity));
    objects = (size_t*)malloc(50 * sizeof(size_t));
    //printf("%d\n", objects);
    char SNESArwing[100] = "C:\\Users\\Will\\Documents\\Custom Edited - Star Fox Customs - Arwing SNES\\arwing\\arwing SNES.obj";
    //":\\Users\\Will\\Desktop\\arwing.obj
    char Castle[150] = "C:\\Users\\Will\\Documents\\DS DSi - Super Mario 64 DS - Peachs Castle Grounds\\Peach's Castle\\Peach's Castle_.obj";
    char DoTheMario[150] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Super Mario 64 - Mario\\Super Mario 64 - Mario_.obj";
    char Arwing[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Arwing\\Arwing\\arwing_.obj";
    char Fox[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Fox McCloud\\FoxMcCloud\\FoxMcCloud_.obj";
    char fileName[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Wolfen\\Wolfen\\wolfen_.obj";
    char fileNameOther[100] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Luigi\\Luigi\\Luigi_.obj";

    Entity* castle = (Entity*)malloc(sizeof(Entity));
    *castle = loadOBJ(Castle);

    Entity* luigi = (Entity*)malloc(sizeof(Entity));
    *luigi = loadOBJ(fileNameOther);

    Entity* thing = (Entity*)malloc(sizeof(Entity));
    *thing = loadOBJ(Arwing);

    Entity* arwing = (Entity*)malloc(sizeof(Entity));
    memset(arwing, 0, sizeof(Entity));
    *arwing = loadOBJ(fileName);

    objects[0] = (size_t)arwing;
    objects[1] = (size_t)luigi;
    objects[2] = (size_t)thing;
    objects[3] = (size_t)castle;

    numObjects = 4;
    printf("Do a barrel roll!\n%d Polygons \n%d KB\n", (*arwing).numTris, sizeof(*arwing)/(1024));
    /**Entity list[50];
    memset(list, 0, sizeof(list));
    printf("%d %d\n", sizeof(list), list[0].coords.z);**/

    //Entity obj = loadOBJ((char *)"C:\\Users\\Will\\Documents\\Custom Edited - Star Fox Customs - Arwing SNES\\arwing\\arwing SNES.obj");
    
    /**Entity obj = *arwing;
    for (int i = 0; i < obj.numTris; i++) {
        printf("Triangle %d\n", i);
        printf("   Vertex 1: x=%f y=%f z=%f\n", obj.tris[i].vert1.x, obj.tris[i].vert1.y, obj.tris[i].vert1.z);
        printf("   Vertex 2: x=%f y=%f z=%f\n", obj.tris[i].vert2.x, obj.tris[i].vert2.y, obj.tris[i].vert2.z);
        printf("   Vertex 3: x=%f y=%f z=%f\n", obj.tris[i].vert3.x, obj.tris[i].vert3.y, obj.tris[i].vert3.z);
    }**/

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Star Fox 3");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyDown);
    glutKeyboardUpFunc(handleKeyUp);
    initGL();
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}