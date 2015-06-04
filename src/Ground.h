#ifndef GROUND_H
#define GROUND_H

#include <GL/glut.h>
#include "Texture.h"

class Ground {
public:
    Ground() {}
    void initialize(float minX, float maxX,
                    float minZ, float maxZ, Texture *groundTexture);
    void render();
private:
    float minX;
    float maxX;
    float minZ;
    float maxZ;
    Texture *groundTexture;
    int listIdx;
};

#endif