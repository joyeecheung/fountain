#include "TouringCamera.h"
#include "Vector3.tpp"
#include "Vector2.tpp"

#include <GL/glut.h>

void TouringCamera::setViewport(int viewportWidth, int viewportHeight) {
    this->viewportWidth = viewportWidth;
    this->viewportHeight = viewportHeight;
}

void TouringCamera::init(FVector3 pos, FVector3 up, FVector3 forward,
                         float movingSpeed, float rotatingSpeed,
                         float viewportWidth, float viewportHeight) {
    this->pos = pos;
    this->up = up;
    this->z = forward.normalize();
    this->x = forward.cross(up).normalize();
    this->y = x.cross(forward).normalize();
    this->movingSpeed = movingSpeed;
    this->rotatingSpeed = rotatingSpeed;
    setViewport(viewportWidth, viewportHeight);
}

void TouringCamera::update(const IVector2 &mousePos) {
    // go ahead
    pos.addBy(z * movingSpeed);
    // rotate
    FVector2 dragVector;
    dragVector.x = mousePos.x / (viewportWidth / 2) - 1;
    dragVector.y = -(mousePos.y / (viewportHeight / 2) - 1);  // reverse y
    rotate(dragVector * rotatingSpeed);
}

void TouringCamera::rotate(const FVector2 &dragVector) {
    z.addBy(x * dragVector.x);
    z.addBy(y * dragVector.y);
    x = z.cross(up).normalize(),
    y = x.cross(z).normalize();
}

void TouringCamera::look() const {
    FVector3 target = pos + z;
    gluLookAt(pos.x, pos.y, pos.z,
              target.x, target.y, target.z,
              y.x, y.y, y.z);
}

void TouringCamera::accel(float acceleration) {
    movingSpeed += acceleration;
}

void TouringCamera::stop() {
    movingSpeed = 0;
}
