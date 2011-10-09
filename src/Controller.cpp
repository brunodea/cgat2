#include "Controller.h"

#include "GL/glfw.h"

#include "util/MatrixStack.h"
#include "macros.h"

Controller *Controller::m_sInstance = NULL;

Controller::Controller()
    : m_Camera(math::vector3f(0.f,0.f,10.f), math::vector3f(0.f,0.f,0.f), math::vector3f(0.f,1.f,0.f)), 
    m_pGround(new model::Ground(100.f, "res/ground1.vert", "res/ground1.frag","res/normal_2.bmp")), m_fYaw(0.f), m_fPitch(0.f)
{
    m_Camera.setSpeed(10.f);
}

Controller::~Controller()
{
    delete m_pGround;
}

Controller *Controller::instance()
{
    if(m_sInstance == NULL)
        m_sInstance = new Controller();
    return m_sInstance;
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
    cameraMove();
}

void Controller::onRender()
{
    float v = 172.f/255.f;
    glClearColor(v,v,v,1.f);
    //math::Vector3 e = m_Camera.eye();
    //math::Vector3 d = m_Camera.direction();
    ////math::Vector3 u = m_Camera.up();

    //m_Camera.setTarget(e+d);
    //m_Camera.adjustAxes();
    
    //util::MATRIXSTACK->pushMatrix();
        util::MATRIXSTACK->loadIdentity();
        
        math::Vector3 e = m_Camera.eye();
        e *= -1;
        
        util::MATRIXSTACK->rotate(math::degreeToRad(m_fYaw), m_Camera.up());
        util::MATRIXSTACK->rotate(math::degreeToRad(m_fPitch), m_Camera.right());
        util::MATRIXSTACK->scale(math::Vector3(10.f));
        util::MATRIXSTACK->translate(e);
        m_pGround->onRender();
    //util::MATRIXSTACK->popMatrix();
}

void Controller::cameraMove()
{
    float ang = 10.f;
    if(glfwGetKey('W') == GLFW_PRESS)
        m_Camera.moveForward();
    else if(glfwGetKey('S') == GLFW_PRESS)
        m_Camera.moveBackwards();

    if(glfwGetKey('A') == GLFW_PRESS)
        m_fYaw++;
    else if(glfwGetKey('D') == GLFW_PRESS)
        m_fYaw--;

    if(glfwGetKey('Z') == GLFW_PRESS)
        m_fPitch++;
    else if(glfwGetKey('X') == GLFW_PRESS)
        m_fPitch--;
    /*if(glfwGetKey('A') == GLFW_PRESS)
        m_Camera.moveLeft();
    else if(glfwGetKey('D') == GLFW_PRESS)
        m_Camera.moveRight();

    if(glfwGetKey('Z') == GLFW_PRESS)
        m_Camera.moveUp();
    else if(glfwGetKey('X') == GLFW_PRESS)
        m_Camera.moveDown();*/
}
