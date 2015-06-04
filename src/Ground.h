#ifndef GROUND_H
#define GROUND_H

#include <GL/glut.h>
#include "Texture.h"

class Ground {
public:
    Ground() {}
    void initialize(float minX, float maxX,
                    float minZ, float maxZ, Texture *groundTexture);
    void render() const;
private:
    float minX, maxX;
    float minZ, maxZ;
    Texture *groundTexture;
    int listIdx;  // index to the display list
};

#endif