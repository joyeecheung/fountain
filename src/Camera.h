#ifndef CAMERA_H
#define CAMERA_H

#include <gl\glut.h>
#include "FVector.h"

/* Note: angles are in [0, 90]
 */
class Camera {
public:
    Camera() : position(0.0f, 0.0f, 0.0f),
               direction(0.0f, 0.0f, -1.0f),
               directionChanged(false),
               rotation(0.0f, 0.0f, 0.0f) {}
    /* Executes glRotate and glTranslate.
       Should be called after calling glLoadIdentity. */
    void render(void) const;
    void move(FVector3 direction);
    void moveZ(GLfloat distance);
    void moveX(GLfloat distance);
    void rotate(FVector3 angles);
    void rotateX(GLfloat angle);
    void rotateY(GLfloat angle);
    void rotateZ(GLfloat angle);
private:
    FVector3 position;
    /* needs to mark the update by setting directionChanged to true
       before calling any move function */
    FVector3 direction; 
    bool directionChanged;
    FVector3 rotation;
    void updateDirection(void);
};

#endif