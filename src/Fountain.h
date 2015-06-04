#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include "FVector.h"
#include "Pool.h"

class FountainInitializer {
public:
    FountainInitializer(GLint levels, GLint raysPerStep, GLint dropsPerRay,
                        GLfloat angleMin, GLfloat angleMax,
                        GLfloat randomAngle, GLfloat acceleration) {
        this->levels = levels;
        this->raysPerStep = raysPerStep;
        this->dropsPerRay = dropsPerRay;
        this->angleMin = angleMin;
        this->angleMax = angleMax;
        this->randomAngle = randomAngle;
        this->acceleration = acceleration;
    }
    GLint levels;
    GLint raysPerStep;
    GLint dropsPerRay;
    GLfloat angleMin;
    GLfloat angleMax;
    GLfloat randomAngle;
    GLfloat acceleration;
};

class Fountain;

class Drop {
public:
    void setSpeed(FVector3 newSpeed);
    void setAcceleration(GLfloat newAcc);
    void setTime(GLfloat newTime);
    void updatePosition(FVector3 & positionVertex, float dtime,
                        Pool * pool, Fountain * fountain);
private:
    GLfloat time;  //How many steps the drop was "outside", when it falls into the water, time is set back to 0
    FVector3 speed;  //See the fountain doc for explanation of the physics
    GLfloat acc;
};

class Fountain {
public:
    Fountain() : vertices(nullptr), drops(nullptr) {}

    Fountain(GLint levels, GLint raysPerStep, GLint dropsPerRay,
             GLfloat angleMin, GLfloat angleMax,
             GLfloat randomAngle, GLfloat acceleration)
             : vertices(nullptr), drops(nullptr) {
        initialize(levels, raysPerStep, dropsPerRay, angleMin, angleMax,
                   randomAngle, acceleration);
    }

    void initialize(GLint levels, GLint raysPerStep, GLint dropsPerRay,
                    GLfloat angleMin, GLfloat angleMax,
                    GLfloat randomAngle, GLfloat acceleration);

    void initialize(FountainInitializer init) {
        initialize(init.levels, init.raysPerStep, init.dropsPerRay, init.angleMin,
                   init.angleMax, init.randomAngle, init.acceleration);
    }
    
    void render();
    void update(float dtime, Pool * pPool);
    ~Fountain() {
        delete [] drops;
        delete [] vertices;
    }

    GLint numDrops;
    FVector3 center;
private:
    FVector3 * vertices;
    Drop * drops;
};

#endif