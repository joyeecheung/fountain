#include "Dragger.h"

void Dragger::setCallback(DragCallback dcb) {
    this->dcb = dcb;
}

void Dragger::startDragging(int x, int y) {
    dragging = true;
    lastPos.set(x, y);
}

void Dragger::stopDragging() {
    dragging = false;
}

void Dragger::move(int x, int y) {
    IVector2 curPos;
    curPos.set(x, y);
    if (dragging) {
        IVector2 dragVector(curPos - lastPos);
        dcb(dragVector);
    }
    lastPos = curPos;
}

void Dragger::set(int x, int y) {
    glutWarpPointer(x, y);
    lastPos.set(x, y);
}
