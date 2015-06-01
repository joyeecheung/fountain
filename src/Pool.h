#ifndef POOL_H
#define POOL_H

#include <gl\glut.h>

struct Oscillator {
    GLfloat x, y, z;     //position
    GLfloat nx, ny, nz;  //normal vector
    GLfloat u, v;	   //texture coordinates

    GLfloat upSpeed;
    GLfloat newY;
};

class Pool {
protected:
    //vertex data for the waves:
    Oscillator * oscillators;
    GLuint * indices;

    int numOscillators;  //x*z
    int xSize, zSize;

    int numIndices;

    float oscillatorDistance;
    float oscillatorWeight;
    float damping;

public:
    float getOscillatorDistance();
    void initialize(int xSize, int zSize, float oscillatorDistance,
                    float oscillatorWeight, float damping,
                    float textureStretchX, float textureStretchZ);
    void reset();
    void affectOscillator(int xPos, int zPos, float deltaY);
    void update(float deltaTime);
    void render();
};

#endif