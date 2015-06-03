#ifndef GROUND_H
#define GROUND_H

#include <gl\glut.h>
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
    GLuint listIdx;
};

#endif