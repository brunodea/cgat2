#ifndef _CGA_T2_CONTROLLER_H_
#define _CGA_T2_CONTROLLER_H_

#include "model/Ground.hpp"
#include "math/matrix_functions.hpp"
#include "Camera.hpp"
#include "model/Player.hpp"
#include "GL/glfw.h"

class Controller
{
public:
    static Controller *instance();
    ~Controller();

    void run();

    void keyEvent(int key, int state);
    void mouseMoved(int x, int y);
    
    void drawScene();
private:
    Controller();
    void onUpdate();
    void onRender();

    void cameraMove();

private:
    static Controller *m_sInstance;
    Camera m_Camera;
    model::Ground *m_pGround;
    model::Player *m_pPlayer;
    model::Player *m_pCube[3];
    
    float m_fYaw;
}; //end of class controller.

#endif
