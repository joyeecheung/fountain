#include "Texture.h"

void Texture::load(char *filename, int repeat, int type) {
    // set up
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // load the image and get the metadata
    FIBITMAP *bitmap = FreeImage_Load(FIF_BMP, filename, type);
    width = FreeImage_GetWidth(bitmap);
    height = FreeImage_GetHeight(bitmap);

    // build a mipmap
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,
                      GL_BGR_EXT, GL_UNSIGNED_BYTE,
                      FreeImage_GetBits(bitmap));

    // set wrap, filters, hint
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // free memory
    FreeImage_Unload(bitmap);
}

void Texture::bind() {
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

