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
    /*************** constructors  *************/
    // default
    Pool() : oscillators(nullptr), indices(nullptr) {}
    // explicit arguments
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

    /*************** initializers *****************/
    // explicit arguments
    void initialize(int sizeX, int sizeZ, float height,
                    float odistance, float oweight,
                    float damping, float splash,
                    float texRepeatX, float texRepeatZ,
                    Texture *floorTexture);

    // get the distance of the oscillators
    float getODistance() const {
        return oDistance;
    }

    // render the pool
    void render() const;

    // update the oscillator at a position
    // this should be caused by a drop
    void splashOscillator(int posX, int posZ);
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