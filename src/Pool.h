#ifndef POOL_H
#define POOL_H

#include <memory>
#include <GL/glut.h>

#include "FVector.h"
#include "Texture.h"

struct Oscillator {
    // NOTE: the positions of these fields can not be changed!!
    // The rendering of the pool depends on this layout!!
    float x, y, z;  // position
    float nx, ny, nz;  // normal
    float texX, texY;  // texture coordinates
    float speedY;  // speed along the y direction
    float newY;  // for storing calculated y temperorily
};

class Pool {
public:
    /*************** constructors  *************/
    // default
    Pool() : oscillators(nullptr), indices(nullptr) {}
    // explicit arguments
    Pool(int oNumX, int oNumZ, float height,
         float oDistance, float oWeight, float damping, float splash,
         float texRepeatX, float texRepeatZ,
         std::unique_ptr<Texture> floorTexture)
        : oscillators(nullptr), indices(nullptr) {
        initialize(oNumX, oNumZ, height,
                   oDistance, oWeight, damping, splash,
                   texRepeatX, texRepeatZ,
                   std::move(floorTexture));
    }

    /*************** initializers *****************/
    // explicit arguments
    void initialize(int oNumX, int oNumZ, float height,
                    float oDistance, float oWeight,
                    float damping, float splash,
                    float texRepeatX, float texRepeatZ,
                    std::unique_ptr<Texture> floorTexture);

    // get the distance of the oscillators
    float getODistance() const {
        return oDistance;
    }

    // render the pool
    void render() const;

    // update the oscillator at a position
    // this should be caused by a drop
    void splashOscillator(int idxX, int idxZ);
    void update(float deltaTime);
    void reset();

    ~Pool() {
        delete [] oscillators;
        delete [] indices;
    }
private:
    Oscillator * oscillators;  // oscillators for simulating the waves
    int *indices;  // indices for drawing
    std::unique_ptr<Texture> floorTexture;  // texture for the floor of the pool

    float oDistance; // distance between oscillators
    float oWeight;  //  weight of oscillators
    int oNumX, oNumZ;  // number of oscillators in the pool
    int oNum;  // sizeX * sizeZ
    float splash, damping;
    float height;  // height of the pool
    int indicesNum;  // number of indices, roughly 2 * oscillatorsNum
};

#endif