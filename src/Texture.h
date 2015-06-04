#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <FreeImage.h>

// this should have been in gl/glext.h
#define GL_CLAMP_TO_EDGE 0x812F

class Texture {
public:
    Texture() {}
    int getWidth() const;
    int getHeight() const;
    unsigned int getID() const;
    // load from file
    void load(char *filename, int repeat = GL_REPEAT, int type = BMP_DEFAULT);
    // bind to the context
    void bind();
private:
    int width, height;
    unsigned int ID;
};

#endif
