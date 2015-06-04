#include <cstdlib>
#include <cmath>
#include "Fountain.h"

#define RAND_FACTOR 2.0f

GLfloat randf(GLfloat range) {
    return (GLfloat)rand() / (GLfloat)RAND_MAX * range * RAND_FACTOR;
}

void Drop::setSpeed(FVector3 newSpeed) {
    speed = newSpeed;
}

void Drop::setAcceleration(GLfloat newAcc) {
    acc = newAcc;
}

void Drop::setTime(GLfloat newTime) {
    time = newTime;
}

void Drop::updatePosition(FVector3 & vertex, float dtime,
                           Pool * pool, Fountain *fountain) {
    time += dtime * 20.0f;
    if (time > 0.0f) {
        FVector3 position(speed.x * time,
                          speed.y * time - acc * time * time,
                          speed.z * time);
        vertex = position;
        if (position.y < 0.0) {
            /* the drop has fallen into the water. The problem is now that we cannot
                set time to 0.0, because if there are more "DropsPerRay"
                than "TimeNeeded" (See InitFountain())
                several drops would be seen as one. Check it out.
            */
            time = time - int(time);
            if (time > 0.0) time -= 1.0;

            // The drop creates a little wave in the pool:
            float distance = pool->getODistance();
            int oX = (int)((position.x + fountain->center.x) / distance);
            int oZ = (int)((position.z + fountain->center.z) / distance);
            // change this to make the waves stronger/weaker
            pool->updateOscillator(oX, oZ, -0.1f);
        }
    } else {
        vertex.set(0.0f, 0.0f, 0.0f);
    }
}

/********************************************************************/

void Fountain::initialize(GLint levels, GLint raysPerStep, GLint dropsPerRay,
                          GLfloat angleMin, GLfloat angleMax,
                          GLfloat randomAngle, GLfloat acceleration) {
    //This function needn't be and isn't speed optimized
    this->center = center;

    this->numDrops = levels * raysPerStep * dropsPerRay;

    if (this->drops != nullptr) delete[] this->drops;
    if (this->vertices != nullptr) delete[] this->vertices;
    this->drops = new Drop[numDrops];
    this->vertices = new FVector3[numDrops];

    GLfloat randAcc; // acceleration changed randomly
    GLfloat initialTime;  // initial elapsed time for each drop
    FVector3 initialSpeed;  // initial speed for each drop
    GLfloat angleZ; // angle in the front view
    GLfloat angleY;  // angle in the top view

    for (int k = 0; k < levels; k++) {
        for (int j = 0; j < raysPerStep; j++) {
            for (int i = 0; i < dropsPerRay; i++) {
                randAcc = acceleration + randf(0.005f);
                if (levels > 1) {
                    angleZ = angleMin + (angleMax - angleMin)
                    *GLfloat(k) / (levels - 1) + randf(randomAngle);
                } else {
                    angleZ = angleMin + randf(randomAngle);
                }

                // speed update by levels
                initialSpeed.x = cos(angleZ * PI / 180.0) * (0.2 + 0.04 * k);
                initialSpeed.y = sin(angleZ * PI / 180.0) * (0.2 + 0.04 * k);

                // speed update by rays
                // +12.0 causes a rotation (12?
                angleY = (GLfloat)j / (GLfloat)raysPerStep * 360.0f + 12.0f;

                // for the next computations "newSpeed.x" is the radius.
                // DON'T swap the two lines, because the second one changes newSpeed.x
                initialSpeed.z = initialSpeed.x * sin(angleY * PI / 180.0);
                initialSpeed.x = initialSpeed.x * cos(angleY * PI / 180.0);

                initialSpeed.x *= 3.0f;
                initialSpeed.y *= 3.0f;
                initialSpeed.z *= 3.0f;

                // Calculate how many levels are required
                // that a drop comes out and falls down again
                initialTime = initialSpeed.y / randAcc;
                int idx = i + j * dropsPerRay + k * dropsPerRay * raysPerStep;
                drops[idx].setSpeed(initialSpeed);
                drops[idx].setAcceleration(randAcc);
                drops[idx].setTime(initialTime * i / dropsPerRay);
            }
        }
    }

}

void Fountain::update(float dtime, Pool * pPool) {
    for (int i = 0; i < numDrops; i++)
        drops[i].updatePosition(vertices[i], dtime, pPool, this);
}

void Fountain::render() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPointSize(2.0);
    glVertexPointer(3,          // x, y, z-components
                    GL_FLOAT,   // data type of SVertex
                    0,         // the vertices are tightly packed
                    vertices);
    glPushMatrix();
    // move the fountain to the center of the pool
    glTranslatef(center.x, center.y, center.z);
    glDrawArrays(GL_POINTS, 0, numDrops);

    glPopMatrix();
}