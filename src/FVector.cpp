#include <cmath>
#include "FVector.h"

GLfloat FVector3::length() const {
    return (GLfloat)(sqrt(x * x + y * y + z * z));
}

FVector3 FVector3::normalize() const {
    float l = length();
    if (fabs(l - 0.0f) < 1e-6)
        return FVector3(0.0f, 0.0f, 0.0f);
    else
        return FVector3(x / l, y / l, z / l);
}

FVector3 FVector3::cross(const FVector3 &v) const {
    return FVector3(y * v.z - z * v.y,
                    z * v.x - x * v.z,
                    x * v.y - y * v.x);
}

FVector3 FVector3::operator+ (const FVector3 &v) const {
    return FVector3(x + v.x, y + v.y, z + v.z);
}

FVector3 FVector3::operator- (const FVector3 &v) const {
    return FVector3(x - v.x, y - v.y, z - v.z);
}

GLfloat FVector3::operator* (const FVector3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

FVector3 FVector3::operator* (float r) const {
    return FVector3(x * r, y * r, z * r);
}

FVector3 FVector3::operator/ (float r) const {
    return FVector3(x / r, y / r, z / r);
}

FVector3 & FVector3::operator= (const FVector3 &other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

void FVector3::addBy(const FVector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
}