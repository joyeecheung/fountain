#include <GL/glut.h>
#include <memory>
#include <type_traits>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Texture.h"
#include "Camera.h"
#include "Basin.h"
#include "SkyBox.h"
#include "Ground.h"
#include "Pool.h"
#include "Fountain.h"
#include "Dragger.h"

/***********************
 * Sky and ground Configuration
 ***********************/
const float SKY_BOX_SIZE = 30.0f;
const float GROUND_SIZE = 30.0f;
const float GROUND_TEX_REPEAT = 4.0f;

/***********************
 * Pool Configuration
 ***********************/
const int OSCILLATORS_NUM_X = 110;
const int OSCILLATORS_NUM_Z = 110;
const float OSCILLATOR_DISTANCE = 0.04f;
const float OSCILLATOR_WEIGHT = 0.0002f;
const float OSCILLATOR_SPLASH = -0.015f;
const float OSCILLATOR_DAMPING = 0.005f;
const float POOL_HEIGHT = 0.3f;
const float POOL_TEX_REPEAT_X = 3.0f;
const float POOL_TEX_REPEAT_Z = 3.0f;

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
    FInitializer(4, 30, 8, DROP_SIZE, 80.0f, 90.0f, 0.2f, 0.08f),   // 2
    FInitializer(2, 40, 10, DROP_SIZE, 50.0f, 90.0f, 1.5f, 0.13f),  // 3
    FInitializer(3, 5, 100, DROP_SIZE, 75.0f, 90.0f, 0.4f, 0.07f),  // 4
    FInitializer(3, 50, 35, DROP_SIZE, 30.0f, 90.0f, 0.2f, 0.15f),  // 5
    FInitializer(1, 20, 60, DROP_SIZE, 50.0f, 60.0f, 5.0f, 0.13f),  // 6
    FInitializer(6, 20, 30, DROP_SIZE, 90.0f, 90.0f, 1.0f, 0.12f),  // 7
    FInitializer(2, 10, 60, DROP_SIZE, 73.0f, 85.0f, 6.0f, 0.08f)   // 8
};

const float WATER_COLOR[] = { 0.9f, 0.9f, 0.9f, 0.4f };
const float TIME_DELTA = 0.004f;
const int FPS = 60;

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
const float CAMERA_POS[] = {
    BASIN_INNER_X / 2.0f, 1.8f, BASIN_INNER_Z + 3.5f
};
const float CAMERA_Y[] = {
    0, 1, 0
};
const float CAMERA_Z[] = {
    0, 0, -1
};

const float MOVE_SPEED = 0.1f;
const float ROTATE_SPEED = 1.0f;
const float ROTATE_FACTOR = 0.25f;
const float CAMERA_POSITION[] = {
    BASIN_INNER_X / 2.0f, 1.8f, BASIN_INNER_Z + 3.5f
};
const float CAMERA_ROTATION[] = {
    -5.0f, 0.0f, 0.0f
};

/***********************
 * Viewport and Window Configuration
 ***********************/
const double FIELD_OF_VIEW = 45.0;
const double CLIP_NEAR = 1.0;
const double CLIP_FAR = 100.0;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;
int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;


/***********************
 * Objects in the scene
 ***********************/
Dragger dragger;

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
bool mouseControl = false;

int mouseX = 0, mouseY = 0;
void mouseMove(int x, int y) {
    if (mouseControl) {
        camera.rotateX((mouseY - y) * ROTATE_FACTOR);
        camera.rotateY((mouseX - x) * ROTATE_FACTOR);
    }
    mouseX = x;
    mouseY = y;
}

void keyDown(unsigned char key, int x, int y) {
    switch (key) {
    case 27:  // ESC
        exit(0);
        break;
    case 'f':  // full screen
        if (!isFullScreen) {
            glutFullScreen();
            isFullScreen = true;
            glutWarpPointer(mouseX, mouseY);
        } else {
            glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
            isFullScreen = false;
            glutWarpPointer(mouseX, mouseY);
        }
        break;
    case 'c':
        mouseControl = !mouseControl;
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

    if (mouseControl)
        return;

    /***************************
     * Camera controls
     ***************************/
    switch (key) {
        case 'r':
            camera.rotateX(ROTATE_SPEED);
            break;
        case 'v':
            camera.rotateX(-ROTATE_SPEED);
            break;
        case 'a':
            camera.moveX(-MOVE_SPEED);
            break;
        case 'd':
            camera.moveX(MOVE_SPEED);
            break;
        case 's':
            camera.moveY(-MOVE_SPEED);
            break;
        case 'w':
            camera.moveY(MOVE_SPEED);
            break;
    }
}

void spKeyDown(int key, int x, int y) {
    if (mouseControl) return;

    switch (key) {
        /***************************
        * Camera controls
        ***************************/
        case GLUT_KEY_LEFT:
            camera.rotateY(ROTATE_SPEED);
            break;
        case GLUT_KEY_RIGHT:
            camera.rotateY(-ROTATE_SPEED);
            break;
        case GLUT_KEY_UP:
            camera.moveZ(-MOVE_SPEED);
            break;
        case GLUT_KEY_DOWN:
            camera.moveZ(MOVE_SPEED);
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

void renderBitmapString(float x, float y, float z,
                        void *font, const char *string) {
    const char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void vecToString(char *buffer, const char *format, FVector3 vec) {
    sprintf(buffer, format, vec.x, vec.y, vec.z);
}

int lastTime = 0;
int thisTime = 0;
void showText() {
    char frameInfo[30], positionInfo[50],
        rotationInfo[50], directionInfo[50];

    sprintf(frameInfo, "fps: %4.2f", 1000.0 / (thisTime - lastTime));
    camera.updateDirection();
    vecToString(positionInfo,
                "Camera position %3.2f, %3.2f, %3.2f", camera.getPosition());
    vecToString(rotationInfo,
                "Camera rotation %3.2f, %3.2f, %3.2f", camera.getRotation());
    vecToString(directionInfo,
                "Camera direction %3.2f, %3.2f, %3.2f", camera.getDirection());

    // draw status text
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 200, 0, 200);
    glMatrixMode(GL_MODELVIEW);

    // render the string
    renderBitmapString(5, 10, 0.0, GLUT_BITMAP_TIMES_ROMAN_24, frameInfo);
    renderBitmapString(5, 160, 0.0, GLUT_BITMAP_TIMES_ROMAN_24, positionInfo);
    renderBitmapString(5, 170, 0.0, GLUT_BITMAP_TIMES_ROMAN_24, rotationInfo);
    renderBitmapString(5, 180, 0.0, GLUT_BITMAP_TIMES_ROMAN_24, directionInfo);

    if (mouseControl)
        renderBitmapString(160, 180, 0.0, GLUT_BITMAP_TIMES_ROMAN_24, "Mouse Mode");
    else
        renderBitmapString(160, 180, 0.0, GLUT_BITMAP_TIMES_ROMAN_24, "Keyboard Mode");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.render();

    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_POSITION_1);
    glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_POSITION_2);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    drawScene();
    showText();
    glutSwapBuffers();
}

void idle(void) {
    thisTime = glutGet(GLUT_ELAPSED_TIME);
    if (thisTime - lastTime > 1000 / FPS) {
        // update the fountain and the pool
        fountain.update(TIME_DELTA, pool);
        pool.update(TIME_DELTA);

        //render the scene:
        display();
        lastTime = thisTime;
    }
}

void reshape(int x, int y) {
    windowWidth = x;
    windowHeight = y;
    if (y == 0 || x == 0) return;  // invisible

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, (GLdouble)x / (GLdouble)y,
                   CLIP_NEAR, CLIP_FAR);
    glViewport(0, 0, x, y);
    glMatrixMode(GL_MODELVIEW);
}

void mouseButton(int button, int state, int x, int y) {
    // scroll up
    if (button == 3) {
        camera.moveZ(-MOVE_SPEED);
    // scroll down
    } else if (button == 4) {
        camera.moveZ(MOVE_SPEED);
    }
}

int main(int argc, char **argv) {
    static_assert(std::is_pod<FVector3>::value, "FVector must be a POD!");
    static_assert(std::is_pod<Oscillator>::value, "Oscillator must be a POD!");

    // initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Fountain");

    // textures
    std::unique_ptr<Texture> pebbleTexture(new Texture);
    std::unique_ptr<Texture> basinTexture(new Texture);
    std::unique_ptr<Texture> groundTexture(new Texture);
    std::unique_ptr<Texture[]> skyTextures(new Texture[SKY_BOX_FACES]);

    pebbleTexture->load("resource/pebbles.bmp");
    basinTexture->load("resource/wall.bmp");
    groundTexture->load("resource/grass.bmp");

    skyTextures[SKY_FRONT].load("resource/skybox/front.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_RIGHT].load("resource/skybox/right.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_LEFT].load("resource/skybox/left.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_BACK].load("resource/skybox/back.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_UP].load("resource/skybox/up.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_DOWN].load("resource/skybox/down.bmp", GL_CLAMP_TO_EDGE);

    // initialize the scene
    skybox.initialize(-SKY_BOX_SIZE, SKY_BOX_SIZE,
                      -SKY_BOX_SIZE, SKY_BOX_SIZE,
                      -SKY_BOX_SIZE, SKY_BOX_SIZE, std::move(skyTextures));

    pool.initialize(OSCILLATORS_NUM_X, OSCILLATORS_NUM_Z, POOL_HEIGHT,
                    OSCILLATOR_DISTANCE, OSCILLATOR_WEIGHT,
                    OSCILLATOR_DAMPING, OSCILLATOR_SPLASH,
                    POOL_TEX_REPEAT_X, POOL_TEX_REPEAT_Z,
                    std::move(pebbleTexture));

    fountain.initialize(initializers[0]);

    basin.initialize(BASIN_BORDER_HEIGHT + POOL_HEIGHT, BASIN_BORDER_WIDTH,
                     BASIN_INNER_X, BASIN_INNER_Z, std::move(basinTexture));

    ground.initialize(-GROUND_SIZE, GROUND_SIZE,
                      -GROUND_SIZE, GROUND_SIZE,
                      std::move(groundTexture), GROUND_TEX_REPEAT);

    // place the fountain in the center of the pool
    fountain.center.set(BASIN_INNER_X / 2.0f, POOL_HEIGHT, BASIN_INNER_Z / 2.0f);

    // initialize camera:
    FVector3 cposition, crotation;
    cposition.set(CAMERA_POSITION[0], CAMERA_POSITION[1], CAMERA_POSITION[2]);
    camera.move(cposition);
    crotation.set(CAMERA_ROTATION[0], CAMERA_ROTATION[1], CAMERA_ROTATION[2]);
    camera.rotate(crotation);

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

    printf("1 - 8:\tChange the shape of the fountain\n");
    printf("f:\tToggle fullscreen\n");
    printf("c:\tSwitch between mouse mode / keyboard mode\n");

    printf("----------- Keyboard Mode -----------------\n");
    printf("up, down:\tMove camera forward / backword\n");
    printf("left, right:\tTurn camera right / left\n");
    printf("r, v:\tTurn camera up / down\n");
    printf("w, s:\tMove camera up / down\n");
    printf("a, d:\tMove camera left / right\n");

    printf("----------- Mouse Mode -----------------\n");
    printf("Mouse move:\tRotate camera\n");
    printf("Mouse scroll:\tMove forward / backward\n");

    printf("ESC:\tExit\n");

    // register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutPassiveMotionFunc(mouseMove);
    glutSpecialFunc(spKeyDown);
    glutMouseFunc(mouseButton);
    glutIdleFunc(idle);

    // hide cursor
    glutSetCursor(GLUT_CURSOR_NONE);

    // start
    glutMainLoop();

    return 0;
}
