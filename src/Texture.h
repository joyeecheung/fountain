#include <GL\glaux.h>

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    _AUX_RGBImageRec *im;
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
