#include <GL/glut.h>

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "FVector.h"
#include "Texture.h"
#include "Camera.h"
#include "Basin.h"
#include "SkyBox.h"
#include "Ground.h"
#include "Pool.h"
#include "Fountain.h"

/***********************
 * Sky and ground Configuration
 ***********************/
const float SKY_BOX_SIZE = 30.0f;
const float GROUND_SIZE = 30.0f;

/***********************
 * Pool Configuration
 ***********************/
const int OSCILLATORS_NUM_X = 110;
const int OSCILLATORS_NUM_Z = 110;
const float OSCILLATOR_DISTANCE = 0.04f;
const float OSCILLATOR_WEIGHT = 0.00015f;
const float OSCILLATOR_SPLASH = -0.03f;
const float OSCILLATOR_DAMPING = 0.005f;
const float POOL_HEIGHT = 0.3f;
const float POOL_TEX_REPEAT_X = 2.0f;
const float POOL_TEX_REPEAT_Z = 2.0f;

/***********************
 * Basin Configuration
 ***********************/
const float BASIN_BORDER_WIDTH = 0.4f;
const float BASIN_BORDER_HEIGHT = 0.2f;
const float BASIN_INNER_X = (OSCILLATORS_NUM_X * OSCILLATOR_DISTANCE);
const float BASIN_INNER_Z = (OSCILLATORS_NUM_Z * OSCILLATOR_DISTANCE);

/***********************
 * Fountain Configuration
 ***********************/
const float DROP_SIZE = 4.0f;
FInitializer initializers[] = {
    FInitializer(4, 30, 30, DROP_SIZE, 75.0f, 90.0f, 0.2f, 0.10f),  // 1
    FInitializer(4, 30, 8, DROP_SIZE, 80.0f, 90.0f, 0.2f, 0.08f),  // 2
    FInitializer(2, 40, 10, DROP_SIZE, 50.0f, 90.0f, 1.5f, 0.13f), // 3
    FInitializer(3, 5, 100, DROP_SIZE, 75.0f, 90.0f, 0.4f, 0.07f), // 4
    FInitializer(3, 50, 35, DROP_SIZE, 30.0f, 90.0f, 0.2f, 0.15f), // 5
    FInitializer(1, 20, 60, DROP_SIZE, 50.0f, 60.0f, 5.0f, 0.13f), // 6
    FInitializer(6, 20, 30, DROP_SIZE, 90.0f, 90.0f, 1.0f, 0.12f), // 7
    FInitializer(2, 10, 60, DROP_SIZE, 73.0f, 85.0f, 6.0f, 0.08f)// 8
};

const float WATER_COLOR[] = { 0.9f, 0.9f, 0.9f, 0.4f };
const float TIME_DELTA = 0.001f;

/***********************
 * Lighting configuration
 ***********************/
const float LIGHT_AMBIENT_1[] = { 0.1f, 0.1f, 0.1f, 0.0f };
const float LIGHT_DIFFUSE_1[] = {
    211.0f / 255.0f, 183.0f / 255.0f, 133.0f / 255.0f, 0.0f
};
const float LIGHT_POSITION_1[] = { 0.8f, 0.4f, -0.5f, 0.0f };

const float LIGHT_AMBIENT_2[] = { 0.2f, 0.2f, 0.2f, 0.0f };
const float LIGHT_DIFFUSE_2[] = {
    211.0f / 255.0f, 183.0f / 255.0f, 133.0f / 255.0f, 0.0f
};
const float LIGHT_POSITION_2[] = { 0.8f, -0.2f, -0.5f, 0.0f };


/***********************
 * Camera Configuration
 ***********************/
const float MOVE_FACTOR = 0.1f;
const float ROTATE_FACTOR = 1.0f;
const FVector3 CAMERA_POSITION(BASIN_INNER_X / 2.0f, 1.8f,
                               BASIN_INNER_Z + 3.5f);
const FVector3 CAMERA_ROTATION(-5.0f, 0.0f);

/***********************
 * Viewport and Window Configuration
 ***********************/
const double FIELD_OF_VIEW = 45.0;
const double CLIP_NEAR = 1.0;
const double CLIP_FAR = 100.0;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;


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

bool isFullScreen = false;

void keyDown(unsigned char key, int x, int y) {
    switch (key) {
    /***************************
     * Camera controls
     ***************************/
    case 27:    //ESC
        exit(0);
        break;
    case 'f':
        if (!isFullScreen) {
            glutFullScreen();
            isFullScreen = true;
        } else {
            glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
            isFullScreen = false;
        }
        break;
    case 'r':
        camera.rotateX(ROTATE_FACTOR);
        break;
    case 'v':
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
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glEnable(GL_LIGHTING);
    // set up the scene
    pool.render();
    basin.render();
    ground.render();
    // sky
    glDisable(GL_LIGHTING);
    skybox.render();

    // water in the air
    glDisable(GL_TEXTURE_2D);
    glColor4fv(WATER_COLOR);
    fountain.render();
    glEnable(GL_LIGHTING);

    glDisable(GL_BLEND);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.render();

    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION_1);
    glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION_2);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    drawScene();
    glutSwapBuffers();
}

void reshape(int x, int y) {
    if (y == 0 || x == 0) return;  // invisible

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, (GLdouble)x / (GLdouble)y,
                   CLIP_NEAR, CLIP_FAR);
    glViewport(0, 0, x, y);
    glMatrixMode(GL_MODELVIEW);
}

void idle(void) {
    // update the fountain and the pool
    fountain.update(TIME_DELTA, pool);
    pool.update(TIME_DELTA);

    //render the scene:
    display();
}

int main(int argc, char **argv) {
    // initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Fountain");

    // textures
    Texture pebbleTexture;
    Texture basinTexture;
    Texture groundTexture;
    Texture skyTextures[6];

    pebbleTexture.load("resource/pebbles.bmp");
    basinTexture.load("resource/wall.bmp");
    groundTexture.load("resource/grass.bmp");

    skyTextures[SKY_FRONT].load("resource/skybox/front.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_RIGHT].load("resource/skybox/right.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_LEFT].load("resource/skybox/left.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_BACK].load("resource/skybox/back.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_UP].load("resource/skybox/up.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_DOWN].load("resource/skybox/down.bmp", GL_CLAMP_TO_EDGE);

    // initialize the scene
    skybox.initialize(-SKY_BOX_SIZE, SKY_BOX_SIZE,
                      -SKY_BOX_SIZE, SKY_BOX_SIZE,
                      -SKY_BOX_SIZE, SKY_BOX_SIZE, skyTextures);

    pool.initialize(OSCILLATORS_NUM_X, OSCILLATORS_NUM_Z, POOL_HEIGHT,
                    OSCILLATOR_DISTANCE, OSCILLATOR_WEIGHT,
                    OSCILLATOR_DAMPING, OSCILLATOR_SPLASH,
                    POOL_TEX_REPEAT_X, POOL_TEX_REPEAT_Z,
                    &pebbleTexture);

    fountain.initialize(initializers[0]);

    basin.initialize(BASIN_BORDER_HEIGHT + POOL_HEIGHT, BASIN_BORDER_WIDTH,
                     BASIN_INNER_X, BASIN_INNER_Z, &basinTexture);

    ground.initialize(-GROUND_SIZE, GROUND_SIZE,
                      -GROUND_SIZE, GROUND_SIZE, &groundTexture);

    // place the fountain in the center of the pool
    fountain.center = FVector3(BASIN_INNER_X / 2.0f, POOL_HEIGHT,
                               BASIN_INNER_Z / 2.0f);

    // initialize camera:
    camera.move(CAMERA_POSITION);
    camera.rotate(CAMERA_ROTATION);

    // enable vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // solid rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    // lighting
    glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT_AMBIENT_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT_DIFFUSE_1);
    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_POSITION_1);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, LIGHT_AMBIENT_2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, LIGHT_DIFFUSE_2);
    glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_POSITION_2);
    glEnable(GL_LIGHT2);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    // settings
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glFrontFace(GL_CCW);   // orientation should be the front face
    glShadeModel(GL_SMOOTH);

    // blending
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // seed
    srand((unsigned)time(NULL));

    printf("1 - 7:\tChange the shape of the fountain\n");
    printf("up, down:\tMove camera forward / backword\n");
    printf("left, right:\tTurn camera right / left\n");
    printf("r, v:\tTurn camera up / down\n");
    printf("w, s:\tMove camera up / down\n");
    printf("a, d:\tMove camera left / right\n");
    printf("ESC:\texit\n");

    // register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutSpecialFunc(spKeyDown);
    glutIdleFunc(idle);

    // start
    glutMainLoop();

    return 0;
}
