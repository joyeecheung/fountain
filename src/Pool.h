#ifndef POOL_H
#define POOL_H

#include <gl\glut.h>
#include <vector>
#include "FVector.h"
#include "Texture.h"

struct Oscillator {
    GLfloat x, y, z;  // position
    GLfloat nx, ny, nz;  // normal
    GLfloat texX, texY;  // texture coordinates
    GLfloat speedY;
    GLfloat newY;
};

class Pool {
public:
    Pool() : oscillators(nullptr), indices(nullptr) {}
    Pool(int sizeX, int sizeZ, float odistance,
         float oweight, float damping,
         float textureStretchX, float textureStretchZ,
         Texture *floorTexture)
         : oscillators(nullptr), indices(nullptr) {
        initialize(sizeX, sizeZ, odistance,
                   oweight, damping,
                   textureStretchX, textureStretchZ,
                   floorTexture);
    }
    /* Oscillator distance */
    float getODistance();
    void initialize(int sizeX, int sizeZ, float odistance,
                    float oweight, float damping,
                    float textureStretchX, float textureStretchZ,
                    Texture *floorTexture);
    void updateOscillator(int posX, int posZ, float deltaY);
    void update(float deltaTime);
    void render();
    void reset();
    ~Pool() {
        delete [] oscillators;
        delete [] indices;
    }
private:
    // vertex data for the waves
    Oscillator * oscillators;
    GLuint *indices;
    Texture *floorTexture;
    int oscillatorsNum;  // sizeX * sizeZ
    float oDistance;
    float oWeight;

    int sizeX, sizeZ;
    int indicesNum;
    float damping;
};

#endif