#include <cstdlib>
#include <cmath>
#include "Fountain.h"

#define RandomFactor 2.0f

GLfloat randf(GLfloat range) {
    return (GLfloat)rand() / (GLfloat)RAND_MAX * range * RandomFactor;
}

void Drop::setConstantSpeed(FVector3 newSpeed) {
    constantSpeed = newSpeed;
}

void Drop::setAccFactor(GLfloat newAccFactor) {
    accFactor = newAccFactor;
}

void Drop::setTime(GLfloat newTime) {
    time = newTime;
}

void Drop::getNewPosition(FVector3 * positionVertex, float dtime,
                           Pool * pPool, Fountain *pFountain) {
    time += dtime * 20.0f;
    if (time > 0.0f) {
        FVector3 position(constantSpeed.x * time,
                          constantSpeed.y * time - accFactor * time * time,
                          constantSpeed.z * time);
        *positionVertex = position;
        if (position.y < 0.0) {
            /* the drop has fallen into the water. The problem is now that we cannot
                set time to 0.0, because if there are more "DropsPerRay"
                than "TimeNeeded" (See InitFountain())
                several drops would be seen as one. Check it out.
            */
            time = time - int(time);
            if (time > 0.0) time -= 1.0;

            //The drop creates a little wave in the pool:
            float distance = pPool->getOscillatorDistance();
            int oscillatorX = (int)((position.x + pFountain->position.x) / distance);
            int oscillatorZ = (int)((position.z + pFountain->position.z) / distance);
            pPool->affectOscillator(oscillatorX, oscillatorZ, -0.1f);  //change this to make the waves stronger/weaker
        }
    } else {
        positionVertex->set(0.0f, 0.0f, 0.0f);
    }
}

/********************************************************************/


void Fountain::initialize(GLint steps, GLint raysPerStep, GLint dropsPerRay,
                              GLfloat angleOfDeepestStep,
                              GLfloat angleOfHighestStep,
                              GLfloat randomAngleAddition,
                              GLfloat accFactor) {
    //This function needn't be and isn't speed optimized

    numDrops = steps * raysPerStep * dropsPerRay;

    fountainDrops = new Drop[numDrops];
    fountainVertices = new FVector3[numDrops];
    FVector3 newSpeed;
    GLfloat dropAccFactor; //different from AccFactor because of the random change
    GLfloat timeNeeded;
    GLfloat stepAngle; //Angle, which the ray gets out of the fountain with
    GLfloat rayAngle;	//Angle you see when you look down on the fountain
    GLint i, j, k;

    for (k = 0; k < steps; k++) {
        for (j = 0; j < raysPerStep; j++) {
            for (i = 0; i < dropsPerRay; i++) {
                dropAccFactor = accFactor + randf(0.005f);
                if (steps > 1) {
                    stepAngle = angleOfDeepestStep + (angleOfHighestStep - angleOfDeepestStep)
                    *GLfloat(k) / (steps - 1) + randf(randomAngleAddition);
                } else {
                    stepAngle = angleOfDeepestStep + randf(randomAngleAddition);
                }

                //This is the speed caused by the step:
                newSpeed.x = cos(stepAngle * PI / 180.0) * (0.2 + 0.04 * k);
                newSpeed.y = sin(stepAngle * PI / 180.0) * (0.2 + 0.04 * k);

                //This is the speed caused by the ray:
                rayAngle = (GLfloat)j / (GLfloat)raysPerStep * 360.0f + 12.0f;  //+12.0 causes a rotation (12?

                //for the next computations "NewSpeed.x" is the radius. Care! Dont swap the two
                //lines, because the second one changes NewSpeed.x!
                newSpeed.z = newSpeed.x * sin(rayAngle * PI / 180.0);
                newSpeed.x = newSpeed.x * cos(rayAngle * PI / 180.0);

                newSpeed.x *= 3.0f;
                newSpeed.y *= 3.0f;
                newSpeed.z *= 3.0f;

                //Calculate how many steps are required, that a drop comes out and falls down again
                timeNeeded = newSpeed.y / dropAccFactor;
                int idx = i + j * dropsPerRay + k * dropsPerRay * raysPerStep;
                fountainDrops[idx].setConstantSpeed(newSpeed);
                fountainDrops[idx].setAccFactor(dropAccFactor);
                fountainDrops[idx].setTime(timeNeeded * i / dropsPerRay);
            }
        }
    }

}

void Fountain::update(float dtime, Pool * pPool) {
    for (int i = 0; i < numDrops; i++)
        fountainDrops[i].getNewPosition(&fountainVertices[i], dtime, pPool, this);
}

void Fountain::render() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3,			//x,y,z-components
                    GL_FLOAT,	//data type of SVertex
                    0,			//the vertices are tightly packed
                    fountainVertices);
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glDrawArrays(GL_POINTS,
                 0,
                 numDrops);

    glPopMatrix();
}

void Fountain::destroy() {
    delete[] fountainDrops;
    delete[] fountainVertices;
}
