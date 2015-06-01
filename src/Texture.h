#include <gl/glut.h>

#include <windows.h>
#include <FreeImage.h>

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    void load(char *filename);
    void setActive();
    int getWidth() const;
    int getHeight() const;
    unsigned int getID() const;
private:
    int width, height;
    unsigned int ID;
};

#endif
