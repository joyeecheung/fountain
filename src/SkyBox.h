#ifndef SKYBOX_H
#define SKYBOX_H

#include <gl/glut.h>
#include "Texture.h"

#define SKY_FRONT 0
#define SKY_RIGHT 1
#define SKY_LEFT 2
#define SKY_BACK 3
#define SKY_UP 4
//#define SKY_DOWN 5

class Skybox {
public:
    Skybox() {}
    void initialize(float minX, float maxX,
                    float minY, float maxY,
                    float minZ, float maxZ,
                    Texture * textures);
    void render();
private:
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
    Texture *textures;
    GLuint listIdx;
};

#endif