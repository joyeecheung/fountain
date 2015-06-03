#include <GL\glut.h>

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "Pool.h"
#include "Fountain.h"
#include "Basin.h"
#include "Camera.h"  //This is my old camera, but it's easier to control 
//for the user and the third rotation axis is not required here
#include "Texture.h"
#include "SkyBox.h"
#include "Ground.h"

//lighting:
GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 0.0f };
GLfloat lightPosition[] = {0.8f, 0.4f, -0.5f, 0.0f };

GLfloat lightAmbient2[] = { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat lightDiffuse2[] = { 0.3f, 0.3f, 0.3f, 0.0f };
GLfloat lightPosition2[] = { 0.8f, -0.2f, -0.5f, 0.0f };

//Constants:
const int NUM_X_OSCILLATORS = 180;
const int NUM_Z_OSCILLATORS = 180;
const float OSCILLATOR_DISTANCE = 0.020f;
const float OSCILLATOR_WEIGHT = 0.00005f;
const float MAXX = (NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE);
const float MAXZ = (NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE);
const float POOL_HEIGHT = 0.2f;
const float MOVE_FACTOR = 0.1f;
const float ROTATE_FACTOR = 1.0f;

FountainInitializer initializers[] = {
    FountainInitializer(4, 100, 45, 76.0f, 90.0f, 0.2f, 0.09f),  // 1
    FountainInitializer(4, 100, 8, 80.0f, 90.0f, 0.2f, 0.08f),  // 2
    FountainInitializer(2, 70, 40, 40.0f, 85.0f, 1.5f, 0.13f), // 3
    FountainInitializer(3, 5, 200, 75.0f, 90.0f, 0.4f, 0.07f), // 4
    FountainInitializer(3, 100, 45, 20.0f, 90.0f, 0.2f, 0.15f), // 5
    FountainInitializer(1, 20, 100, 50.0f, 60.0f, 5.0f, 0.13f), // 6
    FountainInitializer(6, 20, 90, 90.0f, 90.0f, 1.0f, 0.12f), // 7
    FountainInitializer(2, 30, 200, 85.0f, 85.0f, 10.0f, 0.08f)// 8
};

//Camera object:
Camera camera;

//The "pool" which represents the water within the fountain basin
Pool pool;

//water outside the basin is in the air:
Fountain fountain;

// The basin of the foutain
Basin basin;

Skybox skybox;

Ground ground;

bool  g_bRain = true;
bool  g_bFillModePoints = true;
bool  g_bLighting = true;

void keyDown(unsigned char key, int x, int y) {
    switch (key) {
        case 27:	//ESC
            exit(0);
            break;
        case 'a':
            camera.rotateY(ROTATE_FACTOR);
            break;
        case 'd':
            camera.rotateY(-ROTATE_FACTOR);
            break;
        case 'w':
            camera.moveZ(-MOVE_FACTOR);
            break;
        case 's':
            camera.moveZ(MOVE_FACTOR);
            break;
        case 'x':
            camera.rotateX(ROTATE_FACTOR);
            break;
        case 'y':
            camera.rotateX(-ROTATE_FACTOR);
            break;
        case 'c':
            camera.moveX(-MOVE_FACTOR);
            break;
        case 'v':
            camera.moveX(MOVE_FACTOR);
            break;
        case 'f':
            camera.move(FVector3(0.0f, -MOVE_FACTOR, 0.0f));
            break;
        case 'r':
            camera.move(FVector3(0.0f, MOVE_FACTOR, 0.0f));
            break;

            //*************************************
            //Several initialization calls:
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            pool.reset();
            fountain.initialize(initializers[key - '0' - 1]);
            break;
    }
}

void drawScene(void) {

    //Render the pool
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(0.0f, POOL_HEIGHT, 0.0f);
    pool.render();
    glPopMatrix();

    //Render the basin
    basin.render();
    
    // render the ground
    ground.render();
    skybox.render();
    glDisable(GL_TEXTURE_2D);

    //Render the water in the air.
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glColor4f(0.8f, 0.8f, 0.8f, 0.8f);
    fountain.render();

    glDisable(GL_BLEND);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.render();

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    //Turn two sided lighting on:
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    drawScene();
    glFlush();			//Finish rendering
    glutSwapBuffers();
}

void reshape(int x, int y) {
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.3
    //Far clipping plane distance: 50.0
    gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.3, 50.0);
    glViewport(0, 0, x, y);  //Use the whole window for rendering
    glMatrixMode(GL_MODELVIEW);

}

void idle(void) {
    //Do the physical calculation for one step:
    float dtime = 0.002f;
    fountain.update(dtime, &pool);
    pool.update(dtime);

    //render the scene:
    display();
}

int main(int argc, char **argv) {
    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);
    //Create a window with rendering context and everything else we need
    glutCreateWindow("Fountain");

    //Textures:
    Texture waterTexture;  //the image does not contain a water texture, 
    //but it is applied to the water
    Texture rockTexture;
    Texture groundTexture;
    Texture skyboxTextures[5];

    //Load the textures:
    waterTexture.load("resource/pebbles.bmp");
    rockTexture.load("resource/wall.bmp");
    groundTexture.load("resource/grass.bmp");

    skyboxTextures[SKY_FRONT].load("resource/skybox/front.bmp", GL_CLAMP_TO_EDGE);
    skyboxTextures[SKY_RIGHT].load("resource/skybox/right.bmp", GL_CLAMP_TO_EDGE);
    skyboxTextures[SKY_LEFT].load("resource/skybox/left.bmp", GL_CLAMP_TO_EDGE);
    skyboxTextures[SKY_BACK].load("resource/skybox/back.bmp", GL_CLAMP_TO_EDGE);
    skyboxTextures[SKY_UP].load("resource/skybox/up.bmp", GL_CLAMP_TO_EDGE);
    skyboxTextures[SKY_DOWN].load("resource/skybox/down.bmp", GL_CLAMP_TO_EDGE);

    skybox.initialize(-20.0f, 20.0f, -20.0f, 20.0f, -20.0f, 20.0f, skyboxTextures);
    //compute the vertices and indices
    pool.initialize(NUM_X_OSCILLATORS, NUM_Z_OSCILLATORS,
                    OSCILLATOR_DISTANCE, OSCILLATOR_WEIGHT,
                    0.03f, 4.0f, 4.0f, &waterTexture);
    //init the airfountain: (look at KeyDown() to see more possibilities of initialization)
    fountain.initialize(initializers[0]);
    basin.initialize(0.2f + POOL_HEIGHT, 0.4f, MAXX, MAXZ, &rockTexture);
    ground.initialize(-20.0f, 20.0f, -20.0f, 20.0f, &groundTexture);

    //place it in the center of the pool:
    fountain.position = FVector3(MAXX / 2.0f,
                                 POOL_HEIGHT,
                                 MAXZ / 2.0f);
    //initialize camera: 
    camera.move(FVector3(MAXX / 2.0f, 1.8f, MAXZ + 3.5f));
    camera.rotateY(0);
    camera.rotateX(-5);

    //Enable the vertex array functionality:
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //Switch on solid rendering:
    g_bFillModePoints = false;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    //Initialize lighting:
    g_bLighting = true;
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
    glEnable(GL_LIGHT2);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    //Some general settings:
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glFrontFace(GL_CCW);   //Tell OGL which orientation shall be the front face
    glShadeModel(GL_SMOOTH);

    //Initialize blending:
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //initialize generation of random numbers:
    srand((unsigned)time(NULL));

    printf("1-7:\tChange the shape of the fountain\n");
    printf("w, s:\tMove camera forward/backword\n");
    printf("a, d:\tturn camera right / left\n");
    printf("r, f:\tmove camera up / down\n");
    printf("x, y:\tturn camera up / down\n");
    printf("c, v:\tstrafe left / right\n");
    printf("esc:\texit\n");

    //Assign the two used Msg-routines
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutIdleFunc(idle);
    //Let GLUT get the msgs
    glutMainLoop();

    return 0;
}
