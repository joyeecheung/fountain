#ifndef POOL_H
#define POOL_H

#include <gl\glut.h>
#include <vector>
#include "FVector.h"

struct Oscillator {
    GLfloat x, y, z;  // position
    GLfloat nx, ny, nz;  // normal
    GLfloat texX, texY;  // texture coordinates
    GLfloat speedY;
    GLfloat newY;
};

class Pool {
public:
    /* Oscillator distance */
    float getODistance();
    void initialize(int sizeX, int sizeZ, float odistance,
                    float oweight, float damping,
                    float textureStretchX, float textureStretchZ);
    void updateOscillator(int posX, int posZ, float deltaY);
    void update(float deltaTime);
    void render();
    void reset();
    void destroy();
private:
    // vertex data for the waves
    Oscillator * oscillators;
    GLuint *indices;

    int oscillatorsNum;  // sizeX * sizeZ
    float oDistance;
    float oWeight;

    int sizeX, sizeZ;
    int indicesNum;
    float damping;
};

#endif