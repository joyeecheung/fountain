#include <cmath>

#include "windows.h"
#include "Camera.h"

void Camera::move(const FVector3 &dir) {
    position.addBy(dir);
}

void Camera::rotate(const FVector3 &dir) {
    rotation.addBy(dir);
}

void Camera::moveX(GLfloat delta) {
    if (directionChanged)
        updateDirection();
    FVector3 moveVector(direction.z * -delta,
                        0.0f,
                        -direction.x * -delta);
    position.addBy(moveVector);
}

void Camera::moveY(GLfloat delta) {
    if (directionChanged)
        updateDirection();
    position.y += delta;
}

void Camera::moveZ(GLfloat delta) {
    if (directionChanged)
        updateDirection();
    FVector3 moveVector(direction.x * -delta,
                        direction.y * -delta,
                        direction.z * -delta);
    position.addBy(moveVector);
}


void Camera::rotateX(GLfloat angle) {
    rotation.x += angle;
    directionChanged = true;
}

void Camera::rotateY(GLfloat angle) {
    rotation.y += angle;
    directionChanged = true;
}

void Camera::render(void) const {
    glRotatef(-rotation.x, 1.0, 0.0, 0.0);
    glRotatef(-rotation.y, 0.0, 1.0, 0.0);
    glRotatef(-rotation.z, 0.0, 0.0, 1.0);
    glTranslatef(-position.x, -position.y, -position.z);
}

void Camera::updateDirection(void) {
    // rotate around Y-axis
    FVector3 step1(cos((rotation.y + 90.0) * PIdiv180),
                   0.0f,
                   -sin((rotation.y + 90.0) * PIdiv180));
    // rotate around X-axis:
    float cosX = cos(rotation.x * PIdiv180);
    FVector3 step2(step1.x * cosX, sin(rotation.x * PIdiv180), step1.z * cosX);
    // TODO: rotate around Z-axis
    direction = step2;
    directionChanged = false;
}