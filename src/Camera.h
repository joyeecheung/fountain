#include <gl\glut.h>
#include "FVector.h"

//Note: All angles in degrees

class Camera {
private:
    FVector3 position;
    FVector3 viewDir;		/*Not used for rendering the camera, but for "moveforwards"
                            So it is not necessary to "actualize" it always. It is only
                            actualized when ViewDirChanged is true and moveforwards is called*/
    bool viewDirChanged;
    GLfloat rotatedX, rotatedY, rotatedZ;
    void getViewDir(void);
public:
    //inits the values (Position: (0|0|0) Target: (0|0|-1) )
    Camera() : position(0.0f, 0.0f, 0.0f),
               viewDir(0.0f, 0.0f, -1.0f),
               viewDirChanged(false),
               rotatedX(0.0f), rotatedY(0.0f), rotatedZ(0.0f){
    }

    void render(void);	//executes some glRotates and a glTranslate command
    //Note: You should call glLoadIdentity before using Render
    void move(FVector3 direction);
    void rotateX(GLfloat angle);
    void rotateY(GLfloat angle);
    void rotateZ(GLfloat angle);
    void rotateXYZ(FVector3 angles);
    void moveForwards(GLfloat distance);
    void strafeRight(GLfloat distance);
};