#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl/glut.h>
#include <math.h>

int refreshMills = 16;
float cameraZ = -20.0;
int cameraRot = 0;

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
    Triangle tris[1500];
    int numTris;
};

Entity obj;

double r2() {
    return (double)rand() / (double)RAND_MAX;
}

Entity loadOBJ(char* fileName) {
    Entity object;
    FILE* fp = fopen(fileName, "r");
    int eof = 0;
    Vertex verts[4500];
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
        object.numTris = currentFace + 1;


    }

    fclose(fp);
    return object;
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
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
    glTranslatef(0.0, 0.0, cameraZ);
    glRotatef(cameraRot, 1.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < obj.numTris; i++) {
        glColor3f(obj.tris[i].shadeR, obj.tris[i].shadeG, obj.tris[i].shadeB);
        glVertex3f(obj.tris[i].vert1.x, obj.tris[i].vert1.y, obj.tris[i].vert1.z);
        glVertex3f(obj.tris[i].vert2.x, obj.tris[i].vert2.y, obj.tris[i].vert2.z);
        glVertex3f(obj.tris[i].vert3.x, obj.tris[i].vert3.y, obj.tris[i].vert3.z);
    }

    glEnd();
    glutSwapBuffers();
    cameraRot++;
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
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv)
{
    //char fileName[100] = "C:\\Users\\Will\\Documents\\Custom Edited - Star Fox Customs - Arwing SNES\\arwing\\arwing SNES.obj";
    //":\\Users\\Will\\Desktop\\arwing.obj
    char fileName[150] = "C:\\Users\\Will\\Documents\\DS DSi - Super Mario 64 DS - Peachs Castle Grounds\\Peach's Castle\\Peach's Castle_.obj";
    //char fileName[150] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Super Mario 64 - Mario\\Super Mario 64 - Mario_.obj";
    //char fileName[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Arwing\\Arwing\\arwing_.obj";
    //char fileName[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Fox McCloud\\FoxMcCloud\\FoxMcCloud_.obj";
    //char fileName[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Wolfen\\Wolfen\\wolfen_.obj";
    obj = loadOBJ(fileName);
    printf("Do a barrel roll!\n%d Polygons\n", obj.numTris);
    /**Entity list[50];
    memset(list, 0, sizeof(list));
    printf("%d %d\n", sizeof(list), list[0].coords.z);**/

    //Entity obj = loadOBJ((char *)"C:\\Users\\Will\\Documents\\Custom Edited - Star Fox Customs - Arwing SNES\\arwing\\arwing SNES.obj");
    
    /**printf("%d\n", obj.numTris);

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
    initGL();
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}