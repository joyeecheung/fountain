#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>
#include "FVector.h"

/*****
 * Note: angles are all in [0, 90]
 ****/
class Camera {
public:
    Camera() {
        position.set(0.0f, 0.0f, 0.0f);
        direction.set(0.0f, 0.0f, -1.0f);
        directionChanged = false;
        rotation.set(0.0f, 0.0f, 0.0f);
    }
    /* Executes glRotate and glTranslate.
     * Should be called after calling glLoadIdentity. */
    void render(void) const;
    void move(const FVector3 & direction);
    void moveX(float distance);
    void moveY(float distance);
    void moveZ(float distance);
    void rotate(const FVector3 & angles);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
private:
    FVector3 position;
    /* needs to mark the update by setting directionChanged to true
     * before calling any move function */
    FVector3 direction; 
    bool directionChanged;
    FVector3 rotation;
    void updateDirection(void);
};

#endif