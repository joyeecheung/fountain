#include "Texture.h"

void Texture::load(char *filename) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    FIBITMAP *bitmap = FreeImage_Load(FIF_BMP, filename, BMP_DEFAULT);
    width = FreeImage_GetWidth(bitmap);
    height = FreeImage_GetHeight(bitmap);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
                      3,
                      width,
                      height,
                      GL_RGB,
                      GL_UNSIGNED_BYTE,
                      FreeImage_GetBits(bitmap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void Texture::setActive() {
    glBindTexture(GL_TEXTURE_2D, ID);
}

unsigned int Texture::getID() const {
    return ID;
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}

