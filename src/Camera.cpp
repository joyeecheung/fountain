#include <cmath>

#include "windows.h"
#include "Camera.h"

void Camera::getViewDir(void) {
    //Rotate around Y-axis:
    FVector3 step1(cos((rotatedY + 90.0) * PIdiv180),
                   0.0f,
                   -sin((rotatedY + 90.0) * PIdiv180));
    //Rotate around X-axis:
    double cosX = cos(rotatedX * PIdiv180);
    FVector3 step2(step1.x * cosX, sin(rotatedX * PIdiv180), step1.z * cosX);

    //Rotation around Z-axis not yet implemented, so:
    viewDir = step2;
}

void Camera::move(FVector3 dir) {
    position.addBy(dir);
}

void Camera::rotateY(GLfloat angle) {
    rotatedY += angle;
    viewDirChanged = true;
}

void Camera::rotateX(GLfloat angle) {
    rotatedX += angle;
    viewDirChanged = true;
}

void Camera::render(void) {
    glRotatef(-rotatedX, 1.0, 0.0, 0.0);
    glRotatef(-rotatedY, 0.0, 1.0, 0.0);
    glRotatef(-rotatedZ, 0.0, 0.0, 1.0);
    glTranslatef(-position.x, -position.y, -position.z);
}

void Camera::moveForwards(GLfloat distance) {
    if (viewDirChanged) getViewDir();
    FVector3 moveVector(viewDir.x * -distance,
                        viewDir.y * -distance,
                        viewDir.z * -distance);
    position.addBy(moveVector);
}

void Camera::strafeRight(GLfloat distance) {
    if (viewDirChanged) getViewDir();
    FVector3 moveVector(viewDir.z * -distance,
                        0.0f,
                        -viewDir.x * -distance);
    position.addBy(moveVector);
}