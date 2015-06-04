#include <GL\glut.h>

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "Pool.h"
#include "Fountain.h"
#include "Basin.h"
#include "Camera.h"
#include "Texture.h"
#include "SkyBox.h"
#include "Ground.h"

/***********************
* Sky and ground Configuration
***********************/
const float SKY_BOX_SIZE = 20.0f;
const float GROUND_SIZE = 20.0f;

/***********************
 * Pool Configuration
 ***********************/
const int NUM_X_OSCILLATORS = 180;
const int NUM_Z_OSCILLATORS = 180;
const float OSCILLATOR_DISTANCE = 0.020f;
const float OSCILLATOR_WEIGHT = 0.00005f;
const float POOL_SIZE_X = (NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE);
const float POOL_SIZE_Z = (NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE);
const float POOL_HEIGHT = 0.2f;
const float DAMPING = 0.015f;
const float POOL_TEX_REPEAT_X = 2.0f;
const float POOL_TEX_REPEAT_Z = 2.0f;

/***********************
* Basin Configuration
***********************/
const float BASIN_BORDER_WIDTH = 0.4f;
const float BASIN_BORDER_HEIGHT = 0.2f;

/***********************
* Fountain Configuration
***********************/
FountainInitializer initializers[] = {
    FountainInitializer(4, 100, 45, 76.0f, 90.0f, 0.2f, 0.09f),  // 1
    FountainInitializer(4, 100, 8, 80.0f, 90.0f, 0.2f, 0.08f),  // 2
    FountainInitializer(2, 70, 40, 40.0f, 90.0f, 1.5f, 0.13f), // 3
    FountainInitializer(3, 5, 200, 75.0f, 90.0f, 0.4f, 0.07f), // 4
    FountainInitializer(3, 100, 45, 30.0f, 90.0f, 0.2f, 0.15f), // 5
    FountainInitializer(1, 20, 100, 50.0f, 60.0f, 5.0f, 0.13f), // 6
    FountainInitializer(6, 20, 90, 90.0f, 90.0f, 1.0f, 0.12f), // 7
    FountainInitializer(2, 30, 200, 85.0f, 85.0f, 10.0f, 0.08f)// 8
};

/***********************
* Lighting configuration
***********************/
GLfloat lightAmbient1[] = { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat lightDiffuse1[] = { 0.8f, 0.8f, 0.8f, 0.0f };
GLfloat lightPosition1[] = { 0.8f, 0.4f, -0.5f, 0.0f };

GLfloat lightAmbient2[] = { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat lightDiffuse2[] = { 0.3f, 0.3f, 0.3f, 0.0f };
GLfloat lightPosition2[] = { 0.8f, -0.2f, -0.5f, 0.0f };


/***********************
 * Camera Configuration
 ***********************/
const float MOVE_FACTOR = 0.1f;
const float ROTATE_FACTOR = 1.0f;
const FVector3 CAMERA_POSITION(POOL_SIZE_X / 2.0f, 1.8f, POOL_SIZE_Z + 3.5f);
const FVector3 CAMERA_ROTATION(-5.0f, 0.0f);

/***********************
 * Objects in the scene
 ***********************/
Camera camera;

// Water and the floor in the basin
Pool pool;

// Water in the air
Fountain fountain;

// Basin of the fountain
Basin basin;

// Sky
Skybox skybox;

// The ground
Ground ground;


void keyDown(unsigned char key, int x, int y) {
    switch (key) {
        /***************************
         * Camera controls
         ***************************/
        case 27:	//ESC
            exit(0);
            break;
        case 'r':
            camera.rotateX(ROTATE_FACTOR);
            break;
        case 'f':
            camera.rotateX(-ROTATE_FACTOR);
            break;
        case 'a':
            camera.moveX(-MOVE_FACTOR);
            break;
        case 'd':
            camera.moveX(MOVE_FACTOR);
            break;
        case 's':
            camera.moveY(-MOVE_FACTOR);
            break;
        case 'w':
            camera.moveY(MOVE_FACTOR);
            break;

        /***************************
         * Fountain shape control
         ***************************/
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

void spKeyDown(int key, int x, int y) {
    switch (key) {
        /***************************
        * Camera controls
        ***************************/
        case GLUT_KEY_LEFT:
            camera.rotateY(ROTATE_FACTOR);
            break;
        case GLUT_KEY_RIGHT:
            camera.rotateY(-ROTATE_FACTOR);
            break;
        case GLUT_KEY_UP:
            camera.moveZ(-MOVE_FACTOR);
            break;
        case GLUT_KEY_DOWN:
            camera.moveZ(MOVE_FACTOR);
            break;
    }
}

void drawScene(void) {

    // render the pool
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

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
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

    skybox.initialize(-SKY_BOX_SIZE, SKY_BOX_SIZE,
                      -SKY_BOX_SIZE, SKY_BOX_SIZE,
                      -SKY_BOX_SIZE, SKY_BOX_SIZE, skyboxTextures);

    pool.initialize(NUM_X_OSCILLATORS, NUM_Z_OSCILLATORS,
                    OSCILLATOR_DISTANCE, OSCILLATOR_WEIGHT,
                    DAMPING, POOL_TEX_REPEAT_X, POOL_TEX_REPEAT_Z,
                    &waterTexture);

    fountain.initialize(initializers[0]);

    basin.initialize(BASIN_BORDER_HEIGHT + POOL_HEIGHT, BASIN_BORDER_WIDTH,
                     POOL_SIZE_X, POOL_SIZE_Z, &rockTexture);

    ground.initialize(-GROUND_SIZE, GROUND_SIZE,
                      -GROUND_SIZE, GROUND_SIZE, &groundTexture);

    // place the fountain in the center of the pool
    fountain.center = FVector3(POOL_SIZE_X / 2.0f, POOL_HEIGHT, POOL_SIZE_Z / 2.0f);

    //initialize camera: 
    camera.move(FVector3(POOL_SIZE_X / 2.0f, 1.8f, POOL_SIZE_Z + 3.5f));
    camera.rotateY(0);
    camera.rotateX(-5);

    //Enable the vertex array functionality:
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //Switch on solid rendering:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    //Initialize lighting:
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
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

    printf("1 - 7:\tChange the shape of the fountain\n");
    printf("up, down:\tMove camera forward / backword\n");
    printf("left, right:\tTurn camera right / left\n");
    printf("r, f:\tTurn camera up / down\n");
    printf("w, s:\tMove camera up / down\n");
    printf("a, d:\tMove camera left / right\n");
    printf("ESC:\texit\n");

    //Assign the two used Msg-routines
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutSpecialFunc(spKeyDown);
    glutIdleFunc(idle);
    //Let GLUT get the msgs
    glutMainLoop();

    return 0;
}
