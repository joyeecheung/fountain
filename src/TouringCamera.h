#ifndef TOURING_CAMERA_H
#define TOURING_CAMERA_H

#include "Vector3.hpp"
#include "Vector2.hpp"

class TouringCamera {
public:
    void setViewport(int viewportWidth, int viewportHeight);
    void init(FVector3 pos, FVector3 up, FVector3 forward,
              float movingSpeed, float rotatingSpeed,
              float viewportWidth, float viewportHeight);
    void update(const IVector2 &mousePos);
    void look() const;
    void accel(float acceleration);
    void stop();

private:
    FVector3 pos;
    FVector3 up;
    FVector3 x;
    FVector3 y;
    FVector3 z;
    float movingSpeed;
    float rotatingSpeed;
    float viewportWidth;
    float viewportHeight;

    void rotate(const FVector2 &dragVector);
};

#endif
