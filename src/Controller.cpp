#include "Controller.h"

#include "GL/glfw.h"
#include "macros.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::run()
{
    double last_render = 0;
    double last_update = 0;

    double frame_interval = 1.f/60.f;
    double update_interval = 1.f/90.f;

    double cur_time = 0;
    while(glfwGetWindowParam(GLFW_OPENED) && glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS)
    {
        cur_time = glfwGetTime();
        if(cur_time - last_update > update_interval)
        {
            onUpdate();
            last_update = glfwGetTime();
        }
        if(cur_time - last_render > frame_interval) 
        {
            last_render = glfwGetTime();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            onRender();
            glfwSwapBuffers();
        }
        glfwSleep(update_interval - (cur_time + glfwGetTime()));
    }
}

void Controller::keyEvent(int key, int state)
{
}

void Controller::mouseMoved(int x, int y)
{
}

void Controller::onUpdate()
{
}

void Controller::onRender()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f,(float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1,10000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
