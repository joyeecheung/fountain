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

//lighting:
GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat LightDiffuse[] = { 0.8f, 0.8f, 0.8f, 0.0f };
GLfloat LightPosition[] = { 1.0f, -0.5f, -0.5f, 0.0f };

//Constants:
#define NUM_X_OSCILLATORS		170
#define NUM_Z_OSCILLATORS		170
#define OSCILLATOR_DISTANCE		0.020f
#define OSCILLATOR_WEIGHT       0.00008f

#define MAXX					(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE)
#define MAXZ					(NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE)

#define POOL_HEIGHT				0.3f


//Camera object:
Camera camera;

//The "pool" which represents the water within the fountain basin
Pool pool;

//water outside the basin is in the air:
Fountain fountain;

// The basin of the foutain
Basin basin;

//Textures:
Texture waterTexture;  //the image does not contain a water texture, 
//but it is applied to the water
Texture rockTexture;
Texture groundTexture;

bool  g_bRain = true;
bool  g_bFillModePoints = true;
bool  g_bLighting = true;

void keyDown(unsigned char key, int x, int y) {
    switch (key) {
        case 27:	//ESC
            exit(0);
            break;
        case 'a':
            camera.rotateY(5.0f);
            break;
        case 'd':
            camera.rotateY(-5.0f);
            break;
        case 'w':
            camera.moveForwards(-0.15f);
            break;
        case 's':
            camera.moveForwards(0.15f);
            break;
        case 'x':
            camera.rotateX(5.0f);
            break;
        case 'y':
            camera.rotateX(-5.0f);
            break;
        case 'c':
            camera.strafeRight(-0.05f);
            break;
        case 'v':
            camera.strafeRight(0.05f);
            break;
        case 'f':
            camera.move(FVector3(0.0f, -0.1f, 0.0f));
            break;
        case 'r':
            camera.move(FVector3(0.0f, 0.1f, 0.0f));
            break;

            //*************************************
            //Several initialization calls:
        case '1':
            pool.reset();
            fountain.destroy();
            fountain.initialize(3, 50, 25, 76.0f, 90.0f, 0.2f, 0.08f);
            break;
        case '2':
            pool.reset();
            fountain.destroy();
            fountain.initialize(1, 20, 100, 70.0f, 70.0f, 5.0f, 0.12f);
            break;
        case '3':
            pool.reset();
            fountain.initialize(1, 20, 200, 85.0f, 85.0f, 10.0f, 0.08f);
            break;
        case '4':
            pool.reset();
            fountain.initialize(5, 20, 85, 90.0f, 90.0f, 1.0f, 0.12f);
            break;
        case '5':
            pool.reset();
            fountain.initialize(2, 20, 50, 40.0f, 70.0f, 1.5f, 0.15f);
            break;
        case '6':
            pool.reset();
            fountain.initialize(3, 50, 25, 76.0f, 90.0f, 0.2f, 0.10f);
            break;
        case '7':
            pool.reset();
            fountain.initialize(4, 100, 45, 76.0f, 90.0f, 0.2f, 0.10f);
            break;
    }
}

void renderGround(float minX, float maxX, float minZ, float maxZ) {
    //******************
    //ground
    //******************
    groundTexture.setActive();
    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(minX, 0.0, minZ);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(maxX, 0.0, minZ);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(maxX, 0.0, maxZ);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(minX, 0.0, maxZ);
    glEnd();
}

void drawScene(void) {

    //Render the pool
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    waterTexture.setActive();
    glPushMatrix();
    glTranslatef(0.0f, POOL_HEIGHT, 0.0f);
    pool.render();
    glPopMatrix();

    //Render the basin
    basin.render();
    
    // render the ground
    renderGround(-20.0f, 20.0f, -20.0f, 20.0f);

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

    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
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
    float dtime = 0.003f;
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
    glutCreateWindow("Fountain with simulated water");

    //Load the textures:
    waterTexture.load("resource/pebbles.bmp");
    rockTexture.load("resource/wall.bmp");
    groundTexture.load("resource/grass.bmp");

    //compute the vertices and indices
    pool.initialize(NUM_X_OSCILLATORS, NUM_Z_OSCILLATORS,
                    OSCILLATOR_DISTANCE, OSCILLATOR_WEIGHT,
                    0.03f, 4.0f, 4.0f);
    //init the airfountain: (look at KeyDown() to see more possibilities of initialization)
    fountain.initialize(4, 100, 45, 76.0f, 90.0f, 0.2f, 0.10f);
    basin.initialize(0.2f + POOL_HEIGHT, 0.4f, MAXX, MAXZ, &rockTexture);

    //place it in the center of the pool:
    fountain.position = FVector3(MAXX / 2.0f,
                                 POOL_HEIGHT,
                                 MAXZ / 2.0f);
    //initialize camera: 
    camera.move(FVector3(MAXX / 2.0f, 2.0f, MAXX + 5.0f));
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
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    //Some general settings:
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glFrontFace(GL_CCW);   //Tell OGL which orientation shall be the front face
    glShadeModel(GL_SMOOTH);

    //Initialize blending:
    glEnable(GL_BLEND);
    glPointSize(3.0);
    glEnable(GL_POINT_SMOOTH);
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

    //Clean up:
    fountain.destroy();

    return 0;
}
