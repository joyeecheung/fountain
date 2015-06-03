#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include "FVector.h"
#include "Pool.h"

/**

  "Air fountain" is the description for the fountain's water in the air.
  The rest, which is most of the water, resists in the bowl.

  **/

class Fountain;

class Drop {
private:
    GLfloat time;  //How many steps the drop was "outside", when it falls into the water, time is set back to 0
    FVector3 constantSpeed;  //See the fountain doc for explanation of the physics
    GLfloat accFactor;
public:
    void setConstantSpeed(FVector3 newSpeed);
    void setAccFactor(GLfloat newAccFactor);
    void setTime(GLfloat newTime);
    void getNewPosition(FVector3 *positionVertex, float dtime, Pool * pPool, Fountain * pFountain);
};

class Fountain {
public:
    Fountain() : fountainVertices(nullptr), fountainDrops(nullptr) {}
    Fountain(GLint steps, GLint raysPerStep, GLint dropsPerRay,
             GLfloat stepAngleMin, GLfloat stepAngleMax,
             GLfloat randomAngle, GLfloat acceleration)
             : fountainVertices(nullptr), fountainDrops(nullptr) {}
    void initialize(GLint steps, GLint raysPerStep, GLint dropsPerRay,
                    GLfloat stepAngleMin,
                    GLfloat stepAngleMax,
                    GLfloat randomAngle,
                    GLfloat acceleration);
    FVector3 position;
    void render();
    void update(float dtime, Pool * pPool);
    GLint numDrops;
    ~Fountain() {
        delete [] fountainDrops;
        delete [] fountainVertices;
    }
private:
    FVector3 * fountainVertices;
    Drop * fountainDrops;
};

#endif