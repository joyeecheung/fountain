#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glut.h>
#include "Texture.h"

#define SKY_FRONT 0
#define SKY_RIGHT 1
#define SKY_LEFT 2
#define SKY_BACK 3
#define SKY_UP 4
#define SKY_DOWN 5

class Skybox {
public:
    Skybox() {}
    void initialize(float minX, float maxX,
                    float minY, float maxY,
                    float minZ, float maxZ,
                    Texture * textures);
    void render() const;
private:
    float minX, maxX;  // dimension on x-axis
    float minY, maxY;  // dimension on y-axis
    float minZ, maxZ;  // dimension on z-axis
    Texture *textures;  // skybox textures
    int listIdx;  // index to the display list
};

#endif