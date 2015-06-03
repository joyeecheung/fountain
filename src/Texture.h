#ifndef TEXTURE_H
#define TEXTURE_H

#include <gl/glut.h>
#include <FreeImage.h>

class Texture {
public:
    Texture() {}
    int getWidth() const;
    int getHeight() const;
    unsigned int getID() const;
    void load(char *filename);
    void bind();
private:
    int width, height;
    unsigned int ID;
};

#endif
