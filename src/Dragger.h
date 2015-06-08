#ifndef DRAGGER_H
#define DRAGGER_H

#include "Vector2.tpp"

#include <functional>
#include <GL/glut.h>

typedef std::function<void(IVector2)> DragCallback;

class Dragger {
public:
    void setCallback(DragCallback dcb);
    void startDragging(int x, int y);
    void stopDragging();
    void move(int x, int y);
    void set(int x, int y);

    IVector2 lastPos;

private:
    DragCallback dcb;
    bool dragging;
};

#endif
