#include <cmath>
#include "FVector.h"

float FVector3::length() const {
    return (float)(sqrt(x * x + y * y + z * z));
}

FVector3 FVector3::normalize() const {
    float len = length();
    if (fabs(len - 0.0f) < 1e-6)
        return FVector3(0.0f, 0.0f, 0.0f);
    else
        return FVector3(x / len, y / len, z / len);
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

float FVector3::operator* (const FVector3 &v) const {
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