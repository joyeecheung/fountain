#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#include <vector>
#include "Vector3.hpp"
#include "Pool.h"

// fountain initializers
class FInitializer {
public:
    FInitializer(int levels, int raysPerStep, int dropsPerRay,
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
    void updatePosition(FVector3 & position, float dtime,
                        Pool & pool, Fountain & fountain);
private:
    float time;  // time after the drop take off
    FVector3 speed;
    float acceleration;
};

class Fountain {
public:
    /*************** constructors  *************/
    // default
    Fountain() {}
    // explicit arguments
    Fountain(int levels, int raysPerStep, int dropsPerRay,
             float dropSize, float angleMin, float angleMax,
             float randomAngle, float acceleration){
        initialize(levels, raysPerStep, dropsPerRay,
                   dropSize, angleMin, angleMax,
                   randomAngle, acceleration);
    }

    /*************** initializers *****************/
    // explicit arguments
    void initialize(int levels, int raysPerStep, int dropsPerRay,
                    float dropSize, float angleMin, float angleMax,
                    float randomAngle, float acceleration);

    // use initializer
    void initialize(FInitializer init) {
        initialize(init.levels, init.raysPerStep, init.dropsPerRay,
                   init.dropSize, init.angleMin, init.angleMax,
                   init.randomAngle, init.acceleration);
    }

    // render the fountain
    void render() const;
    // update the pool with moved drops
    void update(float dtime, Pool & pool);

    FVector3 center;  // center of this fountain
private:
    int numDrops;  // number of drops
    float dropSize;  // size of drops
    std::vector<FVector3> dropPositions;  // positions of the drops this fountain emits
    std::vector<Drop> drops;  // drop info(speed, accelerations, etc.)
};

#endif
