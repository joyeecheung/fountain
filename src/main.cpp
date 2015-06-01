#include <GL\glut.h>

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>


#include "Pool.h"
#include "Fountain.h"
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
#define OSCILLATOR_DISTANCE		0.025
#define OSCILLATOR_WEIGHT       0.0001

#define MAXX					(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE)
#define MAXZ					(NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE)

#define POOL_HEIGHT				0.3


//Camera object:
Camera camera;

//The "pool" which represents the water within the fountain bowl
Pool pool;

//water outside the bowl is in the air:
Fountain fountain;

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
            camera.move(FVector3(0.0, -0.1, 0.0));
            break;
        case 'r':
            camera.move(FVector3(0.0, 0.1, 0.0));
            break;

            //*************************************
            //Several initialization calls:
        case '1':
            pool.reset();
            fountain.destroy();
            fountain.initialize(3, 8, 35, 76, 90, 0.5, 0.11);
            break;
        case '2':
            pool.reset();
            fountain.destroy();
            fountain.initialize(1, 20, 100, 70, 70, 5.0, 0.15);
            break;
        case '3':
            pool.reset();
            fountain.initialize(1, 20, 200, 85, 85, 10, 0.1);
            break;
        case '4':
            pool.reset();
            fountain.initialize(5, 20, 85, 90, 90, 1.0, 0.15);
            break;
        case '5':
            pool.reset();
            fountain.initialize(2, 20, 50, 40, 70, 1.5, 0.2);
            break;
        case '6':
            pool.reset();
            fountain.initialize(3, 50, 25, 76, 90, 0.2, 0.11);
            break;
        case '7':
            pool.reset();
            fountain.initialize(4, 100, 45, 76, 90, 0.2, 0.11);
            break;


    }
}
void RenderBowl(void) {
    float bowlheight = 0.2 + POOL_HEIGHT;
    float bowlwidth = 0.4;

    //******************
    //ground
    //******************
    float texBorderDistance = bowlwidth / (MAXX + 2 * bowlwidth);
    groundTexture.setActive();
    glBegin(GL_QUADS);

    float minX = -15.0;
    float minZ = -15.0;
    float maxX = 20.0;
    float maxZ = 20.0;

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

    //******************
    //bowl
    //******************

    rockTexture.setActive();

    glBegin(GL_QUADS);

    //******************
    //top
    //******************
    glNormal3f(0.0f, 1.0f, 0.0);

    glTexCoord2f(texBorderDistance, texBorderDistance);
    glVertex3f(0.0f, bowlheight, 0.0);
    glTexCoord2f(1.0 - texBorderDistance, texBorderDistance);
    glVertex3f(MAXX, bowlheight, 0.0);
    glTexCoord2f(1.0 - texBorderDistance, 0.0);
    glVertex3f(MAXX, bowlheight, -bowlwidth);
    glTexCoord2f(texBorderDistance, 0.0);
    glVertex3f(0.0f, bowlheight, -bowlwidth);

    glTexCoord2f(texBorderDistance, 0.0);
    glVertex3f(0.0f, bowlheight, -bowlwidth);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-bowlwidth, bowlheight, -bowlwidth);
    glTexCoord2f(0.0, 1.0 - texBorderDistance);
    glVertex3f(-bowlwidth, bowlheight, MAXZ);
    glTexCoord2f(texBorderDistance, 1.0 - texBorderDistance);
    glVertex3f(0.0f, bowlheight, MAXZ);

    glTexCoord2f(1.0, 0.0);
    glVertex3f(MAXX + bowlwidth, bowlheight, -bowlwidth);
    glTexCoord2f(1.0 - texBorderDistance, 0.0);
    glVertex3f(MAXX, bowlheight, -bowlwidth);
    glTexCoord2f(1.0 - texBorderDistance, 1.0 - texBorderDistance);
    glVertex3f(MAXX, bowlheight, MAXZ);
    glTexCoord2f(1.0, 1.0 - texBorderDistance);
    glVertex3f(MAXX + bowlwidth, bowlheight, MAXZ);

    glTexCoord2f(1.0, 1.0 - texBorderDistance);
    glVertex3f(MAXX + bowlwidth, bowlheight, MAXZ);
    glTexCoord2f(0.0, 1.0 - texBorderDistance);
    glVertex3f(-bowlwidth, bowlheight, MAXZ);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-bowlwidth, bowlheight, MAXZ + bowlwidth);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(MAXX + bowlwidth, bowlheight, MAXZ + bowlwidth);

    //******************
    //front
    //******************
    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(texBorderDistance, texBorderDistance);
    glVertex3f(0.0f, bowlheight, 0.0);
    glTexCoord2f(1.0 - texBorderDistance, texBorderDistance);
    glVertex3f(MAXX, bowlheight, 0.0);
    glTexCoord2f(1.0 - texBorderDistance, 0.0);
    glVertex3f(MAXX, 0.0f, 0.0);
    glTexCoord2f(texBorderDistance, 0.0);
    glVertex3f(0.0f, 0.0f, 0.0);


    glTexCoord2f(0.0, 1.0 - texBorderDistance);
    glVertex3f(-bowlwidth, bowlheight, MAXZ + bowlwidth);
    glTexCoord2f(1.0, 1.0 - texBorderDistance);
    glVertex3f(MAXX + bowlwidth, bowlheight, MAXZ + bowlwidth);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(MAXX + bowlwidth, 0.0f, MAXZ + bowlwidth);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-bowlwidth, 0.0f, MAXZ + bowlwidth);


    //******************
    //back
    //******************
    glNormal3f(0.0, 0.0, -1.0f);

    glTexCoord2f(texBorderDistance, texBorderDistance);
    glVertex3f(0.0f, bowlheight, MAXZ);
    glTexCoord2f(1.0 - texBorderDistance, texBorderDistance);
    glVertex3f(MAXX, bowlheight, MAXZ);
    glTexCoord2f(1.0 - texBorderDistance, 0.0);
    glVertex3f(MAXX, 0.0f, MAXZ);
    glTexCoord2f(texBorderDistance, 0.0);
    glVertex3f(0.0f, 0.0f, MAXZ);

    glTexCoord2f(0.0, 1.0 - texBorderDistance);
    glVertex3f(-bowlwidth, bowlheight, -bowlwidth);
    glTexCoord2f(1.0, 1.0 - texBorderDistance);
    glVertex3f(MAXX + bowlwidth, bowlheight, -bowlwidth);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(MAXX + bowlwidth, 0.0f, -bowlwidth);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-bowlwidth, 0.0f, -bowlwidth);


    //******************
    //side
    //******************
    glNormal3f(-1.0, 0.0, 0.0);

    glTexCoord2f(1.0 - texBorderDistance, texBorderDistance);
    glVertex3f(MAXX, bowlheight, 0.0);
    glTexCoord2f(1.0, 1.0 - texBorderDistance);
    glVertex3f(MAXX, bowlheight, MAXZ);
    glTexCoord2f(1.0 - texBorderDistance, 1.0 - texBorderDistance);
    glVertex3f(MAXX, 0.0f, MAXZ);
    glTexCoord2f(1.0, texBorderDistance);
    glVertex3f(MAXX, 0.0f, 0.0);


    glTexCoord2f(0.0, 0.0);
    glVertex3f(-bowlwidth, bowlheight, -bowlwidth);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-bowlwidth, bowlheight, MAXZ + bowlwidth);
    glTexCoord2f(1.0, texBorderDistance);
    glVertex3f(-bowlwidth, 0.0f, MAXZ + bowlwidth);
    glTexCoord2f(0.0, texBorderDistance);
    glVertex3f(-bowlwidth, 0.0f, -bowlwidth);

    glNormal3f(1.0, 0.0, 0.0);

    glTexCoord2f(1.0 - texBorderDistance, 0.0);
    glVertex3f(0.0f, bowlheight, MAXZ);
    glTexCoord2f(texBorderDistance, 0.0);
    glVertex3f(0.0f, bowlheight, 0.0);
    glTexCoord2f(texBorderDistance, texBorderDistance);
    glVertex3f(0.0f, 0.0f, 0.0);
    glTexCoord2f(1.0 - texBorderDistance, texBorderDistance);
    glVertex3f(0.0f, 0.0f, MAXZ);

    glTexCoord2f(1.0, 1.0);
    glVertex3f(MAXX + bowlwidth, bowlheight, MAXZ + bowlwidth);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(MAXX + bowlwidth, bowlheight, -bowlwidth);
    glTexCoord2f(1.0 - texBorderDistance, 0.0);
    glVertex3f(MAXX + bowlwidth, 0.0f, -bowlwidth);
    glTexCoord2f(1.0 - texBorderDistance, 1.0);
    glVertex3f(MAXX + bowlwidth, 0.0f, MAXZ + bowlwidth);

    glEnd();
}

void DrawScene(void) {

    //Render the pool
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    waterTexture.setActive();
    glPushMatrix();
    glTranslatef(0.0, POOL_HEIGHT, 0.0);
    pool.render();
    glPopMatrix();

    //Render the bowl
    RenderBowl();

    glDisable(GL_TEXTURE_2D);

    //Render the water in the air.
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glColor4f(0.8, 0.8, 0.8, 0.8);
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

    DrawScene();
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
    float dtime = 0.006;
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
    //compute the vertices and indices
    pool.initialize(NUM_X_OSCILLATORS, NUM_Z_OSCILLATORS, OSCILLATOR_DISTANCE, OSCILLATOR_WEIGHT, 0.05, 4.0, 4.0);
    //init the airfountain: (look at KeyDown() to see more possibilities of initialization)
    fountain.initialize(5, 70, 40, 70, 90, 0.2, 0.11);


    //place it in the center of the pool:
    fountain.position = FVector3(MAXX / 2.0f,
                                 POOL_HEIGHT,
                                 MAXZ / 2.0f);
    //initialize camera: 
    camera.move(FVector3(MAXX / 2, 2.0, MAXX + 5.0));
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

    //Load the textures:
    waterTexture.load("resource/pebbles.bmp");
    rockTexture.load("resource/wall.bmp");
    groundTexture.load("resource/grass.bmp");

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
