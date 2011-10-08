#ifndef _CGA_T2_CONTROLLER_H_
#define _CGA_T2_CONTROLLER_H_

class Controller
{
public:
    Controller();
    ~Controller();

    void run();

    void keyEvent(int key, int state);
    void mouseMoved(int x, int y);

private:
    void onUpdate();
    void onRender();

private:
}; //end of class controller.

#endif
