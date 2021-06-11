#define _CRT_SECURE_NO_DEPRECATE
#pragma check_stack(off)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl/glut.h>
#include <math.h>
#define polyLimit 5000

int* boob;
int wireframe = 0;

int refreshMills = 16;
float cameraZ = -100.0;
float cameraY = 0.0;
float cameraX = 0.0;
int cameraRotX = 0;
int cameraRotY = 0;
char keys[256];
float xRot = 0;

struct Message {
    float sender;
    float messageType;
};

struct Vector {
    float rotX;
    float rotY;
    float rotZ;
};

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
    Triangle tris[polyLimit];
    //Triangle* tris;
    int numTris;
    int visible;
    Vector rotation;
    char* scriptPtr;
    int scriptSize;
    float state[8];
    Message messages[128];
    char messagePointer;
};

//size_t* objects;
int numObjects = 0;
Entity luigi;
Entity objects[100];

float* matmul(float matrix[3][3], float* vector) {
    float* newFloats = (float*)malloc(3 * sizeof(float));
    int i, o;

    for (i = 0; i < 3; i++) {
        float temp = 0.0;
        for (o = 0; o < 3; o++) {
            temp += (matrix[i][o] * vector[o]);
        }
        newFloats[i] = temp;
    }

    return newFloats;

}

Triangle formTri(Vertex vert1, Vertex vert2, Vertex vert3, unsigned short shade) {
    Triangle temp;
    temp.vert1 = vert1;
    temp.vert2 = vert2;
    temp.vert3 = vert3;
    temp.shadeR = 0.0;
    temp.shadeG = 0.0;
    temp.shadeB = 1.0;
    return temp;
}

Entity formCube(unsigned short shade) {
    Entity temporary;
    Vertex verts[8];
    int i;

    for (i = 0; i < 8; i++) {
        if (((i % 4) == 0) || (i % 4) == 1) {
            verts[i].x = 5.0;
        }
        else {
            verts[i].x = -5.0;
        }
        if (i < 4) {
            verts[i].y = -5.0;
        }
        else {
            verts[i].y = 5.0;
        }
        if ((i == 7) || (i == 4) || (i == 3) || (i == 0)) {
            verts[i].z = -5.0;
        }
        else {
            verts[i].z = 5.0;
        }
    }

    temporary.tris[0] = formTri(verts[1], verts[2], verts[3], shade);
    temporary.tris[1] = formTri(verts[7], verts[6], verts[5], shade);
    temporary.tris[2] = formTri(verts[4], verts[5], verts[1], shade);
    temporary.tris[3] = formTri(verts[5], verts[6], verts[2], shade);
    temporary.tris[4] = formTri(verts[2], verts[6], verts[7], shade);
    temporary.tris[5] = formTri(verts[0], verts[3], verts[7], shade);
    temporary.tris[6] = formTri(verts[0], verts[1], verts[3], shade);
    temporary.tris[7] = formTri(verts[4], verts[7], verts[5], shade);
    temporary.tris[8] = formTri(verts[0], verts[4], verts[1], shade);
    temporary.tris[9] = formTri(verts[1], verts[5], verts[2], shade);
    temporary.tris[10] = formTri(verts[3], verts[2], verts[7], shade);
    temporary.tris[11] = formTri(verts[4], verts[0], verts[7], shade);
    temporary.numTris = 12;
    temporary.coords.x = 0;
    temporary.coords.y = 0;
    temporary.coords.z = 0;


    return temporary;
}

Entity loadOBJ(char* fileName, float scaleFactorX, float scaleFactorY, float scaleFactorZ) {
    Entity object;
    FILE* fp = fopen(fileName, "r");
    int eof = 0;
    Vertex verts[polyLimit*3];
    Triangle tris[polyLimit];
    int currentVert = 0;
    int currentFace = 0;
    float currentR = 0.0;
    float currentG = 0.0;
    float currentB = 0.0;

    while (!eof) {
        char lineHeader[512];
        int res = fscanf(fp, "%s", lineHeader);

        if (res == -1) { eof = 1; break; }

        if (strcmp(lineHeader, "v") == 0) {
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
            currentTri.vert1 = verts[face1 - 1];
            currentTri.vert2 = verts[face2 - 1];
            currentTri.vert3 = verts[face3 - 1];
            currentTri.shadeR = currentR;
            currentTri.shadeG = currentG;
            currentTri.shadeB = currentB;

            //scale vert1
            currentTri.vert1.x *= scaleFactorX;
            currentTri.vert1.y *= scaleFactorY;
            currentTri.vert1.z *= scaleFactorZ;
            currentTri.vert2.x *= scaleFactorX;
            currentTri.vert2.y *= scaleFactorY;
            currentTri.vert2.z *= scaleFactorZ;
            currentTri.vert3.x *= scaleFactorX;
            currentTri.vert3.y *= scaleFactorY;
            currentTri.vert3.z *= scaleFactorZ;

            object.tris[currentFace] = currentTri;
            currentFace++;
        }
        else if (strcmp(lineHeader, "usemtl") == 0) {
            fscanf(fp, "%f %f %f\n", &currentR, &currentG, &currentB);
        }
        object.numTris = currentFace;


    }

    //object.tris = (Triangle*)malloc(object.numTris * sizeof(Triangle));
    //memcpy(object.tris, tris, object.numTris * sizeof(Triangle));
    fclose(fp);
    object.visible = 1;
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

void runEventScript();

void runEntityScript(Entity* entity) {
    //(*entity).rotation.rotY+=0.1;

    int currentIndex = 0;

    while ((*entity).scriptSize > currentIndex) {
        //fetch command
    }
}

void display() {
    //Entity obj = loadOBJ((char*)"C:\\Users\\Will\\Documents\\Custom Edited - Star Fox Customs - Arwing SNES\\arwing\\arwing SNES.obj");
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(cameraX, cameraY, cameraZ);
    glRotatef(cameraRotY, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraRotX, 0.0f, 1.0f, 0.0f);
    //printf("hello");

    glBegin(GL_TRIANGLES);
    
    for (int current = 0; current < numObjects; current++) {
        runEntityScript(&(objects[current]));
        Entity obj = objects[current];

        if (obj.visible == 1) {
            for (int i = 0; i < obj.numTris; i++) {
                glColor3f(obj.tris[i].shadeR, obj.tris[i].shadeG, obj.tris[i].shadeB);
                /*glVertex3f(obj.tris[i].vert1.x + obj.coords.x, obj.tris[i].vert1.y + obj.coords.y, obj.tris[i].vert1.z + obj.coords.z);
                glVertex3f(obj.tris[i].vert2.x + obj.coords.x, obj.tris[i].vert2.y + obj.coords.y, obj.tris[i].vert2.z + obj.coords.z);
                glVertex3f(obj.tris[i].vert3.x + obj.coords.x, obj.tris[i].vert3.y + obj.coords.y, obj.tris[i].vert3.z + obj.coords.z);*/

                float rotationX[3][3] = {
                    {1, 0, 0}, 
                    {0, cos(obj.rotation.rotX), -sin(obj.rotation.rotX)},
                    {0, sin(obj.rotation.rotX), cos(obj.rotation.rotX)}
                };

                float rotationY[3][3] = {
                    {cos(obj.rotation.rotY), 0, -sin(obj.rotation.rotY)},
                    {0, 1, 0}, 
                    {sin(obj.rotation.rotY), 0, cos(obj.rotation.rotY)}
                };

                float rotationZ[3][3] = {
                    {cos(obj.rotation.rotZ), -sin(obj.rotation.rotZ), 0},
                    {sin(obj.rotation.rotZ), cos(obj.rotation.rotZ), 0},
                    {0, 0, 1} 
                };

                //add matrix multiplication functions

                //float vert1[3] = {obj.tris[i].vert1.x + obj.coords.x, obj.tris[i].vert1.y + obj.coords.y, obj.tris[i].vert1.z + obj.coords.z};
                float vert1[3] = { obj.tris[i].vert1.x, obj.tris[i].vert1.y, obj.tris[i].vert1.z};
                float* vert = matmul(rotationX, vert1);
                vert = matmul(rotationY, vert);
                vert = matmul(rotationZ, vert);
                glVertex3f(vert[0] + obj.coords.x, vert[1] + obj.coords.y, vert[2] + obj.coords.z);

                //float vert2[3] = { obj.tris[i].vert2.x + obj.coords.x, obj.tris[i].vert2.y + obj.coords.y, obj.tris[i].vert2.z + obj.coords.z };
                float vert2[3] = { obj.tris[i].vert2.x, obj.tris[i].vert2.y, obj.tris[i].vert2.z };
                vert = matmul(rotationX, vert2);
                vert = matmul(rotationY, vert);
                vert = matmul(rotationZ, vert);
                glVertex3f(vert[0] + obj.coords.x, vert[1] + obj.coords.y, vert[2] + obj.coords.z);

                //float vert3[3] = { obj.tris[i].vert3.x + obj.coords.x, obj.tris[i].vert3.y + obj.coords.y, obj.tris[i].vert3.z + obj.coords.z };
                float vert3[3] = { obj.tris[i].vert3.x, obj.tris[i].vert3.y, obj.tris[i].vert3.z };
                vert = matmul(rotationX, vert3);
                vert = matmul(rotationY, vert);
                vert = matmul(rotationZ, vert);
                glVertex3f(vert[0] + obj.coords.x, vert[1] + obj.coords.y, vert[2] + obj.coords.z);

            }
        }
    }

    glEnd();
    glutSwapBuffers();
    objects[0].rotation.rotZ += 0.01;

    if (keys[115]) { cameraY++; }
    if (keys[119]) { cameraY--; }
    if (keys[97])  { cameraX++; }
    if (keys[100]) { cameraX--; }
    if (keys[101]) { cameraZ++; }
    if (keys[113]) { cameraZ--; }
    if (keys[105]) { cameraRotY--; }
    if (keys[107]) { cameraRotY++; }
    if (keys[106]) { cameraRotX--; }
    if (keys[108]) { cameraRotX++; }

    if (keys[32]) { 
        if (!wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframe = 1;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframe = 0;
        }
    }
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

void sendMessage(int entityID, int senderID, float message) {
    Message tempMessage;
    tempMessage.sender = (float)senderID;
    tempMessage.messageType = message;
    objects[entityID].messages[objects[entityID].messagePointer++] = tempMessage;
}

Message popMessage(int entityID) { //vm calling convention will be handleMessage(sender, messageType)
    return objects[entityID].messages[objects[entityID].messagePointer--];
}

void spawnObject(char* fileName, float scaleX, float scaleY, float scaleZ, float Xpos, float Ypos, float Zpos) {
    Entity temporary = loadOBJ(fileName, scaleX, scaleY, scaleZ);
    temporary.coords.x = Xpos;
    temporary.coords.y = Ypos;
    temporary.coords.z = Zpos;
    objects[numObjects] = temporary;
    numObjects++;
}

void destroyObject(int index) {
    Entity* bufferBefore = (Entity *)malloc(index * sizeof(Entity)); //initialize buffers
    Entity* bufferAfter = (Entity *)malloc((numObjects - index) * sizeof(Entity));
    memcpy(bufferBefore, objects, index * sizeof(Entity)); //copy all objects before index to spare buffer 1
    memcpy(bufferAfter, objects + (index * sizeof(Entity)), (numObjects - index) * sizeof(Entity)); //copy all objects after index to spare buffer 2
    //copy spare buffer 1 into objects
    memcpy(objects, bufferBefore, index * sizeof(Entity));
    //copy spare buffer 2 into objects
    memcpy(objects + (index * sizeof(Entity)), bufferAfter, (numObjects - index) * sizeof(Entity));
    //decrement number of objects
    numObjects--;
    //cleanup
    free(bufferBefore);
    free(bufferAfter);
}

void gameLoop() {
    //objects[0].visible = ~(objects[0].visible);
}

int main(int argc, char** argv)
{
    char luigiFileName[100] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Luigi\\Luigi\\Luigi_.obj";
    char SNESArwingFileName[100] = "C:\\Users\\Will\\Documents\\Custom Edited - Star Fox Customs - Arwing SNES\\arwing\\arwing SNES.obj";
    char castleFileName[150] = "C:\\Users\\Will\\Documents\\DS DSi - Super Mario 64 DS - Peachs Castle Grounds\\Peach's Castle\\Peach's Castle_.obj";
    char MarioFaceFileName[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Super Mario 64 - Marios Head\\mariohead_.obj";
    char marioFileName[150] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Super Mario 64 - Mario\\Super Mario 64 - Mario_.obj";
    char arwingFileName[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Arwing\\Arwing\\arwing_.obj";
    char foxFileName[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Fox McCloud\\FoxMcCloud\\FoxMcCloud_.obj";
    char battlefieldFileName[150] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Super Mario 64 - Bob-omb Battlefield\\Bob-omb Battlefield\\bob_.obj";
    char wolfenFileName[100] = "C:\\Users\\Will\\Documents\\Nintendo 64 - Star Fox 64 - Wolfen\\Wolfen\\wolfen_.obj";
    char gloomFileName[100] = "C:\\Users\\Will\\Documents\\GameCube - Pokemon Colosseum - 044 - Gloom\\Gloom\\gloom_.obj";
    char krystalFileName[150] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Luigi\\Luigi\\Krystal_.obj";
    char peachFileName[100] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Peach\\Peach\\Peach_.obj";
    char marioMeleeFileName[150] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Mario\\Mario\\Mario_.obj";
    char marioGalaxyFileName[150] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Luigi\\Luigi\\mario_.obj";
    char pikachuFileName[100] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Pikachu\\Pikachu\\Pikachu_.obj";
    char roomFileName[100] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Luigi\\Luigi\\ECPrivateRoom_.obj";
    char naziFileName[100] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Luigi\\Luigi\\landtable_.obj";
    char sewerFileName[100] = "C:\\Users\\Will\\Documents\\GameCube - Super Smash Bros Melee - Luigi\\Luigi\\biff_.obj";

    /**Entity luigi = loadOBJ(luigiFileName, 0.05, 0.05, 0.05);
    printf("%d Polygons\n", luigi.numTris);

    objects[0] = luigi;
    objects[0].visible = 1;

    numObjects = 1;**/

    spawnObject(luigiFileName, 0.1, 0.1, 0.1, 0, 3.2, 10);
    spawnObject(castleFileName, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0);
    spawnObject(gloomFileName, 5, 5, 5, 0, 0, 0);

    destroyObject(0);

    //spawnObject(SNESArwingFileName, 1, 1, 1, 0, 0, 0);
    //spawnObject(arwingFileName, 4, 4, 4, 1, 1, 20);
    //objects[0] = formCube(0);
    //numObjects = 1;
    //spawnObject(luigiFileName, 0.1, 0.1, 0.1, 0, 3.2, 10);
    //destroyObject(1);

    for (int i = 0; i < numObjects; i++) { printf("Entity %d: %d Triangles/Polys\n", i, objects[i].numTris); }

    /**Entity obj = objects[0];
    for (int i = 0; i < obj.numTris; i++) {
        printf("Triangle %d\n", i);
        printf("   Vertex 1: x=%f y=%f z=%f\n", obj.tris[i].vert1.x, obj.tris[i].vert1.y, obj.tris[i].vert1.z);
        printf("   Vertex 2: x=%f y=%f z=%f\n", obj.tris[i].vert2.x, obj.tris[i].vert2.y, obj.tris[i].vert2.z);
        printf("   Vertex 3: x=%f y=%f z=%f\n", obj.tris[i].vert3.x, obj.tris[i].vert3.y, obj.tris[i].vert3.z);
    }**/

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Star Fox 3");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(handleKeyDown);
    glutKeyboardUpFunc(handleKeyUp);
    glutIdleFunc(gameLoop);
    initGL();
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}