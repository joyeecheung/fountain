#include "Ground.h"

void Ground::render() {
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
}