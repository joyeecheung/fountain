#include <Vector>  //dynamic array "vector"

#include "Pool.h"
#include "FVector.h"  //3d-vectors, my own routines...

using namespace std;

void Pool::initialize(int xSize, int zSize, float oDistance,
                      float oWeight, float damping,
                      float textureStretchX, float textureStretchZ) {
    //assign member variables
    this->xSize = xSize;
    this->zSize = zSize;
    this->numOscillators = xSize*zSize;
    this->oscillatorDistance = oDistance;
    this->oscillatorWeight = oWeight;
    this->damping = damping;

    //temporary vector for indies:
    vector <GLuint> idxVector;  // we first put the indices into this vector
    // then copy them to the array below

    oscillators = new Oscillator[numOscillators];
    idxVector.clear();  //to be sure it is empty
    for (int xc = 0; xc < xSize; xc++)
        for (int zc = 0; zc < zSize; zc++) {
            oscillators[xc + zc*xSize].position.x = oscillatorDistance*float(xc);
            oscillators[xc + zc*xSize].position.y = 0.0f;
            oscillators[xc + zc*xSize].position.z = oscillatorDistance*float(zc);

            oscillators[xc + zc*xSize].normal.x = 0.0f;
            oscillators[xc + zc*xSize].normal.y = 1.0f;
            oscillators[xc + zc*xSize].normal.z = 0.0f;

            oscillators[xc + zc*xSize].texX = (float)xc / (float)xSize * textureStretchX;
            oscillators[xc + zc*xSize].texY = 1.0f - (float)zc / (float)zSize * textureStretchZ;

            oscillators[xc + zc*xSize].upSpeed = 0;

            // create two triangles:
            if ((xc < xSize - 1) && (zc < zSize - 1)) {
                idxVector.push_back(xc + zc*xSize);
                idxVector.push_back((xc + 1) + zc*xSize);
                idxVector.push_back((xc + 1) + (zc + 1)*xSize);

                idxVector.push_back(xc + zc*xSize);
                idxVector.push_back((xc + 1) + (zc + 1)*xSize);
                idxVector.push_back(xc + (zc + 1)*xSize);
            }

        }

    //copy the indices:
    indices = new GLuint[idxVector.size()];  //allocate the required memory
    for (size_t i = 0; i < idxVector.size(); i++) {
        indices[i] = idxVector[i];
    }


    numIndices = idxVector.size();
    idxVector.clear();  //no longer needed, takes only memory
}

void Pool::reset() {
    for (int xc = 0; xc < xSize; xc++)
        for (int zc = 0; zc < zSize; zc++) {
            oscillators[xc + zc*xSize].position.y = 0.0f;
            oscillators[xc + zc*xSize].upSpeed = 0.0f;
            oscillators[xc + zc*xSize].normal.x = 0.0f;
            oscillators[xc + zc*xSize].normal.y = 1.0f;
            oscillators[xc + zc*xSize].normal.z = 0.0f;
        }
}

void Pool::affectOscillator(int xPos, int zPos, float deltaY) {
    if ((xPos >= 0) && (xPos < xSize) && (zPos >= 0) && (zPos < zSize)) {
        //THIS LINE IS REQUIRED FOR FOUNTAINS WITH MANY DROPS!!!
        if (oscillators[xPos + zPos*xSize].position.y > -0.15)
            oscillators[xPos + zPos*xSize].position.y += deltaY;
    }
}


void Pool::update(float deltaTime) {
    //********
    // Here we do the physical calculations: 
    // The m_Oscillators are moved according to their neighbors.
    // The parameter bEndIsFree indicates, whether the m_Oscillators in the edges can move or not.
    // The new position may be assigned not before all calculations are done!

    // PLEASE NOTE: THESE ARE APPROXIMATIONS AND I KNOW THIS! (but is looks good, doesn't it?)

    //if we use two loops, it is a bit easier to understand what I do here.

    int xc, zc;

    for (xc = 0; xc < xSize; xc++) {
        for (zc = 0; zc < zSize; zc++) {
            int idx = xc + zc*xSize;

            oscillators[idx].newY = oscillators[idx].position.y;

            //check, if this oscillator is on an edge (=>end)
            if ((xc == 0) || (xc == xSize - 1) || (zc == 0) || (zc == zSize - 1))
                ;//TBD: calculating m_Oscillators at the edge (if the end is free)
            else {
                //calculate the new speed:

                //Change the speed (=accelerate) according to the oscillator's 4 direct neighbors:
                float avgDifference = oscillators[idx - 1].position.y //left neighbor
                    + oscillators[idx + 1].position.y 		//right neighbor
                    + oscillators[idx - xSize].position.y  //upper neighbor
                    + oscillators[idx + xSize].position.y  //lower neighbor
                    - 4 * oscillators[idx].position.y;	  //subtract the pos of the current osc. 4 times	

                oscillators[idx].upSpeed += avgDifference*(deltaTime / oscillatorWeight);

                oscillators[idx].upSpeed *= (1.0f - damping);

                //calculate the new position, but do not yet store it in "y"
                // (this would affect the calculation of the other osc.s)
                oscillators[idx].newY += oscillators[idx].upSpeed * deltaTime;

            }
        }
    }

    //copy the new position to y:
    for (xc = 0; xc < xSize; xc++) {
        for (int zc = 0; zc < zSize; zc++) {
            oscillators[xc + zc*xSize].position.y = oscillators[xc + zc*xSize].newY;
        }
    }
    //calculate new normal vectors (according to the oscillator's neighbors):
    for (xc = 0; xc < xSize; xc++) {
        for (int zc = 0; zc < zSize; zc++) {
            ///
            //Calculating the normal:
            //Take the direction vectors 1.) from the left to the right neighbor 
            // and 2.) from the upper to the lower neighbor.
            //The vector orthogonal to these 

            FVector3 u, v, p1, p2;	//u and v are direction vectors. p1 / p2: temporary used (storing the points)

            if (xc > 0) {
                p1 = FVector3(oscillators[xc - 1 + zc*xSize].position.x,
                              oscillators[xc - 1 + zc*xSize].position.y,
                              oscillators[xc - 1 + zc*xSize].position.z);
            } else {
                p1 = FVector3(oscillators[xc + zc*xSize].position.x,
                              oscillators[xc + zc*xSize].position.y,
                              oscillators[xc + zc*xSize].position.z);
            }

            if (xc < xSize - 1) {
                p2 = FVector3(oscillators[xc + 1 + zc*xSize].position.x,
                              oscillators[xc + 1 + zc*xSize].position.y,
                              oscillators[xc + 1 + zc*xSize].position.z);
            } else {
                p2 = FVector3(oscillators[xc + zc*xSize].position.x,
                              oscillators[xc + zc*xSize].position.y,
                              oscillators[xc + zc*xSize].position.z);
            }

            u = p2 - p1; //vector from the left neighbor to the right neighbor
            if (zc > 0) {
                p1 = FVector3(oscillators[xc + (zc - 1)*xSize].position.x,
                              oscillators[xc + (zc - 1)*xSize].position.y,
                              oscillators[xc + (zc - 1)*xSize].position.z);
            } else {
                p1 = FVector3(oscillators[xc + zc*xSize].position.x,
                              oscillators[xc + zc*xSize].position.y,
                              oscillators[xc + zc*xSize].position.z);
            }

            if (zc < zSize - 1) {
                p2 = FVector3(oscillators[xc + (zc + 1)*xSize].position.x,
                              oscillators[xc + (zc + 1)*xSize].position.y,
                              oscillators[xc + (zc + 1)*xSize].position.z);
            } else {
                p2 = FVector3(oscillators[xc + zc*xSize].position.x,
                              oscillators[xc + zc*xSize].position.y,
                              oscillators[xc + zc*xSize].position.z);
            }
            v = p2 - p1; //vector from the upper neighbor to the lower neighbor
            //calculate the normal:
            FVector3 normal = u.cross(v).normalize();

            //assign the normal:
            if (normal.y > 0.0) {  //normals always look upward!
                oscillators[xc + zc*xSize].normal.x = normal.x;
                oscillators[xc + zc*xSize].normal.y = normal.y;
                oscillators[xc + zc*xSize].normal.z = normal.z;
            } else {
                oscillators[xc + zc*xSize].normal.x = -normal.x;
                oscillators[xc + zc*xSize].normal.y = -normal.y;
                oscillators[xc + zc*xSize].normal.z = -normal.z;
            }
        }
    }

}


void Pool::render() {
    //There might be more vertex arrays. Thus, pass the pointers each time you use them:
    glVertexPointer(3,   //3 components per vertex (x,y,z)
                    GL_FLOAT,
                    sizeof(Oscillator),
                    oscillators);
    glTexCoordPointer(2,
                      GL_FLOAT,
                      sizeof(Oscillator),
                      &oscillators[0].texX);


    glNormalPointer(GL_FLOAT,
                    sizeof(Oscillator),
                    &oscillators[0].normal.x);  //Pointer to the first normal

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    //Draw the array:
    glColor3f(1.0, 1.0, 1.0);
    glDrawElements(GL_TRIANGLES, //mode
                   numIndices,  //count, ie. how many indices
                   GL_UNSIGNED_INT, //type of the index array
                   indices);
}

float Pool::getOscillatorDistance() {
    return oscillatorDistance;
}
