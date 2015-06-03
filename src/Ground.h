#ifndef GROUND_H
#define GROUND_H

#include <gl\glut.h>
#include "Texture.h"

class Ground {
public:
    Ground() {}
    void initialize(float minX, float maxX,
                    float minZ, float maxZ, Texture *groundTexture) {
        this->minX = minX;
        this->maxX = maxX;
        this->minZ = minZ;
        this->maxZ = maxZ;
        this->groundTexture = groundTexture;
    }
    void render();
private:
    float minX;
    float maxX;
    float minZ;
    float maxZ;
    Texture *groundTexture;
};

#endif