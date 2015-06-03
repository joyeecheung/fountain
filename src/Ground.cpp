#include "Ground.h"
void Ground::initialize(float minX, float maxX,
                        float minZ, float maxZ,
                        Texture *groundTexture) {
    this->minX = minX;
    this->maxX = maxX;
    this->minZ = minZ;
    this->maxZ = maxZ;
    this->groundTexture = groundTexture;

    this->listIdx = glGenLists(1);

    glNewList(listIdx, GL_COMPILE);
    groundTexture->bind();
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(minX, 0.0, minZ);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(maxX, 0.0, minZ);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(maxX, 0.0, maxZ);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(minX, 0.0, maxZ);
    glEnd();
    glEndList();
}

void Ground::render() {
    glCallList(listIdx);
}