#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/glut.h>
#include <FreeImage.h>

#define GL_CLAMP_TO_EDGE 0x812F

class Texture {
public:
    Texture() {}
    int getWidth() const;
    int getHeight() const;
    unsigned int getID() const;
    void load(char *filename, int repeat = GL_REPEAT, int type = BMP_DEFAULT);
    void bind();
private:
    int width, height;
    unsigned int ID;
};

#endif
