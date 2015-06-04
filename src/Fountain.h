#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include "FVector.h"
#include "Pool.h"

class FountainInitializer {
public:
    FountainInitializer(int levels, int raysPerStep, int dropsPerRay,
                        float dropSize, float angleMin, float angleMax,
                        float randomAngle, float acceleration) {
        this->levels = levels;
        this->raysPerStep = raysPerStep;
        this->dropsPerRay = dropsPerRay;
        this->dropSize = dropSize;
        this->angleMin = angleMin;
        this->angleMax = angleMax;
        this->randomAngle = randomAngle;
        this->acceleration = acceleration;
    }
    int levels;
    int raysPerStep;
    int dropsPerRay;
    float dropSize;
    float angleMin;
    float angleMax;
    float randomAngle;
    float acceleration;
};

class Fountain;

class Drop {
public:
    void setSpeed(FVector3 newSpeed);
    void setAcceleration(float newAcc);
    void setTime(float newTime);
    void updatePosition(FVector3 & positionVertex, float dtime,
                        Pool * pool, Fountain * fountain);
private:
    float time;  //How many steps the drop was "outside", when it falls into the water, time is set back to 0
    FVector3 speed;  //See the fountain doc for explanation of the physics
    float acc;
};

class Fountain {
public:
    Fountain() : vertices(nullptr), drops(nullptr) {}

    Fountain(int levels, int raysPerStep, int dropsPerRay,
             float dropSize, float angleMin, float angleMax,
             float randomAngle, float acceleration)
             : vertices(nullptr), drops(nullptr) {
        initialize(levels, raysPerStep, dropsPerRay, dropSize, angleMin, angleMax,
                   randomAngle, acceleration);
    }

    void initialize(int levels, int raysPerStep, int dropsPerRay,
                    float dropSize, float angleMin, float angleMax,
                    float randomAngle, float acceleration);

    void initialize(FountainInitializer init) {
        initialize(init.levels, init.raysPerStep, init.dropsPerRay,
                   init.dropSize, init.angleMin, init.angleMax,
                   init.randomAngle, init.acceleration);
    }
    
    void render();
    void update(float dtime, Pool * pPool);
    ~Fountain() {
        delete [] drops;
        delete [] vertices;
    }

    int numDrops;
    FVector3 center;
private:
    float dropSize;
    FVector3 * vertices;
    Drop * drops;
};

#endif