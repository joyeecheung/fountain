#ifndef GROUND_H
#define GROUND_H

#include <memory>
#include <GL/glut.h>

#include "Texture.h"

class Ground {
public:
    Ground() {}
    void initialize(float minX, float maxX,
                    float minZ, float maxZ,
                    std::unique_ptr<Texture> groundTexture, float texRepeat);
    void render() const;
private:
    float minX, maxX;
    float minZ, maxZ;
    int listIdx;  // index to the display list
};

#endif
