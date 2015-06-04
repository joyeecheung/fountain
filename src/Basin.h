#ifndef BASIN_H
#define BASIN_H

#include <GL/glut.h>
#include "Texture.h"

class Basin {
public:
    Basin() {};
    void initialize(const float height, const float borderWidth,
                    const float innerSizeX, const float innerSizeZ,
                    Texture *texture);
    void render();
private:
    float height;
    float borderWidth;
    float innerSizeX;
    float innerSizeZ;
    float texBorder;
    Texture *texture;
    int listIdx;
};

#endif