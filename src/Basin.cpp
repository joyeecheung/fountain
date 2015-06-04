#include "Basin.h"

void Basin::initialize(const float height, const float borderWidth,
                const float innerSizeX, const float innerSizeZ,
                Texture *texture) {
    this->height = height;
    this->borderWidth = borderWidth;
    this->innerSizeX = innerSizeX;
    this->innerSizeZ = innerSizeZ;
    this->texBorder = borderWidth / (innerSizeX + 2 * borderWidth);
    this->texture = texture;

    this->listIdx = glGenLists(1);
    glNewList(listIdx, GL_COMPILE);
    glPushMatrix();
    texture->bind();
    glBegin(GL_QUADS);

    /******************
     * top surface
     ******************/
    glNormal3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(texBorder, texBorder);
    glVertex3f(0.0f, height, 0.0f);
    glTexCoord2f(1.0f - texBorder, texBorder);
    glVertex3f(innerSizeX, height, 0.0f);
    glTexCoord2f(1.0f - texBorder, 0.0f);
    glVertex3f(innerSizeX, height, -borderWidth);
    glTexCoord2f(texBorder, 0.0f);
    glVertex3f(0.0f, height, -borderWidth);

    glTexCoord2f(texBorder, 0.0f);
    glVertex3f(0.0f, height, -borderWidth);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-borderWidth, height, -borderWidth);
    glTexCoord2f(0.0f, 1.0f - texBorder);
    glVertex3f(-borderWidth, height, innerSizeZ);
    glTexCoord2f(texBorder, 1.0f - texBorder);
    glVertex3f(0.0f, height, innerSizeZ);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(innerSizeX + borderWidth, height, -borderWidth);
    glTexCoord2f(1.0f - texBorder, 0.0f);
    glVertex3f(innerSizeX, height, -borderWidth);
    glTexCoord2f(1.0f - texBorder, 1.0f - texBorder);
    glVertex3f(innerSizeX, height, innerSizeZ);
    glTexCoord2f(1.0f, 1.0f - texBorder);
    glVertex3f(innerSizeX + borderWidth, height, innerSizeZ);

    glTexCoord2f(1.0f, 1.0f - texBorder);
    glVertex3f(innerSizeX + borderWidth, height, innerSizeZ);
    glTexCoord2f(0.0f, 1.0f - texBorder);
    glVertex3f(-borderWidth, height, innerSizeZ);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-borderWidth, height, innerSizeZ + borderWidth);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(innerSizeX + borderWidth, height, innerSizeZ + borderWidth);

    /******************
     * front sides
     ******************/
    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(texBorder, texBorder);
    glVertex3f(0.0f, height, 0.0f);
    glTexCoord2f(1.0f - texBorder, texBorder);
    glVertex3f(innerSizeX, height, 0.0f);
    glTexCoord2f(1.0f - texBorder, 0.0f);
    glVertex3f(innerSizeX, 0.0f, 0.0f);
    glTexCoord2f(texBorder, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);

    glTexCoord2f(0.0f, 1.0f - texBorder);
    glVertex3f(-borderWidth, height, innerSizeZ + borderWidth);
    glTexCoord2f(1.0f, 1.0f - texBorder);
    glVertex3f(innerSizeX + borderWidth, height, innerSizeZ + borderWidth);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(innerSizeX + borderWidth, 0.0f, innerSizeZ + borderWidth);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-borderWidth, 0.0f, innerSizeZ + borderWidth);

    /******************
    * back sides
    ******************/
    glNormal3f(0.0f, 0.0f, -1.0f);

    glTexCoord2f(texBorder, texBorder);
    glVertex3f(0.0f, height, innerSizeZ);
    glTexCoord2f(1.0f - texBorder, texBorder);
    glVertex3f(innerSizeX, height, innerSizeZ);
    glTexCoord2f(1.0f - texBorder, 0.0f);
    glVertex3f(innerSizeX, 0.0f, innerSizeZ);
    glTexCoord2f(texBorder, 0.0f);
    glVertex3f(0.0f, 0.0f, innerSizeZ);

    glTexCoord2f(0.0f, 1.0f - texBorder);
    glVertex3f(-borderWidth, height, -borderWidth);
    glTexCoord2f(1.0f, 1.0f - texBorder);
    glVertex3f(innerSizeX + borderWidth, height, -borderWidth);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(innerSizeX + borderWidth, 0.0f, -borderWidth);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-borderWidth, 0.0f, -borderWidth);

    /******************
    *  left sides
    ******************/
    glNormal3f(-1.0f, 0.0f, 0.0f);

    glTexCoord2f(1.0f - texBorder, texBorder);
    glVertex3f(innerSizeX, height, 0.0f);
    glTexCoord2f(1.0f, 1.0f - texBorder);
    glVertex3f(innerSizeX, height, innerSizeZ);
    glTexCoord2f(1.0f - texBorder, 1.0f - texBorder);
    glVertex3f(innerSizeX, 0.0f, innerSizeZ);
    glTexCoord2f(1.0f, texBorder);
    glVertex3f(innerSizeX, 0.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-borderWidth, height, -borderWidth);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-borderWidth, height, innerSizeZ + borderWidth);
    glTexCoord2f(1.0f, texBorder);
    glVertex3f(-borderWidth, 0.0f, innerSizeZ + borderWidth);
    glTexCoord2f(0.0f, texBorder);
    glVertex3f(-borderWidth, 0.0f, -borderWidth);

    /******************
    *  right sides
    ******************/
    glNormal3f(1.0f, 0.0f, 0.0f);

    glTexCoord2f(1.0f - texBorder, 0.0f);
    glVertex3f(0.0f, height, innerSizeZ);
    glTexCoord2f(texBorder, 0.0f);
    glVertex3f(0.0f, height, 0.0f);
    glTexCoord2f(texBorder, texBorder);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f - texBorder, texBorder);
    glVertex3f(0.0f, 0.0f, innerSizeZ);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(innerSizeX + borderWidth, height, innerSizeZ + borderWidth);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(innerSizeX + borderWidth, height, -borderWidth);
    glTexCoord2f(1.0f - texBorder, 0.0f);
    glVertex3f(innerSizeX + borderWidth, 0.0f, -borderWidth);
    glTexCoord2f(1.0f - texBorder, 1.0f);
    glVertex3f(innerSizeX + borderWidth, 0.0f, innerSizeZ + borderWidth);

    glEnd();
    glPopMatrix();
    glEndList();
}

void Basin::render() {
    glCallList(listIdx);
}