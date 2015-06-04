#ifndef POOL_H
#define POOL_H

#include <gl\glut.h>
#include <vector>
#include "FVector.h"
#include "Texture.h"

struct Oscillator {
    // NOTE: the positions of these fields can not be changed!!
    // The rendering of the pool depends on this layout!!
    GLfloat x, y, z;  // position
    GLfloat nx, ny, nz;  // normal
    GLfloat texX, texY;  // texture coordinates
    GLfloat speedY;
    GLfloat newY;
};

class Pool {
public:
    Pool() : oscillators(nullptr), indices(nullptr) {}

    Pool(int sizeX, int sizeZ, float height,
         float odistance, float oweight, float damping, float splash,
         float texRepeatX, float texRepeatZ,
         Texture *floorTexture)
         : oscillators(nullptr), indices(nullptr) {
        initialize(sizeX, sizeZ, height,
                   odistance, oweight, damping, splash,
                   texRepeatX, texRepeatZ,
                   floorTexture);
    }

    void initialize(int sizeX, int sizeZ, float height,
                    float odistance, float oweight,
                    float damping, float splash,
                    float texRepeatX, float texRepeatZ,
                    Texture *floorTexture);

    float getODistance() const {
        return oDistance;
    }

    void render() const;

    void updateOscillator(int posX, int posZ);
    void update(float deltaTime);    
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
    float oDistance, oWeight, splash, damping;
    int sizeX, sizeZ;
    float height;
    int indicesNum;
};

#endif