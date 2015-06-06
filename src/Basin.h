#ifndef BASIN_H
#define BASIN_H

#include <memory>
#include <GL/glut.h>

#include "Texture.h"

class Basin {
public:
    Basin() {};
    void initialize(const float height, const float borderWidth,
                    const float innerSizeX, const float innerSizeZ,
                    std::unique_ptr<Texture> texture);
    void render();
private:
    float height;
    float borderWidth;
    float innerSizeX;
    float innerSizeZ;
    float texBorder;
    int listIdx;
};

#endif