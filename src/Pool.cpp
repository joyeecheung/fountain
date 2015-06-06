#include <vector>
#include "Pool.h"
#include "FVector.h" 

void Pool::initialize(int oNumX, int oNumZ, float height,
                      float oDistance, float oWeight,
                      float damping, float splash,
                      float texRepeatX, float texRepeatZ,
                      Texture * floorTexture) {
    // initializes the members
    this->oNumX = oNumX;
    this->oNumZ = oNumZ;
    this->height = height;
    this->oNum = oNumX * oNumZ;
    this->oDistance = oDistance;
    this->oWeight = oWeight;
    this->damping = damping;
    this->splash = splash;
    this->floorTexture = floorTexture;

    std::vector <int> idxVector; // temporary vector for indices

    if (oscillators != nullptr) delete [] oscillators;
    oscillators = new Oscillator[oNum];

    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            int idx = i + j * oNumX;
            // positions
            oscillators[idx].x = oDistance * float(i);
            oscillators[idx].y = 0.0f;  // on the plane initially
            oscillators[idx].z = oDistance * float(j);

            // normals. pointing up initially
            oscillators[idx].nx = 0.0f;
            oscillators[idx].ny = 1.0f;
            oscillators[idx].nz = 0.0f;

            // texture coordinates
            oscillators[idx].texX = (float)i / (float)oNumX * texRepeatX;
            oscillators[idx].texY = 1.0f - (float)j / (float)oNumZ * texRepeatZ;

            // initial speed
            oscillators[idx].speedY = 0;

            // create a peek for it. that's two triangles
            if ((i < oNumX - 1) && (j < oNumZ - 1)) {
                idxVector.push_back(i + j * oNumX);
                idxVector.push_back((i + 1) + j * oNumX);
                idxVector.push_back((i + 1) + (j + 1) * oNumX);

                idxVector.push_back(i + j * oNumX);
                idxVector.push_back((i + 1) + (j + 1) * oNumX);
                idxVector.push_back(i + (j + 1) * oNumX);
            }

        }
    }

    // copy the indices
    if (indices != nullptr) delete [] indices;
    indices = new int[idxVector.size()];
    for (size_t i = 0; i < idxVector.size(); i++) {
        indices[i] = idxVector[i];
    }

    indicesNum = idxVector.size();
}

void Pool::reset() {
    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            int idx = i + j * oNumX;
            // normal points up
            oscillators[idx].nx = 0.0f;
            oscillators[idx].ny = 1.0f;
            oscillators[idx].nz = 0.0f;
            // back to the plane
            oscillators[idx].y = 0.0f;
            // initial speed
            oscillators[idx].speedY = 0.0f;
        }
    }
}

void Pool::splashOscillator(int posX, int posZ) {
    // if in the range.
    // this is needed by fountains with drops out of range
    if ((posX >= 0) && (posX < oNumX) && (posZ >= 0) && (posZ < oNumZ)) {
        int idx = posX + posZ * oNumX;
        if (oscillators[idx].y > -0.15)  // TODO: move out this hard threshold
            oscillators[idx].y += splash;
    }
}

void Pool::update(float deltaTime) {
    /********
     * The movements of the oscillators are affected by their neighbors
     * The calculation must be done before update the positions.
     *********/
    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            int idx = i + j * oNumX;
            // store the y temperorily
            oscillators[idx].newY = oscillators[idx].y;

            if ((i == 0) || (i == oNumX - 1) || (j == 0) || (j == oNumZ - 1)) {
                ; /* NOTE: this condition can make the oscillators
                   * at the boundaries always have y = 0
                   * which causes a bounce effect. */
            } else { // calculate the new speed
                // update the speed (i.e.accelerate)
                // according to the 4 neighbors
                float avgdiff = oscillators[idx - 1].y   //left
                    + oscillators[idx + 1].y // right
                    + oscillators[idx - oNumX].y //upper
                    + oscillators[idx + oNumX].y  // lower
                    - 4 * oscillators[idx].y;  // subtract itself all at one

                oscillators[idx].speedY += avgdiff * (deltaTime / oWeight);
                oscillators[idx].speedY *= (1.0f - damping);

                // store the new position.
                // NOTE: can't just update it
                // because the neighbors needs the old y
                oscillators[idx].newY += oscillators[idx].speedY * deltaTime;
            }
        }
    }

    // calculation has been done.
    // update the y's
    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            int idx = i + j * oNumX;
            oscillators[idx].y = oscillators[idx].newY;
        }
    }

    // update normals using the newly positioned neighbors
    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            // the new normal is orthogonal to
            // 1. the vector from the left to the right neighbor
            // 2. the vector from the upper to the lower neighbor
            int idx = i + j * oNumX,
                ileft = i - 1 + j * oNumX, iright = i + 1 + j * oNumX,
                iup = i + (j + 1) * oNumX, idown = i + (j - 1) * oNumX;
            FVector3 p1, p2; // store the points for calculating u and v
                             // needed because of the boundaries.
            FVector3 u, v;  // direction vectors
            int ip1 = idx, ip2 = idx;  // indices for p1 and p2

            // calculate left-to-right direction vector
            ip1 = i > 0 ? ileft : idx;
            ip2 = i < oNumX - 1 ? iright : idx;
            p1.set(oscillators[ip1].x,
                   oscillators[ip1].y,
                   oscillators[ip1].z);
            p2.set(oscillators[ip2].x,
                   oscillators[ip2].y,
                   oscillators[ip2].z);
            u = p2 - p1;

            // calculate upper-to-lower direction vector
            ip1 = j > 0 ? idown : idx;
            ip2 = j < oNumZ - 1 ? iup : idx;
            p1.set(oscillators[ip1].x,
                   oscillators[ip1].y,
                   oscillators[ip1].z);
            p2.set(oscillators[ip2].x,
                   oscillators[ip2].y,
                   oscillators[ip2].z);
            v = p2 - p1;

            // cross product to get the orthogonal vector
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
    glTexCoordPointer(2, GL_FLOAT, sizeof(Oscillator), &(oscillators[0].texX));
    glNormalPointer(GL_FLOAT, sizeof(Oscillator), &(oscillators[0].nx));

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