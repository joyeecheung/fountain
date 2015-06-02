#ifndef BASIN_H
#define BASIN_H

#include <gl\glut.h>
#include "Texture.h"

class Basin {
public:
    Basin() {};
    void initialize(const float height, const float borderWidth,
          const float innerSizeX, const float innerSizeZ,
          Texture *texture) {
        this->height = height;
        this->borderWidth = borderWidth;
        this->innerSizeX = innerSizeX;
        this->innerSizeZ = innerSizeZ;
        this->texBorder = borderWidth / (innerSizeX + 2 * borderWidth);
        this->texture = texture;
    }
    void render();
private:
    float height;
    float borderWidth;
    float innerSizeX;
    float innerSizeZ;
    float texBorder;
    Texture *texture;
};

#endif