#include "Skybox.h"

void Skybox::initialize(float minX, float maxX,
                        float minY, float maxY,
                        float minZ, float maxZ,
                        Texture *textures) {
    this->minX = minX;
    this->maxX = maxX;
    this->minY = minY;
    this->maxY = maxY;
    this->minZ = minZ;
    this->maxZ = maxZ;

    this->listIdx = glGenLists(1);
    glNewList(listIdx, GL_COMPILE);

    textures[SKY_RIGHT].bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(minX, minY, minZ);
    glTexCoord2f(1, 0); glVertex3f(maxX, minY, minZ);
    glTexCoord2f(1, 1); glVertex3f(maxX, maxY, minZ);
    glTexCoord2f(0, 1); glVertex3f(minX, maxY, minZ);
    glEnd();

    textures[SKY_FRONT].bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(maxX, minY, minZ);
    glTexCoord2f(1, 0); glVertex3f(maxX, minY, maxZ);
    glTexCoord2f(1, 1); glVertex3f(maxX, maxY, maxZ);
    glTexCoord2f(0, 1); glVertex3f(maxX, maxY, minZ);
    glEnd();

    textures[SKY_LEFT].bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(maxX, minY, maxZ);
    glTexCoord2f(1, 0); glVertex3f(minX, minY, maxZ);
    glTexCoord2f(1, 1); glVertex3f(minX, maxY, maxZ);
    glTexCoord2f(0, 1); glVertex3f(maxX, maxY, maxZ);
    glEnd();

    textures[SKY_BACK].bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(minX, minY, maxZ);
    glTexCoord2f(1, 0); glVertex3f(minX, minY, minZ);
    glTexCoord2f(1, 1); glVertex3f(minX, maxY, minZ);
    glTexCoord2f(0, 1); glVertex3f(minX, maxY, maxZ);
    glEnd();

    /* Top and Bottom */
    textures[SKY_UP].bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(minX, maxY, minZ);
    glTexCoord2f(1, 0); glVertex3f(maxX, maxY, minZ);
    glTexCoord2f(1, 1); glVertex3f(maxX, maxY, maxZ);
    glTexCoord2f(0, 1); glVertex3f(minX, maxY, maxZ);
    glEnd();

    /*textures[SKY_DOWN].bind();
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1); glVertex3f(maxX, minY, minZ);
    glTexCoord2f(0, 1); glVertex3f(minX, minY, minZ);
    glTexCoord2f(0, 0); glVertex3f(minX, minY, maxZ);
    glTexCoord2f(1, 0); glVertex3f(maxX, minY, maxZ);
    glEnd();*/

    glEndList();
}

void Skybox::render() {
    glCallList(listIdx);
}