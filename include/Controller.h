#ifndef _CGA_T2_CONTROLLER_H_
#define _CGA_T2_CONTROLLER_H_

#include "model/Ground.hpp"
#include "math/matrix_functions.hpp"
#include "Camera.hpp"

class Controller
{
public:
    static Controller *instance();
    ~Controller();

    void run();

    void keyEvent(int key, int state);
    void mouseMoved(int x, int y);

private:
    Controller();
    void onUpdate();
    void onRender();

    void cameraMove();
private:
    static Controller *m_sInstance;
    Camera m_Camera;
    model::Ground *m_pGround;

    float m_fYaw;
    float m_fPitch;
}; //end of class controller.

#endif
