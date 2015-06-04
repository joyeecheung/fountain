#include <Vector>
#include "Pool.h"
#include "FVector.h" 

void Pool::initialize(int sizeX, int sizeZ, float height,
                      float oDistance, float oWeight,
                      float damping, float splash,
                      float texSizeX, float texSizeZ,
                      Texture * floorTexture) {
    this->sizeX = sizeX;
    this->sizeZ = sizeZ;
    this->height = height;
    this->oscillatorsNum = sizeX * sizeZ;
    this->oDistance = oDistance;
    this->oWeight = oWeight;
    this->damping = damping;
    this->splash = splash;
    this->floorTexture = floorTexture;

    std::vector <GLuint> idxVector; // temporary vector for indices

    if (oscillators != nullptr) delete [] oscillators;
    oscillators = new Oscillator[oscillatorsNum];

    for (int i = 0; i < sizeX; i++) {
        for (int j = 0; j < sizeZ; j++) {
            int idx = i + j * sizeX;
            oscillators[idx].x = oDistance * float(i);
            oscillators[idx].y = 0.0f;
            oscillators[idx].z = oDistance * float(j);

            oscillators[idx].nx = 0.0f;
            oscillators[idx].ny = 1.0f;
            oscillators[idx].nz = 0.0f;

            oscillators[idx].texX = (float)i / (float)sizeX * texSizeX;
            oscillators[idx].texY = 1.0f - (float)j / (float)sizeZ * texSizeZ;

            oscillators[idx].speedY = 0;

            // create two triangles
            if ((i < sizeX - 1) && (j < sizeZ - 1)) {
                idxVector.push_back(i + j * sizeX);
                idxVector.push_back((i + 1) + j * sizeX);
                idxVector.push_back((i + 1) + (j + 1) * sizeX);

                idxVector.push_back(i + j * sizeX);
                idxVector.push_back((i + 1) + (j + 1) * sizeX);
                idxVector.push_back(i + (j + 1) * sizeX);
            }

        }
    }

    // copy the indices
    if (indices != nullptr) delete [] indices;
    indices = new GLuint[idxVector.size()];
    for (size_t i = 0; i < idxVector.size(); i++) {
        indices[i] = idxVector[i];
    }

    indicesNum = idxVector.size();
}

void Pool::reset() {
    for (int xc = 0; xc < sizeX; xc++) {
        for (int zc = 0; zc < sizeZ; zc++) {
            int idx = xc + zc * sizeX;
            oscillators[idx].y = 0.0f;
            oscillators[idx].speedY = 0.0f;
            oscillators[idx].nx = 0.0f;
            oscillators[idx].ny = 1.0f;
            oscillators[idx].nz = 0.0f;
        }
    }
}

void Pool::updateOscillator(int posX, int posZ) {
    if ((posX >= 0) && (posX < sizeX) && (posZ >= 0) && (posZ < sizeZ)) {
        // THIS LINE IS REQUIRED FOR FOUNTAINS WITH MANY DROPS!!!
        int idx = posX + posZ * sizeX;
        if (oscillators[idx].y > -0.15)
            oscillators[idx].y += splash;
    }
}


void Pool::update(float deltaTime) {
    //********
    // Here we do the physical calculations: 
    // The m_Oscillators are moved according to their neighbors.
    // The parameter bEndIsFree indicates, whether the m_Oscillators in the edges can move or not.
    // The new position may be assigned not before all calculations are done!

    // PLEASE NOTE: THESE ARE APPROXIMATIONS AND I KNOW THIS! (but is looks good, doesn't it?)

    // if we use two loops, it is a bit easier to understand what I do here.

    int xc, zc;
    for (xc = 0; xc < sizeX; xc++) {
        for (zc = 0; zc < sizeZ; zc++) {
            int idx = xc + zc * sizeX;

            oscillators[idx].newY = oscillators[idx].y;

            //check, if this oscillator is on an edge (=>end)
            if ((xc == 0) || (xc == sizeX - 1) || (zc == 0) || (zc == sizeZ - 1))
                ;//TBD: calculating m_Oscillators at the edge (if the end is free)
            else {
                //calculate the new speed:

                //Change the speed (=accelerate) according to the oscillator's 4 direct neighbors:
                float avgDifference = oscillators[idx - 1].y //left neighbor
                    + oscillators[idx + 1].y 		//right neighbor
                    + oscillators[idx - sizeX].y  //upper neighbor
                    + oscillators[idx + sizeX].y  //lower neighbor
                    - 4 * oscillators[idx].y;	  //subtract the pos of the current osc. 4 times	

                oscillators[idx].speedY += avgDifference*(deltaTime / oWeight);

                oscillators[idx].speedY *= (1.0f - damping);

                //calculate the new position, but do not yet store it in "y"
                // (this would affect the calculation of the other osc.s)
                oscillators[idx].newY += oscillators[idx].speedY * deltaTime;
            }
        }
    }

    // copy the new position to y:
    for (xc = 0; xc < sizeX; xc++) {
        for (int zc = 0; zc < sizeZ; zc++) {
            int idx = xc + zc * sizeX;
            oscillators[idx].y = oscillators[idx].newY;
        }
    }

    // calculate new normal vectors (according to the oscillator's neighbors):
    for (xc = 0; xc < sizeX; xc++) {
        for (int zc = 0; zc < sizeZ; zc++) {
            // Calculating the normal:
            // Take the direction vectors 1.) from the left to the right neighbor 
            // and 2.) from the upper to the lower neighbor.
            // The vector orthogonal to these 

            FVector3 u, v, p1, p2;	//u and v are direction vectors. p1 / p2: temporary used (storing the points)
            int idx = xc + zc * sizeX,
                ileft = xc - 1 + zc * sizeX, iright = xc + 1 + zc * sizeX,
                iup = xc + (zc + 1) * sizeX, idown = xc + (zc - 1) * sizeX;

            int ip1 = idx, ip2 = idx;

            ip1 = xc > 0 ? ileft : idx;
            ip2 = xc < sizeX - 1 ? iright : idx;
            p1 = FVector3(oscillators[ip1].x,
                          oscillators[ip1].y,
                          oscillators[ip1].z);
            p2 = FVector3(oscillators[ip2].x,
                          oscillators[ip2].y,
                          oscillators[ip2].z);

            u = p2 - p1; //vector from the left neighbor to the right neighbor

            ip1 = zc > 0 ? idown : idx;
            ip2 = zc < sizeZ - 1 ? iup : idx;

            p1 = FVector3(oscillators[ip1].x,
                          oscillators[ip1].y,
                          oscillators[ip1].z);
            p2 = FVector3(oscillators[ip2].x,
                          oscillators[ip2].y,
                          oscillators[ip2].z);

            v = p2 - p1; //vector from the upper neighbor to the lower neighbor
            FVector3 normal = u.cross(v).normalize();

            // the normal should always points up
            float sign = normal.y > 0.0 ? 1.0f : -1.0f;
            oscillators[idx].nx = normal.x * sign;
            oscillators[idx].ny = normal.y * sign;
            oscillators[idx].nz = normal.z * sign;
        }
    }

}

void Pool::render() const {
    // set up pointers
    glVertexPointer(3, GL_FLOAT, sizeof(Oscillator), oscillators);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Oscillator), &oscillators[0].texX);
    glNormalPointer(GL_FLOAT, sizeof(Oscillator), &oscillators[0].nx);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // draw
    glPushMatrix();
    glTranslatef(0.0f, height, 0.0f);
    floorTexture->bind();
    // fill it. NOTE: this will affect the whole scene!!!
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glDrawElements(GL_TRIANGLES, indicesNum,  GL_UNSIGNED_INT, indices);
    glPopMatrix();
}